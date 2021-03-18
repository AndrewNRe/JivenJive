//TODO: Perhaps have a read only sector of memory that's for the vertex data only. Later
#define faceone_position_zeroX 0.0f
#define faceone_position_zeroY 0.7f
#define faceone_position_zeroZ 0.0f //0
#define faceone_position_1X -0.3f
#define faceone_position_1Y 0.3f
#define faceone_position_1Z 0.1f //1
#define faceone_position_2X -0.3f
#define faceone_position_2Y 0.7f
#define faceone_position_2Z 0.0f //2
#define faceone_position_3X -0.6f
#define faceone_position_3Y 0.3f
#define faceone_position_3Z 0.0f//3
#define faceone_position_4X 0.3f
#define faceone_position_4Y 0.7f
#define faceone_position_4Z  0.0f//4
#define faceone_position_5X 0.3f
#define faceone_position_5Y 0.3f
#define faceone_position_5Z 0.1f//5
#define faceone_position_6X 0.6f
#define faceone_position_6Y 0.3f
#define faceone_position_6Z 0.0f//6
#define faceone_position_7X 0.1f
#define faceone_position_7Y 0.25f
#define faceone_position_7Z 0.05f//7
#define faceone_position_8X -0.1f
#define faceone_position_8Y 0.25f
#define faceone_position_8Z 0.05f//8
#define faceone_position_9X -0.6f
#define faceone_position_9Y 0.05f
#define faceone_position_9Z 0.0f //9
#define faceone_position_10X -0.55f
#define faceone_position_10Y 0.05f
#define faceone_position_10Z 0.0f//10
#define faceone_position_11X -0.3f
#define faceone_position_11Y 0.15f
#define faceone_position_11Z 0.0f //11(right upper eye point)
#define faceone_position_12X -0.15f
#define faceone_position_12Y 0.05f
#define faceone_position_12Z 0.0f//12
#define faceone_position_13X -0.09f
#define faceone_position_13Y -0.3f
#define faceone_position_13Z 0.1f//13
#define faceone_position_14X 0.6f
#define faceone_position_14Y 0.05f
#define faceone_position_14Z 0.0f//14
#define faceone_position_15X 0.55f
#define faceone_position_15Y 0.05f
#define faceone_position_15Z 0.0f//15
#define faceone_position_16X 0.3f
#define faceone_position_16Y 0.15f
#define faceone_position_16Z 0.0f //16(left upper eye point.)
#define faceone_position_17X 0.15f
#define faceone_position_17Y 0.05f
#define faceone_position_17Z 0.0f //17
#define faceone_position_18X 0.09f
#define faceone_position_18Y -0.3f
#define faceone_position_18Z 0.1f//18
#define faceone_position_19X -0.6f
#define faceone_position_19Y -0.2f
#define faceone_position_19Z 0.0f//19
#define faceone_position_20X -0.4f
#define faceone_position_20Y -0.02f
#define faceone_position_20Z 0.02f //20(lower right eye point)
#define faceone_position_21X -0.4f
#define faceone_position_21Y -0.24f
#define faceone_position_21Z 0.0f//21
#define faceone_position_22X 0.6f
#define faceone_position_22Y -0.2f
#define faceone_position_22Z 0.0f//22
#define faceone_position_23X 0.4f
#define faceone_position_23Y -0.02f
#define faceone_position_23Z 0.02f //23(lower left eye point)
#define faceone_position_24X 0.4f
#define faceone_position_24Y -0.24f
#define faceone_position_24Z 0.0f//24
#define faceone_position_25X -0.55f
#define faceone_position_25Y -0.6f
#define faceone_position_25Z 0.0f//25
#define faceone_position_26X -0.1f
#define faceone_position_26Y -0.54f
#define faceone_position_26Z 0.05f //26(right top corner of mouth)
#define faceone_position_27X -0.14f
#define faceone_position_27Y -0.6f
#define faceone_position_27Z 0.05f //27(right middle corner of mouth)
#define faceone_position_28X -0.1f
#define faceone_position_28Y -0.7f
#define faceone_position_28Z 0.05f //28(right lower corner of mouth)
#define faceone_position_29X 0.55f
#define faceone_position_29Y -0.6f
#define faceone_position_29Z 0.0f //29
#define faceone_position_30X 0.1f
#define faceone_position_30Y -0.54f
#define faceone_position_30Z 0.05f //30(left top corner of mouth)
#define faceone_position_31X 0.14f
#define faceone_position_31Y -0.6f
#define faceone_position_31Z 0.05f //31(left middle corner of mouth)
#define faceone_position_32X 0.1f
#define faceone_position_32Y -0.7f
#define faceone_position_32Z 0.05f //32(left lower corner of mouth)
#define faceone_position_33X -0.35f
#define faceone_position_33Y -0.9f
#define faceone_position_33Z 0.0f//33
#define faceone_position_34X -0.05f
#define faceone_position_34Y -0.94f
#define faceone_position_34Z 0.065f //34
#define faceone_position_35X 0.35f
#define faceone_position_35Y -0.9f
#define faceone_position_35Z 0.0f//35
#define faceone_position_36X 0.05f
#define faceone_position_36Y -0.94f
#define faceone_position_36Z 0.065f//36
#define maxemotionalvalue 20.0f
struct FaceAnimation
{ 
    bit32 timerID;
    vec3 base;
    vec3 start;
    vec3 end;
};
struct FaceEmotionBuffer
{
    s16 happiness;
    s16 peacefulness;
};
struct FaceManager
{
    f32 rotationamt; f32 timer; f32 staytimer; f32 mouthtimer;
    bool32 flags;
    bit32 frametimer; bit32 responselength;
    FaceEmotionBuffer emotion;
    bit32 starthertz; bit32 endhertz; f32 interpolateamount;
    FaceAnimation anim[37]; 
    bit32 AV = 0; //TODO: stop using AV! And AI because I know the fixed amounts now!
    bit32 AI = 0;
    VertexArenaVector vector[37];
    bit16 index[148];
    f32 unitcircleprogression; 
};

