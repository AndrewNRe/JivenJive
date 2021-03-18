bool32 isrunning = true; //NOTE: GLOBAL FOR GAME RUNNING!

#if debug
bit64 lastframeAudioThreadMS = 0;
#endif

__forceinline void JJ_main()
{
    for(;isrunning>0;)
    {
        platform_queryinput();
        
        {//SettingsProc()
            if(settingscontroller->inputdelay >= minframesforinput)
            {
                if(input_buttoncheck(KEY_ESC) || input_buttoncheck(MM_DOWN)) 
                {//Set State()
                    if(settingscontroller->isactive == 1)
                    {
                        platform_filewrite("Settings.JJsettings", &settings.refreshrate, sizeof(settings.refreshrate)+sizeof(settings.volume)+sizeof(settings.whitenoisecompensation));
                        settingscontroller->isactive = 0; 
                    } 
                    else
                    { settingscontroller->isactive = 1; }
                    settingscontroller->inputdelay = 0;
                }
            }
            else{settingscontroller->inputdelay++;}
            if(settingscontroller->isactive)
            {//Display and Edit Settings()
                {//SetFaceAndHandToGray
                    for(bit32 i = 0; i < facemanager->AV; i++)
                    { facemanager->vector[i].color.x = .2; facemanager->vector[i].color.y = .2; facemanager->vector[i].color.z = .2; facemanager->vector[i].color.w = 1; }
                    for(bit32 i = 0; i < handmanager->AV; i++) { handmanager->vector[i].color.x = .3; handmanager->vector[i].color.y = .3; handmanager->vector[i].color.z = .3; handmanager->vector[i].color.w =  1; } 
                }
                {//Volume()
                    vec2 tempinput;
                    tempinput.x = input->rightaxis.x - settingscontroller->vertex[3].position.x;
                    tempinput.y = input->rightaxis.y - settingscontroller->vertex[3].position.y;
                    vec2 upperleft;
                    upperleft.x = settingscontroller->vertex[0].position.x - settingscontroller->vertex[3].position.x;
                    upperleft.y = settingscontroller->vertex[0].position.y - settingscontroller->vertex[3].position.y;
                    if(tempinput.y >= 0 && tempinput.y <= upperleft.y  &&
                       tempinput.x >= 0 && tempinput.x <= upperleft.x)
                    {
                        if(input_buttoncheck(LMB_DOWN))
                        {
                            f32 lowX = settingscontroller->vertex[3].position.x; f32 maxX = settingscontroller->vertex[1].position.x;
                            settings.volume = (input->rightaxis.x - lowX) * (f32)maxsignedsixteenvalue;
                            settingscontroller->vertex[2].position.x = input->rightaxis.x;
                            settingscontroller->vertex[5].position.x = input->rightaxis.x;
                        }
                    }
                }
                {//WhiteNoise()
                    vec2 tempinput;
                    tempinput.x = input->rightaxis.x - settingscontroller->vertex[9].position.x;
                    tempinput.y = input->rightaxis.y - settingscontroller->vertex[9].position.y;
                    vec2 upperleft;
                    upperleft.x = settingscontroller->vertex[6].position.x - settingscontroller->vertex[9].position.x;
                    upperleft.y = settingscontroller->vertex[6].position.y - settingscontroller->vertex[9].position.y;
                    if(tempinput.y >= 0 && tempinput.y <= upperleft.y &&
                       tempinput.x >= 0 && tempinput.x <= upperleft.x)
                    {
                        if(input_buttoncheck(LMB_DOWN))
                        {
                            f32 lowX = settingscontroller->vertex[9].position.x; f32 maxX = settingscontroller->vertex[7].position.x;
                            settings.whitenoisecompensation = (input->rightaxis.x - lowX) * (f32)maxsignedsixteenvalue;
                            settingscontroller->vertex[8].position.x = input->rightaxis.x;
                            settingscontroller->vertex[11].position.x = input->rightaxis.x;
                        }
                    }
                }
                platform_showcursor();
                goto frame_end_proc;
            }
            else
            {//ColorHandAndFace
                setfacedefaultcolor(); sethanddefaultcolor();
            }
        }
        
        {//MicProc()
            platform_getmicinput(); 
#if debug
            if(DEBUG_micFinputdelayinframes >= DEBUG_maxinputdelayinframes)
            { 
                if(input_buttoncheck(KEY_F) == KEY_F) 
                {
                    DEBUG_showmicdebug ^= 1; DEBUG_micFinputdelayinframes = 0;
                    if(DEBUG_showgeneratedaudiobuffer){DEBUG_showgeneratedaudiobuffer = 0;}
                }
            }
            else {DEBUG_micFinputdelayinframes++;}
#endif
            s16* data = mic->micbuffer;
            bit32 currentsize = mic->currentsize; 
            bit32 framesize = mic->framesize;
            bit32 validframesize = currentsize - framesize; 
            for(bit32 i = validframesize; i < currentsize; i++) { if(data[i] > 0 || data[i] < 0) {goto contproc;} }
            if(mic->numberofframes > minframesforinput)
            { 
                readmicdata:
#if debug
                {//write the mic input to the graphics debug()
                    bit32 maxonechannelindex = validframesize/2;
                    bit32 onechannelindex = 0;
                    for(bit32 i = 0; i < validframesize; i+=2, onechannelindex++)
                    {
                        vec3 R; vec3 L;
                        R.x = ((f32)onechannelindex/((f32)maxonechannelindex/2)) - 1; 
                        R.y = ((f32)data[i]/2) / ((f32)maxsignedsixteenvalue/2) + .5f; 
                        R.z = 0;
                        L.x = ((f32)onechannelindex/((f32)maxonechannelindex/2)) - 1; 
                        L.y = ((f32)data[i+1]/2) / ((f32)maxsignedsixteenvalue/2)-.5f; 
                        L.z = 0;
                        DEBUG_RightChannel->vector[onechannelindex].position = R;
                        //NOTE: IF you use this debug on RPI2, you MUST REMOVE THE INITALIZATION LISTS BECAUSE THEY DO NOT WORK ON GNU'S COMPILER, OR REQUIRE CRT TO WORK!
                        if(data[i] == 0) { DEBUG_RightChannel->vector[onechannelindex].color = {1, 0, 0, 1}; } else { DEBUG_RightChannel->vector[onechannelindex].color = {1, 1, 1, 1}; }
                        DEBUG_LeftChannel->vector[onechannelindex].position = L;
                        if(data[i+1] == 0){ DEBUG_LeftChannel->vector[onechannelindex].color = {1, 0, 0, 1}; } else { DEBUG_LeftChannel->vector[onechannelindex].color = {1, 1, 1, 1}; }
                    }
                    DEBUG_RightChannel->AV = onechannelindex;
                    DEBUG_LeftChannel->AV = onechannelindex;
                }
#endif
                {//Zero crossing()
                    bit32 RzeroC = 0; bit32 LzeroC = 0;
                    {//Count zero crossings.
                        bool32 crossingvalues = 0; 
                        for(bit32 i = 0; i < validframesize; i+=2)
                        {
                            s16 Rsample = data[i]; s16 Lsample = data[i+1]; bool32 recentflag = 0;
                            if(Rsample > 0) { crossingvalues |= 0x00000001; recentflag |= 0x00000001;  }
                            else if(Rsample < 0) { crossingvalues |= 0x00000002; recentflag |= 0x00000002; }
                            if(Lsample > 0){ crossingvalues |= 0x00000004;  recentflag |= 0x00000004; }
                            else if(Lsample < 0) {crossingvalues |= 0x00000008; recentflag |= 0x00000008; }
                            if( (crossingvalues&0x00000003) == 0x00000003){RzeroC++; crossingvalues = (crossingvalues&0xFFFFFFFC); crossingvalues |= (recentflag&0x00000003);}
                            if( (crossingvalues&0x0000000C) == 0x0000000C){LzeroC++; crossingvalues = (crossingvalues&0xFFFFFFF3); crossingvalues |= (recentflag&0x0000000C);}
                        }
                    }
                    {//Get fundamental frequency in ONE SECOND!
                        bit32 leftoverframes = settings.refreshrate - mic->numberofframes; if(leftoverframes <= 0){leftoverframes = 1;}
                        bit32 result = (RzeroC*leftoverframes) + (LzeroC*leftoverframes);
                        mic->direction = (f32)result - mic->lastnumberofzerocrossing; 
                        mic->wasinput = 1;
                        mic->lastnumberofzerocrossing = result;
                    }
                }
            }
            mic->currentsize = 0;
            mic->numberofframes = 0;
            goto endproc;
            contproc:
            mic->numberofframes++;
            if(mic->numberofframes >= settings.refreshrate) { goto readmicdata; }
            endproc: 
            mic->framesize = 0;
        }
        
        {//FACEONE GAMEPLAY PROC()
#if debug
            {//SetupDataToDrawDebugEmotionalAxes
                {//Check if show emotional axes
                    if(DEBUG_emotionalaxesinputdelay>=DEBUG_maxinputdelayinframes)
                    {if(input_buttoncheck(KEY_R)){ DEBUG_emotionalaxesdisplay ^= 1; DEBUG_emotionalaxesinputdelay = 0;}}
                    else{DEBUG_emotionalaxesinputdelay++;}
                }
                {//Check if show generated audio buffer
                    if(DEBUG_generateaudiobufferinputdelay >= DEBUG_maxinputdelayinframes)
                    { 
                        if(input_buttoncheck(KEY_T))
                        {
                            DEBUG_showgeneratedaudiobuffer ^= 1; DEBUG_generateaudiobufferinputdelay++;
                            if(DEBUG_showmicdebug){DEBUG_showmicdebug = 0;}
                        } 
                    }
                    else{DEBUG_generateaudiobufferinputdelay++;}
                }
            }
#endif
#if controllerplayable
            if(DEBUG_changeemotionusingkeydelay >= DEBUG_maxinputdelayinframes)
            {
                if(input_buttoncheck(KEY_W))
                {//1
                    DEBUG_changeemotionusingkeydelay = 0;
                    facemanager->emotion.peacefulness++;
                    goto DEBUG_skippitchdetection;
                }
                else if(input_buttoncheck(KEY_S))
                {//2
                    DEBUG_changeemotionusingkeydelay = 0;
                    facemanager->emotion.peacefulness--;
                    goto DEBUG_skippitchdetection;
                }
                else if(input_buttoncheck(KEY_D))
                {//3
                    DEBUG_changeemotionusingkeydelay = 0;
                    facemanager->emotion.happiness++;
                    goto DEBUG_skippitchdetection;
                }
                else if(input_buttoncheck(KEY_A))
                {//4
                    DEBUG_changeemotionusingkeydelay = 0;
                    facemanager->emotion.happiness--;
                    goto DEBUG_skippitchdetection;
                }
            }
            else
            { DEBUG_changeemotionusingkeydelay++; }
#endif
            
            if(input_buttoncheck(KEY_SPACEBAR) || input_buttoncheck(LMB_DOWN))
            {//HandProc()
                handmanager->isactive = 1;
                handmanager->rotation += OneDegreesInRadians; if(handmanager->rotation >= 2*PI32) { handmanager->rotation = 0; } //RotateHand
                handmanager->handposition = input->rightaxis; 
                if(handmanager->handposition.x >= 1.0f){handmanager->handposition.x = 1.0f;}
                else if(handmanager->handposition.x <= -1.0f){handmanager->handposition.x = -1.0f;}
                if(handmanager->handposition.y >= 1.0f){handmanager->handposition.y = 1.0f;}
                else if(handmanager->handposition.y <= -1.0f){handmanager->handposition.y = -1.0f;}
                platform_hidecursor();
            }
            else{handmanager->isactive = 0; platform_showcursor();}
            //if(handmanager->isactive)
            if(mic->wasinput > 0)
            {
                mic->wasinput = 0;
                {//MoveEmotionalPosition()
                    if(facemanager->emotion.happiness >= 0)
                    {
                        if(facemanager->emotion.peacefulness >= 0)
                        {//Q1 pitch detection
                            if(mic->direction > 0)
                            {
                                if(handmanager->isactive) {  facemanager->emotion.happiness++; facemanager->emotion.peacefulness--; }
                                else {  facemanager->emotion.happiness++; facemanager->emotion.peacefulness++; }
                            }
                            else if(mic->direction < 0)
                            {
                                if(handmanager->isactive) {facemanager->emotion.happiness--; facemanager->emotion.peacefulness++;}
                                else{facemanager->emotion.happiness--; facemanager->emotion.peacefulness--;}
                            }
                        }
                        else
                        {//Q4 pitch detection
                            if(mic->direction > 0)
                            {
                                if(handmanager->isactive) {facemanager->emotion.happiness++;}
                                else { facemanager->emotion.peacefulness++;}
                            }
                            else if (mic->direction < 0)
                            {
                                if(handmanager->isactive){facemanager->emotion.peacefulness-=2; facemanager->emotion.happiness-=2;}
                                else{facemanager->emotion.happiness--;}
                            }
                        }
                    }
                    else
                    {
                        if(facemanager->emotion.peacefulness > 0)
                        {//Q2 pitch detection
                            if(mic->direction > 0)
                            {
                                if(handmanager->isactive){facemanager->emotion.peacefulness--; facemanager->emotion.happiness+=2;}
                                else{facemanager->emotion.peacefulness-=2; facemanager->emotion.happiness++;}
                            }
                            else if(mic->direction < 0)
                            {
                                if(handmanager->isactive){facemanager->emotion.peacefulness++;}
                                else{facemanager->emotion.happiness++;}
                            }
                        }
                        else
                        {//Q3 pitch detection 
                            if(mic->direction > 0)
                            {
                                if(handmanager->isactive){facemanager->emotion.peacefulness-=2; facemanager->emotion.happiness--;}
                                else{facemanager->emotion.peacefulness++; facemanager->emotion.happiness++;}
                            }
                            else if(mic->direction < 0)
                            {
                                if(handmanager->isactive){facemanager->emotion.peacefulness--; facemanager->emotion.happiness-=2;}
                                else{facemanager->emotion.peacefulness++; facemanager->emotion.happiness--;}
                            }
                        }
                    }
#if controllerplayable
                    DEBUG_skippitchdetection:
#endif
                    if(facemanager->emotion.happiness >= maxemotionalvalue){ facemanager->emotion.happiness = maxemotionalvalue; }
                    else if (facemanager->emotion.happiness <= -maxemotionalvalue) {facemanager->emotion.happiness = -maxemotionalvalue;}
                    if(facemanager->emotion.peacefulness >= maxemotionalvalue) { facemanager->emotion.peacefulness = maxemotionalvalue; } 
                    else if(facemanager->emotion.peacefulness <= -maxemotionalvalue){facemanager->emotion.peacefulness = -maxemotionalvalue;}
                }
                
                {//Figure out next animation() 次のアニメーションを見分ける()
                    bit32 numberofanimationclips = 0;
                    bit32 ID = 1; bit32 FID = 0;
                    {//FindClosestAnimationClip()
                        vec2 emotionalposition[8];
                        numberofanimationclips = sizeof(emotionalposition) / sizeof(vec2);
                        emotionalposition[0].x = .1; emotionalposition[0].y = .1; //CalmPeaceful
                        emotionalposition[1].x = 1; emotionalposition[1].y = 1; //VeryHappyPeace
                        emotionalposition[2].x = -.1; emotionalposition[2].y = .1; //KindaSadPeaceful
                        emotionalposition[3].x = -1; emotionalposition[3].y = 1; //SuperSadPeace
                        emotionalposition[4].x = -.1; emotionalposition[4].y = -.1; //LowAngrySad
                        emotionalposition[5].x = -1; emotionalposition[5].y = -1; //SuperSadAngry
                        emotionalposition[6].x = .1; emotionalposition[6].y = -.1; //HappyIrritated
                        emotionalposition[7].x = 1; emotionalposition[7].y = -1; //HappyIrritated
                        vec2 normalizedemotionalaxes;
                        normalizedemotionalaxes.x = facemanager->emotion.happiness / maxemotionalvalue; normalizedemotionalaxes.y = facemanager->emotion.peacefulness/maxemotionalvalue;
                        f32 closestdot = dot_vec2(normalizedemotionalaxes, emotionalposition[0]);
                        for(bit32 i = 1; i < numberofanimationclips; i++)
                        {
                            f32 dot = dot_vec2(normalizedemotionalaxes, emotionalposition[i]);
                            if(dot > closestdot){closestdot = dot; ID = FID; FID = i; }
                        }
                        {//Interpolate
                            vec2 difference =  ((normalizedemotionalaxes-emotionalposition[FID]) - (emotionalposition[ID] - emotionalposition[FID]));
                            facemanager->interpolateamount = dot_vec2(difference, difference);
                            facemanager->interpolateamount = (facemanager->interpolateamount > 1.0f) ? 1.0f : facemanager->interpolateamount; 
                        }
#if debug
                        {//DebugEmotionalDataSetupForDraw()
                            DEBUG_emotionaldata.DEBUG_animID = ID; DEBUG_emotionaldata.DEBUG_animFID = FID;
                            DEBUG_emotionaldata.numberofanimations = numberofanimationclips;
                            vec4 basecolor = {.2, .1, 0, 1};
                            for(bit32 i = 0; i < numberofanimationclips; i++, DEBUG_emotionaldata.emotions.AV++)
                            { 
                                DEBUG_emotionaldata.emotions.vector[i].position = {emotionalposition[i].x, emotionalposition[i].y, 0};
                                DEBUG_emotionaldata.emotions.vector[i].color.x = (f32)((f32)i/(f32)numberofanimationclips) + basecolor.x;
                                DEBUG_emotionaldata.emotions.vector[i].color.y = (f32)((f32)i/(f32)numberofanimationclips) + basecolor.y;
                                DEBUG_emotionaldata.emotions.vector[i].color.z = basecolor.z;
                                DEBUG_emotionaldata.emotions.vector[i].color.w = basecolor.w;
                            }
                        }
#endif
                    }
                    {//WriteAnimationInformation()
                        bit32 vectorallocamt = facemanager->AV*sizeof(vec3);
                        bit32 timerallocamt = facemanager->AV*sizeof(bit32);
                        vec3* vectorbuffer = (vec3*)stackpush(vectorallocamt); setfaceonedefaultanimdata(vectorbuffer);
                        bit32* timerbuffer = (bit32*)stackpush(timerallocamt); for(bit32 i = 0; i < facemanager->AV; i++){timerbuffer[i] = 0;}
                        bit32 currenthertz = 0; bool32 finished = 0; 
                        start_proc:
                        if(ID == 0) { goto CalmPeaceful_Animation; }
                        else if(ID == 1) { goto VeryHappyPeace_Animation; }
                        else if(ID == 2) { goto KindaSadPeace_Animation; }
                        else if(ID == 3) { goto SuperSadPeace_Animation; }
                        else if(ID == 4) { goto LowAngrySad_Animation; }
                        else if(ID ==5) { goto SuperSadAngry_Animation; }
                        else if(ID == 6){ goto HappyIrritated_Animation; }
                        else if(ID == 7){ goto VeryHappyAnger_Animation; }
                        else {Assert(0); goto end_proc;}
                        //Animations Start
                        {//Happy And Peaceful Animations 「Q1」 vec2(POSITIVE, POSITIVE)
                            { CalmPeaceful_Animation:
                                {//Nose()
                                    timerbuffer[13] = 0; timerbuffer[18] = 0;
                                    vectorbuffer[13].x = -.14; vectorbuffer[13].y = -.3; vectorbuffer[13].z = .1; 
                                    vectorbuffer[18].x = .14; vectorbuffer[18].y =  -.3; vectorbuffer[18].z = .1;
                                }
                                {//Lower cheek area()
                                    timerbuffer[21] = 1; timerbuffer[24] = 1;
                                    vectorbuffer[21].x = -.4; vectorbuffer[21].y = -.20; vectorbuffer[21].z = 0; 
                                    vectorbuffer[24].x = .4; vectorbuffer[24].y = -.20; vectorbuffer[24].z = 0;
                                }
                                currenthertz = 440;
                                goto end_proc;
                            }
                            { VeryHappyPeace_Animation:
                                {//Forhead()
                                    timerbuffer[1] = 1; timerbuffer[5] = 1;
                                    vectorbuffer[1].x = -.27; vectorbuffer[1].y = .55; vectorbuffer[1].z = .12; 
                                    vectorbuffer[5].x = .27; vectorbuffer[5].y = .55; vectorbuffer[5].z = .12;
                                }
                                {//eyebrow bones()
                                    timerbuffer[8] = 0; timerbuffer[7] = 0;
                                    vectorbuffer[8].x = -.08; vectorbuffer[8].y = .27; vectorbuffer[8].z = .05; 
                                    vectorbuffer[7].x = .08; vectorbuffer[7].y = .27; vectorbuffer[7].z = .05;
                                }
                                {//Nose()
                                    timerbuffer[13] = 0; timerbuffer[18] = 0;
                                    vectorbuffer[13].x = -.14; vectorbuffer[13].y = -.3; vectorbuffer[13].z = .1; 
                                    vectorbuffer[18].x = .14; vectorbuffer[18].y = -.3; vectorbuffer[18].z = .1;
                                }
                                {//Lower cheek area()
                                    timerbuffer[21] = 1; timerbuffer[24] = 1;
                                    vectorbuffer[21].x = -.4; vectorbuffer[21].y = -.20; vectorbuffer[21].z = 0; 
                                    vectorbuffer[24].x = .4; vectorbuffer[24].y = -.20; vectorbuffer[24].z = 0;
                                }
                                currenthertz = 450;
                                goto end_proc;
                            }
                        }
                        {//Sad And Peaceful Animations 「Q2」 vec2(NEGATIVE, POSITIVE)
                            { KindaSadPeace_Animation:
                                {//Move far corner of eyes()
                                    timerbuffer[10] = 1; timerbuffer[15] = 1;
                                    vectorbuffer[10].x = -.50; vectorbuffer[10].y = .1; vectorbuffer[10].z = .1;
                                    vectorbuffer[15].x = .50; vectorbuffer[15].y = .1; vectorbuffer[15].z = .1;
                                }
                                {//Move upper vertex of eyes()
                                    timerbuffer[11] = 1; timerbuffer[16] = 1;
                                    vectorbuffer[11].x = -.2; vectorbuffer[11].y = .13; vectorbuffer[11].z = 0;
                                    vectorbuffer[16].x = .2; vectorbuffer[16].y = .13; vectorbuffer[16].z = 0;
                                }
                                {//Move forhead vertexes()
                                    timerbuffer[1] = 1; timerbuffer[5] = 1;
                                    vectorbuffer[1].x = -.25; vectorbuffer[1].y = .28; vectorbuffer[1].z = .1; 
                                    vectorbuffer[5].x = .25; vectorbuffer[5].y = .28; vectorbuffer[5].z = .1;
                                }
                                currenthertz = 494;
                                goto end_proc;
                            }
                            { SuperSadPeace_Animation:
                                {//Shrinkcheeks()
                                    timerbuffer[25] = 0; timerbuffer[29] = 0;
                                    vectorbuffer[25].x = -.54; vectorbuffer[25].y = -.59; vectorbuffer[25].z = .05; 
                                    vectorbuffer[29].x = .54; vectorbuffer[29].y = -.59; vectorbuffer[29].z = .05;
                                }
                                {//ShrinkTemples()
                                    timerbuffer[3] = 0; timerbuffer[6] = 0;
                                    vectorbuffer[3].x = -.59; vectorbuffer[3].y = .29; vectorbuffer[3].z = 0; 
                                    vectorbuffer[6].x = .59; vectorbuffer[6].y = .29; vectorbuffer[6].z = 0;
                                }
                                {//LowerInnerEyeCorner()
                                    timerbuffer[12] = 1; timerbuffer[17] = 1;
                                    vectorbuffer[12].x = -.14; vectorbuffer[12].y = -.01; vectorbuffer[12].z = 0; 
                                    vectorbuffer[17].x = .14; vectorbuffer[17].y = -.01; vectorbuffer[17].z = 0;
                                }
                                {//Move far corner of eyes()
                                    timerbuffer[10] = 1; timerbuffer[15] = 1;
                                    vectorbuffer[10].x = -.50; vectorbuffer[10].y = .1; vectorbuffer[10].z = .1;
                                    vectorbuffer[15].x = .50; vectorbuffer[15].y = .1; vectorbuffer[15].z = .1;
                                }
                                {//Move upper vertex of eyes()
                                    timerbuffer[11] = 1; timerbuffer[16] = 1;
                                    vectorbuffer[11].x = -.2; vectorbuffer[11].y = .13; vectorbuffer[11].z = 0; 
                                    vectorbuffer[16].x = .2; vectorbuffer[16].y= .13; vectorbuffer[16].z = 0;
                                }
                                {//Move forhead vertexes()
                                    timerbuffer[1] = 1; timerbuffer[5] = 1;
                                    vectorbuffer[1].x = -.25; vectorbuffer[1].y = .28; vectorbuffer[1].z = .1; 
                                    vectorbuffer[5].x = .25; vectorbuffer[5].y = .28; vectorbuffer[5].z = .1;
                                }
                                currenthertz = 480;
                                goto end_proc;
                            }
                        }
                        {//Angry And Sad Animations 「Q3」 vec2(NEGATIVE, NEGATIVE)
                            { LowAngrySad_Animation:
                                {//Move upper nose bridge()
                                    timerbuffer[8] = 1; timerbuffer[7] = 1;
                                    vectorbuffer[8].x = -.08; vectorbuffer[8].y = .20; vectorbuffer[8].z = .05; 
                                    vectorbuffer[7].x = .08; vectorbuffer[7].y = .20; vectorbuffer[7].z = .05;
                                }
                                {//Move upper vertex of eyes()
                                    timerbuffer[11] = 1; timerbuffer[16] = 1;
                                    vectorbuffer[11].x = -.2; vectorbuffer[11].y = .11; vectorbuffer[11].z = 0; 
                                    vectorbuffer[16].x = .2; vectorbuffer[16].y =.11; vectorbuffer[16].z = 0;
                                }
                                {//Move inner corner of eyes()
                                    timerbuffer[12] = 1; timerbuffer[17] = 1;
                                    vectorbuffer[12].x = -.16; vectorbuffer[12].y = .025; vectorbuffer[12].z = 0; 
                                    vectorbuffer[17].x = .16; vectorbuffer[17].y = .025; vectorbuffer[17].z = 0;
                                }
                                {//Move far corner of eyes()
                                    timerbuffer[10] = 1; timerbuffer[15] = 1;
                                    vectorbuffer[10].x = -.55; vectorbuffer[10].y = .12; vectorbuffer[10].z = .1; 
                                    vectorbuffer[15].x = .55; vectorbuffer[15].y = .12; vectorbuffer[15].z = .1;
                                }
                                {//Move forhead vertexes()
                                    timerbuffer[1] = 1; timerbuffer[5] = 1;
                                    vectorbuffer[1].x = -.28; vectorbuffer[1].y = .28; vectorbuffer[1].z = .1;
                                    vectorbuffer[5].x = .28; vectorbuffer[5].y = .28; vectorbuffer[5].z = .1;
                                }
                                currenthertz = 523;
                                goto end_proc;
                            }
                            { SuperSadAngry_Animation:
                                {//ShirnkCheeks()
                                    timerbuffer[25] = 1; timerbuffer[29] = 1;
                                    vectorbuffer[25].x = -.48; vectorbuffer[25].y = -.55; vectorbuffer[25].z = 0;
                                    vectorbuffer[29].x = .48; vectorbuffer[29].y = -.55; vectorbuffer[29].z = 0;
                                }
                                {//Shrinktemples()
                                    timerbuffer[3] = 1; timerbuffer[6] = 1;
                                    vectorbuffer[3].x = -.57; vectorbuffer[3].y = .26; vectorbuffer[3].z = .05; 
                                    vectorbuffer[6].x = .57; vectorbuffer[6].y = .26; vectorbuffer[6].z = .05;
                                }
                                {//Move upper nose bridge()
                                    timerbuffer[8] = 1; timerbuffer[7] = 1;
                                    vectorbuffer[8].x = -.02; vectorbuffer[8].y = .1; vectorbuffer[8].z = .06; 
                                    vectorbuffer[7].x = .02; vectorbuffer[7].y = .1; vectorbuffer[7].z = .06;
                                }
                                {//Move upper vertex of eyes()
                                    timerbuffer[11] = 1; timerbuffer[16] = 1;
                                    vectorbuffer[11].x = -.15; vectorbuffer[11].y = .08; vectorbuffer[11].z = 0;
                                    vectorbuffer[16].x = .15; vectorbuffer[16].y = .08; vectorbuffer[16].z = 0;
                                }
                                {//Move inner corner of eyes()
                                    timerbuffer[12] = 1; timerbuffer[17] = 1;
                                    vectorbuffer[12].x = -.16; vectorbuffer[12].y = -.01; vectorbuffer[12].z = 0;
                                    vectorbuffer[17].x = .16; vectorbuffer[17].y = -.01; vectorbuffer[17].z = 0;
                                }
                                {//Move far corner of eyes()
                                    timerbuffer[10] = 1; timerbuffer[15] = 1;
                                    vectorbuffer[10].x = -.55; vectorbuffer[10].y = .12; vectorbuffer[10].z = .1; 
                                    vectorbuffer[15].x = .55; vectorbuffer[15].y = .12; vectorbuffer[15].z = .1;
                                }
                                {//Move forhead vertexes()
                                    timerbuffer[1] = 1; timerbuffer[5] = 1;
                                    vectorbuffer[1].x  = -.28; vectorbuffer[1].y = .28; vectorbuffer[1].z = .1;
                                    vectorbuffer[5].x = .28; vectorbuffer[5].y = .28; vectorbuffer[5].z = .1;
                                }
                                currenthertz = 622;
                                goto end_proc;
                            }
                        }
                        {//Happy And Angry Animations 「Q4」 vec2(POSITIVE, NEGATIVE)
                            { HappyIrritated_Animation:
                                {//Move far corner of eyes()
                                    timerbuffer[10] = 1; timerbuffer[15] = 1;
                                    vectorbuffer[10].x = -.57; vectorbuffer[10].y = .08; vectorbuffer[10].z = .1;
                                    vectorbuffer[15].x = .57; vectorbuffer[15].y = .08; vectorbuffer[15].z = .1;
                                }
                                {//Move upper vertex of eyes()
                                    timerbuffer[11] = 1; timerbuffer[16] = 1;
                                    vectorbuffer[11].x = -.31; vectorbuffer[11].y = .10; vectorbuffer[11].z = 0;
                                    vectorbuffer[16].x = .31; vectorbuffer[16].y = .10; vectorbuffer[16].z = 0;
                                }
                                {//Lower eye vertexes()
                                    timerbuffer[20] = 1; timerbuffer[23] = 1;
                                    vectorbuffer[20].x = -.35; vectorbuffer[20].y = -.02; vectorbuffer[20].z = .02; 
                                    vectorbuffer[23].x = .35; vectorbuffer[23].y = -.02; vectorbuffer[23].z = .02;
                                }
                                {//Move upper nose bridge()
                                    timerbuffer[8] = 0; timerbuffer[7] = 0;
                                    vectorbuffer[8].x = -.11; vectorbuffer[8].y = .19; vectorbuffer[8].z = .05; 
                                    vectorbuffer[7].x = .11; vectorbuffer[7].y = .19; vectorbuffer[7].z = .05;
                                }
                                {//Move forhead vertexes()
                                    timerbuffer[1] = 0; timerbuffer[5] = 0;
                                    vectorbuffer[1].x = -.36; vectorbuffer[1].y = .36; vectorbuffer[1].z = .1; 
                                    vectorbuffer[5].x = .36; vectorbuffer[5].y = .36; vectorbuffer[5].z = .1;
                                }
                                currenthertz = 523;
                                goto end_proc;
                            }
                            { VeryHappyAnger_Animation:
                                {//Move far corner of eyes()
                                    timerbuffer[10] = 1; timerbuffer[15] = 1;
                                    vectorbuffer[10].x = -.57; vectorbuffer[10].y = .08; vectorbuffer[10].z = .1; 
                                    vectorbuffer[15].x = .57; vectorbuffer[15].y = .08; vectorbuffer[15].z = .1;
                                }
                                {//Move upper vertex of eyes()
                                    timerbuffer[11] = 1; timerbuffer[16] = 1;
                                    vectorbuffer[11].x = -.31; vectorbuffer[11].y = .10; vectorbuffer[11].z = 0; 
                                    vectorbuffer[16].x = .31; vectorbuffer[16].y =.10; vectorbuffer[16].z = 0;
                                }
                                {//Lower eye vertexes()
                                    timerbuffer[20] = 1; timerbuffer[23] = 1;
                                    vectorbuffer[20].x = -.35; vectorbuffer[20].y = -.02; vectorbuffer[20].z = .02;
                                    vectorbuffer[23].x = .35; vectorbuffer[23].y = -.02; vectorbuffer[23].z = .02;
                                }
                                {//Move upper nose bridge()
                                    timerbuffer[8] = 0; timerbuffer[7] = 0;
                                    vectorbuffer[8].x = -.11; vectorbuffer[8].y = .19; vectorbuffer[8].z = .05; 
                                    vectorbuffer[7].x = .11; vectorbuffer[7].y = .19; vectorbuffer[7].z = .05;
                                }
                                {//Move forhead vertexes()
                                    timerbuffer[1] = 0; timerbuffer[5] = 0;
                                    vectorbuffer[1].x = -.36; vectorbuffer[1].y = .36; vectorbuffer[1].z = .1; 
                                    vectorbuffer[5].x = .36; vectorbuffer[5].y = .36; vectorbuffer[5].z = .1;
                                }
                                {//NoseWiden()
                                    timerbuffer[13] = 1; timerbuffer[18] = 1;
                                    vectorbuffer[13].x = -.13; vectorbuffer[13].y =  -.3; vectorbuffer[13].z = .1;
                                    vectorbuffer[18].x = .13; vectorbuffer[18].y = -.3; vectorbuffer[18].z = .1;
                                }
                                {//cheekshorten()
                                    timerbuffer[25] = 1; timerbuffer[29] = 1;
                                    vectorbuffer[25].x = -.49; vectorbuffer[25].y = -.59; vectorbuffer[25].z = 0; 
                                    vectorbuffer[29].x = .49; vectorbuffer[29].y = -.59; vectorbuffer[29].z = 0;
                                }
                                currenthertz = 622;
                                goto end_proc;
                            }
                        }
                        
                        end_proc:
                        if(finished == 0)
                        {
                            for(bit32 i = 0; i < facemanager->AV; i++){facemanager->anim[i].timerID = 0;}
                            finished = 1; ID = FID;
                            for(bit32 i = 0; i < facemanager->AV; i++) 
                            { 
                                facemanager->anim[i].timerID = timerbuffer[i];
                                facemanager->anim[i].start = vectorbuffer[i];  
                            }
                            facemanager->starthertz = currenthertz;
                            goto start_proc;
                        }
                        for(bit32 i = 0; i < facemanager->AV; i++) 
                        { 
                            facemanager->anim[i].timerID = (facemanager->anim[i].timerID == 0) ? facemanager->anim[i].timerID : timerbuffer[i];
                            facemanager->anim[i].end = vectorbuffer[i]; 
                        }
                        facemanager->endhertz = currenthertz;
                        stackpop(timerallocamt);
                        stackpop(vectorallocamt);
                        for(bit32 i = 0; i < facemanager->AV; i++)
                        {//Set the base postions for flip animation()
                            facemanager->anim[i].base = facemanager->vector[i].position;
                        }
                    }
                }
                facemanager->frametimer = 0; facemanager->responselength = settings.refreshrate*2;
                facemanager->timer = 0; facemanager->mouthtimer = 0;
                facemanager->staytimer = 0; facemanager->flags |= 0x00000004;
            }
            
            {//UpdateTimerProcedure()
                f32 incamt = 0.05f;
                {//RepeatingTimer()
                    if(facemanager->flags & 0x00000001) { facemanager->timer-=incamt; }
                    else { facemanager->timer+=incamt; }
                    if(facemanager->timer >= 1.0f)
                    {
                        facemanager->timer = 1.0f; facemanager->flags |= 1; }
                    else if(facemanager->timer <= 0.0f){ facemanager->timer = 0.0f; facemanager->flags ^= 0x00000001; }
                }
                {//StayTimer()
                    facemanager->staytimer+=incamt;
                    if(facemanager->staytimer >= 1.0f){facemanager->staytimer = 1.0f;}
                }
                {//MouthTimer()
                    if(facemanager->frametimer < facemanager->responselength)
                    {
                        if(facemanager->flags & 0x00000002){facemanager->mouthtimer-=0.05f;}
                        else{facemanager->mouthtimer+=0.05f;}
                        if(facemanager->mouthtimer >= 1.0f){ facemanager->mouthtimer = 1.0f; facemanager->flags |= 0x00000002; }
                        else if(facemanager->mouthtimer <= 0.0f){ facemanager->mouthtimer = 0.0f; facemanager->flags ^= 0x00000002; }
                    }
                    else
                    {
                        facemanager->mouthtimer-=0.05f; facemanager->flags = (facemanager->flags&0xFFFFFFFD);
                        if(facemanager->mouthtimer <= 0.0f){facemanager->mouthtimer = 0.0f;}
                    }
                }
            }
            
            if(facemanager->flags & 0x00000004)
            {if(facemanager->staytimer >= 1.0f) { facemanager->flags = facemanager->flags & 0xFFFFFFFB; setfaceonedefaultbasepositions(); } }
            {//FromRecentInputFrameWindow()
                if(facemanager->frametimer < facemanager->responselength)
                {//Talk()
                    bit32 reducedresponselen = facemanager->responselength/2;
                    
                    {//Animate mouth()
                        if(facemanager->frametimer < reducedresponselen)
                        {
                            {//Move upper corners
                                facemanager->anim[26].start.x = -.11; facemanager->anim[26].start.y = -.50; facemanager->anim[26].start.z = .07; 
                                facemanager->anim[30].start.x = .11; facemanager->anim[30].start.y = -.50; facemanager->anim[30].start.z = .07;
                                facemanager->anim[26].end.x = -.17; facemanager->anim[26].end.y = -.50; facemanager->anim[26].end.z = .07; 
                                facemanager->anim[30].end.x = .17; facemanager->anim[30].end.y = -.50; facemanager->anim[30].end.z = .07;
                                facemanager->anim[26].timerID = 2; facemanager->anim[30].timerID = 2;
                            }
                            {//Move corners
                                facemanager->anim[27].start.x = -.15; facemanager->anim[27].start.y = -.54; facemanager->anim[27].start.z = .07; 
                                facemanager->anim[31].start.x = .15; facemanager->anim[31].start.y = -.54; facemanager->anim[31].start.z = .07;
                                facemanager->anim[27].end.x = -.20; facemanager->anim[27].end.y = -.54; facemanager->anim[27].end.z = .07; 
                                facemanager->anim[31].end.x = .20; facemanager->anim[31].end.y = -.54; facemanager->anim[31].end.z = .07;
                                facemanager->anim[27].timerID = 2; facemanager->anim[31].timerID = 2;
                            }
                            {//Move bottom parts()
                                facemanager->anim[28].start.x = -.11; facemanager->anim[28].start.y = -.72; facemanager->anim[28].start.z = .07; 
                                facemanager->anim[32].start.x = .11; facemanager->anim[32].start.y = -.72; facemanager->anim[32].start.z = .07;
                                facemanager->anim[28].end.x = -.17; facemanager->anim[28].end.y = -.72; facemanager->anim[28].end.z = .07;
                                facemanager->anim[32].end.x = .17; facemanager->anim[32].end.y = -.72; facemanager->anim[32].end.z = .07;
                                facemanager->anim[28].timerID = 2; facemanager->anim[32].timerID = 2;
                            }
                        }
                        else
                        {
                            {//Move upper corners
                                facemanager->anim[26].start.x = -.11; facemanager->anim[26].start.y = -.50; facemanager->anim[26].start.z = .07; 
                                facemanager->anim[30].start.x = .11; facemanager->anim[30].start.y = -.50; facemanager->anim[30].start.z = .07;
                                facemanager->anim[26].end.x = -.11; facemanager->anim[26].end.y = -.50; facemanager->anim[26].end.z = .07; 
                                facemanager->anim[30].end.x = .11; facemanager->anim[30].end.y = -.50; facemanager->anim[30].end.z = .07;
                                facemanager->anim[26].timerID = 2; facemanager->anim[30].timerID = 2;
                            }
                            {//Move corners
                                facemanager->anim[27].start.x = -.15; facemanager->anim[27].start.y = -.54; facemanager->anim[27].start.z = .07; 
                                facemanager->anim[31].start.x = .15; facemanager->anim[31].start.y = -.54; facemanager->anim[31].start.z = .07;
                                facemanager->anim[27].end.x = -.15; facemanager->anim[27].end.y = -.54; facemanager->anim[27].end.z = .07; 
                                facemanager->anim[31].end.x = .15; facemanager->anim[31].end.y = -.54; facemanager->anim[31].end.z = .07;
                                facemanager->anim[27].timerID = 2; facemanager->anim[31].timerID = 2;
                            }
                            {//Move bottom parts()
                                facemanager->anim[28].start.x = -.11; facemanager->anim[28].start.y = -.72; facemanager->anim[28].start.z = .07; 
                                facemanager->anim[32].start.x = .11; facemanager->anim[32].start.y = -.72; facemanager->anim[32].start.z = .07;
                                facemanager->anim[28].end.x = -.11; facemanager->anim[28].end.y = -.72; facemanager->anim[28].end.z = .07;
                                facemanager->anim[32].end.x = .11; facemanager->anim[32].end.y = -.72; facemanager->anim[32].end.z = .07;
                                facemanager->anim[28].timerID = 2; facemanager->anim[32].timerID = 2;
                            }
                        }
                    }
                    
                    facemanager->frametimer++;
                }
            }
            
            {//AnimateFace()
                for(bit32 i = 0; i < facemanager->AV; i++)
                {
                    f32 timer = 0;
                    
                    if(facemanager->anim[i].timerID == 0) {timer = facemanager->timer;}
                    else if (facemanager->anim[i].timerID == 1){timer = facemanager->staytimer;}
                    else if(facemanager->anim[i].timerID == 2) {timer = facemanager->mouthtimer;}
                    vec3 end = lerp_vec3(facemanager->anim[i].start, facemanager->anim[i].end, facemanager->interpolateamount);
                    vec3 dest = end - facemanager->anim[i].base;
                    facemanager->vector[i].position = (timer * dest) + facemanager->anim[i].base;
                }
            }
        }
        
        { frame_end_proc:
            platform_render();
#if debug
            {//endprintf()
#if GNU
                //NOTE: If you want direct mapping between MSVC and GNU, you need both to call printf with unicode single byte arrays, can't do string on GNU!
                //So this messy workaround is implemented at the moment. But it solves the current problem, therefore it's good.
                bit32 CurrentLine = 100; bit32 Lineincrement = 20; bit32 XStart = 2;
#endif
                if(DEBUG_emotionalaxesdisplay)
                {
#if GNU
                    bit8 SA[7]; SA[0]=0x82;SA[1]=0x20;SA[2]=0x20;SA[3]=' ';SA[4]=0x83;SA[5]=0x20;SA[6]=0x20;
                    IntegerToAscii(&SA[1], DEBUG_emotionaldata.DEBUG_animID); IntegerToAscii(&SA[5], DEBUG_emotionaldata.DEBUG_animFID);
                    RenderLetterArray(SA, sizeof(SA), XStart, CurrentLine); CurrentLine+=Lineincrement;
                    bit8 SB[13];SB[0]=0x20;SB[1]=0x20;SB[2]=0x20;SB[3]=0x20; SB[4]=0x20;SB[5]=0x20; SB[6]=' '; SB[7]=0x20;SB[8]=0x20;SB[9]=0x20;SB[10]=0x20;
                    SB[11]=0x20;SB[12]=0x20;
                    if((facemanager->emotion.happiness&0x80000000)>0){SB[0]=0x88;SB[1]=0x85;SB[2]=0x81;SB[3]=0x84;} else{SB[0]=0x87;SB[1]=0x8A;SB[2]=0x84;SB[3]=0x20;}
                    IntegerToAscii(&SB[5], facemanager->emotion.happiness); 
                    if((facemanager->emotion.peacefulness&0x80000000)>0){SB[7]=0x88;SB[8]=0x85;SB[9]=0x81;SB[10]=0x84;}else{SB[7]=0x87;SB[8]=0x8A;SB[9]=0x84;SB[10]=0x20;}
                    IntegerToAscii(&SB[11], facemanager->emotion.peacefulness);
                    RenderLetterArray(SB, sizeof(SB), XStart, CurrentLine); CurrentLine+=Lineincrement;
#else
                    my_printf("ア%u イ%u\n", DEBUG_emotionaldata.DEBUG_animID, DEBUG_emotionaldata.DEBUG_animFID);
                    my_printf("%i %i\n", facemanager->emotion.happiness, facemanager->emotion.peacefulness);
#endif
                }
#if GNU
                bit32 keyXadvance = XStart;
#endif
                for(bit8 i = 0; i < sizeof(bit64); i++)
                {
                    bit8 key = ( input->input >> (i*8));
#if GNU
                    bit8 KS[2];KS[0]=0x20;KS[1]=0x20;
                    IntegerToAscii(&KS[0], key);
                    RenderLetterArray(KS, sizeof(KS), keyXadvance, CurrentLine); keyXadvance += 20;
#else
                    my_printf("%u ", key);
#endif
                }
                bit64 lastframedurationMS = platform_frametime();
#if GNU
                {//GNU only, display happiness and sadness  values (this should be the KEY_R display, but can't use enough buttons to do that atm).
                    CurrentLine+=Lineincrement; //Keyからyを進める。
                    bit8 EP[7]; EP[0]='H';EP[1]=' ';EP[2]='X';EP[3]='X';EP[4]=' ';EP[5]='Y';EP[6]='Y';
                    if(facemanager->emotion.happiness<0){EP[1]='-';}else{EP[1]='+';}
                    IntegerToAscii(&EP[2], facemanager->emotion.happiness);
                    if(facemanager->emotion.peacefulness<0){EP[4]='-';}else{EP[4]='+';}
                    IntegerToAscii(&EP[5], facemanager->emotion.peacefulness);
                    RenderLetterArray(EP, sizeof(EP), XStart, CurrentLine);
                    CurrentLine+=Lineincrement;
                }
                { //Print main thread's elapsed miliseconds.
                    bit8 FS[8];FS[0]=0x86;FS[1]=0x8B;FS[2]=0x81;FS[3]=0x89;FS[4]=0x80;FS[5]=0x20;FS[6]=0x20;FS[7]=0x20;
                    IntegerToAscii(&FS[5], lastframedurationMS);
                    RenderLetterArray(FS, sizeof(FS), XStart, CurrentLine);
                    CurrentLine+=Lineincrement;
                }
                {//Print audio thread's elapsed miliseconds
                    bit8 audiothreadDBG[12];audiothreadDBG[0]=0x86;audiothreadDBG[1]=0x8B;audiothreadDBG[2]=0x81;audiothreadDBG[3]=0x89;audiothreadDBG[4]=0x80;audiothreadDBG[5]=0x20;audiothreadDBG[6]=0x20;audiothreadDBG[7]=0x20;audiothreadDBG[8]=':';audiothreadDBG[9]='A';audiothreadDBG[10]='U';audiothreadDBG[11]='D';
                    IntegerToAscii(&audiothreadDBG[5], lastframeAudioThreadMS);
                    RenderLetterArray(audiothreadDBG, sizeof(audiothreadDBG), XStart, CurrentLine);
                    CurrentLine+=Lineincrement;
                }
#else
                my_printf("\nフレームが%u", lastframedurationMS);
                my_printf("\nAUDフレームが%u", lastframeAudioThreadMS);
                render_printf();
#endif
            }
#endif
            
            platform_bufferswap();
            platform_sleep();
            platform_updateframetimer();
        }
        
    }//NOTE: Loop end
    return;
}