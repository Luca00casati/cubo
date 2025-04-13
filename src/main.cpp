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
//#define CUBO_DEBUG
const float myscreenwidth = 800.0f;
const float myscreenheight = 600.0f;
// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraPos);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 4.0f;

bool firstMouse = true;
float mouseSensitivity = 0.1f;
bool followMouse = true;
bool wireframe = false;
float yaw = -50.0f;
float pitch = -30.0f;
double savedX, savedY;
double lastX = myscreenwidth / 2.0;
double lastY = myscreenheight / 2.0;
float fov = 45.0f;
// timing
float deltaTime = 0.0f;  // time between current frame and last frame
float lastFrame = 0.0f;

const float cross_size = 0.02f;

glm::vec3 cubecolor = mycolor::green;
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
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window =
      glfwCreateWindow(myscreenwidth, myscreenheight, "CUBO", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  //glfwSetInputMode(window, GLFW_STICKY_KEYS, true);
  //glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, myscreenwidth, myscreenheight);
  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  //init key lock
  KeyLock keylock;
  memset(&keylock, 0, sizeof(keylock));

  const char* cubevertexsrc = R"(
  #version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // or just aNormal for cubes

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";

  const char* cubefragmentsrc = R"(
  #version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 baseColor;

void main()
{
    vec3 viewDir = normalize(cameraPos - FragPos);
    float intensity = max(dot(normalize(Normal), viewDir), 0.0);

    // Optional: Clamp intensity range
    intensity = clamp(intensity, 0.3, 1.0);

    vec3 color = baseColor * intensity;
    FragColor = vec4(color, 1.0);
}
)";

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
uniform sampler2D screenTexture;
uniform vec2 resolution;
void main() {
    vec2 uv = gl_FragCoord.xy / resolution;
    vec3 color = texture(screenTexture, uv).rgb;
    FragColor = vec4(vec3(1.0) - color, 1.0);
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

  // const float cubevertices[]; //define in header

  std::vector<glm::vec3> veccubePositions = {
      glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, 0.0f),
      glm::vec3(-1.5f, -4.2f, 0.0f), glm::vec3(-3.8f, -4.0f, 0.0f),
      glm::vec3(2.4f, -2.4f, 0.0f),  glm::vec3(-1.7f, 3.0f, 0.0f),
      glm::vec3(1.3f, -5.0f, 0.0f),  glm::vec3(1.5f, 2.0f, 0.0f),
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

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  unsigned int texColorBuffer;
  glGenTextures(1, &texColorBuffer);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myscreenwidth, myscreenheight, 0,
               GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texColorBuffer, 0);
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, myscreenwidth,
                        myscreenheight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);
#ifdef CUBO_DEBUG
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
#endif
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    input(window, &keylock);

    // First pass: render to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(bgcolor.x, bgcolor.y, bgcolor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(cubeprogramm);
    glm::mat4 projection = glm::perspective(
        glm::radians(fov), myscreenwidth / myscreenheight, 0.1f, 100.0f);
    setMat4(cubeprogramm, "projection", projection);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    setMat4(cubeprogramm, "view", view);
    setVec3(cubeprogramm, "cameraPos", cameraPos);
    setVec3(cubeprogramm, "baseColor", cubecolor);
    glBindVertexArray(cubeVAO);
    for (auto& pos : veccubePositions) {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
      setMat4(cubeprogramm, "model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Second pass: draw crosshair to default framebuffer using inverted sampled texture
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(bgcolor.x, bgcolor.y, bgcolor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, myscreenwidth, myscreenheight, 0, 0, myscreenwidth,
                      myscreenheight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glUseProgram(crosshairProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    setInt(crosshairProgram, "screenTexture", 0);
    setVec2(crosshairProgram, "resolution",
            glm::vec2(myscreenwidth, myscreenheight));
    glBindVertexArray(crossVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteVertexArrays(1, &crossVAO);
  glDeleteBuffers(1, &crossVBO);
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
      glfwGetCursorPos(window, &savedX, &savedY);
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      glfwGetCursorPos(window, &lastX, &lastY);
    } else {
      glfwSetCursorPos(window, savedX, savedY);
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

  //mouse
  if (followMouse) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);  // Get current cursor position

    // Calculate mouse offset (movement)
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // Reverse the y-axis to make it intuitive
    lastX = xpos;
    lastY = ypos;

    // Apply mouse sensitivity
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    // Update yaw and pitch based on mouse movement
    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to avoid camera flip
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;

    // Update the camera front vector based on yaw and pitch
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
  }
}
