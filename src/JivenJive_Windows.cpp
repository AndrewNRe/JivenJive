#include <math.h>

#define debug 0
#define controllerplayable 0
#define extragnuinlineinfo

#if debug
#include <cstdio>
void my_printf(const char* string, ...);
void render_printf();
#endif

//Windows Specific Default Defines
#define defaultrefreshrate 60
#define defaultwindowresWIDTH 1920
#define defaultwindowresHEIGHT 1080
#define defaultvolume 16384
#define defaultwhitenoisecompensation 200
#define defaultaudiosamplerate 44100 //NOTE: main pc's value
#define defaultaudiosamplesperframe (defaultaudiosamplerate / defaultrefreshrate)
#define defaultaudiochannels 2

//My files / Data
#include <JJ_types.h>
#include <JJ_vectormath.cpp>
#include <JJ_generalmath.cpp>
#include <JJ_platform_abstraction.cpp>
#include <JJ_assert.cpp>
#include <JJ_memory.cpp>
#include <JJ_fileloaders.cpp>
#include <JJ_input.cpp>
#include <JJ_graphics.cpp>
#include <JJ_settings.cpp>
#include <JJ_signals.cpp> 
#include <JJ_hand.cpp>
#include <JJ_face.cpp>
#include <JJ_main.cpp>
#include <JJ_audiothread.cpp>

//Windows specific headers.
#include <Windows.h>
#include <realtimeapiset.h> 
#include <windowsx.h> 
#include <audioclient.h>  //Audio
#include <mmdeviceapi.h> //Audio
extern "C"
{
#include <Hidsdi.h>
}

//Graphics platform stuff
#include <gl/gl.h>
#include <JJ_opengl.cpp> 
//windows gl defines.
typedef HGLRC WINAPI wgl_Create_Context_Attribs_ARB(HDC hDC, HGLRC hShareContext, const int *attribList);
wgl_Create_Context_Attribs_ARB* wglCreateContextAttribsARB = NULL;
typedef BOOL WINAPI wgl_Choose_Pixel_Format_ARB(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wgl_Choose_Pixel_Format_ARB* wglChoosePixelFormatARB = NULL;
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_ARB_framebuffer_sRGB 1
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
//end of the wgl defines.

#if debug
#include <JJ_printf.cpp>
#endif

//My windows #defines
#define WINDOWS_numberofinputdevices 4
#if debug
#define WINDOWS_OPENGL_MAXVERTEXBUFFERSIZE (sizeof(VertexArena) * 22)
#else
#define WINDOWS_OPENGL_MAXVERTEXBUFFERSIZE 4096 //146 vectors are supported
#endif
#define WINDOWS_OPENGL_MAXINDEXBUFFERSIZE 1024 //512 indicies supported. 

//NOTE: global {Windows platform global(s)}
static IAudioClient* WINDOWS_wasapi_audioclient;
static IAudioRenderClient* WINDOWS_wasapi_audiorenderclient;
static IAudioClient* WINDOWS_wasapi_micclient;
static IAudioCaptureClient* WINDOWS_wasapi_audiocaptureclient;
static HDC WINDOWS_OPENGL_DEVICECONTEXT = 0;
static GLuint WINDOWS_OPENGL_STREAM_VERTEX_BUFFER = 0;
static GLuint WINDOWS_OPENGL_STREAM_INDEX_BUFFER = 0;
static GLuint WINDOWS_OPENGL_BASIC_SHADER = 0;
static bool32 WINDOWS_oktosleep = 0;
static bit64 WINDOWS_frequency = 0;
LARGE_INTEGER WINDOWS_StartCounter; 
bool32 GameIsActive = 1;

#if debug
void APIENTRY GLErrorDebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
	printf("%i, %i, %i, %i, %i, %s\n", (int)source, (int)type, (int)id, (int)severity, (int)length, (const char*)message);
}
#endif

__forceinline void platform_sleep()
{
    bit64 lastframedurationMS = platform_frametime();
    if(lastframedurationMS < settings.target_timeperframe)
    {
        if(WINDOWS_oktosleep) {Sleep(settings.target_timeperframe - lastframedurationMS);}
    }
}

__forceinline void platform_playaudio()
{
    bit32 padding = 0;
    if ( SUCCEEDED( WINDOWS_wasapi_audioclient->GetCurrentPadding(&padding) ) )
    {
        bit32 samplestowrite = settings.audiosamplerate - padding;
        if( samplestowrite > settings.audiosamplesperframe ) { samplestowrite = settings.audiosamplesperframe; }
        else
        {
#if debug
            printf("WINDOWS platform_playaudio(): Samplestowrite is less than required amount to write for good sync!\n"); //Assert(0);
#endif
        }
        BYTE* WindowsAudioBufferBYTE = nullptr;
        if( SUCCEEDED( WINDOWS_wasapi_audiorenderclient->GetBuffer(samplestowrite, &WindowsAudioBufferBYTE) ) )
        {
            s16* WindowsAudioBufferS16 = (s16*)WindowsAudioBufferBYTE;
            bit32 d = 0;
            for(bit32 i = 0; i < samplestowrite; i++)
            {
                WindowsAudioBufferS16[d] = audiobuffer[d]; d++; 
                WindowsAudioBufferS16[d] = audiobuffer[d]; d++; 
            }
            if( SUCCEEDED( WINDOWS_wasapi_audiorenderclient->ReleaseBuffer(samplestowrite, 0) ) ) { return; }
            else 
            {
#if debug
                printf("WINDOWS platform_playaudio(): Couldn't (release) the audio buffer!\n"); //Assert(0);
#endif
            }
        }
        else
        {
#if debug
            printf("WINDOWS platform_playaudio(): Couldn't get audio buffer of requested size!\n"); //Assert(0);
#endif
        }
    }
    else
    {
#if debug
        printf("WINDOWS platform_playaudio(): Couldn't get the padding of the wasapi buffer!\n"); //Assert(0);
#endif
    }
}

__forceinline void platform_getmicinput()
{
    bit32 sizeofmicinput = 0; WINDOWS_wasapi_audiocaptureclient->GetNextPacketSize(&sizeofmicinput);
    bit32 actualframesize = 0; bit64 deviceposition; bit64 perfcounter = 0; DWORD flags = 0;
    BYTE* byteptr = (BYTE*)stackpush(sizeofmicinput*2);
    WINDOWS_wasapi_audiocaptureclient->GetBuffer(&byteptr, &actualframesize, &flags, &deviceposition, &perfcounter);
    Assert(sizeofmicinput == actualframesize); 
    //Done getting stuff from Windows.
    s16* data = (s16*)byteptr;
    s32 w = settings.whitenoisecompensation;
    bit32 s = mic->currentsize;
    Assert(s+(sizeofmicinput*2) < settings.micsamplerate*2);
    for(bit32 i = 0; i < sizeofmicinput*2;)
    {
        mic->micbuffer[s] = (data[i] & maxsignedsixteenvalue) ? ( ((data[i]+w) > 0) ? 0 : data[i]+w ) : ( ((data[i]-w)<0) ? 0 : data[i]-w ) ; i++;  s++;
        mic->micbuffer[s] = (data[i] & maxsignedsixteenvalue) ? ( ((data[i]+w) > 0) ? 0 : data[i]+w ) : ( ((data[i]-w)<0) ? 0 : data[i]-w ) ; i++; s++;
    }
    mic->framesize = sizeofmicinput*2;
    mic->currentsize = s;
    stackpop(sizeofmicinput*2);
    WINDOWS_wasapi_audiocaptureclient->ReleaseBuffer(sizeofmicinput);
}

