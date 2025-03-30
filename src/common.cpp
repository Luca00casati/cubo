#include "../include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"

#include "../include/learnopengl/shader_m.h"
#include "../include/common.h"

#include <iostream>
// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, GlobalState * gs) {
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
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    gs->followMouse = !gs->followMouse;
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
if (gs->followMouse){
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (gs->firstMouse) {
    gs->lastX = xpos;
    gs->lastY = ypos;
    gs->firstMouse = false;
  }

  float xoffset = xpos - gs->lastX;
  float yoffset =
      gs->lastY - ypos; // reversed since y-coordinates go from bottom to top
  gs->lastX = xpos;
  gs->lastY = ypos;

  float sensitivity = 0.1f; // change this value to your liking
  xoffset *= sensitivity;
  yoffset *= sensitivity;

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
