#ifndef COMMON_H
#define COMMON_H
#include "../include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"

#include <iostream>

#define CUBO_DEBUG

#define AND &&
#define OR ||
#define TRUE 1
#define FALSE 0

typedef unsigned char uchar;
typedef unsigned long usize;

namespace mycolor {
// 17 incredible colors aveiables
inline const glm::vec3 red(1.0f, 0.0f, 0.0f);
inline const glm::vec3 maroon(0.5f, 0.0f, 0.0f);
inline const glm::vec3 baige(0.8f, 0.7f, 0.5f);
inline const glm::vec3 lime(0.0f, 1.0f, 0.0f);
inline const glm::vec3 green(0.0f, 0.5f, 0.0f);
inline const glm::vec3 blue(0.0f, 0.0f, 1.0f);
inline const glm::vec3 teal(0.0f, 0.5f, 0.5f);
inline const glm::vec3 navy(0.0f, 0.0f, 0.5f);
inline const glm::vec3 yellow(1.0f, 1.0f, 0.0f);
inline const glm::vec3 orange(1.0f, 0.5f, 0.0f);
inline const glm::vec3 magenta(1.0f, 0.0f, 1.0f);
inline const glm::vec3 purple(0.5f, 0.0f, 0.5f);
inline const glm::vec3 cyan(0.0f, 1.0f, 1.0f);
inline const glm::vec3 grey(0.5f, 0.5f, 0.5f);
inline const glm::vec3 peach(1.0f, 0.8f, 0.7f);
inline const glm::vec3 white(1.0f, 1.0f, 1.0f);
inline const glm::vec3 black(0.0f, 0.0f, 0.0f);
}  // namespace mycolor

inline const float cubevertices[] = {
    // world space positions of our cubes
    // Front face (z = +0.5, normal = (0, 0, 1))
    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,
    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,
    0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,
    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,
    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    0.0f,
    1.0f,

    // Back face (z = -0.5, normal = (0, 0, -1))
    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    0.0f,
    -1.0f,
    0.5f,
    0.5f,
    -0.5f,
    0.0f,
    0.0f,
    -1.0f,
    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    0.0f,
    -1.0f,

    0.5f,
    0.5f,
    -0.5f,
    0.0f,
    0.0f,
    -1.0f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    0.0f,
    -1.0f,
    -0.5f,
    0.5f,
    -0.5f,
    0.0f,
    0.0f,
    -1.0f,

    // Left face (x = -0.5, normal = (-1, 0, 0))
    -0.5f,
    -0.5f,
    -0.5f,
    -1.0f,
    0.0f,
    0.0f,
    -0.5f,
    -0.5f,
    0.5f,
    -1.0f,
    0.0f,
    0.0f,
    -0.5f,
    0.5f,
    0.5f,
    -1.0f,
    0.0f,
    0.0f,

    -0.5f,
    0.5f,
    0.5f,
    -1.0f,
    0.0f,
    0.0f,
    -0.5f,
    0.5f,
    -0.5f,
    -1.0f,
    0.0f,
    0.0f,
    -0.5f,
    -0.5f,
    -0.5f,
    -1.0f,
    0.0f,
    0.0f,

    // Right face (x = +0.5, normal = (1, 0, 0))
    0.5f,
    -0.5f,
    -0.5f,
    1.0f,
    0.0f,
    0.0f,
    0.5f,
    0.5f,
    0.5f,
    1.0f,
    0.0f,
    0.0f,
    0.5f,
    -0.5f,
    0.5f,
    1.0f,
    0.0f,
    0.0f,

    0.5f,
    0.5f,
    0.5f,
    1.0f,
    0.0f,
    0.0f,
    0.5f,
    -0.5f,
    -0.5f,
    1.0f,
    0.0f,
    0.0f,
    0.5f,
    0.5f,
    -0.5f,
    1.0f,
    0.0f,
    0.0f,

    // Top face (y = +0.5, normal = (0, 1, 0))
    -0.5f,
    0.5f,
    -0.5f,
    0.0f,
    1.0f,
    0.0f,
    -0.5f,
    0.5f,
    0.5f,
    0.0f,
    1.0f,
    0.0f,
    0.5f,
    0.5f,
    0.5f,
    0.0f,
    1.0f,
    0.0f,

    0.5f,
    0.5f,
    0.5f,
    0.0f,
    1.0f,
    0.0f,
    0.5f,
    0.5f,
    -0.5f,
    0.0f,
    1.0f,
    0.0f,
    -0.5f,
    0.5f,
    -0.5f,
    0.0f,
    1.0f,
    0.0f,

    // Bottom face (y = -0.5, normal = (0, -1, 0))
    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,
    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,
    -0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,

    0.5f,
    -0.5f,
    0.5f,
    0.0f,
    -1.0f,
    0.0f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,
    0.5f,
    -0.5f,
    -0.5f,
    0.0f,
    -1.0f,
    0.0f,
};

uint createandstuffshaderprogram(const char* vertexShaderCode,
                                 const char* fragmentShaderCode);

void setBool(uint& shaderprogramm, const std::string& name, bool value);
void setInt(uint& shaderprogramm, const std::string& name, int value);
void setFloat(uint& shaderprogramm, const std::string& name, float value);
void setVec2(uint& shaderprogramm, const std::string& name,
             const glm::vec2& value);
void setVec2(uint& shaderprogramm, const std::string& name, float x, float y);
void setVec3(uint& shaderprogramm, const std::string& name,
             const glm::vec3& value);
void setVec3(uint& shaderprogramm, const std::string& name, float x, float y,
             float z);
void setVec4(uint& shaderprogramm, const std::string& name,
             const glm::vec4& value);
void setVec4(uint& shaderprogramm, const std::string& name, float x, float y,
             float z, float w);
void setMat2(uint& shaderprogramm, const std::string& name,
             const glm::mat2& mat);
void setMat3(uint& shaderprogramm, const std::string& name,
             const glm::mat3& mat);
void setMat4(uint& shaderprogramm, const std::string& name,
             const glm::mat4& mat);
#endif
