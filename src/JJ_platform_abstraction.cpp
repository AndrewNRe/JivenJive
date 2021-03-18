//NOTE: THESE FUNCTIONS NEED DEFINITIONS PER PLATFORM. THEY ARE ONLY DEFINED HERE!
/*
The reason behind a platform layer:
It's quite simple. Modern computing environments are different from each other for one reason or another.
To bypass this issue, using a platform layer that "wraps" the platform's functionality into one function that the game calls makes it so I can be on any platform.
And all I have to do on X platform is simply write a procedure in that platform's platform layer cpp specific to THAT platform.
So, for Windows, I have JivenJive_Windows.cpp
That source file is the ONLY place you will EVER find ANYTHING Windows related.
The rest of the game's code is just the game code. No platform specific stuff. It just accesses the function to do a particular thing that only the platform can supply.
*/

__forceinline void platform_sleep() extragnuinlineinfo;
__forceinline void platform_playaudio() extragnuinlineinfo;
__forceinline void platform_getmicinput() extragnuinlineinfo;
__forceinline bit32 platform_fileload(const char* filepath, void* file_io_memory) extragnuinlineinfo;
__forceinline bit32 platform_filewrite(const char* filename, void* file_data, bit32 sizeofdata) extragnuinlineinfo;
__forceinline bool32 platform_filecopy(const char* filename) extragnuinlineinfo;
__forceinline void platform_render() extragnuinlineinfo;
__forceinline void platform_bufferswap() extragnuinlineinfo;
__forceinline void platform_queryinput() extragnuinlineinfo;
__forceinline void platform_hidecursor() extragnuinlineinfo;
__forceinline void platform_showcursor() extragnuinlineinfo;
__forceinline bit64 platform_frametime() extragnuinlineinfo;
__forceinline void platform_updateframetimer() extragnuinlineinfo;
__forceinline void platform_launchthreads() extragnuinlineinfo;
__forceinline void platform_threadinformation() extragnuinlineinfo;