#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <shader.hpp>
#include <camera.hpp>
#include <material.hpp>

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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

  Shader shader(SHADERS_PATH "11_shader.vert", SHADERS_PATH "11_object_shader.frag");
  Shader lightingShader(SHADERS_PATH "11_shader.vert", SHADERS_PATH "11_light_shader.frag");

  float vertices[] = {
    // Positions
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  // we only need to bind to the VBO, the container's VBO's data already contains the data.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // set the vertex attribute 
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  g_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

  while (!glfwWindowShouldClose(window)) {
    g_currentFrame = static_cast<float>(glfwGetTime());
    g_deltaTime = g_currentFrame - g_lastFrame;
    g_lastFrame = g_currentFrame;

    processInput(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    shader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f); //0.2f, 0.2f, 0.2f);
    shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f); // 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light.position", lightPos);

    shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);

    shader.setVec3("viewPos", camera.Position);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    shader.setMat4("model", model);

    // Emerald
    const Material& emerald = getMaterial(MaterialType::Emerald);
    shader.setVec3("material.ambient", emerald.ambient[0], emerald.ambient[1], emerald.ambient[2]);
    shader.setVec3("material.diffuse", emerald.diffuse[0], emerald.diffuse[1], emerald.diffuse[2]);
    shader.setVec3("material.specular", emerald.specular[0], emerald.specular[1], emerald.specular[2]);
    shader.setFloat("material.shininess", emerald.shininess);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    float y = 0.0f;
    for (int i = 1; i < static_cast<int>(MaterialType::Count); i++) {

      glm::mat4 model2 = glm::mat4(1.0f);
      float x = 1.5f * (i % 10);
      if (i % 10 == 0) {
        y -= 1.5f;
      }
      model2 = glm::translate(model2, glm::vec3(x, y, 0.0f));
      model2 = glm::rotate(model2, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      shader.setMat4("model", model2);
      MaterialType materialType = static_cast<MaterialType>(i);

      const Material& material = getMaterial(materialType);
      shader.setVec3("material.ambient", material.ambient[0], material.ambient[1], material.ambient[2]);
      shader.setVec3("material.diffuse", material.diffuse[0], material.diffuse[1], material.diffuse[2]);
      shader.setVec3("material.specular", material.specular[0], material.specular[1], material.specular[2]);
      shader.setFloat("material.shininess", material.shininess);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);

    }

    lightingShader.use();
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightingShader.setMat4("model", model);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader.setMat4("projection", projection);

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

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (g_firstMouse) {
    g_lastX = xpos;
    g_lastY = ypos;
    g_firstMouse = false;
  }

  float xoffset = xpos - g_lastX;
  float yoffset = g_lastY - ypos; // reversed since y-coordinates range from bottom to top
  g_lastX = xpos;
  g_lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, g_deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, g_deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, g_deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, g_deltaTime);
}


/**
 * Light in real world is very complicated, we can use the pfhisics law to model the light in
 * 3D computer rendering.
 * One of theese model is the Phong Lighting Model and consists of three components:
 *  - Ambient lighting: even when is dark there is usually still eome light somewhere (like the moon).
 *                      To simulate the ambient light we can set a constant light to an object.
 *  - Diffuse Lighting: simulates the directional impact a light object has on an object.
 *  - Speculare lighting: simulates the bright spot of a light that appears on shiny objects.
 *                        Specular highlights are more inclined to the color of the light than
 *                        the color of the object.
 *
 * All the component are calculated by the fragment shaders.
 *
 *  - Ambient Lighting: To apply the ambient light to an object we need to multiply the light color with the
 *                       ambient strength and then multiply the result with the object color.
 *  - Diffuse Lighting: The diffuse light is the ligth that is reflected by the surface in function of the angle
 *                      between the light source and the normal vector of the surface.
 *                      Then in order to calculate the diffuse light of a surface we need to:
 *                        - determinate the normal vector
 *                        - calculate the light direction by substracting the light position vector
 *                          to the surface(fragment) position vector
 *                        - calculate the diffuse component by the dot product beween the normal vector
 *                          and the light direction vector.
 *                          If the angle between the normal and the light vectors are greather than 90 degree
 *                          means that the light is behind the surface, then the diffusion component should be
 *                          forced to 0.
 *                        - calculate the diffuse color multiplying the diffuse component whith the light color.
 *                      Finally we can sum the ambient and diffuse compoents and multiply the sum with the object color.
 *                      Note: If we apply a non-uniform scale to an object, the normal vector of a surface can change
 *                            is direction. This problem could be solve by this operation: Normal = mat3(transpose(inverse(model))) * aNormal;
 *                            Inversing matrices is a costly operation for shaders, so wherever possible try to avoid doing inverse operations
 *                            since they have to be done on each vertex of your scene. For learning purposes this is fine, but for an efficient
 *                            application you'll likely want to calculate the normal matrix on the CPU and send it to the shaders via a uniform
 *                            before drawing (just like the model matrix).
 *  - Specular lighting: The speculare direction is calculated in function of the view direction.
 *                       If we watch an object to the right angle, we can see the reflection of the light on the object.
 *                       The light is reflected around the normal vector, the impact of the speculare light is calculated
 *                       on function of the angle between the relected light vector and the view direction vector.
 *                       The closer the angle between them, the greater the impact of the specular light.
 *                       The resulting effect is that we see a bit of a highlight when we're looking at the
 *                       light's direction reflected via the surface.
 *                       In order to calculate the speculare light we need to:
 *                         - pass the view position to the shader by an uniform
 *                         - calculate the view direction vector by subtracting the view position and the fragment position
 *                         - calculate the reflected light direction vector reflecting the light direction (reversed) around the normal vector
 *                         - calculate the diffuse component by the dot product beween the view direction and the reflected lisght direction vectors (or 0 for same reason of the diffuse light)
 *                         - powing teh value by the shininess value assigned to the object. (The higher the shininess value of an object, the more it properly reflects the light instead of scattering it all around and thus the smaller the highlight becomes)
 *                      Finally we can sum the ambient, the diffuse and the specular compoents and multiply the sum with the object color.
 *
 * Gouraud Shading Model:
 * In the earlier days of lighting shaders, developers used to implement the Phong lighting model in the vertex shader. The advantage of doing lighting
 * in the vertex shader is that it is a lot more efficient since there are generally a lot less vertices compared to fragments, so the (expensive)
 * lighting calculations are done less frequently. However, the resulting color value in the vertex shader is the resulting lighting color of
 * that vertex only and the color values of the surrounding fragments are then the result of interpolated lighting colors.
 * When the Phong lighting model is implemented in the vertex shader it is called Gouraud shading instead of Phong shading.
 * Note that due to the interpolation the lighting looks somewhat off. The Phong shading gives much smoother lighting results.
 */