#if controllerplayable
bit32 DEBUG_changeemotionusingkeydelay = 0;
#endif

#if debug
struct DEBUG_GeneratedAudioBuffer
{
    bit32 AV = 0;
    bit32 storedframecount = 0;
    VertexArenaVector vector[735*60];//NOTE: if you want to debug with a different refresh rate. CHANGE THIS! devAudioSamplesPerFrame*devMonitorRefreshHz
};
//NOTE: LOTS of DEBUG globals.
DEBUG_GeneratedAudioBuffer* DEBUG_generatedaudiobufferR;
DEBUG_GeneratedAudioBuffer* DEBUG_generatedaudiobufferL;
bool32 DEBUG_showgeneratedaudiobuffer = 0; 
bit32 DEBUG_generateaudiobufferinputdelay = DEBUG_maxinputdelayinframes; 
bool32 DEBUG_emotionalaxesdisplay = 0; 
bit32 DEBUG_emotionalaxesinputdelay = DEBUG_maxinputdelayinframes; 
struct DEBUG_EmotionalAxes
{
    bit32 numberofanimations = 0;
    bit32 DEBUG_animID = 0; bit32 DEBUG_animFID = 0;
    VertexArena emotions;
};
DEBUG_EmotionalAxes DEBUG_emotionaldata;
#endif

