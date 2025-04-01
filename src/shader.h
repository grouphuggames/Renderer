#pragma once
#include "basics.h"
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>

#ifdef _WIN32
#include "win32-gl.h"
#else
#include "x11-gl.h"
#endif

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
