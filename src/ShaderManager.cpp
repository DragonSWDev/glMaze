#include "ShaderManager.hpp"

#include "glad/glad.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

bool ShaderManager::loadShaders(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    std::string vertexShaderCode, fragmentShaderCode;
    std::ifstream vertexShaderFile, fragmentShaderFile;

    vertexShaderFile.open(vertexShaderPath);
    fragmentShaderFile.open(fragmentShaderPath);

    if (vertexShaderFile.is_open() && fragmentShaderFile.is_open())
    {
        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexShaderCode = vertexShaderStream.str();
        fragmentShaderCode = fragmentShaderStream.str();
    }
    else
    {
        return false;
    }
    
    const char *vertexCode = vertexShaderCode.c_str();
    const char *fragmentCode = fragmentShaderCode.c_str();

    GLint success;
    GLchar compileLog[512];

    GLuint vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    //Vertex shader compilation failed
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, compileLog);
        std::cerr << "Vertex shader compilation failed!" << std::endl << "Log: " << compileLog << std::endl;
        return false;
    }

    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, compileLog);
        std::cerr << "Fragment shader compilation failed!" << std::endl << "Log: " << compileLog << std::endl;
        return false;
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(programID, 512, NULL, compileLog);
        std::cerr << "Program link compilation failed!" << std::endl << "Log: " << compileLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void ShaderManager::useShader()
{
    glUseProgram(programID);
}

void ShaderManager::setUniformMatrix4fv(const char* name, glm::mat4 uniform)
{
    GLint uniformLocation = glGetUniformLocation(programID, name);

    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(uniform));
}

void ShaderManager::setUniformVec3fv(const char *name, glm::vec3 uniform)
{
    GLint uniformLocation = glGetUniformLocation(programID, name);

    glUniform3fv(uniformLocation, 1, glm::value_ptr(uniform));
}