FaceManager* facemanager; //NOTE: facemanager global.
#define setfaceonedefaultbasepositions()\
facemanager->anim[0].base.x = faceone_position_zeroX;  facemanager->anim[1].base.x = faceone_position_1X;      facemanager->anim[2].base.x = faceone_position_2X;\
facemanager->anim[0].base.y = faceone_position_zeroY;  facemanager->anim[1].base.y = faceone_position_1Y;      facemanager->anim[2].base.y = faceone_position_2Y;\
facemanager->anim[0].base.z = faceone_position_zeroZ;  facemanager->anim[1].base.z = faceone_position_1Z;      facemanager->anim[2].base.z = faceone_position_2Z;\
facemanager->anim[3].base.x = faceone_position_3X;     facemanager->anim[4].base.x = faceone_position_4X;      facemanager->anim[5].base.x = faceone_position_5X;\
facemanager->anim[3].base.y = faceone_position_3Y;     facemanager->anim[4].base.y = faceone_position_4Y;      facemanager->anim[5].base.y = faceone_position_5Y;\
facemanager->anim[3].base.z = faceone_position_3Z;     facemanager->anim[4].base.z = faceone_position_4Z;      facemanager->anim[5].base.z = faceone_position_5Z;\
facemanager->anim[6].base.x = faceone_position_6X;     facemanager->anim[7].base.x = faceone_position_7X;      facemanager->anim[8].base.x = faceone_position_8X;\
facemanager->anim[6].base.y = faceone_position_6Y;     facemanager->anim[7].base.y = faceone_position_7Y;      facemanager->anim[8].base.y = faceone_position_8Y;\
facemanager->anim[6].base.z = faceone_position_6Z;     facemanager->anim[7].base.z = faceone_position_7Z;      facemanager->anim[8].base.z = faceone_position_8Z;\
facemanager->anim[9].base.x = faceone_position_9X;     facemanager->anim[10].base.x = faceone_position_10X;     facemanager->anim[11].base.x = faceone_position_11X;\
facemanager->anim[9].base.y = faceone_position_9Y;     facemanager->anim[10].base.y = faceone_position_10Y;     facemanager->anim[11].base.y = faceone_position_11Y;\
facemanager->anim[9].base.z = faceone_position_9Z;     facemanager->anim[10].base.z = faceone_position_10Z;     facemanager->anim[11].base.z = faceone_position_11Z;\
facemanager->anim[12].base.x = faceone_position_12X;  facemanager->anim[13].base.x = faceone_position_13X;   facemanager->anim[14].base.x = faceone_position_14X;\
facemanager->anim[12].base.y = faceone_position_12Y;  facemanager->anim[13].base.y = faceone_position_13Y;   facemanager->anim[14].base.y = faceone_position_14Y;\
facemanager->anim[12].base.z = faceone_position_12Z;  facemanager->anim[13].base.z = faceone_position_13Z;   facemanager->anim[14].base.z = faceone_position_14Z;\
facemanager->anim[15].base.x = faceone_position_15X;  facemanager->anim[16].base.x = faceone_position_16X;    facemanager->anim[17].base.x = faceone_position_17X;\
facemanager->anim[15].base.y = faceone_position_15Y;  facemanager->anim[16].base.y = faceone_position_16Y;    facemanager->anim[17].base.y = faceone_position_17Y;\
facemanager->anim[15].base.z = faceone_position_15Z;  facemanager->anim[16].base.z = faceone_position_16Z;    facemanager->anim[17].base.z = faceone_position_17Z;\
facemanager->anim[18].base.x = faceone_position_18X;  facemanager->anim[19].base.x = faceone_position_19X;    facemanager->anim[20].base.x = faceone_position_20X;\
facemanager->anim[18].base.y = faceone_position_18Y;  facemanager->anim[19].base.y = faceone_position_19Y;    facemanager->anim[20].base.y = faceone_position_20Y;\
facemanager->anim[18].base.z = faceone_position_18Z;  facemanager->anim[19].base.z = faceone_position_19Z;    facemanager->anim[20].base.z = faceone_position_20Z;\
facemanager->anim[21].base.x = faceone_position_21X;facemanager->anim[22].base.x = faceone_position_22X;  facemanager->anim[23].base.x = faceone_position_23X;\
facemanager->anim[21].base.y = faceone_position_21Y;facemanager->anim[22].base.y = faceone_position_22Y;  facemanager->anim[23].base.y = faceone_position_23Y;\
facemanager->anim[21].base.z = faceone_position_21Z;facemanager->anim[22].base.z = faceone_position_22Z;  facemanager->anim[23].base.z = faceone_position_23Z;\
facemanager->anim[24].base.x = faceone_position_24X;facemanager->anim[25].base.x = faceone_position_25X;  facemanager->anim[26].base.x = faceone_position_26X;\
facemanager->anim[24].base.y = faceone_position_24Y;facemanager->anim[25].base.y = faceone_position_25Y;  facemanager->anim[26].base.y = faceone_position_26Y;\
facemanager->anim[24].base.z = faceone_position_24Z;facemanager->anim[25].base.z = faceone_position_25Z;  facemanager->anim[26].base.z = faceone_position_26Z;\
facemanager->anim[27].base.x = faceone_position_27X;facemanager->anim[28].base.x = faceone_position_28X;  facemanager->anim[29].base.x = faceone_position_29X;\
facemanager->anim[27].base.y = faceone_position_27Y;facemanager->anim[28].base.y = faceone_position_28Y;  facemanager->anim[29].base.y = faceone_position_29Y;\
facemanager->anim[27].base.z = faceone_position_27Z;facemanager->anim[28].base.z = faceone_position_28Z;  facemanager->anim[29].base.z = faceone_position_29Z;\
facemanager->anim[30].base.x = faceone_position_30X;  facemanager->anim[31].base.x = faceone_position_31X;  facemanager->anim[32].base.x = faceone_position_32X;\
facemanager->anim[30].base.y = faceone_position_30Y;  facemanager->anim[31].base.y = faceone_position_31Y;  facemanager->anim[32].base.y = faceone_position_32Y;\
facemanager->anim[30].base.z = faceone_position_30Z;  facemanager->anim[31].base.z = faceone_position_31Z;  facemanager->anim[32].base.z = faceone_position_32Z;\
facemanager->anim[33].base.x = faceone_position_33X;facemanager->anim[34].base.x = faceone_position_34X;  facemanager->anim[35].base.x = faceone_position_35X;\
facemanager->anim[33].base.y = faceone_position_33Y;facemanager->anim[34].base.y = faceone_position_34Y;  facemanager->anim[35].base.y = faceone_position_35Y;\
facemanager->anim[33].base.z = faceone_position_33Z;facemanager->anim[34].base.z = faceone_position_34Z;  facemanager->anim[35].base.z = faceone_position_35Z;\
facemanager->anim[36].base.x = faceone_position_36X;\
facemanager->anim[36].base.y = faceone_position_36Y;\
facemanager->anim[36].base.z = faceone_position_36Z;
#define setfaceonedefaultstartpositions()\
facemanager->anim[0].start.x = faceone_position_zeroX;  facemanager->anim[1].start.x = faceone_position_1X;      facemanager->anim[2].start.x = faceone_position_2X;\
facemanager->anim[0].start.y = faceone_position_zeroY;  facemanager->anim[1].start.y = faceone_position_1Y;      facemanager->anim[2].start.y = faceone_position_2Y;\
facemanager->anim[0].start.z = faceone_position_zeroZ;  facemanager->anim[1].start.z = faceone_position_1Z;      facemanager->anim[2].start.z = faceone_position_2Z;\
facemanager->anim[3].start.x = faceone_position_3X;     facemanager->anim[4].start.x = faceone_position_4X;      facemanager->anim[5].start.x = faceone_position_5X;\
facemanager->anim[3].start.y = faceone_position_3Y;     facemanager->anim[4].start.y = faceone_position_4Y;      facemanager->anim[5].start.y = faceone_position_5Y;\
facemanager->anim[3].start.z = faceone_position_3Z;     facemanager->anim[4].start.z = faceone_position_4Z;      facemanager->anim[5].start.z = faceone_position_5Z;\
facemanager->anim[6].start.x = faceone_position_6X;     facemanager->anim[7].start.x = faceone_position_7X;      facemanager->anim[8].start.x = faceone_position_8X;\
facemanager->anim[6].start.y = faceone_position_6Y;     facemanager->anim[7].start.y = faceone_position_7Y;      facemanager->anim[8].start.y = faceone_position_8Y;\
facemanager->anim[6].start.z = faceone_position_6Z;     facemanager->anim[7].start.z = faceone_position_7Z;      facemanager->anim[8].start.z = faceone_position_8Z;\
facemanager->anim[9].start.x = faceone_position_9X;     facemanager->anim[10].start.x = faceone_position_10X;     facemanager->anim[11].start.x = faceone_position_11X;\
facemanager->anim[9].start.y = faceone_position_9Y;     facemanager->anim[10].start.y = faceone_position_10Y;     facemanager->anim[11].start.y = faceone_position_11Y;\
facemanager->anim[9].start.z = faceone_position_9Z;     facemanager->anim[10].start.z = faceone_position_10Z;     facemanager->anim[11].start.z = faceone_position_11Z;\
facemanager->anim[12].start.x = faceone_position_12X;  facemanager->anim[13].start.x = faceone_position_13X;   facemanager->anim[14].start.x = faceone_position_14X;\
facemanager->anim[12].start.y = faceone_position_12Y;  facemanager->anim[13].start.y = faceone_position_13Y;   facemanager->anim[14].start.y = faceone_position_14Y;\
facemanager->anim[12].start.z = faceone_position_12Z;  facemanager->anim[13].start.z = faceone_position_13Z;   facemanager->anim[14].start.z = faceone_position_14Z;\
facemanager->anim[15].start.x = faceone_position_15X;  facemanager->anim[16].start.x = faceone_position_16X;    facemanager->anim[17].start.x = faceone_position_17X;\
facemanager->anim[15].start.y = faceone_position_15Y;  facemanager->anim[16].start.y = faceone_position_16Y;    facemanager->anim[17].start.y = faceone_position_17Y;\
facemanager->anim[15].start.z = faceone_position_15Z;  facemanager->anim[16].start.z = faceone_position_16Z;    facemanager->anim[17].start.z = faceone_position_17Z;\
facemanager->anim[18].start.x = faceone_position_18X;  facemanager->anim[19].start.x = faceone_position_19X;    facemanager->anim[20].start.x = faceone_position_20X;\
facemanager->anim[18].start.y = faceone_position_18Y;  facemanager->anim[19].start.y = faceone_position_19Y;    facemanager->anim[20].start.y = faceone_position_20Y;\
facemanager->anim[18].start.z = faceone_position_18Z;  facemanager->anim[19].start.z = faceone_position_19Z;    facemanager->anim[20].start.z = faceone_position_20Z;\
facemanager->anim[21].start.x = faceone_position_21X;facemanager->anim[22].start.x = faceone_position_22X;  facemanager->anim[23].start.x = faceone_position_23X;\
facemanager->anim[21].start.y = faceone_position_21Y;facemanager->anim[22].start.y = faceone_position_22Y;  facemanager->anim[23].start.y = faceone_position_23Y;\
facemanager->anim[21].start.z = faceone_position_21Z;facemanager->anim[22].start.z = faceone_position_22Z;  facemanager->anim[23].start.z = faceone_position_23Z;\
facemanager->anim[24].start.x = faceone_position_24X;facemanager->anim[25].start.x = faceone_position_25X;  facemanager->anim[26].start.x = faceone_position_26X;\
facemanager->anim[24].start.y = faceone_position_24Y;facemanager->anim[25].start.y = faceone_position_25Y;  facemanager->anim[26].start.y = faceone_position_26Y;\
facemanager->anim[24].start.z = faceone_position_24Z;facemanager->anim[25].start.z = faceone_position_25Z;  facemanager->anim[26].start.z = faceone_position_26Z;\
facemanager->anim[27].start.x = faceone_position_27X;facemanager->anim[28].start.x = faceone_position_28X;  facemanager->anim[29].start.x = faceone_position_29X;\
facemanager->anim[27].start.y = faceone_position_27Y;facemanager->anim[28].start.y = faceone_position_28Y;  facemanager->anim[29].start.y = faceone_position_29Y;\
facemanager->anim[27].start.z = faceone_position_27Z;facemanager->anim[28].start.z = faceone_position_28Z;  facemanager->anim[29].start.z = faceone_position_29Z;\
facemanager->anim[30].start.x = faceone_position_30X;  facemanager->anim[31].start.x = faceone_position_31X;  facemanager->anim[32].start.x = faceone_position_32X;\
facemanager->anim[30].start.y = faceone_position_30Y;  facemanager->anim[31].start.y = faceone_position_31Y;  facemanager->anim[32].start.y = faceone_position_32Y;\
facemanager->anim[30].start.z = faceone_position_30Z;  facemanager->anim[31].start.z = faceone_position_31Z;  facemanager->anim[32].start.z = faceone_position_32Z;\
facemanager->anim[33].start.x = faceone_position_33X;facemanager->anim[34].start.x = faceone_position_34X;  facemanager->anim[35].start.x = faceone_position_35X;\
facemanager->anim[33].start.y = faceone_position_33Y;facemanager->anim[34].start.y = faceone_position_34Y;  facemanager->anim[35].start.y = faceone_position_35Y;\
facemanager->anim[33].start.z = faceone_position_33Z;facemanager->anim[34].start.z = faceone_position_34Z;  facemanager->anim[35].start.z = faceone_position_35Z;\
facemanager->anim[36].start.x = faceone_position_36X;\
facemanager->anim[36].start.y = faceone_position_36Y;\
facemanager->anim[36].start.z = faceone_position_36Z;
#define setfaceoneendposequaltocurrentendpos()\
facemanager->anim[0].end  = facemanager->anim[0].start; facemanager->anim[1].end  = facemanager->anim[1].start;  facemanager->anim[2].end = facemanager->anim[2].start;\
facemanager->anim[3].end  = facemanager->anim[3].start; facemanager->anim[4].end  = facemanager->anim[4].start;  facemanager->anim[5].end = facemanager->anim[5].start;\
facemanager->anim[6].end  = facemanager->anim[6].start; facemanager->anim[7].end  = facemanager->anim[7].start;  facemanager->anim[8].end = facemanager->anim[8].start;\
facemanager->anim[9].end  = facemanager->anim[9].start; facemanager->anim[10].end = facemanager->anim[10].start; facemanager->anim[11].end = facemanager->anim[11].start;\
facemanager->anim[12].end = facemanager->anim[12].start;facemanager->anim[13].end = facemanager->anim[13].start; facemanager->anim[14].end = facemanager->anim[14].start;\
facemanager->anim[15].end = facemanager->anim[15].start;facemanager->anim[16].end = facemanager->anim[16].start; facemanager->anim[17].end = facemanager->anim[17].start;\
facemanager->anim[18].end = facemanager->anim[18].start;facemanager->anim[19].end = facemanager->anim[19].start; facemanager->anim[20].end = facemanager->anim[20].start;\
facemanager->anim[21].end = facemanager->anim[21].start;facemanager->anim[22].end = facemanager->anim[22].start; facemanager->anim[23].end = facemanager->anim[23].start;\
facemanager->anim[24].end = facemanager->anim[24].start;facemanager->anim[25].end = facemanager->anim[25].start; facemanager->anim[26].end = facemanager->anim[26].start;\
facemanager->anim[27].end = facemanager->anim[27].start;facemanager->anim[28].end = facemanager->anim[28].start; facemanager->anim[29].end = facemanager->anim[29].start;\
facemanager->anim[30].end = facemanager->anim[30].start;facemanager->anim[31].end = facemanager->anim[31].start; facemanager->anim[32].end = facemanager->anim[32].start;\
facemanager->anim[33].end = facemanager->anim[33].start;facemanager->anim[34].end = facemanager->anim[34].start; facemanager->anim[35].end = facemanager->anim[35].start;\
facemanager->anim[36].end = facemanager->anim[36].start;

