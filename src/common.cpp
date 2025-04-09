#include "../include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"

#include "../include/common.h"

#include <iostream>

glm::vec3 reversepixel(float x, float y) {
  uchar reversecolor[4];
  glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, reversecolor);
  float r = reversecolor[0] / 255.0f;
  float g = reversecolor[1] / 255.0f;
  float b = reversecolor[2] / 255.0f;
  float invertedr = 1.0f - r;
  float invertedg = 1.0f - g;
  float invertedb = 1.0f - b;
  return glm::vec3(invertedr, invertedg, invertedb);
}

uint createandstuffshaderprogram(const char* vertexShaderCode,
                                 const char* fragmentShaderCode) {
  uint vertex, fragment, shaderprogramm;
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertexShaderCode, NULL);
  glCompileShader(vertex);
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
  glCompileShader(fragment);
  shaderprogramm = glCreateProgram();
  glAttachShader(shaderprogramm, vertex);
  glAttachShader(shaderprogramm, fragment);
  glLinkProgram(shaderprogramm);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return shaderprogramm;
}

void setBool(uint& shaderprogramm, const std::string& name, bool value) {
  glUniform1i(glGetUniformLocation(shaderprogramm, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void setInt(uint& shaderprogramm, const std::string& name, int value) {
  glUniform1i(glGetUniformLocation(shaderprogramm, name.c_str()), value);
}
// ------------------------------------------------------------------------
void setFloat(uint& shaderprogramm, const std::string& name, float value) {
  glUniform1f(glGetUniformLocation(shaderprogramm, name.c_str()), value);
}
// ------------------------------------------------------------------------
void setVec2(uint& shaderprogramm, const std::string& name,
             const glm::vec2& value) {
  glUniform2fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
               &value[0]);
}
void setVec2(uint& shaderprogramm, const std::string& name, float x, float y) {
  glUniform2f(glGetUniformLocation(shaderprogramm, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void setVec3(uint& shaderprogramm, const std::string& name,
             const glm::vec3& value) {
  glUniform3fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
               &value[0]);
}
void setVec3(uint& shaderprogramm, const std::string& name, float x, float y,
             float z) {
  glUniform3f(glGetUniformLocation(shaderprogramm, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void setVec4(uint& shaderprogramm, const std::string& name,
             const glm::vec4& value) {
  glUniform4fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
               &value[0]);
}
void setVec4(uint& shaderprogramm, const std::string& name, float x, float y,
             float z, float w) {
  glUniform4f(glGetUniformLocation(shaderprogramm, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void setMat2(uint& shaderprogramm, const std::string& name,
             const glm::mat2& mat) {
  glUniformMatrix2fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void setMat3(uint& shaderprogramm, const std::string& name,
             const glm::mat3& mat) {
  glUniformMatrix3fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void setMat4(uint& shaderprogramm, const std::string& name,
             const glm::mat4& mat) {
  glUniformMatrix4fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}
