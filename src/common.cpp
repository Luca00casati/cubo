#include "../include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"

#include "../include/common.h"
//#include "../include/learnopengl/shader_m.h"

#include <iostream>
//  #include <filesystem>
// #include <cassert>
// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, GlobalState* gs) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = static_cast<float>(2.5 * gs->deltaTime);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    gs->cameraPos += cameraSpeed * gs->cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    gs->cameraPos -= cameraSpeed * gs->cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    gs->cameraPos -=
        glm::normalize(glm::cross(gs->cameraFront, gs->cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    gs->cameraPos +=
        glm::normalize(glm::cross(gs->cameraFront, gs->cameraUp)) * cameraSpeed;
  bool spacebarPressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

  // Toggle followMouse only when spacebar is pressed and wasn't pressed in the last frame
  if (spacebarPressed && !gs->spacebarPressedLastFrame) {
    gs->followMouse = !gs->followMouse;
  }

  // Update the spacebar pressed state for the next frame
  gs->spacebarPressedLastFrame = spacebarPressed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  if (!gs->followMouse) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    return;
  }
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (gs->firstMouse) {
    gs->lastX = xpos;
    gs->lastY = ypos;
    gs->firstMouse = false;
  }

  float xoffset = xpos - gs->lastX;
  float yoffset =
      gs->lastY - ypos;  // reversed since y-coordinates go from bottom to top
  gs->lastX = xpos;
  gs->lastY = ypos;

  xoffset *= gs->mouseSensitivity;
  yoffset *= gs->mouseSensitivity;

  gs->yaw += xoffset;
  gs->pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (gs->pitch > 89.0f)
    gs->pitch = 89.0f;
  if (gs->pitch < -89.0f)
    gs->pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(gs->yaw)) * cos(glm::radians(gs->pitch));
  front.y = sin(glm::radians(gs->pitch));
  front.z = sin(glm::radians(gs->yaw)) * cos(glm::radians(gs->pitch));
  gs->cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
/*
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  fov -= (float)yoffset;
  if (fov < 1.0f)
    fov = 1.0f;
  if (fov > 45.0f)
    fov = 45.0f;
}
*/

/* 
  constexpr std::string abspath(const char* path){
  std::filesystem::path p = path;
 assert(std::filesystem::exists(p) && "Error: Path does not exist.");
  return std::filesystem::absolute(p).string();
}
*/
unsigned int createandstuffshaderprogram(const char* vertexShaderCode,
                                         const char* fragmentShaderCode) {
  unsigned int vertex, fragment, shaderprogramm;
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

void setBool(unsigned int& shaderprogramm, const std::string& name,
             bool value) {
  glUniform1i(glGetUniformLocation(shaderprogramm, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void setInt(unsigned int& shaderprogramm, const std::string& name,
            int value) {
  glUniform1i(glGetUniformLocation(shaderprogramm, name.c_str()), value);
}
// ------------------------------------------------------------------------
void setFloat(unsigned int& shaderprogramm, const std::string& name,
              float value) {
  glUniform1f(glGetUniformLocation(shaderprogramm, name.c_str()), value);
}
// ------------------------------------------------------------------------
void setVec2(unsigned int& shaderprogramm, const std::string& name,
             const glm::vec2& value) {
  glUniform2fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
               &value[0]);
}
void setVec2(unsigned int& shaderprogramm, const std::string& name, float x,
             float y) {
  glUniform2f(glGetUniformLocation(shaderprogramm, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void setVec3(unsigned int& shaderprogramm, const std::string& name,
             const glm::vec3& value) {
  glUniform3fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
               &value[0]);
}
void setVec3(unsigned int& shaderprogramm, const std::string& name, float x,
             float y, float z) {
  glUniform3f(glGetUniformLocation(shaderprogramm, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void setVec4(unsigned int& shaderprogramm, const std::string& name,
             const glm::vec4& value) {
  glUniform4fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
               &value[0]);
}
void setVec4(unsigned int& shaderprogramm, const std::string& name, float x,
             float y, float z, float w) {
  glUniform4f(glGetUniformLocation(shaderprogramm, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void setMat2(unsigned int& shaderprogramm, const std::string& name,
             const glm::mat2& mat) {
  glUniformMatrix2fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void setMat3(unsigned int& shaderprogramm, const std::string& name,
             const glm::mat3& mat) {
  glUniformMatrix3fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void setMat4(unsigned int& shaderprogramm, const std::string& name,
             const glm::mat4& mat) {
  glUniformMatrix4fv(glGetUniformLocation(shaderprogramm, name.c_str()), 1,
                     GL_FALSE, &mat[0][0]);
}
