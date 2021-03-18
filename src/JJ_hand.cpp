#define hand_position_zero {0, -.02, 0}
#define hand_position_1 {.02, 0, 0}
#define hand_position_2 {0, 0, -.02}
#define hand_position_3 {-.02, 0, 0}
#define hand_position_4 {0, 0, .02}
#define hand_position_5 {-.01, .03, 0}
#define hand_position_6 {0, .02, 0}
#define hand_position_7 {.01, .03, 0}

#define sethanddefaultcolor()\
handmanager->vector[0].color.x = 0.8f; handmanager->vector[0].color.y = 0.5f; handmanager->vector[0].color.z = 0.0f; handmanager->vector[0].color.w = 1.0f;\
handmanager->vector[1].color.x = 0.8f; handmanager->vector[1].color.y = 0.6f; handmanager->vector[1].color.z = 0.0f; handmanager->vector[1].color.w = 1.0f;\
handmanager->vector[2].color.x = 0.8f; handmanager->vector[2].color.y = 0.6f; handmanager->vector[2].color.z = 0.0f; handmanager->vector[2].color.w = 1.0f;\
handmanager->vector[3].color.x = 0.8f; handmanager->vector[3].color.y = 0.6f; handmanager->vector[3].color.z = 0.0f; handmanager->vector[3].color.w = 1.0f;\
handmanager->vector[4].color.x = 0.8f; handmanager->vector[4].color.y = 0.6f; handmanager->vector[4].color.z = 0.0f; handmanager->vector[4].color.w = 1.0f;\
handmanager->vector[5].color.x = 0.8f; handmanager->vector[5].color.y = 0.65f; handmanager->vector[5].color.z = 0.0f; handmanager->vector[5].color.w = 1.0f;\
handmanager->vector[6].color.x = 0.8f; handmanager->vector[6].color.y = 0.7f; handmanager->vector[6].color.z = 0.0f; handmanager->vector[6].color.w = 1.0f;\
handmanager->vector[7].color.x = 0.8f; handmanager->vector[7].color.y = 0.65f; handmanager->vector[7].color.z = 0.0f; handmanager->vector[7].color.w = 1.0f;

struct HandManager
{
    f32 rotation; bool32 isactive;
    vec2 handposition;
    bit32 AV = 0;
    bit32 AI = 0;
    VertexArenaVector vector[8];
    bit16 index[36];
};

HandManager* handmanager; //NOTE: hand manager global.

__forceinline void loadhand()
{
    {//SetupHandmanager()
        handmanager = (HandManager*)recordpush(sizeof(HandManager));
        handmanager->rotation = 0; handmanager->isactive = 0; handmanager->handposition = {0, 0};
    }
    {//Setup Vertex Data
        handmanager->AV = 0;
        handmanager->vector[handmanager->AV].position = hand_position_zero; handmanager->vector[handmanager->AV].color = {.8, .5, 0, 1}; handmanager->AV++;
        handmanager->vector[handmanager->AV].position = hand_position_1; handmanager->vector[handmanager->AV].color = {.8, .6, 0, 1}; handmanager->AV++;
        handmanager->vector[handmanager->AV].position = hand_position_2; handmanager->vector[handmanager->AV].color = {.8, .6, 0, 1}; handmanager->AV++;
        handmanager->vector[handmanager->AV].position = hand_position_3; handmanager->vector[handmanager->AV].color = {.8, .6, 0, 1}; handmanager->AV++;
        handmanager->vector[handmanager->AV].position = hand_position_4; handmanager->vector[handmanager->AV].color = {.8, .6, 0, 1}; handmanager->AV++;
        handmanager->vector[handmanager->AV].position = hand_position_5; handmanager->vector[handmanager->AV].color = {.8, .65, 0, 1}; handmanager->AV++;
        handmanager->vector[handmanager->AV].position = hand_position_6; handmanager->vector[handmanager->AV].color = {.8, .7, 0, 1}; handmanager->AV++;
        handmanager->vector[handmanager->AV].position = hand_position_7; handmanager->vector[handmanager->AV].color = {.8, .65, 0, 1}; handmanager->AV++;
    }
    {//Setup Index Buffer
        handmanager->AI = 0;
        handmanager->index[handmanager->AI] = 0; handmanager->AI++; handmanager->index[handmanager->AI] = 1; handmanager->AI++; handmanager->index[handmanager->AI] = 2; handmanager->AI++;
        handmanager->index[handmanager->AI] = 0; handmanager->AI++; handmanager->index[handmanager->AI] = 2; handmanager->AI++; handmanager->index[handmanager->AI] = 3; handmanager->AI++;
        handmanager->index[handmanager->AI] = 0; handmanager->AI++; handmanager->index[handmanager->AI] = 3; handmanager->AI++; handmanager->index[handmanager->AI] = 4; handmanager->AI++;
        handmanager->index[handmanager->AI] = 0; handmanager->AI++; handmanager->index[handmanager->AI] = 4; handmanager->AI++; handmanager->index[handmanager->AI] = 1; handmanager->AI++;
        handmanager->index[handmanager->AI] = 1; handmanager->AI++; handmanager->index[handmanager->AI] = 7; handmanager->AI++; handmanager->index[handmanager->AI] = 2; handmanager->AI++;
        handmanager->index[handmanager->AI] = 2; handmanager->AI++; handmanager->index[handmanager->AI] = 6; handmanager->AI++; handmanager->index[handmanager->AI] = 7; handmanager->AI++;
        handmanager->index[handmanager->AI] = 2; handmanager->AI++; handmanager->index[handmanager->AI] = 6; handmanager->AI++; handmanager->index[handmanager->AI] = 5; handmanager->AI++;
        handmanager->index[handmanager->AI] = 2; handmanager->AI++; handmanager->index[handmanager->AI] = 5; handmanager->AI++; handmanager->index[handmanager->AI] = 3; handmanager->AI++;
        handmanager->index[handmanager->AI] = 3; handmanager->AI++; handmanager->index[handmanager->AI] = 5; handmanager->AI++; handmanager->index[handmanager->AI] = 4; handmanager->AI++;
        handmanager->index[handmanager->AI] = 4; handmanager->AI++; handmanager->index[handmanager->AI] = 6; handmanager->AI++; handmanager->index[handmanager->AI] = 5; handmanager->AI++;
        handmanager->index[handmanager->AI] = 4; handmanager->AI++; handmanager->index[handmanager->AI] = 6; handmanager->AI++; handmanager->index[handmanager->AI] = 7; handmanager->AI++;
        handmanager->index[handmanager->AI] = 4; handmanager->AI++; handmanager->index[handmanager->AI] = 7; handmanager->AI++; handmanager->index[handmanager->AI] = 1; handmanager->AI++;
    }
}