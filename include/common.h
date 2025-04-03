#ifndef COMMON_H
#define COMMON_H
#include "../include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"

#include <iostream>
typedef struct GlobalState {
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;

  // camera
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  bool firstMouse = true;
  float mouseSensitivity = 0.1f;  // change this value to your liking
  bool followMouse = true;
  bool spacebarPressedLastFrame = false;
  float yaw = -90.0f;  // yaw is initialized to -90.0 degrees since a yaw of 0.0
      // results in a direction vector pointing to the right so we
      // initially rotate a bit to the left.
  float pitch = 0.0f;
  float lastX = 800.0f / 2.0;
  float lastY = 600.0 / 2.0;
  float fov = 45.0f;
  // timing
  float deltaTime = 0.0f;  // time between current frame and last frame
  float lastFrame = 0.0f;
  size_t ncube = 0;
} GS;
extern GlobalState* gs;
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void processInput(GLFWwindow* window, GlobalState* gs);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
// std::string abspath(const char* path);
unsigned int createandstuffshaderprogram(const char* vertexShaderCode,
                                         const char* fragmentShaderCode);

void setBool(unsigned int& shaderprogramm, const std::string& name,
             bool value) ;
void setInt(unsigned int& shaderprogramm, const std::string& name,
            int value) ;
void setFloat(unsigned int& shaderprogramm, const std::string& name,
              float value) ;
void setVec2(unsigned int& shaderprogramm, const std::string& name,
             const glm::vec2& value) ;
void setVec2(unsigned int& shaderprogramm, const std::string& name, float x,
             float y) ;
void setVec3(unsigned int& shaderprogramm, const std::string& name,
             const glm::vec3& value) ;
void setVec3(unsigned int& shaderprogramm, const std::string& name, float x,
             float y, float z) ;
void setVec4(unsigned int& shaderprogramm, const std::string& name,
             const glm::vec4& value) ;
void setVec4(unsigned int& shaderprogramm, const std::string& name, float x,
             float y, float z, float w) ;
void setMat2(unsigned int& shaderprogramm, const std::string& name,
             const glm::mat2& mat) ;
void setMat3(unsigned int& shaderprogramm, const std::string& name,
             const glm::mat3& mat) ;
void setMat4(unsigned int& shaderprogramm, const std::string& name,
             const glm::mat4& mat) ;
#endif
