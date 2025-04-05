#include "../include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"
// #define STB_IMAGE_IMPLEMENTATION
// #include "../include/stb_image.h"

#include "../lib/glm/glm/glm.hpp"
#include "../lib/glm/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/glm/gtc/type_ptr.hpp"

#include "../include/common.h"

#include <iostream>

// global
const uint SCR_WIDTH = 800;
const uint SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 4.0f;

bool firstMouse = true;
float mouseSensitivity = 0.1f;  // change this value to your liking
bool followMouse = true;
bool wireframe = false;
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
int lastframekey = 0;
int lastframemod = 0;
uint ncube = 0;

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
  //glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
  glfwSetCursorPosCallback(window, mouse_callback);
  // glfwSetScrollCallback(window, scroll_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  const char* cubevertexsrc = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
})";

  const char* cubefragmentsrc = R"(
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
})";

  // build and compile our shader zprogram
  // ------------------------------------
  uint cubeprogramm =
      createandstuffshaderprogram(cubevertexsrc, cubefragmentsrc);

  glm::vec3 red(1.0f, 0.0f, 0.0f);
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float crosshairVertices[] = {// Horizontal line
                               -0.05f, 0.0f, 0.0f, 0.05f, 0.0f, 0.0f,
                               // Vertical line
                               0.0f, 0.05f, 0.0f, 0.0f, -0.05f, 0.0f};
  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
  // world space positions of our cubes
  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(3.3f, -1.0f, -2.5f),  glm::vec3(3.5f, 1.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
  ncube = 12;
  uint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate shader
    glUseProgram(cubeprogramm);
    setVec3(cubeprogramm, "ourColor", red);
    // pass projection matrix to shader (note that in this case it could
    // change every frame)
    glm::mat4 projection = glm::perspective(
        glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    setMat4(cubeprogramm, "projection", projection);

    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    setMat4(cubeprogramm, "view", view);

    // render boxes
    glBindVertexArray(VAO);
    for (uint i = 0; i < ncube; i++) {
      // calculate the model matrix for each object and pass it to shader
      // before drawing
      glm::mat4 model = glm::mat4(
          1.0f);  // make sure to initialize matrix to identity matrix first
      model = glm::translate(model, cubePositions[i]);
      // float angle = 20.0f * i;
      // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f,
      // 0.5f));
      setMat4(cubeprogramm, "model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse
    // moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                 int mod) {

  lastframekey = key;
  lastframemod = key;
  if (key == GLFW_KEY_ESCAPE and action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float camspeed = cameraSpeed * deltaTime;
  if (MY_KEY_REPEAT(GLFW_KEY_W)) {
    cameraPos += glm::vec3(0.0f, 0.0f, -1.0f) * camspeed;
  }
  if (MY_KEY_REPEAT(GLFW_KEY_S)) {
    cameraPos += glm::vec3(0.0f, 0.0f, 1.0f) * camspeed;
  }
  if (MY_KEY_REPEAT(GLFW_KEY_A)) {
    cameraPos += glm::vec3(-1.0f, 0.0f, 0.0f) * camspeed;
  }
  if (MY_KEY_REPEAT(GLFW_KEY_D)) {
    cameraPos += glm::vec3(1.0f, 0.0f, 0.0f) * camspeed;
  }
  if (MY_KEY_REPEAT(GLFW_KEY_SPACE)) {
    cameraPos += glm::vec3(0.0f, 1.0f, 0.0f) * camspeed;
  }
  if (MY_KEY_REPEAT(GLFW_KEY_LEFT_SHIFT)) {
    cameraPos += glm::vec3(0.0f, -1.0f, 0.0f) * camspeed;
  }
  if (key == GLFW_KEY_Q and action == GLFW_RELEASE)
    followMouse = !followMouse;
  if (followMouse) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetCursorPos(window, lastX, lastY);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
  if (key == GLFW_KEY_K and action == GLFW_RELEASE) {
    wireframe = !wireframe;  // Toggle wireframe mode
    if (wireframe) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Set wireframe mode
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Set solid mode
    }
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  if (!followMouse) {
    return;
  }

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos;  // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;

  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}
