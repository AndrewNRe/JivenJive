#define nullkey 0
#define KEY_W 1 //NOTE: wasd is only used in debug build.
#define KEY_A 2
#define KEY_S 3
#define KEY_D 4
#define KEY_O 5 //TODO: remove OKLSEMI?
#define KEY_K 6
#define KEY_L 7
#define KEY_Q 8 //Also remove?
#define KEY_E 9
#define KEY_I 10
#define KEY_P 11
#define KEY_F 12
#define KEY_T 13
#define KEY_J 14
#define KEY_U 15
#define KEY_SEMICOLON 16
#define KEY_ENTER 17
#define KEY_BACKSPACE 18
#define LMB_DOWN 19
#define RMB_DOWN 20
#define MOUSE_MOVEMENT_AND_LEFTCLICK 21
#define MOUSE_SCROLL 22
#define KEY_SPACEBAR 23
#define KEY_R 24 //NOTE: used in debug.
#define KEY_ESC 25
#define MM_DOWN 26
#define devMouseSensitivity 1024.0f 
#define minframesforinput 4

struct Input
{
    bit64 input;
    vec2 leftaxis;
    vec2 rightaxis;
    f32 wheeldelta;
};

Input* input; //NOTE global for input.

__forceinline void loadinput()
{
    input = (Input*)recordpush(sizeof(Input));
    input->input = 0; input->leftaxis = {0, 0};input->rightaxis = {0, 0};  input->wheeldelta = 0.0f;
}

void input_fillbuffer(bit8 button)
{
    bit64 ri = 100;
    for(bit64 i = 0; i < sizeof(bit64); i++)
    { //MSVC  NOTE: compiler isn't unrolling this. Therefore, if you want the perf gain, unroll it. (i.e just do the op 8 times instead of letting the compiler put jmp instructions, as a jmp results in the cpu typically flushing itself (so losing all the work it just did)).
        bit8 key = ( ( input->input >> (i*(bit64)8) )  & 0x00000000000000FF );
        if ( key == nullkey && ri == 100)
        { ri = i; }
        if(key == button)
        { return; }
    }
    input->input |= (bit64)((bit64)button << (ri*(bit64)8));
    return;
}

void input_removebuffer(bit8 button)
{
    for(bit64 i = 0; i < sizeof(bit64); i++)
    {
        if (  ( (input->input >> (i*8))  & 0xFF) == button )
        {
            //NOTE: if you ever change nullkey to be ANYTHING but 0. YOU ABSOLUTELY MUST change how you clear bits here!
            input->input &= (bit64)(~((bit8)0xFF << (i*8)));
        }
    }
}

void input_clearbuffer()
{
    //NOTE: if NULLKEY changes value from 0. This MUST BE CHANGED TO OR THE BITS PROPERLY!
    input->input = nullkey;
}

bit8 input_buttoncheck(bit8 button)
{
    for(bit8 i = 0; i < sizeof(bit64); i++)
    {
        if( ( ( input->input >> (i*8))  & 0xFF) == button ) { return button; }
    }
    return nullkey;
}