__forceinline bit32 platform_fileload(const char* filepath, void* file_io_memory)
{
    HANDLE file_strm = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    LARGE_INTEGER LI = LARGE_INTEGER();
    GetFileSizeEx(file_strm, &LI);
    DWORD bytesread = 0;
    if( ReadFile(file_strm, file_io_memory, LI.QuadPart, &bytesread, 0) == 0 )
    {
#if debug
        switch( GetLastError() )
        {
            case ERROR_IO_PENDING: printf("WINDOWS platform_fileload: ReadFile()| ERROR_IO_PENDING\n"); break;
            case ERROR_INVALID_USER_BUFFER : printf("WINDOWS platform_fileload: ReadFile()| ERROR_INVALID_USER_BUFFER \n"); break;
            case ERROR_NOT_ENOUGH_MEMORY : printf("WINDOWS platform_fileload: ReadFile()| ERROR_NOT_ENOUGH_MEMORY \n"); break;
            case ERROR_OPERATION_ABORTED: printf("WINDOWS platform_fileload: ReadFile()| ERROR_OPERATION_ABORTED\n"); break;
            case ERROR_NOT_ENOUGH_QUOTA: printf("WINDOWS platform_fileload: ReadFile()| ERROR_NOT_ENOUGH_QUOTA\n"); break;
            case ERROR_INSUFFICIENT_BUFFER: printf("WINDOWS platform_fileload: ReadFile()| ERROR_INSUFFICIENT_BUFFER\n"); break;
            case ERROR_BROKEN_PIPE: printf("WINDOWS platform_fileload: ReadFile()| ERROR_BROKEN_PIPE\n"); break;
            case ERROR_MORE_DATA: printf("WINDOWS platform_fileload: ReadFile()| ERROR_MORE_DATA\n"); break;
            case ERROR_NOACCESS: printf("WINDOWS platform_fileload: ReadFile()| ERROR_NOACCESS\n"); break;
        }
#endif
    }
    CloseHandle(file_strm);
    return (bit32)bytesread;
}

__forceinline bit32 platform_filewrite(const char* filename, void* file_data, bit32 sizeofdata)
{
    HANDLE file_strm = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    Assert(file_strm != INVALID_HANDLE_VALUE);
    WriteFile(file_strm, file_data, sizeofdata, nullptr, NULL);
    CloseHandle(file_strm);
    return sizeofdata;
}

__forceinline void platform_render()
{
    bit32 vertexpushoffset = 0; bit32 indexpushoffset = 0;
    GLuint posID = 0; GLuint colorID = 1;
    {//Invalidate old buffers
        glBindBuffer(GL_ARRAY_BUFFER, WINDOWS_OPENGL_STREAM_VERTEX_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, WINDOWS_OPENGL_MAXVERTEXBUFFERSIZE, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, WINDOWS_OPENGL_STREAM_INDEX_BUFFER);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, WINDOWS_OPENGL_MAXINDEXBUFFERSIZE,  nullptr, GL_DYNAMIC_DRAW);
        glUseProgram(WINDOWS_OPENGL_BASIC_SHADER);
        glEnableVertexAttribArray(posID);
        glEnableVertexAttribArray(colorID);
        glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, 7*sizeof(f32), 0);
        glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 7*sizeof(f32), (void*) (3*sizeof(f32)) );
    }
    { //Non-rotated face draw
        glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), facemanager->AV * sizeof(VertexArenaVector), facemanager->vector);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexpushoffset*sizeof(bit16), facemanager->AI * sizeof(bit16), facemanager->index);
        glDrawElements(GL_TRIANGLES, facemanager->AI, GL_UNSIGNED_SHORT, (void*)(bit64)indexpushoffset);
        vertexpushoffset+=facemanager->AV;
        indexpushoffset+=facemanager->AI;
    }
    if(handmanager->isactive)
    {//Draw Hand
        bit32 vertexbufferalloc = handmanager->AV*sizeof(VertexArenaVector); bit32 indexbufferalloc = handmanager->AI*sizeof(bit16);
        VertexArenaVector* vectorbuffer = (VertexArenaVector*)stackpush(vertexbufferalloc); bit16* indexbuffer = (bit16*)stackpush(indexbufferalloc);
        mat3x3 rotation = rotate3x3Y(handmanager->rotation);
        for(bit32 i = 0; i < handmanager->AV; i++)
        {
            vec3 translation = {handmanager->handposition.x, handmanager->handposition.y, -.01};
            vectorbuffer[i].position = (rotation * handmanager->vector[i].position) + translation;
            vectorbuffer[i].color = handmanager->vector[i].color;
        }
        for(bit32 i = 0; i < handmanager->AI; i++) { indexbuffer[i] = handmanager->index[i] + vertexpushoffset; }
        glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), handmanager->AV*sizeof(VertexArenaVector), vectorbuffer); 
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexpushoffset*sizeof(bit16), handmanager->AI*sizeof(bit16), indexbuffer);
        glDrawElements(GL_TRIANGLES, handmanager->AI, GL_UNSIGNED_SHORT, (void*)(bit64)(indexpushoffset*sizeof(bit16)) );
        vertexpushoffset += handmanager->AV;
        indexpushoffset += handmanager->AI;
        stackpop(vertexbufferalloc); stackpop(indexbufferalloc);
    }
    if(settingscontroller->isactive)
    {//Draw Settings()
        {//Draw Volume and Whitenoise Reduction Bars()
            bit32 numberofindicies = sizeof(settingscontroller->indexbuffer)/sizeof(bit16);
            bit32 indexbufferalloc = sizeof(settingscontroller->indexbuffer);
            bit16* movedindexbuffer = (bit16*)stackpush(indexbufferalloc);
            for(bit32 i = 0; i < numberofindicies; i++){ movedindexbuffer[i] = settingscontroller->indexbuffer[i] + vertexpushoffset; }
            glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), sizeof(settingscontroller->vertex), settingscontroller->vertex);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexpushoffset*sizeof(bit16), indexbufferalloc, movedindexbuffer);
            glDrawElements(GL_TRIANGLES, numberofindicies, GL_UNSIGNED_SHORT, (void*)(bit64)(indexpushoffset*sizeof(bit16)) );
            vertexpushoffset += sizeof(settingscontroller->vertex) / sizeof(VertexArenaVector);
            indexpushoffset += numberofindicies;
            stackpop(indexbufferalloc);
        }
        {//Draw Volume and Mic Icon()
            bit32 numberofindicies = sizeof(settingscontroller->iconindexbuffer)/sizeof(bit16);
            bit32 indexbufferalloc = sizeof(settingscontroller->iconindexbuffer);
            bit16* movedindexbuffer = (bit16*)stackpush(indexbufferalloc);
            for(bit32 i = 0; i < numberofindicies; i++){ movedindexbuffer[i] = settingscontroller->iconindexbuffer[i] + vertexpushoffset; }
            glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), sizeof(settingscontroller->icon), settingscontroller->icon);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexpushoffset*sizeof(bit16), indexbufferalloc, movedindexbuffer);
            glDrawElements(GL_TRIANGLES, numberofindicies, GL_UNSIGNED_SHORT, (void*)(bit64)(indexpushoffset*sizeof(bit16)));
            vertexpushoffset+=sizeof(settingscontroller->icon)/sizeof(VertexArenaVector);
            indexpushoffset += numberofindicies;
            stackpop(indexbufferalloc);
        }
    }
