#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* WINDOW_TITLE = "My first window :)";
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // The viewport is the content of the window
  // I'm seting the starting coords of the viewport on the window at (0,0) lower-left corder
  // and the size of the view port to 800x600 px.
  // Then glViewport transform the width/height ranges (0, 800)/(0, 600) in the range (-1, 1)
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  // glfwSetKeyCallback(window, key_callback);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // The glfwSwapBuffers(window) will swap the color buffer (a large 2D buffer that contains color values for each pixel
    // in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
    // There is a front buffer that contain a lists of pixel ready to be shown, then exits a back buffer that is used
    // to prepare the next frame while the current frame is shown.
    glfwSwapBuffers(window);
    // The glfwPollEvents() checks if any events are triggered (like keyboard input or mouse movement events),
    // updates the window state, and calls the corresponding functions (which we can register via callback methods)
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

// The OS manage the window resizing, I need to update the viewport during window resize
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
