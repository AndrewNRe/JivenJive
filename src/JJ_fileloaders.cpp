//NOTE: keeping this code in the codebase for knowledge reasons.
__forceinline bit32 loadwavfile(const char* filepath, void* file_io_memory, bit8* placetowritememory)
{
    bit32 bytesread = platform_fileload(filepath, file_io_memory); bit32 byteswritten = 0; 
    for (bit32 i = 0; i < ((bit32*)file_io_memory)[10] / sizeof(s16); i++, byteswritten+=sizeof(s16))
    {
        ((s16*)placetowritememory)[i] = ((s16*)file_io_memory)[i+11];
    }
    return byteswritten;
}