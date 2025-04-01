#pragma once
#include <GL/gl.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "glext.h"
#include "glxext.h"
#include "camera.h"

Display* display;
Window win;
GLXContext glc;
XEvent xev;

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
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM1IPROC glUniform1i;

void InitGLFunctions()
{
    glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress((const unsigned char*)"glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((const unsigned char*)"glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((const unsigned char*)"glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((const unsigned char*)"glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((const unsigned char*)"glGetShaderInfoLog");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress((const unsigned char*)"glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((const unsigned char*)"glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((const unsigned char*)"glGetProgramInfoLog");
    glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((const unsigned char*)"glUseProgram");
    glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((const unsigned char*)"glDeleteShader");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((const unsigned char*)"glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const unsigned char*)"glEnableVertexAttribArray");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((const unsigned char*)"glGenVertexArrays");
    glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glGenBuffers");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((const unsigned char*)"glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const unsigned char*)"glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const unsigned char*)"glBufferData");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((const unsigned char*)"glDeleteVertexArrays");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((const unsigned char*)"glDeleteBuffers");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress((const unsigned char*)"glGenerateMipmap");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((const unsigned char*)"glUniformMatrix4fv");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((const unsigned char*)"glGetUniformLocation");
    glUniform3f = (PFNGLUNIFORM3FPROC)glXGetProcAddress((const unsigned char*)"glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)glXGetProcAddress((const unsigned char*)"glUniform4f");
    glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)glXGetProcAddress((const unsigned char*)"glVertexAttribDivisor");
    glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)glXGetProcAddress((const unsigned char*)"glDrawElementsInstanced");
    glUniform1f = (PFNGLUNIFORM1FPROC)glXGetProcAddress((const unsigned char*)"glUniform1f");
    glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress((const unsigned char*)"glUniform1i");
}

void InitGL()
{
  InitGLFunctions();
  
  glViewport(0, 0, window_width, window_height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
}

void InitDisplay()
{
    display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);
    s32 att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo* vi = glXChooseVisual(display, 0, att);
    Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;
    win = XCreateWindow(display, root, 0, 0, window_width, window_height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(display, win);
    XStoreName(display, win, window_title);
    glc = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, win, glc);
}

void ProcessInput(Camera& cam)
{
if(XCheckWindowEvent(display, win, KeyPressMask, &xev)) 
      {
        u32 keycode = xev.xkey.keycode;
        if (keycode == 25)
        {
          cam.position += cam.speed * cam.front;
        }
        else if (keycode == 39)
        {
          cam.position -= cam.speed * cam.front;
        }
        else if (keycode == 38)
        {
        cam.position -= cam.speed * vec3::normalize(vec3::cross(cam.front, cam.up));
        }
        else if (keycode == 40)
        {
        cam.position += cam.speed * vec3::normalize(vec3::cross(cam.front, cam.up));
        }
        else if (keycode == 9)
        {
        glXMakeCurrent(display, None, NULL);
        glXDestroyContext(display, glc);
        XDestroyWindow(display, win);
        XCloseDisplay(display);
        running = false;
        }
      }
}

void SwapGLBuffers()
{
  glXSwapBuffers(display, win);
}
