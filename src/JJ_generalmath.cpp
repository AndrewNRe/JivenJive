#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)
#define PI32 3.14159265359f
#define eulernumber 2.7182818284590452353602874713527f //NOTE: this is for my own personal reference.
#define Radians(Degrees) (Degrees * (PI32/180))
#define OneDegreesInRadians 0.0174533f
#define maxsignedsixteenvalue 0x8000

inline f32 f_lerp(f32 a, f32 b, f32 weight)
{
	return a * (1 - weight) + b * weight;
}

//NOTE: the reason why I use a void* here is that I want to endian swap signed and unsigned 16 bit integers. (Same applies for others).
inline void EndianSwap16(void* data) 
{
    bit16 V = ((bit16*)data)[0];
    V = ( (V<<8) | (V>>8) );
    ((bit16*)data)[0]= V;
}

inline void EndianSwap32(void* data)
{
    bit32 V = ((bit32*)data)[0];
    V = ( (V<<24) |  ((V<<8)&0x00FF0000) | ((V>>8)&0x0000FF00) | (V >> 24) );
    ((bit32*)data)[0] = V;
}

inline void EndianSwap64(void* data)
{
    bit64 V = ((bit64*)data)[0];
    V = ( (V<<56) | ((V<<40)&0x00FF000000000000) | ((V<<24)&0x0000FF0000000000) | ((V<<8)&0x000000FF00000000) | ((V>>8)&0x00000000FF000000)
         | ((V>>24)&0x0000000000FF0000) | ((V>>40)&0x000000000000FF00) | (V>>56) );
    ((bit64*)data)[0]= V;
}