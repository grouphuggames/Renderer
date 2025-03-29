#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <gl/GL.h>
#include "glext.h"
#include "wglext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using u32 = uint32_t;
using s32 = int32_t;
using f32 = float;

const f32 pi = 3.14159265f;

inline f32 DegreesToRadians(f32 degrees) { return degrees * pi / 180.f; }

bool running = true;
const char* CLASS_NAME = "Window Class";
const char* window_title = "Renderer";
s32 window_width = 2560;
s32 window_height = 1080;
bool wireframe = false;
HGLRC render_context;
HWND render_window;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
    }
    return 0;
    case WM_DESTROY:
    {
        running = false;
        PostQuitMessage(0);
        return 0;
    }
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_SPACE:
        {
            if (wireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireframe = false;
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireframe = true;
            }
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void CreateWin32DebugConsole()
{
    LPCTSTR console_name = "Debug Console";
    AllocConsole();
    SetConsoleTitle(console_name);
    freopen("CONOUT$", "w", stdout);
}

void CreateWin32Surface(HINSTANCE hInstance, s32 nShowCmd, s32 width, s32 height, bool fullscreen)
{
    LPCTSTR window_name = "Renderer";
    const char* window_title = "Renderer";

    RECT rect = { 0, 0, width, height };
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    AdjustWindowRectEx(&rect, style, FALSE, ex_style);

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = window_name;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&wc);

    render_window = CreateWindowEx(NULL, window_name, window_title, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
    if (!render_window)
    {
    }

    ShowWindow(render_window, nShowCmd);
    UpdateWindow(render_window);
}

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

const s32 pixel_format_attrib_list[] =
{
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 24,
    WGL_STENCIL_BITS_ARB, 8,
    0
};

const s32 context_attrib_list[] =
{
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
    0
};

void InitGLFunctions()
{
    glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
    glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress("glVertexAttribDivisor");
    glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
    glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
}

bool InitGL()
{

    PIXELFORMATDESCRIPTOR pfd =
    {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      32,
      0, 0, 0, 0, 0, 0,
      0,
      0,
      0,
      0, 0, 0, 0,
      24,
      8,
      0,
      PFD_MAIN_PLANE,
      0,
      0, 0, 0
    };

    HDC device_context = GetDC(render_window);
    s32 tmp_pixel_format_index = ChoosePixelFormat(device_context, &pfd);

    if (tmp_pixel_format_index == 0)
    {
        return false;
    }

    if (!SetPixelFormat(device_context, tmp_pixel_format_index, &pfd))
    {
        return false;
    }

    HGLRC tmp_render_context = wglCreateContext(device_context);
    if (tmp_render_context == NULL)
    {
        return false;
    }

    wglMakeCurrent(device_context, tmp_render_context);
    InitGLFunctions();

    s32 pixel_format_index;
    u32 num_formats;

    wglChoosePixelFormatARB(device_context, pixel_format_attrib_list, NULL, 1, &pixel_format_index, &num_formats);

    if (!SetPixelFormat(device_context, pixel_format_index, &pfd))
    {
        return false;
    }

    render_context = wglCreateContextAttribsARB(device_context, NULL, context_attrib_list);

    wglDeleteContext(tmp_render_context);
    wglMakeCurrent(device_context, render_context);
    glViewport(0, 0, window_width, window_height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

class vec2
{
public:
    vec2() : x(0.f), y(0.f) {}
    vec2(f32 _x, f32 _y) : x(_x), y(_y) {}

    friend vec2 operator+(const vec2& left, const vec2& right)
    {
        return vec2(left.x + right.x, left.y + right.y);
    }

    friend vec2 operator-(const vec2& left, const vec2& right)
    {
        return vec2(left.x - right.x, left.y - right.y);
    }

    friend vec2 operator*(const f32 left, const vec2& right)
    {
        return vec2(right.x * left, right.y * left);
    }

    friend bool operator==(const vec2& a, const vec2 b)
    {
        return (a.x == b.x && a.y == b.y);
    }

    f32 x, y;
};

class vec3
{
public:
    vec3() : x(0.f), y(0.f), z(0.f) {}
    vec3(f32 val) : x(val), y(val), z(val) {}
    vec3(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z) {}

    static f32 length(const vec3& vec)
    {
        return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    static vec3 normalize(const vec3& vec)
    {
        f32 k = length(vec);
        return vec3(vec.x / k, vec.y / k, vec.z / k);
    }

    static f32 dot(const vec3& u, const vec3& v)
    {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    static vec3 cross(const vec3& u, const vec3& v)
    {
        return vec3(u.y * v.z - u.z * v.y, -(u.x * v.z - u.z * v.x), u.x * v.y - u.y * v.x);
    }

    friend bool operator==(const vec3& a, const vec3 b)
    {
        return (a.x == b.x && a.y == b.y && a.z == b.z);
    }

    friend vec3 operator*(const f32 left, const vec3& right)
    {
        return vec3(right.x * left, right.y * left, right.z * left);
    }

    vec3 operator*(const vec3& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;

        return *this;
    }

    friend vec3 operator+(const vec3& left, const vec3& right)
    {
        return vec3(left.x + right.x, left.y + right.y, left.z + right.z);
    }

    friend vec3 operator-(const vec3& left, const vec3& right)
    {
        return vec3(left.x - right.x, left.y - right.y, left.z - right.z);
    }

    vec3& operator+=(const vec3& other)
    {
        *this = *this + other;
        return *this;
    }

    vec3& operator-=(const vec3& other)
    {
        *this = *this - other;
        return *this;
    }

    f32 x, y, z;
};

class vec4
{
public:
    vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    vec4(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(_w) {}

    f32 x, y, z, w;
};

class mat4
{
public:
    mat4() {}

    static mat4 Identity()
    {
        mat4 result;

        result.elements[0] = 1.f;
        result.elements[5] = 1.f;
        result.elements[10] = 1.f;
        result.elements[15] = 1.f;

        return result;
    }

    // static mat4 Orthographic(f32 left, f32 right, f32 bottom, f32 top)
    // {
    //   mat4 result = mat4::Identity();

    //   result.elements[0] = 2.f / (right - left);
    //   result.elements[3] = -(right + left) / (right - left);
    //   result.elements[5] = 2.f / (top - bottom);
    //   result.elements[7] = -(top + bottom) / (top - bottom);
    //   result.elements[10] = -1.f;

    //   return result;
    // }

    static mat4 Perspective(f32 fov, f32 aspect, f32 _near, f32 _far)
    {
        mat4 result;
        f32 q = 1.f / tanf(0.5f * fov);

        result.elements[0] = q / aspect;
        result.elements[5] = q;
        result.elements[10] = _far / (_near - _far);
        result.elements[11] = -1.f;
        result.elements[14] = (2 * _near * _far) / (_near - _far);

        return result;
    }

    static mat4 LookAt(vec3 position, vec3 target, vec3 up)
    {
        vec3 a = vec3::normalize(target - position);
        vec3 b = vec3::normalize(vec3::cross(a, up));
        vec3 c = vec3::cross(b, a);

        mat4 result = mat4::Identity();
        result.elements[0] = b.x;
        result.elements[1] = c.x;
        result.elements[2] = -a.x;
        result.elements[4] = b.y;
        result.elements[5] = c.y;
        result.elements[6] = -a.y;
        result.elements[8] = b.z;
        result.elements[9] = c.z;
        result.elements[10] = -a.z;
        result.elements[12] = -1.f * vec3::dot(b, position);
        result.elements[13] = -1.f * vec3::dot(c, position);
        result.elements[14] = vec3::dot(a, position);

        return result;
    }

    static mat4 Translate(const vec3& translation)
    {
        mat4 result = Identity();

        result.elements[12] = translation.x;
        result.elements[13] = translation.y;
        result.elements[14] = translation.z;

        return result;
    }

    static mat4 Rotate(f32 angle, const vec3& axis)
    {
        f32 sine = sinf(angle);
        f32 cosine = cosf(angle);
        f32 one_minus_cosine = 1.f - cosine;
        vec3 norm_axis = vec3::normalize(axis);

        mat4 result = mat4::Identity();

        result.elements[0] = cosine + norm_axis.x * norm_axis.x * one_minus_cosine;
        result.elements[1] = norm_axis.x * norm_axis.y * one_minus_cosine + norm_axis.z * sine;
        result.elements[2] = norm_axis.x * norm_axis.z * one_minus_cosine - norm_axis.y * sine;
        result.elements[4] = norm_axis.y * norm_axis.x * one_minus_cosine - norm_axis.z * sine;
        result.elements[5] = cosine + norm_axis.y * norm_axis.y * one_minus_cosine;
        result.elements[6] = norm_axis.y * norm_axis.z * one_minus_cosine + norm_axis.x * sine;
        result.elements[8] = norm_axis.z * norm_axis.x * one_minus_cosine + norm_axis.y * sine;
        result.elements[9] = norm_axis.z * norm_axis.y * one_minus_cosine - norm_axis.x * sine;
        result.elements[10] = cosine + norm_axis.z * norm_axis.z * one_minus_cosine;
        result.elements[15] = 1.f;

        return result;
    }

    static mat4 Scale(const vec3& scale)
    {
        mat4 result = Identity();

        result.elements[0] = scale.x;
        result.elements[5] = scale.y;
        result.elements[10] = scale.z;

        return result;
    }

    friend mat4 operator*(mat4 left, const mat4& right)
    {
        mat4 result;

        for (s32 y = 0; y < 4; y++)
        {
            for (s32 x = 0; x < 4; x++)
            {
                f32 sum = 0.f;

                for (s32 e = 0; e < 4; e++)
                {
                    sum += left.elements[x + e * 4] * right.elements[e + y * 4];
                }

                result.elements[x + y * 4] = sum;
            }
        }

        return result;
    }

    mat4& operator*=(const mat4& other)
    {
        *this = *this * other;
        return *this;
    }

    f32 elements[16] = {};
};

enum class SHADER_TYPE
{
    NONE = -1,
    VERTEX,
    FRAGMENT
};

u32 CreateGLShader(const char* filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream shader_streams[2];
    SHADER_TYPE type = SHADER_TYPE::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = SHADER_TYPE::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = SHADER_TYPE::FRAGMENT;
            }
            else
            {
                char error_type[128] = "Unsupported shader type: ";
                strcat(error_type, line.c_str());
            }
        }
        else
        {
            shader_streams[(s32)type] << line << '\n';
        }
    }

    stream.close();

    u32 vs = glCreateShader(GL_VERTEX_SHADER);
    std::string vs_source_data = shader_streams[(s32)SHADER_TYPE::VERTEX].str();
    const char* vs_source = vs_source_data.c_str();
    glShaderSource(vs, 1, &vs_source, nullptr);
    glCompileShader(vs);
    s32 success = 0;
    char log_info[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vs, 512, nullptr, log_info);
    }

    u32 fs = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fs_source_data = shader_streams[(s32)SHADER_TYPE::FRAGMENT].str().c_str();
    const char* fs_source = fs_source_data.c_str();
    glShaderSource(fs, 1, &fs_source, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fs, 512, nullptr, log_info);
    }

    u32 program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, log_info);
    }

    glUseProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    shader_streams[0].str("");
    shader_streams[1].str("");

    return program;
}

