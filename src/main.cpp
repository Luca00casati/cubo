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
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 4.0f;

bool firstMouse = true;
float mouseSensitivity = 0.1f;
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

const float cross_size = 0.02f;

glm::vec3 bgcolor = mycolor::white;

//locked keys
struct KeyLock {
  bool q;
  bool k;
};

void input(GLFWwindow* window, KeyLock* keylock);

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
  //glfwSetKeyCallback(window, keyCallback);
  //glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
  //glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

  //init key lock
  KeyLock keylock;
  memset(&keylock, 0, sizeof(keylock));

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

  const char* crosshairVertexSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos, 1.0f);
}
)";

  const char* crosshairFragmentSrc = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 reversecolor;
void main() {
        FragColor = vec4(reversecolor, 1.0);
}
)";

  // build and compile our shader zprogram
  // ------------------------------------
  uint cubeprogramm =
      createandstuffshaderprogram(cubevertexsrc, cubefragmentsrc);
  uint crosshairProgram =
      createandstuffshaderprogram(crosshairVertexSrc, crosshairFragmentSrc);
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------

  const float crossvertices[] = {-cross_size, 0.0f, 0.0f,       cross_size,
                                 0.0f,        0.0f, 0.0f,       -cross_size,
                                 0.0f,        0.0f, cross_size, 0.0f};
  float cubevertices[] = {
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
  std::vector<glm::vec3> veccubePositions = {
      glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, 0.0f),
      glm::vec3(-1.5f, -2.2f, 0.0f), glm::vec3(-3.8f, -2.0f, 0.0f),
      glm::vec3(2.4f, -0.4f, 0.0f),  glm::vec3(-1.7f, 3.0f, 0.0f),
      glm::vec3(1.3f, -2.0f, 0.0f),  glm::vec3(1.5f, 2.0f, 0.0f),
      glm::vec3(3.3f, -1.0f, 0.0f),  glm::vec3(3.5f, 1.0f, 0.0f)};
  //cross
  uint crossVAO, crossVBO;
  glGenVertexArrays(1, &crossVAO);
  glGenBuffers(1, &crossVBO);

  glBindVertexArray(crossVAO);
  glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(crossvertices), crossvertices,
               GL_STATIC_DRAW);

  // Setup vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  //cube
  uint cubeVBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);

  glBindVertexArray(cubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubevertices), cubevertices,
               GL_STATIC_DRAW);

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
    input(window, &keylock);
    glClearColor(bgcolor.x, bgcolor.y, bgcolor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate shader
    glUseProgram(cubeprogramm);
    setVec3(cubeprogramm, "ourColor", mycolor::red);
    // pass projection matrix to shader (note that in this case it could
    // change every frame)
    glm::mat4 projection = glm::perspective(
        glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    setMat4(cubeprogramm, "projection", projection);

    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    setMat4(cubeprogramm, "view", view);

    // render boxes
    glBindVertexArray(cubeVAO);
    for (uint i = 0; i < veccubePositions.size(); i++) {
      // calculate the model matrix for each object and pass it to shader
      // before drawing
      glm::mat4 model = glm::mat4(
          1.0f);  // make sure to initialize matrix to identity matrix first
      model = glm::translate(model, veccubePositions[i]);
      setMat4(cubeprogramm, "model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glm::vec3 reversepixelvec3 =
        reversepixel((float)SCR_WIDTH / 2, (float)SCR_HEIGHT / 2);
    glUseProgram(crosshairProgram);
    setVec3(crosshairProgram, "reversecolor", reversepixelvec3);
    glBindVertexArray(crossVAO);
    glDrawArrays(GL_LINES, 0, 4);  // Draw 4 vertices as a line
    glBindVertexArray(0);
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse
    // moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &crossVAO);
  glDeleteBuffers(1, &crossVBO);
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

void input(GLFWwindow* window, KeyLock* keylock) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float camspeed = cameraSpeed * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cameraPos += cameraFront * camspeed;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cameraPos -= cameraFront * camspeed;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camspeed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camspeed;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    cameraPos += cameraUp * camspeed;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    cameraPos -= cameraUp * camspeed;
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    if (!keylock->q) {
      followMouse = !followMouse;
      keylock->q = true;
    }
    if (followMouse) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
      glfwSetCursorPos(window, lastX, lastY);
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  } else {
    keylock->q = false;
  }
  if ((glfwGetKey(window, GLFW_KEY_K)) == GLFW_PRESS) {
    if (!keylock->k) {
      wireframe = !wireframe;  // Toggle wireframe mode
      keylock->k = true;
    }
    if (wireframe) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Set wireframe mode
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Set solid mode
    }
  } else {
    keylock->k = false;
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
