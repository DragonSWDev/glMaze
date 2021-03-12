#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <glm/glm.hpp>

class ShaderManager
{
    private:
    unsigned int programID;

    public:
    bool loadShaders(const char *vertexShaderPath, const char *fragmentShaderPath);
    void useShader();
    void setUniformMatrix4fv(const char *name, glm::mat4 uniform);
    void setUniformVec3fv(const char *name, glm::vec3 uniform);
};

#endif