#if debug
    {//Render Either Debug Wave()
        if(DEBUG_showmicdebug)
        {
            glPointSize(1.0f);
            glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), DEBUG_RightChannel->AV * sizeof(VertexArenaVector), DEBUG_RightChannel->vector);
            glDrawArrays(GL_POINTS, vertexpushoffset, DEBUG_RightChannel->AV);
            vertexpushoffset+=DEBUG_RightChannel->AV;
            glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), DEBUG_LeftChannel->AV * sizeof(VertexArenaVector), DEBUG_LeftChannel->vector);
            glDrawArrays(GL_POINTS, vertexpushoffset, DEBUG_LeftChannel->AV);
            vertexpushoffset+=DEBUG_LeftChannel->AV;
        }
        else if(DEBUG_showgeneratedaudiobuffer)
        {
            glPointSize(2.0f);
            glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), DEBUG_generatedaudiobufferR->AV * sizeof(VertexArenaVector), DEBUG_generatedaudiobufferR->vector);
            glDrawArrays(GL_POINTS, vertexpushoffset, DEBUG_generatedaudiobufferR->AV);
            vertexpushoffset+=DEBUG_generatedaudiobufferR->AV;
            glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), DEBUG_generatedaudiobufferL->AV*sizeof(VertexArenaVector), DEBUG_generatedaudiobufferL->vector);
            glDrawArrays(GL_POINTS, vertexpushoffset, DEBUG_generatedaudiobufferL->AV);
            vertexpushoffset+=DEBUG_generatedaudiobufferL->AV;
        }
    }
    if(DEBUG_emotionalaxesdisplay)
    {//DrawDataAssociatedWithEmotionalAxes()
        glPointSize(20.0f);
        {//DrawEmotionalPoints()
            glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), sizeof(VertexArenaVector)*DEBUG_emotionaldata.numberofanimations, DEBUG_emotionaldata.emotions.vector);
            glDrawArrays(GL_POINTS, vertexpushoffset, DEBUG_emotionaldata.numberofanimations);
            vertexpushoffset+=DEBUG_emotionaldata.numberofanimations;
        }
        {//DrawEmotionalAxes()
            //max signed 32 value = 2147483648
            f32 normalizeval = maxemotionalvalue;
            VertexArenaVector movedaxesandcoordinatebounds[2];
            movedaxesandcoordinatebounds[0].position = {0.0f, 0.0f}; movedaxesandcoordinatebounds[0].color = {1, 1, 1, 1};
            movedaxesandcoordinatebounds[1].position = {((f32)facemanager->emotion.happiness/normalizeval), ((f32)facemanager->emotion.peacefulness/normalizeval)};
            movedaxesandcoordinatebounds[1].color = {1, .5, 1, 1};
            glBufferSubData(GL_ARRAY_BUFFER, vertexpushoffset*sizeof(VertexArenaVector), sizeof(movedaxesandcoordinatebounds), movedaxesandcoordinatebounds);
            glDrawArrays(GL_POINTS, vertexpushoffset, 2);
            vertexpushoffset+=2;
        }
    }
    glPointSize(3.0f);
#endif
    {//GL cleanup
        glDisableVertexAttribArray(posID);
        glDisableVertexAttribArray(colorID);
    }
}

__forceinline void platform_bufferswap()
{
    SwapBuffers(WINDOWS_OPENGL_DEVICECONTEXT);
    {//Clear buffer for next render.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        vec4 ColorBuffer = {0.0f, 0.0f, 0.0f, 1.0f}; //Black
        glClearBufferfv(GL_COLOR, 0, (const GLfloat*)&ColorBuffer);
        const GLint b = 0; 
    }
}

__forceinline void platform_queryinput()
{
    GameNotActiveQuery:
    MSG msg = { };
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE) != 0)
    {
        if(msg.message == WM_QUIT)
        { isrunning  = false; break; } 
        TranslateMessage(&msg); DispatchMessage(&msg);
    }
    if(!GameIsActive){goto GameNotActiveQuery;}
}

__forceinline void  platform_hidecursor() { SetCursor(NULL); }
__forceinline void platform_showcursor() { SetCursor(LoadCursorA(GetModuleHandleA("JivenJive.exe"), MAKEINTRESOURCE(32512))); }

__forceinline bit64 platform_frametime()
{
    LARGE_INTEGER EndCounter; QueryPerformanceCounter(&EndCounter);
    bit64 lastframedurationMS = (1000*(EndCounter.QuadPart - WINDOWS_StartCounter.QuadPart)) / WINDOWS_frequency;
    return lastframedurationMS;
}

__forceinline void platform_updateframetimer()
{
    LARGE_INTEGER EndCounter; QueryPerformanceCounter(&EndCounter);
    WINDOWS_StartCounter = EndCounter;
}


