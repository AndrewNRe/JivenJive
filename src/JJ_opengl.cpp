/*
Notes on this file:
This loads the GL function pointers on Windows. Because Windows made GL a "black sheep" for "reasons".
*/

typedef char GLchar;
typedef unsigned int GLenum;
#ifdef _WIN64 //this is baked into the msvc compiler. So dependent on if x86 or x64, this code path is taken (x64 in this case).
typedef signed long long int GLintptr;
typedef signed long long int GLsizeiptr;
#else
typedef signed long int GLintptr;
typedef signed long int GLsizeiptr;
#endif

#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_TESS_CONTROL_SHADER            0x8E88
#define GL_TESS_EVALUATION_SHADER         0x8E87
#define GL_COMPILE_STATUS                 0x8B81
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                          0
#define GL_TRUE                           1
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_TRIANGLES                      0x0004
#define GL_POINT_SIZE                     0x0B11
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_LINEAR                         0x2601
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_RGBA                           0x1908
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_TEXTURE0                       0x84C0
#define GL_LINK_STATUS                    0x8B82
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_BLOCKS          0x8A36
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_FLOAT                          0x1406
#define GL_RGBA32F                        0x8814
#define GL_RGBA8                          0x8058
#define GL_STATIC_DRAW                    0x88E4
#define GL_FRAMEBUFFER                    0x8D40
#define GL_DEPTH32F_STENCIL8              0x8CAD
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_SHADER_STORAGE_BUFFER          0x90D2
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_TEXTURE_1D_ARRAY               0x8C18
#define GL_PATCHES                        0x000E
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_PROGRAM_POINT_SIZE             0x8642
#define GL_MULTISAMPLE                    0x809D
#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020
#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
#define GL_STREAM_DRAW                    0x88E0
#define GL_MAP_WRITE_BIT                  0x0002

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY*
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC) (GLenum type);
PFNGLCREATESHADERPROC glCreateShader = NULL;
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
PFNGLATTACHSHADERPROC glAttachShader = NULL; 
typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
PFNGLDELETESHADERPROC glDeleteShader = NULL;
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
PFNGLBUFFERDATAPROC glBufferData = NULL;
typedef void (APIENTRYP PFNGLCLEARBUFFERFVPROC) (GLenum buffer, GLint drawbuffer, const GLfloat *value);
PFNGLCLEARBUFFERFVPROC glClearBufferfv = NULL;
typedef void (APIENTRYP PFNGLCLEARBUFFERIVPROC) (GLenum buffer, GLint drawbuffer, const GLint *value);
PFNGLCLEARBUFFERIVPROC glClearBufferiv = NULL;
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;

#if debug
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
typedef void (APIENTRYP PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
PFNGLBINDBUFFERBASEPROC glBindBufferBase = NULL;
typedef void (APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
PFNGLUNIFORM1FPROC glUniform1f = NULL;
typedef void (APIENTRYP PFNGLUNIFORM1UIPROC) (GLint location, GLuint v0);
PFNGLUNIFORM1UIPROC glUniform1ui = NULL;
typedef void (APIENTRYP GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (APIENTRYP PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, const void *userParam);
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = NULL;
#endif

void* GetAnyGLFuncAddress(const char *name)
{
    void *p = (void *)wglGetProcAddress(name);
    if(p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1) )
    { //Because if wgl fails to get the function pointer. THen you need to load it from the dll.
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void *)GetProcAddress(module, name);
    }
    return p;
}

void platform_WINDOWS_gl_functions_load()
{
    glUseProgram = (PFNGLUSEPROGRAMPROC)GetAnyGLFuncAddress("glUseProgram");
    glCreateShader = (PFNGLCREATESHADERPROC)GetAnyGLFuncAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)GetAnyGLFuncAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)GetAnyGLFuncAddress("glCompileShader");
    glBindBuffer = (PFNGLBINDBUFFERPROC)GetAnyGLFuncAddress("glBindBuffer");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)GetAnyGLFuncAddress("glBufferSubData");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)GetAnyGLFuncAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)GetAnyGLFuncAddress("glBindVertexArray");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)GetAnyGLFuncAddress("glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)GetAnyGLFuncAddress("glVertexAttribPointer");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)GetAnyGLFuncAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)GetAnyGLFuncAddress("glAttachShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)GetAnyGLFuncAddress("glDeleteShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)GetAnyGLFuncAddress("glLinkProgram");
    glGenBuffers = (PFNGLGENBUFFERSPROC)GetAnyGLFuncAddress("glGenBuffers");
    glBufferData = (PFNGLBUFFERDATAPROC)GetAnyGLFuncAddress("glBufferData");
    glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)GetAnyGLFuncAddress("glClearBufferfv");
    glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)GetAnyGLFuncAddress("glClearBufferiv");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)GetAnyGLFuncAddress("glDisableVertexAttribArray");
