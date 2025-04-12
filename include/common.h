#ifndef COMMON_H
#define COMMON_H
#include "../include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"

#include <iostream>

namespace mycolor {
inline glm::vec3 red(1.0f, 0.0f, 0.0f);
inline glm::vec3 green(0.0f, 1.0f, 0.0f);
inline glm::vec3 blue(0.0f, 0.0f, 1.0f);
inline glm::vec3 white(1.0f, 1.0f, 1.0f);
inline glm::vec3 black(0.0f, 0.0f, 0.0f);
}  // namespace mycolor


#define AND &&
#define OR ||
#define TRUE 1
#define FALSE 0

typedef unsigned char uchar;
//void keyCallback(GLFWwindow* window, int key, int scancode, int action,
//                 int mod);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
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
