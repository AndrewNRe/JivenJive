#define __forceinline inline
#define GNU 1
#define RPI2 1

#define debug 1
#define controllerplayable 1
#define extragnuinlineinfo __attribute__((always_inline))

//RPI2 Specific Default Defines
#define defaultrefreshrate 60
#define defaultwindowresWIDTH 640
#define defaultwindowresHEIGHT 480
#define defaultvolume 8191
#define defaultwhitenoisecompensation 200
#define defaultaudiosamplerate 44100
#define defaultaudiosamplesperframe (defaultaudiosamplerate / defaultrefreshrate)
#define defaultaudiochannels 2

#include <JJ_types.h>
#include <arm_neon.h>

//Assembly Procedures
extern "C" void SoftwareFrameBufferSwap(bit32 fillcolor);
extern "C" void SoftwareDrawTriangle(bit32 zeroX, bit32 zeroY, bit32 oneX, bit32 oneY, bit32 twoX, bit32 twoY, bit32 zeroColor, bit32 oneColor, bit32 twoColor);
extern "C" void PlayAudio(bit32 numberofsamples, void* audiodata);
extern "C" bit32 QuerySnesController();
extern "C" bit32 RPI2_alloc(bit32 allocsize, bit32 &actualalloc);
extern "C" bit32 __aeabi_uidiv(bit32, bit32);
extern "C" void dummy(bit32);
extern "C" f32 sinf(f32 a);
extern "C" f32 cosf(f32 a);
extern "C" bit64 RPI2_QuerySystemTimerCounter();
//Thread Procs
extern "C" void OSThreadCheck();
extern "C" bit32 RPI2_Query_ThreadID();

#if debug
extern "C" void SDK_BLINKBOARD(bit32 number_of_flashes);
extern "C" void SDK_CHECK_IF_INTTERUPT();
extern "C" void Cwait(bit32 waittime);
extern "C" void LightFlash(bit32 number_of_flashes);
extern "C" void IntegerToAscii(void* writablemem, bit32 integer);
extern "C" void RenderLetterArray(bit8* letters, bit32 numCharToRender, bit32 xStart, bit32 yStart);
#ifdef asm_debug
extern "C" void UARTPutBYTE(bit32 input);
extern "C" bit32 UARTGetBYTE();
#endif

#define RPI2_FUNCTIME_START(storedtime) ( {storedtime = RPI2_QuerySystemTimerCounter() - LastTimeMicroSeconds; bit8 C[16];C[0]='S';C[1]='T';C[2]='R';C[3]='T';C[4]=':';C[5]=' ';C[6]=' ';C[7]=' ';C[8]=' ';C[9]=' ';C[10]=' ';C[11]=' ';C[12]=' ';C[13]=' ';C[14]=' ';C[15]=' ';\
    IntegerToAscii(&C[6], storedtime);RenderLetterArray(C, sizeof(C), 2, 20);})
#define RPI2_FUNCTIME_END(storedtime) ( {bit32 finaltime = (RPI2_QuerySystemTimerCounter() - LastTimeMicroSeconds); bit8 C[16];C[0]='E';C[1]='N';C[2]='D';C[3]='D';C[4]=':';C[5]=' ';C[6]=' ';C[7]=' ';C[8]=' ';C[9]=' ';C[10]=' ';C[11]=' ';C[12]=' ';C[13]=' ';C[14]=' ';C[15]=' ';\
    IntegerToAscii(&C[6], finaltime);RenderLetterArray(C, sizeof(C), 2, 30);\
    finaltime = storedtime - finaltime; C[0]='D';C[1]='I';C[2]='F';C[3]='F';C[4]=':';C[5]=' ';C[6]=' ';C[7]=' ';C[8]=' ';C[9]=' ';C[10]=' ';C[11]=' ';C[12]=' ';C[13]=' ';C[14]=' ';C[15]=' ';\
    IntegerToAscii(&C[6], finaltime);RenderLetterArray(C, sizeof(C), 2, 40);})

#endif


//RPI2 Specific Global(s)
bit64 LastTimeMicroSeconds = 0; //In microseconds, 1,000,000 = 1 second.


//Game files
#include <JJ_vectormath.cpp>
#include <JJ_generalmath.cpp>
#include <JJ_platform_abstraction.cpp>
#include <JJ_assert.cpp>
#include <JJ_memory.cpp>
#include <JJ_input.cpp>
#include <JJ_graphics.cpp>
#include <JJ_settings.cpp>
#include <JJ_signals.cpp> 
#include <JJ_hand.cpp>
#include <JJ_face.cpp>
#include <JJ_main.cpp>
#include <JJ_audiothread.cpp>

