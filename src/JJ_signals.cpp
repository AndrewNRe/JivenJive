#define estimatedhighesthertzmonitor 340

#if controllerplayable
#define DEBUG_maxinputdelayinframes 6
#endif

#if debug
struct MicSampleDebugVertexArena
{
    bit32 AV = 0;
    VertexArenaVector vector[defaultaudiosamplerate]; //NOTE: if you want dynamic debug ability, you MUST make this a pointer and pushrecord by the dynamic sample rate post figuring out the sample rate!
};
MicSampleDebugVertexArena* DEBUG_RightChannel;
MicSampleDebugVertexArena* DEBUG_LeftChannel;
bit32 DEBUG_micFinputdelayinframes = DEBUG_maxinputdelayinframes;
bool32 DEBUG_showmicdebug = 0;
#endif

struct Mic
{
    bit32 numberofframes = 0; //NOTE: used to check the micbuffer if it is at its limit. 
    bit32 framesize = 0; bit32 currentframesize = 0; bit32 currentsize = 0;
    bit32 lastnumberofzerocrossing = 0;
    bool32 wasinput = 0; f32 direction = 0;
    s16* micbuffer;
};

Mic* mic; //NOTE global for mic.
s16* audiobuffer; //NOTE: global for audiobuffer

__forceinline void loadmic()
{
    mic = (Mic*)recordpush(sizeof(Mic));
    mic->numberofframes = 0;
    mic->framesize = 0; mic->currentframesize = 0; mic->currentsize = 0;
    mic->lastnumberofzerocrossing = 0; 
    mic->wasinput = 0; mic->direction = 0;
    mic->micbuffer = nullptr;
#if debug
    DEBUG_RightChannel = (MicSampleDebugVertexArena*)recordpush(sizeof(MicSampleDebugVertexArena));
    DEBUG_LeftChannel = (MicSampleDebugVertexArena*)recordpush(sizeof(MicSampleDebugVertexArena));
    DEBUG_RightChannel->AV = 0; DEBUG_LeftChannel->AV = 0;
#endif
}