/*#define setfaceoneanimdata()\
facemanager->anim[0].start = faceone_position_zero;\
facemanager->anim[1].start = faceone_position_1;\
facemanager->anim[2].start = faceone_position_2;\
facemanager->anim[3].start = faceone_position_3;\
facemanager->anim[4].start = faceone_position_4;\
facemanager->anim[5].start = faceone_position_5;\
facemanager->anim[6].start = faceone_position_6;\
facemanager->anim[7].start = faceone_position_7;\
facemanager->anim[8].start = faceone_position_8;\
facemanager->anim[9].start = faceone_position_9;\
facemanager->anim[10].start = faceone_position_10;\
facemanager->anim[11].start = faceone_position_11;\
facemanager->anim[12].start = faceone_position_12;\
facemanager->anim[13].start = faceone_position_13;\
facemanager->anim[14].start = faceone_position_14;\
facemanager->anim[15].start = faceone_position_15;\
facemanager->anim[16].start = faceone_position_16;\
facemanager->anim[17].start = faceone_position_17;\
facemanager->anim[18].start = faceone_position_18;\
facemanager->anim[19].start = faceone_position_19;\
facemanager->anim[20].start = faceone_position_20;\
facemanager->anim[21].start = faceone_position_21;\
facemanager->anim[22].start = faceone_position_22;\
facemanager->anim[23].start = faceone_position_23;\
facemanager->anim[24].start = faceone_position_24;\
facemanager->anim[25].start = faceone_position_25;\
facemanager->anim[26].start = faceone_position_26;\
facemanager->anim[27].start = faceone_position_27;\
facemanager->anim[28].start = faceone_position_28;\
facemanager->anim[29].start = faceone_position_29;\
facemanager->anim[30].start = faceone_position_30;\
facemanager->anim[31].start = faceone_position_31;\
facemanager->anim[32].start = faceone_position_32;\
facemanager->anim[33].start = faceone_position_33;\
facemanager->anim[34].start = faceone_position_34;\
facemanager->anim[35].start = faceone_position_35;\
facemanager->anim[36].start = faceone_position_36;\
setfaceonedefaultendpositions();\
setfaceoneendposequaltocurrentendpos();
*/
#define setfacedefaultcolor()\
facemanager->vector[0].color.x = 0.15f; facemanager->vector[0].color.y = 1.0f; facemanager->vector[0].color.z = 1.0f; facemanager->vector[0].color.w =1.0f;\
facemanager->vector[1].color.x = 0.15f; facemanager->vector[1].color.y = 1.0f; facemanager->vector[1].color.z = 1.0f; facemanager->vector[1].color.w =1.0f;\
facemanager->vector[2].color.x = 0.15f; facemanager->vector[2].color.y = 1.0f; facemanager->vector[2].color.z = 1.0f; facemanager->vector[2].color.w =1.0f;\
facemanager->vector[3].color.x = 0.15f; facemanager->vector[3].color.y = 1.0f; facemanager->vector[3].color.z = 1.0f; facemanager->vector[3].color.w =1.0f;\
facemanager->vector[4].color.x = 0.15f; facemanager->vector[4].color.y = 1.0f; facemanager->vector[4].color.z = 1.0f; facemanager->vector[4].color.w =1.0f;\
facemanager->vector[5].color.x = 0.15f; facemanager->vector[5].color.y = 1.0f; facemanager->vector[5].color.z = 1.0f; facemanager->vector[5].color.w =1.0f;\
facemanager->vector[6].color.x = 0.15f; facemanager->vector[6].color.y = 1.0f; facemanager->vector[6].color.z = 1.0f; facemanager->vector[6].color.w =1.0f;\
facemanager->vector[7].color.x = 0.15f; facemanager->vector[7].color.y = 1.0f; facemanager->vector[7].color.z = 1.0f; facemanager->vector[7].color.w =1.0f;\
facemanager->vector[8].color.x = 0.15f; facemanager->vector[8].color.y = 1.0f; facemanager->vector[8].color.z = 1.0f; facemanager->vector[8].color.w =1.0f;\
facemanager->vector[9].color.x = 0.0f; facemanager->vector[9].color.y = 0.9f; facemanager->vector[9].color.z = 1.0f; facemanager->vector[9].color.w =1.0f;\
facemanager->vector[10].color.x = 0.0f; facemanager->vector[10].color.y = 0.9f; facemanager->vector[10].color.z = 1.0f; facemanager->vector[10].color.w =1.0f;\
facemanager->vector[11].color.x = 0.0f; facemanager->vector[11].color.y = 0.9f; facemanager->vector[11].color.z = 1.0f; facemanager->vector[11].color.w =1.0f;\
facemanager->vector[12].color.x = 0.0f; facemanager->vector[12].color.y = 0.9f; facemanager->vector[12].color.z = 1.0f; facemanager->vector[12].color.w =1.0f;\
facemanager->vector[13].color.x = 0.0f; facemanager->vector[13].color.y = 0.9f; facemanager->vector[13].color.z = 1.0f; facemanager->vector[13].color.w =1.0f;\
facemanager->vector[14].color.x = 0.0f; facemanager->vector[14].color.y = 0.9f; facemanager->vector[14].color.z = 1.0f; facemanager->vector[14].color.w =1.0f;\
facemanager->vector[15].color.x = 0.0f; facemanager->vector[15].color.y = 0.9f; facemanager->vector[15].color.z = 1.0f; facemanager->vector[15].color.w =1.0f;\
facemanager->vector[16].color.x = 0.0f; facemanager->vector[16].color.y = 0.9f; facemanager->vector[16].color.z = 1.0f; facemanager->vector[16].color.w =1.0f;\
facemanager->vector[17].color.x = 0.0f; facemanager->vector[17].color.y = 0.9f; facemanager->vector[17].color.z = 1.0f; facemanager->vector[17].color.w =1.0f;\
facemanager->vector[18].color.x = 0.0f; facemanager->vector[18].color.y = 0.9f; facemanager->vector[18].color.z = 1.0f; facemanager->vector[18].color.w =1.0f;\
facemanager->vector[19].color.x = 0.0f; facemanager->vector[19].color.y = 0.8f; facemanager->vector[19].color.z = 0.9f; facemanager->vector[19].color.w =1.0f;\
facemanager->vector[20].color.x = 0.0f; facemanager->vector[20].color.y = 0.8f; facemanager->vector[20].color.z = 0.9f; facemanager->vector[20].color.w =1.0f;\
facemanager->vector[21].color.x = 0.0f; facemanager->vector[21].color.y = 0.8f; facemanager->vector[21].color.z = 0.9f; facemanager->vector[21].color.w =1.0f;\
facemanager->vector[22].color.x = 0.0f; facemanager->vector[22].color.y = 0.8f; facemanager->vector[22].color.z = 0.9f; facemanager->vector[22].color.w =1.0f;\
facemanager->vector[23].color.x = 0.0f; facemanager->vector[23].color.y = 0.8f; facemanager->vector[23].color.z = 0.9f; facemanager->vector[23].color.w =1.0f;\
facemanager->vector[24].color.x = 0.0f; facemanager->vector[24].color.y = 0.8f; facemanager->vector[24].color.z = 0.9f; facemanager->vector[24].color.w =1.0f;\
facemanager->vector[25].color.x = 0.0f; facemanager->vector[25].color.y = 0.7f; facemanager->vector[25].color.z = 0.8f; facemanager->vector[25].color.w =1.0f;\
facemanager->vector[26].color.x = 0.0f; facemanager->vector[26].color.y = 0.7f; facemanager->vector[26].color.z = 0.8f; facemanager->vector[26].color.w =1.0f;\
facemanager->vector[27].color.x = 0.0f; facemanager->vector[27].color.y = 0.7f; facemanager->vector[27].color.z = 0.8f; facemanager->vector[27].color.w =1.0f;\
facemanager->vector[28].color.x = 0.0f; facemanager->vector[28].color.y = 0.7f; facemanager->vector[28].color.z = 0.8f; facemanager->vector[28].color.w =1.0f;\
facemanager->vector[29].color.x = 0.0f; facemanager->vector[29].color.y = 0.7f; facemanager->vector[29].color.z = 0.8f; facemanager->vector[29].color.w =1.0f;\
facemanager->vector[30].color.x = 0.0f; facemanager->vector[30].color.y = 0.7f; facemanager->vector[30].color.z = 0.8f; facemanager->vector[30].color.w =1.0f;\
facemanager->vector[31].color.x = 0.0f; facemanager->vector[31].color.y = 0.7f; facemanager->vector[31].color.z = 0.8f; facemanager->vector[31].color.w =1.0f;\
facemanager->vector[32].color.x = 0.0f; facemanager->vector[32].color.y = 0.7f; facemanager->vector[32].color.z = 0.8f; facemanager->vector[32].color.w =1.0f;\
facemanager->vector[33].color.x = 0.0f; facemanager->vector[33].color.y = 0.6f; facemanager->vector[33].color.z = 0.7f; facemanager->vector[33].color.w =1.0f;\
facemanager->vector[34].color.x = 0.0f; facemanager->vector[34].color.y = 0.6f; facemanager->vector[34].color.z = 0.7f; facemanager->vector[34].color.w =1.0f;\
facemanager->vector[35].color.x = 0.0f; facemanager->vector[35].color.y = 0.6f; facemanager->vector[35].color.z = 0.7f; facemanager->vector[35].color.w =1.0f;\
facemanager->vector[36].color.x = 0.0f; facemanager->vector[36].color.y = 0.6f; facemanager->vector[36].color.z = 0.7f; facemanager->vector[36].color.w =1.0f;