//Platform layer
__forceinline void platform_sleep()
{
    bit64 maxtime = LastTimeMicroSeconds + settings.target_timeperframe;
    bit64 currenttime = RPI2_QuerySystemTimerCounter();
    for(;currenttime < maxtime;){ currenttime = RPI2_QuerySystemTimerCounter();}
}

__forceinline void platform_playaudio()
{
#if 0
    bit32 allocsize = settings.audiosamplesperframe*2;
    s8* audiodata8 = (s8*)stackpush(allocsize);
    s8 value = 122;
    for(bit32 i = 0; i < settings.audiosamplesperframe*2; )
    {
        audiodata8[i] = 0;
        audiodata8[i] |= (audiobuffer[i] >> 15); //Get sign bit
        audiodata8[i] |= (audiobuffer[i] & 0x007F); i++; //Get value
        audiodata8[i] = 0;
        audiodata8[i] |= (audiobuffer[i] >> 15); //Get sign bit
        audiodata8[i] |= (audiobuffer[i] & 0x007F); i++; //Get value
    }
#endif
    PlayAudio(settings.audiosamplesperframe, audiobuffer);
#if 0
    stackpop(allocsize);
#endif
}

__forceinline void platform_getmicinput()
{
    mic->framesize = 0;
    mic->currentsize = 0;
}

__forceinline bit32 platform_fileload(const char* filepath, void* file_io_memory)
{//TODO: "fileIO"
    return 0;
}

__forceinline bit32 platform_filewrite(const char* filename, void* file_data, bit32 sizeofdata)
{//TODO:"filewrite"
    return 0;
}

__forceinline bool32 platform_filecopy(const char* filename)
{//TODO: "fileio"
    return 0;
}