__forceinline void setupWASAPIAUDIO()
{ 
    if(FAILED(CoInitializeEx(0, COINIT_SPEED_OVER_MEMORY))) //INITALIZES COM 
    {
#if debug
        printf("ERROR, COULD NOT SETUP WASAPI @ CoInitalizeEx\n"); Assert(0);
#endif
    }
    IMMDeviceEnumerator* Enumerator;
    if( FAILED(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&Enumerator))) ) //Basically, get a pointer through the COM system to enumerate audio endpoint devices specifically.
    {
#if debug
        printf("ERROR, COULD NOT SETUP WASAPI @ CoCreateInstance\n"); Assert(0);
#endif
    }
    { //Setup audio render()
        //NOTE: This doesn't check for other audio endpoints, so if getting the default audio endpoint doesn't work for some  (or you). You must do more work here to determine what audio to use!
        IMMDevice* AudioOutput;
        if( FAILED(Enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &AudioOutput)) ) //Get the default audio endpoint of the system currently.
        {
#if debug
            printf("ERROR, COULD NOT SETUP WASAPI @ Enumerator->EnumerGetDefaultAudioEndpoint\n"); Assert(0);
#endif
        }
        if( FAILED(AudioOutput->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (LPVOID*)&WINDOWS_wasapi_audioclient)) ) //This memory write is used to check the status of a given buffer. Like padding and other things.
        {
#if debug
            printf("ERROR, COULD NOT SETUP WASAPI @ AudioOutput->Activiate\n"); Assert(0);
#endif
        }
        WAVEFORMATEX* DefaultWaveFormat = nullptr;
        WINDOWS_wasapi_audioclient->GetMixFormat(&DefaultWaveFormat);
        settings.audiosamplerate = DefaultWaveFormat->nSamplesPerSec;
        settings.audiochannels = DefaultWaveFormat->nChannels;
        settings.audiosamplesperframe = settings.audiosamplerate / settings.refreshrate;
        audiobuffer = (s16*)recordpush(settings.audiosamplerate*2);
        WAVEFORMATEXTENSIBLE WaveFormat;
        WaveFormat.Format.cbSize = sizeof(WaveFormat);
        WaveFormat.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
        WaveFormat.Format.wBitsPerSample = sizeof(s16)*8;
        WaveFormat.Format.nChannels = DefaultWaveFormat->nChannels;
        WaveFormat.Format.nSamplesPerSec = DefaultWaveFormat->nSamplesPerSec; 
        WaveFormat.Format.nBlockAlign = (WORD)(WaveFormat.Format.nChannels * WaveFormat.Format.wBitsPerSample / 8);
        WaveFormat.Format.nAvgBytesPerSec = WaveFormat.Format.nSamplesPerSec * WaveFormat.Format.nBlockAlign;
        WaveFormat.Samples.wValidBitsPerSample = 16;
        WaveFormat.dwChannelMask = KSAUDIO_SPEAKER_STEREO;
        WaveFormat.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
        REFERENCE_TIME BufferDuration = 10000000ULL * settings.audiosamplerate / settings.audiosamplerate;
        bit32 test = 0;
        if( FAILED(test = WINDOWS_wasapi_audioclient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_NOPERSIST, BufferDuration, 0, &WaveFormat.Format, nullptr)) ) //Tries to see if the current setup is fine to work with WASAPI
        {
#if debug
            switch(test)
            {
                case AUDCLNT_E_ALREADY_INITIALIZED: printf("AUDCLNT_E_ALREADY_INITIALIZED\n"); break;
                case AUDCLNT_E_WRONG_ENDPOINT_TYPE: printf("AUDCLNT_E_WRONG_ENDPOINT_TYPE\n"); break;
                case AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED: printf("AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED\n"); break;
                case AUDCLNT_E_BUFFER_SIZE_ERROR: printf("AUDCLNT_E_BUFFER_SIZE_ERROR\n"); break;
                case AUDCLNT_E_CPUUSAGE_EXCEEDED: printf("AUDCLNT_E_CPUUSAGE_EXCEEDED\n"); break;
                case AUDCLNT_E_DEVICE_INVALIDATED: printf("AUDCLNT_E_DEVICE_INVALIDATED\n"); break;
                case AUDCLNT_E_DEVICE_IN_USE: printf("AUDCLNT_E_DEVICE_IN_USE\n"); break;
                case AUDCLNT_E_ENDPOINT_CREATE_FAILED: printf("AUDCLNT_E_ENDPOINT_CREATE_FAILED\n"); break;
                case AUDCLNT_E_INVALID_DEVICE_PERIOD: printf("AUDCLNT_E_INVALID_DEVICE_PERIOD\n"); break;
                case AUDCLNT_E_UNSUPPORTED_FORMAT: printf("AUDCLNT_E_UNSUPPORTED_FORMAT\n"); break;
                case AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL: printf("AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL\n"); break;
                case AUDCLNT_E_SERVICE_NOT_RUNNING: printf("AUDCLNT_E_SERVICE_NOT_RUNNING\n"); break;
                case E_POINTER: printf("E_POINTER\n"); break;
                case E_INVALIDARG: printf("E_INVALIDARG\n"); break;
                case E_OUTOFMEMORY: printf("E_OUTOFMEMORY\n"); break;
            }
            printf("ERROR, COULD NOT SETUP WASAPI @ ptr->Initalize\n"); Assert(0);
#endif
        }
        if( FAILED(WINDOWS_wasapi_audioclient->GetService( __uuidof(IAudioRenderClient),  (void**)&WINDOWS_wasapi_audiorenderclient) ) )
        {
#if debug
            printf("ERROR, COULD NOT SETUP WASAPI @ ptr->GetService\n"); Assert(0);
#endif
        }
        bit32 SoundFrameCount = 0;
        if (FAILED(WINDOWS_wasapi_audioclient->GetBufferSize(&SoundFrameCount)))
        {
#if debug
            printf("ERROR, COULD NOT SETUP WASAPI @ "); Assert(0);
#endif
        }
#if debug
        printf("AUDIO: number of samples in a given sound buffer frame you can do: %i\n", SoundFrameCount);
#endif
        if( SUCCEEDED(WINDOWS_wasapi_audioclient->Start()) )
        {
#if debug
            printf("AUDIO: The audio stream has started successfully!\n");
#endif
        }
        else
        {
#if debug
            printf("The audio FAILED to start!\n"); Assert(0);
#endif
        }
    }
    
    jj_audiothread(); //Post setup, go play audio until program ends.
}

__forceinline void platform_launchthreads()
{
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)setupWASAPIAUDIO, NULL, 0, NULL); //No security attributes, default stack size (0), start executing audio thread, no variable(s) to be passed, 
    //thread runs immediately post creation (0), I don't care about the thread identifier as the thread should run until the program closes (null).
}

__forceinline void platform_threadinformation()
{
    if(!GameIsActive){for(;!GameIsActive;){}}
}

__forceinline void WINDOWS_processmouseinput(RAWMOUSE mouse)
{
    switch(mouse.usButtonFlags)
    {
        case RI_MOUSE_BUTTON_1_DOWN: { input_fillbuffer(LMB_DOWN); break;}
        case RI_MOUSE_BUTTON_2_DOWN: { input_fillbuffer(RMB_DOWN); break; }
        case RI_MOUSE_MIDDLE_BUTTON_DOWN: { input_fillbuffer(MM_DOWN); break;}
        case RI_MOUSE_BUTTON_1_UP: { input_removebuffer(LMB_DOWN); break; }
        case RI_MOUSE_BUTTON_2_UP: { input_removebuffer(RMB_DOWN); break; }
        case RI_MOUSE_MIDDLE_BUTTON_UP: { input_removebuffer(MM_DOWN); break; }
        case RI_MOUSE_WHEEL:  { input->wheeldelta = ((f32)((short)mouse.usButtonData) / 32767.0f) * 16; break; }
        default: {  input->wheeldelta = 0; }
    }
}

__forceinline void WINDOWS_processkeyboardinput(RAWKEYBOARD keyboard)
{
    if(keyboard.Flags == RI_KEY_MAKE)
    {
        switch(keyboard.VKey)
        {
            case 0x57: { input_fillbuffer(KEY_W); input->leftaxis.y =  1; break; }
            case 0x41: { input_fillbuffer(KEY_A);  input->leftaxis.x = -1; break; }
            case 0x53: { input_fillbuffer(KEY_S);  input->leftaxis.y = -1; break; }
            case 0x44: { input_fillbuffer(KEY_D);  input->leftaxis.x =  1; break; }
            case 0x4F: { input_fillbuffer(KEY_O);  break; }
            case 0x4B: { input_fillbuffer(KEY_K);  break; }
            case 0x4C: { input_fillbuffer(KEY_L);  break; }
            case VK_OEM_1: { input_fillbuffer(KEY_SEMICOLON);  break; }
            case 0x51: { input_fillbuffer(KEY_Q);  break; }
            case 0x45: { input_fillbuffer(KEY_E);  break; }
            case 0x49: { input_fillbuffer(KEY_I);  break; }
            case 0x50: { input_fillbuffer(KEY_P);  break; }
            case 0x46: { input_fillbuffer(KEY_F);  break; }
            case 0x54: { input_fillbuffer(KEY_T);  break; }
            case 0x4A: { input_fillbuffer(KEY_J);  break; }
            case 0x55: { input_fillbuffer(KEY_U);  break; }
            case 0x52: { input_fillbuffer(KEY_R); break; }
            case VK_ESCAPE: { input_fillbuffer(KEY_ESC); break; }
            case VK_RETURN: { input_fillbuffer(KEY_ENTER);  break; }
            case VK_BACK: { input_fillbuffer(KEY_BACKSPACE);  break; }
            case VK_SPACE: { 
                input_fillbuffer(KEY_SPACEBAR); break; }
        }
    }
    else if (keyboard.Flags == RI_KEY_BREAK)
    {
        switch(keyboard.VKey)
        {
            case 0x57: {input_removebuffer(KEY_W); input->leftaxis.y = 0; break; }
            case 0x41: { input_removebuffer(KEY_A);  input->leftaxis.x = 0; break; }
            case 0x53: { input_removebuffer(KEY_S);  input->leftaxis.y = 0; break; }
            case 0x44: { input_removebuffer(KEY_D);  input->leftaxis.x = 0; break; }
            case 0x4F: { input_removebuffer(KEY_O);  break; }
            case 0x4B: { input_removebuffer(KEY_K);  break; }
            case 0x4C: { input_removebuffer(KEY_L);  break; }
            case VK_OEM_1: { input_removebuffer(KEY_SEMICOLON);  break; }
            case 0x51: { input_removebuffer(KEY_Q);  break; }
            case 0x45: { input_removebuffer(KEY_E);  break; }
            case 0x49: { input_removebuffer(KEY_I);  break; }
            case 0x50: { input_removebuffer(KEY_P);  break; }
            case 0x46: { input_removebuffer(KEY_F);  break; }
            case 0x54: { input_removebuffer(KEY_T);  break; }
            case 0x4A: { input_removebuffer(KEY_J);  break; }
            case 0x55: { input_removebuffer(KEY_U);  break; }
            case 0x52: { input_removebuffer(KEY_R); break; }
            case VK_ESCAPE: { input_removebuffer(KEY_ESC); break; }
            case VK_RETURN: { input_removebuffer(KEY_ENTER);  break; }
            case VK_BACK: { input_removebuffer(KEY_BACKSPACE);  break; }
            case VK_SPACE: { input_removebuffer(KEY_SPACEBAR); break; }
        }
    }
    else
    {
#if debug
        printf("Keyboard EVENT THAT I DO NOT HANDLE ATM! WINDOWS PLATFORM JJWINDOWS\n");
#endif
    }
}