void setfaceonedefaultanimdata(vec3* data)
{
    data[0].x = faceone_position_zeroX;
    data[0].y = faceone_position_zeroY;
    data[0].z = faceone_position_zeroZ; //0
    data[1].x = faceone_position_1X;
    data[1].y = faceone_position_1Y;
    data[1].z = faceone_position_1Z; //1
    data[2].x = faceone_position_2X;
    data[2].y = faceone_position_2Y;
    data[2].z = faceone_position_2Z;//2
    data[3].x = faceone_position_3X;
    data[3].y = faceone_position_3Y;
    data[3].z = faceone_position_3Z;//3
    data[4].x = faceone_position_4X;
    data[4].y = faceone_position_4Y;
    data[4].z = faceone_position_4Z;//4
    data[5].x = faceone_position_5X;
    data[5].y = faceone_position_5Y;
    data[5].z = faceone_position_5Z;//5
    data[6].x = faceone_position_6X;
    data[6].y = faceone_position_6Y;
    data[6].z = faceone_position_6Z;//6
    data[7].x = faceone_position_7X;
    data[7].y = faceone_position_7Y;
    data[7].z = faceone_position_7Z;//7
    data[8].x = faceone_position_8X;
    data[8].y = faceone_position_8Y;
    data[8].z = faceone_position_8Z;//8
    data[9].x = faceone_position_9X;
    data[9].y = faceone_position_9Y;
    data[9].z = faceone_position_9Z;//9
    data[10].x = faceone_position_10X;
    data[10].y = faceone_position_10Y;
    data[10].z = faceone_position_10Z;//10
    data[11].x = faceone_position_11X;
    data[11].y = faceone_position_11Y;
    data[11].z = faceone_position_11Z;//11
    data[12].x = faceone_position_12X;
    data[12].y = faceone_position_12Y;
    data[12].z = faceone_position_12Z;//12
    data[13].x = faceone_position_13X;
    data[13].y = faceone_position_13Y;
    data[13].z = faceone_position_13Z;//13
    data[14].x = faceone_position_14X;
    data[14].y = faceone_position_14Y;
    data[14].z = faceone_position_14Z;//14
    data[15].x = faceone_position_15X;
    data[15].y = faceone_position_15Y;
    data[15].z = faceone_position_15Z;//15
    data[16].x = faceone_position_16X;
    data[16].y = faceone_position_16Y;
    data[16].z = faceone_position_16Z;//16
    data[17].x = faceone_position_17X;
    data[17].y = faceone_position_17Y;
    data[17].z = faceone_position_17Z;//17
    data[18].x = faceone_position_18X;
    data[18].y = faceone_position_18Y;
    data[18].z = faceone_position_18Z;//18
    data[19].x = faceone_position_19X;
    data[19].y = faceone_position_19Y;
    data[19].z = faceone_position_19Z;//19
    data[20].x = faceone_position_20X;
    data[20].y = faceone_position_20Y;
    data[20].z = faceone_position_20Z;//20
    data[21].x = faceone_position_21X;
    data[21].y = faceone_position_21Y;
    data[21].z = faceone_position_21Z;//21
    data[22].x = faceone_position_22X;
    data[22].y = faceone_position_22Y;
    data[22].z = faceone_position_22Z;//22
    data[23].x = faceone_position_23X;
    data[23].y = faceone_position_23Y;
    data[23].z = faceone_position_23Z;//23
    data[24].x = faceone_position_24X;
    data[24].y = faceone_position_24Y;
    data[24].z = faceone_position_24Z;//24
    data[25].x = faceone_position_25X;
    data[25].y = faceone_position_25Y;
    data[25].z = faceone_position_25Z;//25
    data[26].x = faceone_position_26X;
    data[26].y = faceone_position_26Y;
    data[26].z = faceone_position_26Z;//26
    data[27].x = faceone_position_27X;
    data[27].y = faceone_position_27Y;
    data[27].z = faceone_position_27Z;//27
    data[28].x = faceone_position_28X;
    data[28].y = faceone_position_28Y;
    data[28].z = faceone_position_28Z;//28
    data[29].x = faceone_position_29X;
    data[29].y = faceone_position_29Y;
    data[29].z = faceone_position_29Z;//29
    data[30].x = faceone_position_30X;
    data[30].y = faceone_position_30Y;
    data[30].z = faceone_position_30Z;//30
    data[31].x = faceone_position_31X;
    data[31].y = faceone_position_31Y;
    data[31].z = faceone_position_31Z;//31
    data[32].x = faceone_position_32X;
    data[32].y = faceone_position_32Y;
    data[32].z = faceone_position_32Z;//32
    data[33].x = faceone_position_33X;
    data[33].y = faceone_position_33Y;
    data[33].z = faceone_position_33Z;//33
    data[34].x = faceone_position_34X;
    data[34].y = faceone_position_34Y;
    data[34].z = faceone_position_34Z;//34
    data[35].x = faceone_position_35X;
    data[35].y = faceone_position_35Y;
    data[35].z = faceone_position_35Z;//35
    data[36].x = faceone_position_36X;
    data[36].y = faceone_position_36Y;
    data[36].z = faceone_position_36Z;//36
}

