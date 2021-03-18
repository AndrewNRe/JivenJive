
extern "C" void jj_audiothread()
{
    for(bit32 i = 0; i < 0x100000; i++){}//RPI2でこの待っているloopが必要。理由はしらないけど。
    for(;isrunning>0;)
    {
        
        platform_threadinformation();
        
        if(facemanager->frametimer < facemanager->responselength)
        {
            bit32 reducedresponselen = facemanager->responselength/2;
            bit32 hertz = 0;
            {//Find hertz()
                f32 floathertz = 0;
                if(facemanager->endhertz > facemanager->starthertz)
                { floathertz = (f32)facemanager->starthertz + ((f32)facemanager->endhertz - (f32)facemanager->starthertz) * facemanager->interpolateamount;  }
                else{ floathertz = (f32)facemanager->endhertz + ((f32)facemanager->starthertz - (f32)facemanager->endhertz) * facemanager->interpolateamount; }
                hertz = (bit32)floathertz;
            }
            {//GenerateAudioResponse()
                bit32 volume = settings.volume;
                bit32 waveperiod = settings.audiosamplerate/hertz;
                if(facemanager->frametimer > reducedresponselen)
                {//RampDownVolume()
                    bit32 Nyquistreducedresponselen = (facemanager->responselength/2)*2; //NOTE: you need to multiply by two on the responselength len because of the Nyquist Frequency!
                    waveperiod = settings.audiosamplerate/((hertz/Nyquistreducedresponselen) * (facemanager->responselength - facemanager->frametimer));
                }
                
                bit32 numpackets = (settings.audiosamplesperframe*2);
                f32 floatwaveperiod = waveperiod; f32 floatvolume = volume;
                
                for(bit32 i = 0; i < numpackets; facemanager->unitcircleprogression += (2.0f*PI32*1.0f)/floatwaveperiod)
                {
                    if(facemanager->unitcircleprogression>=6.283185307179586){facemanager->unitcircleprogression = 0.0f;}
                    f32 result = sinf(facemanager->unitcircleprogression)*floatvolume;
                    s16 packet = result;
                    audiobuffer[i] = packet; i++;
                    audiobuffer[i] = packet; i++;
                }
#if debug
                s32 RV = 0; s32 LV = 0;
                f32 floatingdevsamplesperframe = (f32)settings.audiosamplesperframe;
                for(bit32 i = 0; i < numpackets; i+=2, RV++, LV++)
                {
                    DEBUG_generatedaudiobufferR->vector[RV].position.x = (f32)RV/(floatingdevsamplesperframe/2) - 1.0f;
                    DEBUG_generatedaudiobufferR->vector[RV].position.y = (f32)(audiobuffer[i]/2) /(f32)(volume*2) + 0.5f;
                    DEBUG_generatedaudiobufferR->vector[RV].position.z = 0;
                    DEBUG_generatedaudiobufferL->vector[LV].position.x = (f32)LV/(floatingdevsamplesperframe/2) - 1.0f;
                    DEBUG_generatedaudiobufferL->vector[LV].position.y = (f32)(audiobuffer[i+1]/2) / (f32)(volume*2) - 0.5f;
                    DEBUG_generatedaudiobufferL->vector[LV].position.z = 0;
                    DEBUG_generatedaudiobufferR->vector[RV].color = {1, 1, 1, 1};
                    DEBUG_generatedaudiobufferL->vector[LV].color = {1, .8, 1, 1};
                }
                DEBUG_generatedaudiobufferR->AV = RV; DEBUG_generatedaudiobufferL->AV = LV;
#endif
            }
        }
        else
        {
            for(bit32 i = 0; i < settings.audiosamplesperframe*2; i++)  { audiobuffer[i] = 0;  }
        }
        
        platform_playaudio();
        
#if debug
        lastframeAudioThreadMS = platform_frametime();
#endif
        
        platform_sleep();
        
    } //For loop end
}