#if debug
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)GetAnyGLFuncAddress("glGetShaderInfoLog");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)GetAnyGLFuncAddress("glGetShaderiv");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)GetAnyGLFuncAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)GetAnyGLFuncAddress("glGetProgramInfoLog");
    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)GetAnyGLFuncAddress("glBindBufferBase");
    glUniform1f = (PFNGLUNIFORM1FPROC)GetAnyGLFuncAddress("glUniform1f");
    glUniform1ui = (PFNGLUNIFORM1UIPROC)GetAnyGLFuncAddress("glUniform1ui");
    glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)GetAnyGLFuncAddress("glDebugMessageCallback");
#endif
}


GLuint compileshader(const char* filepath, void* file_io_memory)
{
    bit32 bytesread = platform_fileload(filepath, file_io_memory);
    ((bit8*)file_io_memory)[bytesread] = 0;
    bit8 c = 0; GLuint shader = 0; s8* filedata = nullptr;
    for (bit16 i = 0; i < bytesread; i++)
    {
        c = ((bit8*)file_io_memory)[i];
        if (c == '~')
        {
            c = ((bit8*)file_io_memory)[i + 1];
            switch (c)
            {
                case 'v':
                shader = glCreateShader(GL_VERTEX_SHADER);
                break;
                case 'f':
                shader = glCreateShader(GL_FRAGMENT_SHADER);
                break;
                case 'c':
                shader = glCreateShader(GL_TESS_CONTROL_SHADER);
                break;
                case 'e':
                shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
                break;
            }
            filedata = &((s8*)file_io_memory)[i + 2];
            break;
        }
    }
    
    const char* GLconstcharstuff[1] = { (const char*)filedata };
    
    glShaderSource(shader, 1, (char**)GLconstcharstuff, NULL);
    glCompileShader(shader);
    
#if debug
    GLint didcompile = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &didcompile);
    if (didcompile == GL_FALSE)
    {
        GLint logsize = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
        char* logmsg = (char*)stackpush(logsize);
        glGetShaderInfoLog(shader, logsize, NULL, logmsg);
        printf("%s\n", logmsg); 
        stackpop(logsize);
    }
#endif
    return shader;
}

#if debug

void queryprogramobject(GLuint &program)
{
    GLint linkstatus = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkstatus);
    if (linkstatus == GL_FALSE)
    {
        GLint err_strlen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &err_strlen);
        char* log_str = (char*)stackpush(err_strlen);
        glGetProgramInfoLog(program, err_strlen, NULL, log_str);
        printf("%s\n", log_str);
        stackpop(err_strlen);
    }
    GLint queryprogram = 0;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &queryprogram);
    printf("Attatched Shaders = %i\n", queryprogram);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &queryprogram);
    printf("Active Attributes = %i\n", queryprogram);
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &queryprogram);
    printf("Active Uniforms = %i\n", queryprogram);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &queryprogram);
    printf("Active Uniform Blocks = %i\n", queryprogram);
}

#endif