s32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, s32 nCmdShow)
{
    CreateWin32DebugConsole();
    CreateWin32Surface(hInstance, nCmdShow, window_width, window_height, false);
    InitGL();

    glEnable(GL_DEPTH_TEST);

    wglSwapIntervalEXT(1);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    vec3 cubePositions[] = {
    vec3(0.0f,  0.0f,  0.0f),
    vec3(2.0f,  5.0f, -15.0f),
    vec3(-1.5f, -2.2f, -2.5f),
    vec3(-3.8f, -2.0f, -12.3f),
    vec3(2.4f, -0.4f, -3.5f),
    vec3(-1.7f,  3.0f, -7.5f),
    vec3(1.3f, -2.0f, -2.5f),
    vec3(1.5f,  2.0f, -2.5f),
    vec3(1.5f,  0.2f, -1.5f),
    vec3(-1.3f,  1.0f, -1.5f)
    };

    u32 shader = CreateGLShader("../res/texture.glsl");

    u32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    u32 vbo, ebo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    u32 texture;
    s32 tex_width, tex_height, tex_num_channels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* tex_data = stbi_load("../res/container.jpg", &tex_width, &tex_height, &tex_num_channels, 0);
    if (tex_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(tex_data);

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "_texture"), 0);

    //f32 angle = 0.f;

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (running)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shader);
        glBindVertexArray(vao);

        for (u32 i = 0; i < 10; i++)
        {
            f32 angle = 20.f * i;
            mat4 model = mat4::Identity();
            model *= mat4::Translate(cubePositions[i]);
            model *= mat4::Rotate(DegreesToRadians(angle), vec3(1.f, 0.3f, 0.5f));

            mat4 view = mat4::Identity();
            view *= mat4::Translate(vec3(0.f, 0.f, -3.f));

            mat4 projection = mat4::Perspective(DegreesToRadians(45.f), (f32)window_width / (f32)window_height, 0.1f, 100.f);

            glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model.elements);
            glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view.elements);
            glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection.elements);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //mat4 model = mat4::Identity();
        //model *= mat4::Rotate(angle, vec3(0.5f, 1.f, 0.f));
        //
        //mat4 view = mat4::Identity();
        //view *= mat4::Translate(vec3(0.f, 0.f, -3.f));
        //
        //mat4 projection = mat4::Perspective(DegreesToRadians(45.f), (f32)window_width / (f32)window_height, 0.1f, 100.f);
        //
        //glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model.elements);
        //glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view.elements);
        //glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection.elements);
        
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glUseProgram(0);

        //angle += 0.003f;

        SwapBuffers(GetDC(render_window));
    }

	return 0;
}
