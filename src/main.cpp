#include "basics.h"
#include "math.h"
#include "shader.h"
#include "camera.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

u32 shader, vao, vbo, ebo, texture;

Camera camera;

void InitScene()
{
    shader = CreateGLShader("../res/texture.glsl");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

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
}

void MainLoop()
{
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
            view = mat4::LookAt(camera.position, camera.position + camera.front, camera.up);

            mat4 projection = mat4::Perspective(DegreesToRadians(45.f), (f32)window_width / (f32)window_height, 0.1f, 100.f);

            glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model.elements);
            glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, view.elements);
            glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection.elements);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
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