__forceinline void platform_render()
{
    f32 SCREEN_X = settings.windowresWIDTH; f32 SCREEN_Y = settings.windowresHEIGHT;
    f32 n = -1; f32 f = 1; f32 r = 1; f32 l = -1; f32 t = 1; f32 b = -1;
    mat4x4 P;
    P.d[0][0] = (2*n)/(r-l); P.d[0][1] = 0; P.d[0][2] = 0; P.d[0][3] = 0; 
    P.d[1][0] = 0; P.d[1][1] = (2/n)/(t-b); P.d[1][2] = 0; P.d[1][3] = 0; 
    P.d[2][0] = (r+l)/(r-l); P.d[2][1] = (t+b)/(t-b); P.d[2][2] = -((f+n)/(f-n)); P.d[2][3] = -1; 
    P.d[3][0] = 0; P.d[3][1] = 0; P.d[3][2] = ((2*n*f)/(f-n)); P.d[3][3] = 0;
    {//FaceRenderProc()
        for(bit32 i = 0; i < facemanager->AI;)
        {
            vec3 A3 = facemanager->vector[facemanager->index[i]].position; 
            vec4 AFc = facemanager->vector[facemanager->index[i]].color;  i++;
            vec3 B3 = facemanager->vector[facemanager->index[i]].position; 
            vec4 BFc = facemanager->vector[facemanager->index[i]].color;   i++;
            vec3 C3 = facemanager->vector[facemanager->index[i]].position; 
            vec4 CFc = facemanager->vector[facemanager->index[i]].color;    i++;
            vec4 A; A.x = A3.x; A.y = A3.y; A.z = A3.z; A.w = 1;
            vec4 B; B.x = B3.x; B.y = B3.y; B.z = B3.z; B.w = 1;
            vec4 C; C.x = C3.x; C.y = C3.y; C.z = C3.z; C.w = 1;
            bit32 Ax = 0; bit32 Ay = 0; bit32 Bx = 0; bit32 By = 0; bit32 Cx = 0; bit32 Cy = 0;
            {//Convert to pixel space
                //Project
                A = P*A;
                B = P*B;
                C = P*C;
                A.z += A.w;
                B.z += B.w;
                C.z += C.w;
                //Do perspective division
                A.x = A.x/A.z;
                A.y = A.y/A.z;
                B.x = B.x/B.z;
                B.y = B.y/B.z;
                C.x = C.x/C.z;
                C.y = C.y/C.z;
                //Flip Y due to the framebuffer being "bottom up" on the rpi2
                A.y = -(A.y);
                B.y = -(B.y);
                C.y = -(C.y);
                //Now, go from floating point to the actual pixel (integer).
                Ax = ((A.x/2)+0.5f) * SCREEN_X;
                Ay = ((A.y/2)+0.5f) * SCREEN_Y;
                Bx = ((B.x/2)+0.5f) * SCREEN_X;
                By = ((B.y/2)+0.5f) * SCREEN_Y;
                Cx = ((C.x/2)+0.5f) * SCREEN_X;
                Cy = ((C.y/2)+0.5f) * SCREEN_Y;
            }
            bit32 Ac = 0; bit32 Bc = 0; bit32 Cc = 0;
            {//Convert JivenJive's colors to RPI2 colors. On the RPI2, the color is suppose to be  A B G R !!!
                Ac = ( (bit32(AFc.w * 255.0f) << 24) | (bit32(AFc.z * 255.0f) << 16) | (bit32(AFc.y * 255.0f) << 8)  | bit32(AFc.x * 255.0f) );
                Bc = ( (bit32(BFc.w * 255.0f) << 24) | (bit32(BFc.z * 255.0f) << 16) | (bit32(BFc.y * 255.0f) << 8)  | bit32(BFc.x * 255.0f) );
                Cc = ( (bit32(CFc.w * 255.0f) << 24) | (bit32(CFc.z * 255.0f) << 16) | (bit32(CFc.y * 255.0f) << 8)  | bit32(CFc.x * 255.0f) );
            }
            SoftwareDrawTriangle(Ax, Ay, Bx, By, Cx, Cy, Ac, Bc, Cc);
        }
    }
    
    //TODO(Andrew) Actually, just make this draw code a function and call it for hte hand instead. Not doing this yet as I don't have a debugger so a simple change could just not work due to compiler / linker bugs.
    if(handmanager->isactive)
    { //Hand Render Proc()
        for(bit32 i = 0; i < handmanager->AI;)
        {
            vec3 A3 = handmanager->vector[handmanager->index[i]].position; 
            vec4 AFc = handmanager->vector[handmanager->index[i]].color;  i++;
            vec3 B3 = handmanager->vector[handmanager->index[i]].position; 
            vec4 BFc = handmanager->vector[handmanager->index[i]].color;   i++;
            vec3 C3 = handmanager->vector[handmanager->index[i]].position; 
            vec4 CFc = handmanager->vector[handmanager->index[i]].color;    i++;
            vec4 A; A.x = A3.x; A.y = A3.y; A.z = A3.z; A.w = 1;
            vec4 B; B.x = B3.x; B.y = B3.y; B.z = B3.z; B.w = 1;
            vec4 C; C.x = C3.x; C.y = C3.y; C.z = C3.z; C.w = 1;
            bit32 Ax = 0; bit32 Ay = 0; bit32 Bx = 0; bit32 By = 0; bit32 Cx = 0; bit32 Cy = 0;
            {
                A = P*A;
                B = P*B;
                C = P*C;
                A.z += A.w;
                B.z += B.w;
                C.z += C.w;
                A.x = A.x/A.z;
                A.y = A.y/A.z;
                B.x = B.x/B.z;
                B.y = B.y/B.z;
                C.x = C.x/C.z;
                C.y = C.y/C.z;
                A.y = -(A.y);
                B.y = -(B.y);
                C.y = -(C.y);
                Ax = ((A.x/2)+0.5f) * SCREEN_X;
                Ay = ((A.y/2)+0.5f) * SCREEN_Y;
                Bx = ((B.x/2)+0.5f) * SCREEN_X;
                By = ((B.y/2)+0.5f) * SCREEN_Y;
                Cx = ((C.x/2)+0.5f) * SCREEN_X;
                Cy = ((C.y/2)+0.5f) * SCREEN_Y;
            }
            bit32 Ac = 0; bit32 Bc = 0; bit32 Cc = 0;
            {
                Ac = ( (bit32(AFc.w * 255.0f) << 24) | (bit32(AFc.z * 255.0f) << 16) | (bit32(AFc.y * 255.0f) << 8)  | bit32(AFc.x * 255.0f) );
                Bc = ( (bit32(BFc.w * 255.0f) << 24) | (bit32(BFc.z * 255.0f) << 16) | (bit32(BFc.y * 255.0f) << 8)  | bit32(BFc.x * 255.0f) );
                Cc = ( (bit32(CFc.w * 255.0f) << 24) | (bit32(CFc.z * 255.0f) << 16) | (bit32(CFc.y * 255.0f) << 8)  | bit32(CFc.x * 255.0f) );
            }
            SoftwareDrawTriangle(Ax, Ay, Bx, By, Cx, Cy, Ac, Bc, Cc);
        }
    }
}

