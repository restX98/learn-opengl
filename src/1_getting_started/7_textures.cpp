#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <shader.hpp>

#include <iostream>
#include <cmath>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int createShaderProgram();

const char* WINDOW_TITLE = "Textures";
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Very bad:
float textureOpactity = 0.2f;

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  Shader shader(SHADERS_PATH "7_shader.vert", SHADERS_PATH "7_shader.frag");

  float vertices[] = {
    // Positions        Colors              Texture coords
    -0.5f, 0.5f, 0.f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // top-left vertex
    0.5f, 0.5f, 0.f,    0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top-right vertex
    -0.5f, -0.5f, 0.f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom-left vertex
    0.5f, -0.5f, 0.f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom-right vertex
  };

  // In order to specify how openGL should apply the texture to the object we want add the texture,
  // we meed to specify the coordinates of the texture to use.
  // The texture coordinates start from (0, 0) bottom-left corner to (1, 1) top-right corner.
  // We are saying that:
  //  - the top-left vertex of the rect is mapped at coord (0, 1) of the texture
  //  - the top-right vertex of the rect is mapped  at coord (1, 1) of the texture
  //  - the bottom-left vertex of the rect is mapped at coord (0, 0) of the texture
  //  - the bottom-right vertex of the rect is mapped at coord (1, 0) of the texture

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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Texture:
  unsigned int texture1;
  glGenTextures(1, &texture1);

  // We are working with 2D texture but exists also 1D and 3D texture.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // Texture Wrapping:
  // We need to specify the wrapping mode of the texture in the case when the texture
  // coords overflows the the range 0-1.
  // If we specify as coords (-1, -1) for the bottom-left corner that means that the texture
  // (that start at coord (0, 0)) leave a blank space that need to replace with something and
  // going to start from the middle of the container that is mapped to the (0, 0) coord. 
  // The default behavior is to repeate the blank space with the texture, but exists various
  // mode:
  //  - GL_REPEAT: The default behavior for textures. Repeats the texture image.
  //  - GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat.
  //  - GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
  //  - GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.
  // This values can be set per coordinate axis (s, t, r[for 3D textures], equivalent to x, y, z).
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Texture Filtering:
  // The texture dimensions are differents to the container dimensions in most of cases.
  // OpenGL need to map the texture pixel (called texel) with the container dimensions and is 
  // very important when we use low resolution texture in large object.
  // To do that OpenGL use the texture filtering, exits a lot of mode to make a texture filtering
  // but the 2 main mode are:
  //  - GL_NEAREST results in blocked patterns where we can clearly see the pixels that form the texture while
  //  - GL_LINEAR produces a smoother pattern where the individual pixels are less visible.
  // GL_LINEAR produces a more realistic output, but some developers prefer a more 8-bit look
  // and as a result pick the GL_NEAREST option.
  // Texture filtering can be set for magnifying and minifying operations (when scaling up or downwards):
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // MipMaps:
  // In a game there is tons of object that could be at differents distances needing various
  // resolution based on the distances but the textures attached could be the same
  // In order to waste less resources openGL use the concept of Mipmaps that consists in a
  // collections of texture of various dimensions (one twice as small as another).
  // The idea is that after a certains distance from the viewer, OpenGL use a different mipmaps
  // texture that best suite that distance.
  //  - GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
  //  - GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level using linear interpolation.
  //  - GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation.
  //  - GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation

  int width, height, nrChannels;
  unsigned char* data = stbi_load(RESOURCES_PATH "textures/container.png", &width, &height, &nrChannels, 0);
  if (data) {
    // Generate a new 2D texture based on the loaded image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D); // Generate the minmap level on the given texture
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // Need to redo all the stuff because all the operation after glBindTexture will be applied
  // To the current bound texture. Yeah, i know! 
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
    glGenerateMipmap(GL_TEXTURE_2D); // Generate the minmap level on the given texture
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  shader.use();
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.setFloat("opacity", textureOpactity);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    textureOpactity += 0.001f;
    if (textureOpactity > 1) {
      textureOpactity = 1.0f;
    }
  } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    textureOpactity -= 0.001f;
    if (textureOpactity < 0) {
      textureOpactity = 0.0f;
    }
  }
}