__forceinline void loadfaceone()
{
    facemanager = (FaceManager*)recordpush(sizeof(FaceManager)); //NOTE: when doing another face in the future. Do this record push in the "main" of whatever platform you're on.
    facemanager->rotationamt = 0; facemanager->timer = 0; facemanager->staytimer = 0; facemanager->mouthtimer = 0;
    facemanager->flags = 0;
    facemanager->frametimer = 0; facemanager->responselength = 0;
    facemanager->emotion.happiness = 1; facemanager->emotion.peacefulness = 1;
    facemanager->starthertz = 0; facemanager->endhertz = 0; facemanager->interpolateamount = 0;
    facemanager->unitcircleprogression = 0.0f;
    for(bit32 i = 0; i < 37; i++) { facemanager->anim[i].timerID = 0; }
#if debug
    DEBUG_generatedaudiobufferR = (DEBUG_GeneratedAudioBuffer*)recordpush(sizeof(DEBUG_GeneratedAudioBuffer));
    DEBUG_generatedaudiobufferL = (DEBUG_GeneratedAudioBuffer*)recordpush(sizeof(DEBUG_GeneratedAudioBuffer));
    DEBUG_generatedaudiobufferR->AV = 0; DEBUG_generatedaudiobufferR->storedframecount = 0;
    DEBUG_generatedaudiobufferL->AV = 0; DEBUG_generatedaudiobufferL->storedframecount = 0;
    DEBUG_emotionaldata.numberofanimations = 0; DEBUG_emotionaldata.DEBUG_animID = 0; DEBUG_emotionaldata.DEBUG_animFID = 0;
    DEBUG_showgeneratedaudiobuffer = 0;  DEBUG_generateaudiobufferinputdelay = DEBUG_maxinputdelayinframes; 
    DEBUG_emotionalaxesdisplay = 0;  DEBUG_emotionalaxesinputdelay = DEBUG_maxinputdelayinframes; 
#endif
    {//SetupFirstFace()
        facemanager->AV = 0;
        facemanager->vector[facemanager->AV].position.x = faceone_position_zeroX; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_zeroY; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_zeroZ; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_1X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_1Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_1Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_2X;  
        facemanager->vector[facemanager->AV].position.y = faceone_position_2Y;  
        facemanager->vector[facemanager->AV].position.z = faceone_position_2Z; facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_3X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_3Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_3Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_4X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_4Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_4Z; facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_5X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_5Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_5Z; facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_6X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_6Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_6Z; facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_7X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_7Y;  
        facemanager->vector[facemanager->AV].position.z = faceone_position_7Z; facemanager->AV++;  
        facemanager->vector[facemanager->AV].position.x = faceone_position_8X;  
        facemanager->vector[facemanager->AV].position.y = faceone_position_8Y;  
        facemanager->vector[facemanager->AV].position.z = faceone_position_8Z; facemanager->AV++; 
        //Right upper eye (with last point being a connection to bottom of nose that forehead established)
        facemanager->vector[facemanager->AV].position.x = faceone_position_9X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_9Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_9Z;  facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_10X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_10Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_10Z;  facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_11X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_11Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_11Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_12X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_12Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_12Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_13X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_13Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_13Z; facemanager->AV++;
        //Left upper eye (same as above comment)
        facemanager->vector[facemanager->AV].position.x = faceone_position_14X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_14Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_14Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_15X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_15Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_15Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_16X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_16Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_16Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_17X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_17Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_17Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_18X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_18Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_18Z; facemanager->AV++;
        //Left lower eye
        facemanager->vector[facemanager->AV].position.x = faceone_position_19X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_19Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_19Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_20X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_20Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_20Z;  facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_21X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_21Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_21Z;  facemanager->AV++;
        //Right lower eye
        facemanager->vector[facemanager->AV].position.x = faceone_position_22X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_22Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_22Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_23X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_23Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_23Z;  facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_24X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_24Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_24Z;  facemanager->AV++;
        //Left check / mouth
        facemanager->vector[facemanager->AV].position.x = faceone_position_25X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_25Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_25Z;  facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_26X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_26Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_26Z;  facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_27X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_27Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_27Z;  facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_28X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_28Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_28Z;  facemanager->AV++;
        //Right cheek / mouth
        facemanager->vector[facemanager->AV].position.x = faceone_position_29X;
        facemanager->vector[facemanager->AV].position.y = faceone_position_29Y;
        facemanager->vector[facemanager->AV].position.z = faceone_position_29Z; facemanager->AV++;
        facemanager->vector[facemanager->AV].position.x = faceone_position_30X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_30Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_30Z;  facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_31X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_31Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_31Z;  facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_32X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_32Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_32Z;  facemanager->AV++; 
        //Left bottom of 
        facemanager->vector[facemanager->AV].position.x = faceone_position_33X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_33Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_33Z;  facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_34X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_34Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_34Z; facemanager->AV++; 
        //Right bottom of face
        facemanager->vector[facemanager->AV].position.x = faceone_position_35X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_35Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_35Z; facemanager->AV++; 
        facemanager->vector[facemanager->AV].position.x = faceone_position_36X; 
        facemanager->vector[facemanager->AV].position.y = faceone_position_36Y; 
        facemanager->vector[facemanager->AV].position.z = faceone_position_36Z; facemanager->AV++; 
        setfacedefaultcolor();
        setfaceonedefaultbasepositions();
        setfaceonedefaultstartpositions();
        setfaceoneendposequaltocurrentendpos();
    }
    {//IndexSetupForFirstFace()
        facemanager->AI = 0;
        facemanager->index[facemanager->AI] = 0; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 1; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 2; facemanager->AI++;
        facemanager->index[facemanager->AI] = 1; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 3; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 2; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 4; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 5; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 0; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 6; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 5; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 4; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 7; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 8; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 0; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 8; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 1; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 0; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 5; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 7; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 0; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 10; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 9; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 3; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 1; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 10; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 3; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 11; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 10; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 1; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 8; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 11; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 1; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 8; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 12; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 11; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 13; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 12; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 8; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 14; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 15; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 6; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 15; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 5; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 6; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 15; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 16; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 5; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 16; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 7; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 5; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 16; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 17; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 7; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 17; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 18; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 7; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 19; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 9; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 10; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 21; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 19; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 20; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 20; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 19; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 10; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 13; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 21; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 20; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 12; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 13; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 20; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 22; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 15; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 14; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 22; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 24; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 23; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 22; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 23; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 15; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 24; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 18; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 23; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 23; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 18; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 17; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 13; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 8; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 7; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 18; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 13; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 7; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 25; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 19; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 21; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 28; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 25; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 27; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 27; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 25; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 21; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 26; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 21; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 13; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 26; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 27; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 21; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 29; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 24; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 22; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 29; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 32; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 31; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 29; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 31; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 24; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 31; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 30; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 24; facemanager->AI++;
        facemanager->index[facemanager->AI] = 30; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 18; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 24; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 30; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 26; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 18; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 18; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 26; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 13; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 33; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 25; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 28; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 34; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 33; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 28; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 35; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 32; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 29; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 35; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 36; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 32; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 36; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 34; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 32; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 32; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 34; facemanager->AI++; 
        facemanager->index[facemanager->AI] = 28; facemanager->AI++; 
    }
}