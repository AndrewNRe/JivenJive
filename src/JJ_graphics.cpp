//#define devTargetMilisecondsPerFrame 16 //NOTE: you have about 40 miliseconds total spare if you're saying each frame must hit 16 exactly. So either do floating point compares (to 16.67 which is the accurate value) or hope this doesn't do anything too bad?
//#define devTargetMilisecondsPerFrame 16.666666666666666666666666666667f //Precise float timing to hit exactly 1000 miliseconds every 60 frames.

struct VertexArenaVector
{
    vec3 position;
    vec4 color;
};
struct VertexArena
{
    bit32 AV = 0; //Allocated Vectors
    bit32 AI = 0; //Allocated Indicies
    VertexArenaVector vector[64];
    bit16 index[256];
};