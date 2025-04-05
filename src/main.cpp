#include "basics.h"
#include "math.h"
#include "shader.h"
#include "camera.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

u32 shader, light_shader, vao, light_vao, vbo;
vec3 light_color = vec3(1.f, 1.f, 1.f);
vec3 object_color = vec3(1.f, 0.5f, 0.31f);
vec3 light_position(1.2f, 1.f, 2.f);
Camera camera;

void InitScene()
{
    camera.position.y += 1.f;
    camera.position.z += 2.f;
    shader = CreateGLShader("../res/basic.glsl");
    light_shader = CreateGLShader("../res/light.glsl");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shader);
}

void MainLoop()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUseProgram(shader);
  glBindVertexArray(vao);

  mat4 model = mat4::Identity();
  
  mat4 view = mat4::Identity();
  view = mat4::LookAt(camera.position, camera.position + camera.front, camera.up);
  
  mat4 projection = mat4::Perspective(DegreesToRadians(45.f), (f32)window_width / (f32)window_height, 0.1f, 100.f);
  
  glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model.elements);
  glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view.elements);
  glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection.elements);

  glUniform3f(glGetUniformLocation(shader, "object_color"), object_color.x, object_color.y, object_color.z);
  glUniform3f(glGetUniformLocation(shader, "light_color"), light_color.x, light_color.y, light_color.z);
  glUniform3f(glGetUniformLocation(shader, "light_position"), light_position.x, light_position.y, light_position.z);
  glUniform3f(glGetUniformLocation(shader, "view_position"), camera.position.x, camera.position.y, camera.position.z);
  
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glUseProgram(0);

  glUseProgram(light_shader);
  glBindVertexArray(light_vao);

  model = mat4::Identity();
  model *= mat4::Translate(light_position);
  model *= mat4::Scale(vec3(0.2f, 0.2f, 0.2f));

  view = mat4::LookAt(camera.position, camera.position + camera.front, camera.up);
  projection = mat4::Perspective(DegreesToRadians(45.f), (f32)window_width / (f32)window_height, 0.1f, 100.f);

  glUniformMatrix4fv(glGetUniformLocation(light_shader, "model"), 1, GL_FALSE, model.elements);
  glUniformMatrix4fv(glGetUniformLocation(light_shader, "view"), 1, GL_FALSE, view.elements);
  glUniformMatrix4fv(glGetUniformLocation(light_shader, "projection"), 1, GL_FALSE, projection.elements);

  glDrawArrays(GL_TRIANGLES, 0, 36);
  glUseProgram(0);
}

#ifdef _WIN32
s32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, s32 nCmdShow)
{
    CreateWin32DebugConsole();
    CreateWin32Surface(hInstance, nCmdShow, window_width, window_height, false);
    InitGL();

    InitScene();
#else
s32 main()
{
  InitDisplay();
    
    InitGL();
    InitScene();
#endif

    while (running)
    {
        ProcessInput(camera);
        MainLoop();
        SwapGLBuffers();
    }

    return 0;
}