__forceinline void WINDOWS_processGamePad(short controllerinput, unsigned long stickamt, USAGE button)
{
    switch(controllerinput)
    {
        case 0x31: //Y axis left stick
        {
            if (stickamt < 30000) //Up on the stick! (windows says that -y is up and pos y is down, so the < is actually up on the stick here...)
            { 
                input->leftaxis.y= (1.0 - ((f32)stickamt / 30000.0f));
            }
            else if(stickamt > 34000) //Down on the stick
            {
                input->leftaxis.y= -( 1.0f - ((f32)(65535 - stickamt) / 31535.0f)); //NOTE: 31535 = 65535 - 34000! 
            }
            else{ input->leftaxis.y = 0; }
            break;
        }
        case 0x30: //X axis left stick 
        {
            if(stickamt > 37000) //Right on control stick (so positive x)
            { 
                input->leftaxis.x = 1.0f - (f32)(65535 - stickamt) / 28535.0f; //NOTE: 28535 is 65535 - 37000!
            }
            else if (stickamt < 33000) //Left on control stick (so negative x)
            {
                input->leftaxis.x = -( 1.0f - ((f32)stickamt / 33000.0f));
            }
            else { input->leftaxis.x = 0; }
            break;
        }
        case 0x32: //Z axis 
        {
            //printf("Z axis %i\n", stickamt);  
            break;
        }
        case 0x35: //rotate-z or something
        {
#if debug
            printf("Rotate-z %i\n", stickamt); 
#endif
            break;
        }
        case 0x39: //D-pad
        {
            switch(stickamt)
            {
                //case 0: break; //Resting position. So no input.
                case 1: input_fillbuffer(KEY_W); input->leftaxis.y =  1; break; //North
                case 2: input_fillbuffer(KEY_W); input_fillbuffer(KEY_D); input->leftaxis.y =  1; input->leftaxis.x =  1; break; //North-East
                case 3: input_fillbuffer(KEY_D); input->leftaxis.x =  1; break; //East
                case 4: input_fillbuffer(KEY_S); input->leftaxis.y = -1; input_fillbuffer(KEY_D);  input->leftaxis.x =  1; break; //South-East
                case 5: input_fillbuffer(KEY_S); input->leftaxis.y = -1; break; //South
                case 6: input_fillbuffer(KEY_S); input_fillbuffer(KEY_W); input->leftaxis.y = -1; input->leftaxis.x = -1; break; //South-West
                case 7: input_fillbuffer(KEY_A);  input->leftaxis.x = -1; break; //West
                case 8: input_fillbuffer(KEY_W); input_fillbuffer(KEY_A); input->leftaxis.y =  1; input->leftaxis.x = -1; break; //North-West
            }
            break;
        }
        case 52: //Right stick y axis
        {
            input_fillbuffer(LMB_DOWN);
            if (stickamt < 30000) //Up on the stick! (windows says that -y is up and pos y is down, so the < is actually up on the stick here...)
            { 
                input->rightaxis.y = (1.0 - ((f32)stickamt / 30000.0f));
            }
            else if(stickamt > 34000) //Down on the stick
            {
                input->rightaxis.y = -( 1.0f - ((f32)(65535 - stickamt) / 31535.0f)); //31535 = 65535 - 34000! 
            }
            else{ input->rightaxis.y = 0; }
        }
        break;
        case 51: //Right stick x axis
        {
            input_fillbuffer(LMB_DOWN);
            if(stickamt > 37000) //Right on control stick (so positive x)
            { 
                input->rightaxis.x = 1.0f - (f32)(65535 - stickamt) / 28535.0f; //28535 is 65535 - 37000!
            }
            else if (stickamt < 33000) //Left on control stick (so negative x)
            {
                input->rightaxis.x = -( 1.0f - ((f32)stickamt / 33000.0f));
            }
            else { input->rightaxis.x = 0; }
            break;
        }
    }
    if(button != 0)
    {
        switch(button)
        {
            case 1: input_fillbuffer(KEY_L); break;
            case 2: input_fillbuffer(KEY_SEMICOLON); break;
            case 3: input_fillbuffer(KEY_K); break;
            case 4: input_fillbuffer(KEY_O); break;
            case 5:
            {
                if (stickamt < 30000) //Up on the stick! (windows says that -y is up and pos y is down, so the < is actually up on the stick here...)
                { 
                    input->wheeldelta = (1.0 - ((f32)stickamt / 30000.0f));
                }
                else if(stickamt > 34000) //Down on the stick
                {
                    input->wheeldelta = -( 1.0f - ((f32)(65535 - stickamt) / 31535.0f)); //31535 = 65535 - 34000! 
                }
                else{ input->wheeldelta = 0; }
                break;
            }
            case 6: input_fillbuffer(KEY_SPACEBAR); break;
            //case 7: break; //Should be select.
            //case 8: break; //Should be start.
            //case 9: break; //Should be left stick down.
            //case 10: break;//Should be right stick down. You can reconfirm these buttons in the gamepad thing on windows.
        }
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT rv = 0;
    switch(uMsg)
    {
        case WM_QUIT:
        case WM_CLOSE:
        case WM_DESTROY: {isrunning = false; } break;
        case WM_WINDOWPOSCHANGING: 
        case WM_WINDOWPOSCHANGED:
        {
            RECT clientrect; GetClientRect(hwnd, &clientrect);
            settings.windowresWIDTH = clientrect.right;
            settings.windowresHEIGHT = clientrect.bottom;
            glViewport(0, 0, settings.windowresWIDTH, settings.windowresHEIGHT);
            rv = DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
        case WM_ACTIVATEAPP:
        {
            if(wParam)
            { GameIsActive = true; }
            else{GameIsActive = false;}
            rv = DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
        case WM_MOUSEMOVE :
        {
            input->rightaxis.x = (f32)((f32)(lParam & 0x000000000000FFFF) / (f32)(settings.windowresWIDTH/2)) -1.0f;
            input->rightaxis.y =  (f32)-(((f32)(lParam >> 16) / (f32)(settings.windowresHEIGHT/2)) - 1.0f);
            rv = DefWindowProc(hwnd, uMsg, wParam, lParam);
        } break;
        case WM_INPUT:
        {
            if(wParam == RIM_INPUT) //The user did some input while the window was in the foreground.
            {
                bit32 buffersize = 0;
                GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &buffersize, sizeof(RAWINPUTHEADER));
                RAWINPUT* rawinputdata = (RAWINPUT*)stackpush(sizeof(RAWINPUT));
                if((s16)GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawinputdata, &buffersize, sizeof(RAWINPUTHEADER)) == -1)
                {
#if debug
                    printf("ERROR WITH GETRAWINPUTDATA! error code = %i\n", GetLastError());
#endif
                }
                switch(rawinputdata->header.dwType)
                {
                    case RIM_TYPEMOUSE:
                    {
                        WINDOWS_processmouseinput(rawinputdata->data.mouse);
                        break;
                    }
                    case RIM_TYPEKEYBOARD:
                    {
                        WINDOWS_processkeyboardinput(rawinputdata->data.keyboard);
                        break;
                    }
                    case RIM_TYPEHID:
                    {
                        void* device[2048];
                        UINT sizeofthing = 0;
                        GetRawInputDeviceInfoA( rawinputdata->header.hDevice, RIDI_PREPARSEDDATA, NULL, &sizeofthing );
                        if( (int)GetRawInputDeviceInfoA( rawinputdata->header.hDevice, RIDI_PREPARSEDDATA, device, &sizeofthing ) > 0 )
                        {
                            //This CAPS struct stores a lot of information related to the controller. Here, it's primarily queried for lengths of things.
                            HIDP_CAPS Caps = HIDP_CAPS();
                            if( HidP_GetCaps( (PHIDP_PREPARSED_DATA)device, &Caps) == HIDP_STATUS_SUCCESS )
                            {
                                //BUTTON CAPS stores button usage information it would appear.
                                //Main thing to take away is that in most cases, there'll only be 1 button cap structure that I actually get and allocate.
                                //However, let's say a controller has a turbo button. That button has "sub buttons" in a sense. So the number of structs allocated for that
                                //might be 3. 1 for the inital "turbo". 2 for "fast" and "normal" or something along those lines.
                                HIDP_BUTTON_CAPS* ButtonCaps = (HIDP_BUTTON_CAPS*)stackpush(sizeof(HIDP_BUTTON_CAPS) * Caps.NumberInputButtonCaps);
                                for(bit32 BCE = 0; BCE < Caps.NumberInputButtonCaps; BCE++) //For each button cap. BCE = buttoncap enumerator
                                {
                                    ButtonCaps[BCE] = HIDP_BUTTON_CAPS();
                                    if( HidP_GetButtonCaps(HidP_Input, (PHIDP_BUTTON_CAPS)&ButtonCaps[BCE], &Caps.NumberInputButtonCaps, (PHIDP_PREPARSED_DATA)device) == HIDP_STATUS_SUCCESS )
                                    {
                                        HIDP_VALUE_CAPS* valuecaps = (HIDP_VALUE_CAPS*)stackpush(sizeof(HIDP_VALUE_CAPS) * Caps.NumberInputValueCaps);
                                        for(bit32 r = 0; r < Caps.NumberInputValueCaps; r++){ valuecaps[r] = HIDP_VALUE_CAPS(); }
                                        if ( HidP_GetValueCaps(HidP_Input, valuecaps, &Caps.NumberInputValueCaps, (PHIDP_PREPARSED_DATA)device) == HIDP_STATUS_SUCCESS )
                                        {
                                            //To explain this part. Basically, USAGE* contains what buttons were actually pressed. So i'm just saving memory to check buttons that were pressed.
                                            unsigned long usageLength = ButtonCaps[BCE].Range.UsageMax - ButtonCaps[BCE].Range.UsageMin + 1; 
                                            USAGE* Usagelist = (USAGE*)stackpush(sizeof(USAGE) * usageLength);
                                            for(bit32 i = 0; i < usageLength; i++) { Usagelist[i] = USAGE(); }
                                            if( HidP_GetUsages(HidP_Input, ButtonCaps[BCE].UsagePage, 0, Usagelist, &usageLength, (PHIDP_PREPARSED_DATA)device, (PCHAR)rawinputdata->data.hid.bRawData, rawinputdata->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS )
                                            {
                                                input_clearbuffer(); //Before processing all the input for the controller state. Reset the button buffer!
                                                for(bit32 i = 0; i < Caps.NumberInputValueCaps; i++)
                                                {
                                                    unsigned long value = 0;
                                                    if( HidP_GetUsageValue(HidP_Input, valuecaps[i].UsagePage, 0, valuecaps[i].Range.UsageMin, &value, (PHIDP_PREPARSED_DATA)device, (PCHAR)rawinputdata->data.hid.bRawData, rawinputdata->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS )
                                                    {
                                                        WINDOWS_processGamePad(valuecaps[i].Range.UsageMin, value, Usagelist[i]);
                                                    }
                                                }
                                            }
                                            stackpop(sizeof(USAGE) * usageLength);
                                        }
                                        stackpop(sizeof(HIDP_VALUE_CAPS) * Caps.NumberInputValueCaps);
                                    }
                                }
                                stackpop(sizeof(HIDP_BUTTON_CAPS) * Caps.NumberInputButtonCaps);
                            }
                        }
                        break;
                    }
                }
                stackpop(sizeof(RAWINPUT));
            }
            else //The game was in the background and the user did some input that was flagged as input. So I don't care about that input as the player wasn't actively engaged with the game!
            {
                rv = DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
            rv = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(hwnd, &Paint);
            EndPaint(hwnd, &Paint);
        } break;
        default:
        {
            rv = DefWindowProc(hwnd, uMsg, wParam, lParam);  //Let windows do what it wants to do. 
            break;
        }
    }
    return rv;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    
    { //Setupmemory()
        //TODO: performance gain can be done here. By doing all the footwork to get MEM_LARGE_PAGES working with virtualloc. The TLB will go faster. SO faster cpu execution!
        gamememory.baseaddress = (bit64)VirtualAlloc(0, gamememory.maxsize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
        if (gamememory.baseaddress == 0){Assert(0)}
        {//LoadLongTermMemory()
            loadsettings();
            loadinput();
            loadhand();
            loadfaceone();
            loadmic();
        }
    }
    
    { //Setup Windows
        HWND hwnd = 0;
        WNDCLASS wc = { 0 };
        wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW; //CS_OWNDC is required for opengl to work.
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = "JivenJive";
        wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
        RegisterClass(&wc);
        hwnd = CreateWindowEx(0, wc.lpszClassName, "JivenJive", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
        if (hwnd == NULL)
        {
            return 0;
        }
        SetPriorityClass(hwnd, HIGH_PRIORITY_CLASS);
#if debug 
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
#endif
    }
    
    { //SetupOpenGL()
        HGLRC RGL = 0;
        HWND hwnd = GetActiveWindow();
        WINDOWS_OPENGL_DEVICECONTEXT = GetDC(hwnd); //Get the device context of my current window. This is the dummy context.
        PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, //Then setup the pixel format that will be associated with the dummy context.
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32,
            0, 0, 0, 0, 0, 0, 0, 0,
            0,
            0, 0, 0, 0,
            24, 8, 0, PFD_MAIN_PLANE,
            0, 0, 0, 0
        };
        int pixelformatnumber = ChoosePixelFormat(WINDOWS_OPENGL_DEVICECONTEXT, &pfd);
        if (pixelformatnumber == NULL) { return 0; }
        SetPixelFormat(WINDOWS_OPENGL_DEVICECONTEXT, pixelformatnumber, &pfd);
        HGLRC GL = wglCreateContext(WINDOWS_OPENGL_DEVICECONTEXT);
        wglMakeCurrent(WINDOWS_OPENGL_DEVICECONTEXT, GL); //This is making a dummy context, so I can actually ensure that the GL context is properly setup because windows.
        platform_WINDOWS_gl_functions_load(); //After setting up a dummy opengl context, you can actually load modern GL pointers. So here's the load.
        //WGL SPECIFIC FUNCTION POINTERS LOAD
        wglCreateContextAttribsARB = (wgl_Create_Context_Attribs_ARB*)wglGetProcAddress("wglCreateContextAttribsARB");
        wglChoosePixelFormatARB = (wgl_Choose_Pixel_Format_ARB*)wglGetProcAddress("wglChoosePixelFormatARB");
        wglMakeCurrent(WINDOWS_OPENGL_DEVICECONTEXT, NULL);
        wglDeleteContext(GL); //Get rid of the dummy context now that all the modern GL function pointers are loaded!
        const int ContextattribList[] = { //Setup the actual context starting here.
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 2,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0, 
        };
        RGL = wglCreateContextAttribsARB(WINDOWS_OPENGL_DEVICECONTEXT, NULL, ContextattribList);
        wglMakeCurrent(WINDOWS_OPENGL_DEVICECONTEXT, RGL);
        const int PixelattribList[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_ARB_framebuffer_sRGB, GL_TRUE,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            0
        };
        int pixelformat;
        UINT numFormats;
        wglChoosePixelFormatARB(WINDOWS_OPENGL_DEVICECONTEXT, PixelattribList, NULL, 1, &pixelformat, &numFormats);
        ShowWindow(hwnd, nCmdShow);
        SetFocus(hwnd); //The real opengl context is now set for the program's lifespan!
        //Final opengl stuff. Just setting up any specific GL settings that are important to the program!
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(3.0f);
        glViewport(0, 0, settings.windowresWIDTH, settings.windowresHEIGHT);
        glDepthRange(0.1, 200.0);
#if debug
        printf("GL version: %s\n", glGetString(GL_VERSION));
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(&GLErrorDebugCallBack, NULL);
#endif
        {//SetupGLBuffer()
            GLuint DummyVertexArray;
            glGenVertexArrays(1, &DummyVertexArray);
            glBindVertexArray(DummyVertexArray);
            glGenBuffers(1, &WINDOWS_OPENGL_STREAM_VERTEX_BUFFER);
            glBindBuffer(GL_ARRAY_BUFFER, WINDOWS_OPENGL_STREAM_VERTEX_BUFFER);
            glBufferData(GL_ARRAY_BUFFER, WINDOWS_OPENGL_MAXVERTEXBUFFERSIZE, nullptr, GL_DYNAMIC_DRAW);
            glGenBuffers(1, &WINDOWS_OPENGL_STREAM_INDEX_BUFFER);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, WINDOWS_OPENGL_STREAM_INDEX_BUFFER);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, WINDOWS_OPENGL_MAXINDEXBUFFERSIZE,  nullptr, GL_DYNAMIC_DRAW);
            WINDOWS_OPENGL_BASIC_SHADER = glCreateProgram();
            void* fileio = (void*)stackpush(Kilobytes(2));
            GLuint vs = compileshader("src/shaders/Vert.shad", fileio);
            GLuint fs = compileshader("src/shaders/Frag.shad", fileio);
            stackpop(Kilobytes(2));
            glAttachShader(WINDOWS_OPENGL_BASIC_SHADER, vs);
            glAttachShader(WINDOWS_OPENGL_BASIC_SHADER, fs);
            glDeleteShader(vs);
            glDeleteShader(fs);
            glLinkProgram(WINDOWS_OPENGL_BASIC_SHADER);
#if debug
            queryprogramobject(WINDOWS_OPENGL_BASIC_SHADER);
#endif
        }
    }
    
#if debug
    {//SetupPrintf()    
        debug_printf.prntfFontData = (bit8*)VirtualAlloc(0, debugmemoryalloc, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
        {//LoadPrintf()
            void* file_io_memory = (void*)stackpush(Kilobytes(40000));
            debug_printf.fontdatasize  += loadttfcVersion2("fonts/meiryo.ttc", file_io_memory, debug_printf.prntfFontData);
            debug_printf.prntfRenderList = (bit32*)&debug_printf.prntfFontData[debug_printf.fontdatasize];
            stackpop(Kilobytes(40000));
        }
        {//Setup GL shaders 
            void* file_io_memory = (void*)stackpush(Kilobytes(3));
            debug_printf.prntfProgram = glCreateProgram();
            GLuint vs = compileshader("src/shaders/PrintfVert.shad", file_io_memory);
            GLuint fs = compileshader("src/shaders/PrintfFrag.shad", file_io_memory);
            glAttachShader(debug_printf.prntfProgram, vs);
            glAttachShader(debug_printf.prntfProgram, fs);
            glDeleteShader(vs);
            glDeleteShader(fs);
            glLinkProgram(debug_printf.prntfProgram);
            queryprogramobject(debug_printf.prntfProgram);
            stackpop(Kilobytes(3));
        }
        {//Setup ssbo for the vertex data buffering!
            glGenBuffers(1, &debug_printf.prntfSSBO);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, debug_printf.prntfSSBO);
            glBufferData(GL_SHADER_STORAGE_BUFFER, prntfsizeofvertexdata*sizeof(vec3), nullptr, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }
    }
#endif
    
    { //SetupRawInput()
        HWND hwnd = GetActiveWindow();
        RAWINPUTDEVICE inputdevices[WINDOWS_numberofinputdevices];
        inputdevices[0].usUsagePage = 1; inputdevices[0].usUsage = 2; inputdevices[0].dwFlags = 0; inputdevices[0].hwndTarget = hwnd; //Mouse
        inputdevices[1].usUsagePage = 1; inputdevices[1].usUsage = 6; inputdevices[1].dwFlags = 0; inputdevices[1].hwndTarget = hwnd; //Keyboard
        inputdevices[2].usUsagePage = 1; inputdevices[2].usUsage = 4; inputdevices[2].dwFlags = 0; inputdevices[2].hwndTarget = hwnd; //Joystick
        inputdevices[3].usUsagePage = 1; inputdevices[3].usUsage = 5; inputdevices[3].dwFlags = 0; inputdevices[3].hwndTarget = hwnd; //Gamepad
        if(RegisterRawInputDevices(inputdevices, WINDOWS_numberofinputdevices, sizeof(RAWINPUTDEVICE)) == FALSE)
        {
#if debug
            printf("ERROR, COULD NOT LOAD ALL THE DEVICES WITH RAWINPUT! ERRORCODE = %i\n", GetLastError());
#endif
        }
    }
    
    {//Setup mic
        IMMDeviceEnumerator* Enumerator;
        HRESULT HRESULTValue;
        HRESULTValue = CoInitialize(NULL);
        if( FAILED(HRESULTValue = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&Enumerator))) ) //Basically, get a pointer through the COM system to enumerate audio endpoint devices specifically.
        {
#if debug
            printf("ERROR, COULD NOT SETUP WASAPI @ CoCreateInstance\n"); Assert(0);
#endif
        }
        IMMDevice* MicOutput;
        if( FAILED( Enumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &MicOutput) ))
        {
#if debug
            printf("ERROR, COULD NOT SETUP MIC @ Enumerator->GetDefaultAudioEndpoint()"); Assert(0);
#endif
        }
        if( FAILED(MicOutput->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (LPVOID*)&WINDOWS_wasapi_micclient)) )
        {
#if debug
            printf("ERROR, COULD NOT SETUP MIC @ MicOutput->Activate()"); Assert(0);
#endif
        }
        WAVEFORMATEX* DefaultWaveFormat = nullptr;
        WINDOWS_wasapi_micclient->GetMixFormat(&DefaultWaveFormat);
        settings.micsamplerate = DefaultWaveFormat->nSamplesPerSec;
        Assert(settings.audiochannels == DefaultWaveFormat->nChannels);
        mic->micbuffer = (s16*)recordpush(settings.micsamplerate*2);
        WAVEFORMATEXTENSIBLE WaveFormat;
        WaveFormat.Format.cbSize = sizeof(WaveFormat);
        WaveFormat.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
        WaveFormat.Format.wBitsPerSample = sizeof(s16)*8;
        WaveFormat.Format.nChannels = DefaultWaveFormat->nChannels;
        WaveFormat.Format.nSamplesPerSec = DefaultWaveFormat->nSamplesPerSec; 
        WaveFormat.Format.nBlockAlign = (WORD)(WaveFormat.Format.nChannels * WaveFormat.Format.wBitsPerSample / 8);
        WaveFormat.Format.nAvgBytesPerSec = WaveFormat.Format.nSamplesPerSec * WaveFormat.Format.nBlockAlign;
        WaveFormat.Samples.wValidBitsPerSample = 16;
        WaveFormat.dwChannelMask = KSAUDIO_SPEAKER_STEREO;
        WaveFormat.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
        REFERENCE_TIME BufferDuration = 10000000ULL * settings.micsamplerate / settings.micsamplerate;
        bit32 test = 0;
        if( FAILED(test = WINDOWS_wasapi_micclient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_NOPERSIST, BufferDuration, 0, &WaveFormat.Format, nullptr)) ) //Tries to see if the current setup is fine to work with WASAPI
        {
#if debug
            switch(test)
            {
                case AUDCLNT_E_ALREADY_INITIALIZED: printf("AUDCLNT_E_ALREADY_INITIALIZED\n"); break;
                case AUDCLNT_E_WRONG_ENDPOINT_TYPE: printf("AUDCLNT_E_WRONG_ENDPOINT_TYPE\n"); break;
                case AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED: printf("AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED\n"); break;
                case AUDCLNT_E_BUFFER_SIZE_ERROR: printf("AUDCLNT_E_BUFFER_SIZE_ERROR\n"); break;
                case AUDCLNT_E_CPUUSAGE_EXCEEDED: printf("AUDCLNT_E_CPUUSAGE_EXCEEDED\n"); break;
                case AUDCLNT_E_DEVICE_INVALIDATED: printf("AUDCLNT_E_DEVICE_INVALIDATED\n"); break;
                case AUDCLNT_E_DEVICE_IN_USE: printf("AUDCLNT_E_DEVICE_IN_USE\n"); break;
                case AUDCLNT_E_ENDPOINT_CREATE_FAILED: printf("AUDCLNT_E_ENDPOINT_CREATE_FAILED\n"); break;
                case AUDCLNT_E_INVALID_DEVICE_PERIOD: printf("AUDCLNT_E_INVALID_DEVICE_PERIOD\n"); break;
                case AUDCLNT_E_UNSUPPORTED_FORMAT: printf("AUDCLNT_E_UNSUPPORTED_FORMAT\n"); break;
                case AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL: printf("AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL\n"); break;
                case AUDCLNT_E_SERVICE_NOT_RUNNING: printf("AUDCLNT_E_SERVICE_NOT_RUNNING\n"); break;
                case E_POINTER: printf("E_POINTER\n"); break;
                case E_INVALIDARG: printf("E_INVALIDARG\n"); break;
                case E_OUTOFMEMORY: printf("E_OUTOFMEMORY\n"); break;
            }
            printf("ERROR, COULD NOT SETUP WASAPI @ ptr->Initalize\n"); Assert(0);
#endif
        }
        WINDOWS_wasapi_micclient;
        bit32 reasonforfailure = 0;
        if( FAILED( reasonforfailure = WINDOWS_wasapi_micclient->GetService(__uuidof(IAudioCaptureClient), (void**)&WINDOWS_wasapi_audiocaptureclient) ) )
        {
#if debug
            switch(reasonforfailure)
            {
                case E_POINTER: printf("E_POINTER\n"); break;
                case E_NOINTERFACE: printf("E_NOINTERFACE\n"); break;
                case AUDCLNT_E_NOT_INITIALIZED: printf("AUDCLNT_E_NOT_INITIALIZED\n"); break;
                case AUDCLNT_E_WRONG_ENDPOINT_TYPE: printf("AUDCLNT_E_WRONG_ENDPOINT_TYPE\n"); break;
                case AUDCLNT_E_DEVICE_INVALIDATED: printf("AUDCLNT_E_DEVICE_INVALIDATED\n"); break;
                case AUDCLNT_E_SERVICE_NOT_RUNNING: printf("AUDCLNT_E_SERVICE_NOT_RUNNING"); break;
            }
            Assert(0);
#endif
        }
        bit32 MicFrameCounter = 0;
        if( FAILED(WINDOWS_wasapi_micclient->GetBufferSize(&MicFrameCounter)) )
        {
#if debug
            printf("MIC: Failed at WINDOWS_wasapi_micclient->GetBufferSize\n"); Assert(0);
#endif
        }
        if( SUCCEEDED(WINDOWS_wasapi_micclient->Start()) )
        {
#if debug
            printf("Mic: Setup was a success!\n");
#endif
        }
        else
        {
#if debug
            printf("FAILED TO SETUP MIC"); Assert(0);
#endif
        }
    }
    
    
    {//Setup Windows Frame Timer()
        { //SetWindowsSchedulerGranularity() This is used to ensure if the thread slept for 2 ms, it would wake up "instantly" instead of waiting ~15 ms for Windows to say wake up.
            UINT DesiredSchedulerGranularity = 1;
            if ( timeBeginPeriod(DesiredSchedulerGranularity) == TIMERR_NOERROR )
            {
                WINDOWS_oktosleep = 1;
            }
        }
        {//SetupWindowsClockRate()
            LARGE_INTEGER FrequencyCounter;
            if ( QueryPerformanceFrequency(&FrequencyCounter) == 0 )
            {
#if debug
                printf("Couldn't setup queryperformancefrequency\n");
#endif
            }
            else
            {
                WINDOWS_frequency = FrequencyCounter.QuadPart;
            }
        }
        QueryPerformanceCounter(&WINDOWS_StartCounter);
    }
    
    platform_launchthreads();
    
    //Start playing game!
    JJ_main();
    
    return 0;
}