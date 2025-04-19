#ifndef COMMON_H
#define COMMON_H
#include "../include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"

#include <string>

#define AND &&
#define OR ||

typedef unsigned char uchar;
typedef unsigned long usize;

namespace mycolor {
// 17 incredible colors aveiables
static const glm::vec3 red(1.0f, 0.0f, 0.0f);
static const glm::vec3 maroon(0.5f, 0.0f, 0.0f);
static const glm::vec3 baige(0.8f, 0.7f, 0.5f);
static const glm::vec3 lime(0.0f, 1.0f, 0.0f);
static const glm::vec3 green(0.0f, 0.5f, 0.0f);
static const glm::vec3 blue(0.0f, 0.0f, 1.0f);
static const glm::vec3 teal(0.0f, 0.5f, 0.5f);
static const glm::vec3 navy(0.0f, 0.0f, 0.5f);
static const glm::vec3 yellow(1.0f, 1.0f, 0.0f);
static const glm::vec3 orange(1.0f, 0.5f, 0.0f);
static const glm::vec3 magenta(1.0f, 0.0f, 1.0f);
static const glm::vec3 purple(0.5f, 0.0f, 0.5f);
static const glm::vec3 cyan(0.0f, 1.0f, 1.0f);
static const glm::vec3 grey(0.5f, 0.5f, 0.5f);
static const glm::vec3 peach(1.0f, 0.8f, 0.7f);
static const glm::vec3 white(1.0f, 1.0f, 1.0f);
static const glm::vec3 black(0.0f, 0.0f, 0.0f);
}  // namespace mycolor

// world space positions of our cubes
static const float cubevertices[] = {
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,
    1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,
    0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,
    0.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f,
    0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,
    -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,
    0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,
    -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f,
    0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
};

glm::vec3 reversepixel(const float x, const float y);

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
