#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <shader.hpp>

#include <iostream>
#include <cmath>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int createShaderProgram();

const char* WINDOW_TITLE = "Two triangle";
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Very bad:
float g_textureOpactity = 0.2f;
glm::vec3 g_cameraPos;
glm::vec3 g_cameraFront;
glm::vec3 g_cameraUp;
float g_deltaTime = 0;
float g_currentFrame = 0;
float g_lastFrame = 0;

float g_lastX = WINDOW_WIDTH / 2;
float g_lastY = WINDOW_HEIGHT / 2;
float g_pitch = 0.0f;
float g_yaw = -90.0f;
bool g_firstMouse = true;

float g_fov = 45.0f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  Shader shader(SHADERS_PATH "9_shader.vert", SHADERS_PATH "9_shader.frag");

  float vertices[] = {
    // Positions          Texture coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  int indices[] = {
    0, 1, 2,
    1, 2, 3
  };

  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture:
  unsigned int texture1;
  glGenTextures(1, &texture1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char* data = stbi_load(RESOURCES_PATH "textures/container.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  unsigned int texture2;
  glGenTextures(1, &texture2);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(RESOURCES_PATH "textures/smile.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  shader.use();
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

  glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  // The view matrix is used to transform all the world coordinates into view coordinates that are rellative
  // to the camera's position and direction.
  // In OpenGL doesn't exists a Camera object, but we can anbstract this concept moving all the world respect to the camera.
  // Every camera should have:
  //  - A position (in the space)
  //  - A direction (front axis)
  //  - A right vector (orizhontal axis)
  //  - An up vector (vertical axis)

  // We can specify the position as a vector of 3 coords (x, y, z).
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

  // We can define the direction by defining the target to point (in this case the origin (0, 0, 0)) and subtract it to
  // the camera position:
  glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); // It point to the opposite direction as the actual camera perispective.
  // The difference of two vectors is the distance beween the two point

  // We can define the right axis by defining an vector points upwars in the world space and then we define the right
  // axis as the cross product of them:
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
  // cross product create a normal vector respect the two multiplied.

  // Note that we normalize the resulting right vector.
  // If we wouldn't normalize this vector, the resulting cross product may return differently sized vectors based on the
  // cameraFront variable. If we would not normalize the vector we would move slow or fast based on the camera's
  // orientation instead of at a consistent movement speed.

  // Having two direction we can apply the same approach of before, to create the upward vector we can just do the
  // cross product between the camera direction and the right axis:
  glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

  // GLM can do it for us. We can just define the position, the origin and the world upward vector:
  // glm::mat4 view = glm::lookAt(
  //   glm::vec3(0.0f, 0.0f, 3.0f), // Camera Position
  //   glm::vec3(0.0f, 0.0f, 0.0f), // Camera Origin (used to calculate the direction vector [opposite to actual direction])
  //   glm::vec3(0.0f, 1.0f, 0.0f) // world upward vector
  // );

  // When we want to move around the sceen, the target change accordling with the position, then to allow us to move around
  // the scene, we define:
  g_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // Position of the camera
  g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Direction vector: this is not to intend as a coord vector but better as a vector that start from the camera position and say us the direction we want to point at
  g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Camera Up: world upward vector

  while (!glfwWindowShouldClose(window)) {
    g_currentFrame = glfwGetTime();
    g_deltaTime = g_currentFrame - g_lastFrame;
    g_lastFrame = g_currentFrame;

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.setFloat("opacity", g_textureOpactity);

    glm::mat4 projection = glm::perspective(glm::radians(g_fov), 800.0f / 600.0f, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    // view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); // Turning turning turning turning turning aroooound

    glm::vec3 direction;
    direction.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    direction.y = sin(glm::radians(g_pitch));
    direction.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    g_cameraFront = glm::normalize(direction);

    // Then the lookAt matrix will be the following:
    glm::mat4 view = glm::lookAt(g_cameraPos, g_cameraPos + g_cameraFront, g_cameraUp);
    // In this case the camera origin change accordling with the camera position in order to watch always in the same direction.
    // Otherwise is like when you was a child and look at your parent while ride a bike.

    shader.setMat4("view", view);

    for (int i = 0; i < 10; i++) {
      // The model matrix consists of translations, scaling and/or rotation we want to apply
      // to transform/move the object mappend in the Local Space to the world space (done also in the transformation chapter)
      glm::mat4 model = glm::mat4(1.0f); // Create 4x4 identity matrix
      // Create rotation matrix fixing the x-axis
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * (i + 1);
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

      shader.setMat4("model", model);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Order is important when multiplying matrices, use the following order:
    // 1. scaling operations
    // 2. rotations operations
    // 3. translations operations
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  // I don't like this approach, in this way we can actually move only when i move the mouse.
  // In my opinion the direction should move based on the distance from the center.

  if (g_firstMouse) {
    g_lastX = xpos;
    g_lastY = ypos;
    g_firstMouse = false;
  }

  float xoffset = xpos - g_lastX;
  float yoffset = g_lastY - ypos; // reversed since y-coordinates range from bottom to top
  g_lastX = xpos;
  g_lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  g_yaw += xoffset;
  g_pitch += yoffset;

  if (g_pitch > 89.0f)
    g_pitch = 89.0f;
  if (g_pitch < -89.0f)
    g_pitch = -89.0f;

  std::cout << "pitch: " << g_pitch << std::endl;
  std::cout << "yaw: " << g_pitch << std::endl;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  g_fov -= (float)yoffset;
  if (g_fov < 1.0f)
    g_fov = 1.0f;
  if (g_fov > 45.0f)
    g_fov = 45.0f;
}

void processInput(GLFWwindow* window) {
  const float cameraSpeed = 2.5f * g_deltaTime;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    g_textureOpactity += 0.001f;
    if (g_textureOpactity > 1) {
      g_textureOpactity = 1.0f;
    }
  } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    g_textureOpactity -= 0.001f;
    if (g_textureOpactity < 0) {
      g_textureOpactity = 0.0f;
    }
  }

  // Better if instead of if else, otherwise we can't move in diagonal
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    g_cameraPos += cameraSpeed * g_cameraFront;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    g_cameraPos -= cameraSpeed * g_cameraFront;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    // To move right we do the cross product of the camera direction and the upward camera vector
    // and we get the normal axis of them:
    g_cameraPos -= glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * cameraSpeed;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    g_cameraPos += glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * cameraSpeed;
  }
}