__forceinline void platform_bufferswap()
{
    bit32 fillcolor = 0xFF000000;
    SoftwareFrameBufferSwap(fillcolor);
}

bit32 inputsim = 0; //NOTE: global for input sim
__forceinline void platform_queryinput()
{
    SDK_CHECK_IF_INTTERUPT(); //RPI2 specific.
    if(inputsim >= 240) //If any input at all.
    {
        inputsim = 0;
        input_fillbuffer(KEY_D); //facemanager->emotion.happiness++
        if(facemanager->emotion.happiness >= maxemotionalvalue)
        {
            facemanager->emotion.happiness = -maxemotionalvalue;
            input_fillbuffer(KEY_W); //facemanager->emotion.peacefulness++
            if(facemanager->emotion.peacefulness >= maxemotionalvalue){facemanager->emotion.peacefulness = -maxemotionalvalue;}
        }
    }
    else
    {
        inputsim++;
        input_removebuffer(KEY_D);
        input_removebuffer(KEY_W);
    }
}

__forceinline void platform_hidecursor(){}//STUB : RPI2はmouse cursorがない。
__forceinline void platform_showcursor(){}//同じ。

bit64 platform_frametime()
{
    bit64 currentframetime = RPI2_QuerySystemTimerCounter();
    bit32 truncated =  (currentframetime - LastTimeMicroSeconds);
    bit64 ret = truncated/1000; // / 1000 to get miliseconds from micro.
    return ret;
}
__forceinline void platform_updateframetimer()
{
    LastTimeMicroSeconds = RPI2_QuerySystemTimerCounter();
} 

bool32 audiothreadisready = 0;
extern "C" void RPI2_threadpool() //NOTE: obligated to do this for RPI2.
{ //NOTE: each thread is running this code.
    for(;true;)
    {
        if(audiothreadisready && RPI2_Query_ThreadID() == 1) { jj_audiothread(); }
    }
}

__forceinline void platform_launchthreads()
{
    audiothreadisready = 1;
}

__forceinline void platform_threadinformation()
{
    OSThreadCheck();
}

extern "C" s32 RPI2_main(void)
{
    {//Set all "global" data to zero.
        {//Gamememory
            gamememory.maxsize = Megabytes(500);
            gamememory.stackposition = Megabytes(500);
            gamememory.recordposition = 0;
            gamememory.baseaddress = 0;
        }
        {//Global Record Pointer null set.
            settingscontroller = nullptr;
            input = nullptr;
            handmanager = nullptr;
            facemanager = nullptr;
            mic = nullptr;
        }
    }
    
    {//Setup game memory
        bit32 actual_alloc = 0; bit32 desiredalloc = gamememory.maxsize;
        gamememory.baseaddress = (bit64)RPI2_alloc(desiredalloc, actual_alloc);
        if(actual_alloc == 0){Assert(0)}
        loadsettings();
        {//Settings reset (all but target_timeperframe should do this, or rather, timeperframe needs to ensure it is in miliseconds!)
            settings.windowresWIDTH = defaultwindowresWIDTH; settings.windowresHEIGHT = defaultwindowresHEIGHT;
            settings.refreshrate = defaultrefreshrate; settings.volume = defaultvolume; settings.whitenoisecompensation = defaultwhitenoisecompensation;
            settings.audiosamplesperframe = defaultaudiosamplesperframe;
            settings.target_timeperframe = ((1000/defaultrefreshrate) * 1000);
            settings.audiosamplerate = defaultaudiosamplerate; settings.micsamplerate = defaultaudiochannels;
            settings.audiochannels = defaultaudiochannels;
        }
        loadinput();
        loadhand();
        loadfaceone();
        loadmic();
    }
    
    {//RPI2 Game Mic Setup
        mic->micbuffer = (s16*)recordpush(settings.micsamplerate*2);
        audiobuffer = (s16*)recordpush(settings.audiosamplerate*2);
    }
    
    {//RPI2 Timer Setup
        platform_updateframetimer();
    }
    
    platform_launchthreads();
    
    //Start the game!
    JJ_main();
}