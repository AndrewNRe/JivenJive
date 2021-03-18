#if 1
#ifdef GNU
#define Assert(Expression) if(!(Expression)) { SDK_BLINKBOARD(1); for(bit32 zxcv = 32; zxcv >0; zxcv--){} }
#else
#define Assert(Expression) if(!(Expression)) {*(s32 *) 0 = 0;}
#endif
#else
#define Assert(Expression)
#endif