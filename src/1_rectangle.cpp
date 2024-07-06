#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* WINDOW_TITLE = "My first window :)";
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// GLSL (OpenGL Shading Language)
// Is a programming language used to define the Shaders.
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

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
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // The viewport is the content of the window
  // I'm seting the starting coords of the viewport on the window at (0,0) lower-left corder
  // and the size of the view port to 800x600 px.
  // Then glViewport transform the width/height ranges (0, 800)/(0, 600) in the range (-1, 1)
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  // Vertex shader is used to define the structure of a vertex.
  // The current vertex has only one attribute called location (at position 0) that is used
  // to store a vector with 3 dimension (x, y, z).
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Check vertex shader compilation status
  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }


  // The fragment shader is needed to set the color of the pixels
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Note: GL_FRAGMENT_SHADER
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Check fragment shader compilation status
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  // The program shader is used to link all the other shader togher
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check linking status
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  // Clean shader from memory after linked it to the program object. Not needed anymore
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Vertices location array.
  // float vertices[] = { // Triangle
  //     -0.5f, -0.5f, 0.0f, // Vertex 0: left
  //      0.5f, -0.5f, 0.0f, // Vertex 1: right
  //      0.0f,  0.5f, 0.0f // Vertex 2: top
  // };

  // To create complex figure we can use a combination of more triangle.
  // For example to create a rectangle we can specify the following vertex:
//   float vertices[] = { // Rectable
//     // first triangle
//      0.5f,  0.5f, 0.0f,  // Vertex 0: top right
//      0.5f, -0.5f, 0.0f,  // Vertex 1: bottom right
//     -0.5f,  0.5f, 0.0f,  // Vertex 2: top left 
//     // second triangle
//      0.5f, -0.5f, 0.0f,  // Vertex 3: bottom right - duplicated
//     -0.5f, -0.5f, 0.0f,  // Vertex 4: bottom left
//     -0.5f,  0.5f, 0.0f   // Vertex 5: top left - duplicated
// };
  // Using the approach above we have an overheaf of 2 vertex that are shared between the 2 triangle.
  // Then to avoid this problem we specify only one vertex for location and then we specify the indexes
  // of the vertex of the triangles:
  float vertices[] = {
     0.5f,  0.5f, 0.0f,  // Vertex 0: top right
     0.5f, -0.5f, 0.0f,  // Vertex 1: bottom right
    -0.5f, -0.5f, 0.0f,  // Vertex 2: bottom left
    -0.5f,  0.5f, 0.0f   // Vertex 3: top left 
  };
  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };

    // The communication between the CPU and the GPU is slow, then is preferred to send
    // as much data as possible at once, to do that we use the buffers.
  unsigned int VBO; // Vertex Buffer Object - Used to store a buffer of vertices
  unsigned int VAO; // Vertex Array Object - Used to store a list of attribute pointer.
  // This is helpful to bind every attribute to each vertex inside the VBO.
  unsigned int EBO; // Element Buffer Object - Used to store the indexes data.

  glGenVertexArrays(1, &VAO); // Generate 1 Vertex Array and assign the ID to the VAO variable
  glGenBuffers(1, &VBO); // Generate 1 Vertex Buffer and assign the ID to the VBO variable
  glGenBuffers(1, &EBO); // Generate 1 Element Buffer and assign the ID to the EBO variable
  glBindVertexArray(VAO); // Bind the array. Every VBO, EBO, glVertexAttribPointer, glEnableVertexAttribArray
  // and glDisableVertexAttribArray call will be stored inside the VAO.

  // OpenGL has many object type, each of them has its own buffer type
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the VBO buffer to the array buffer used for the vertex object.
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Allocate memory and store the value in the specified buffer
  // There are 3 type of storing:
  //  - GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
  //  - GL_STATIC_DRAW: the data is set only once and used many times.
  //  - GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind the EBO buffer to the Element array buffer.
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Allocate memory and store the value in the specified buffer

  // Specify how OpenGL should interpret the vertex buffer data
  // The first argument specify the id of the vertex shader attribute (our case location = 0)
  // The second argument specifies the size of the vertex attributes (3 value/dimension)
  // The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values)
  // The fourth argument specifies if we want the data to be normalized.
  // The fifth argument is known as the stride and tells us the space between consecutive vertex attributes. (our case 3 float position)
  // The last parameter is the offset of where the position data begins in the buffer. Since the position data is at the start of the data array this value is just 0.
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0); // Enabling the pointed attribute (disabled by default)

  // glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer type specified. Could exists more Vertex Buffer and could be useful nullify it. To understand better.

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Do not do it until the VAO is active. To understand better.

  // glBindVertexArray(0); // unbind the Vertex Array. Could exists more Vertex Array and could be useful nullify it. To understand better.

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function: Set the color value that OpenGL use to reset the color buffer
    glClear(GL_COLOR_BUFFER_BIT); // state-using function: if GL_COLOR_BUFFER_BIT is specified clear the color setted using the glClearColor function

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3); // Draw mode: triangle, starting index of the vertices array, number of element in the array
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw mode: triangle, count of indices array, type of the indexes, offset

    // The glfwSwapBuffers(window) will swap the color buffer (a large 2D buffer that contains color values for each pixel
    // in GLFW's window) that is used to render to during this render iteration and show it as output to the screen.
    // There is a front buffer that contain a lists of pixel ready to be shown, then exits a back buffer that is used
    // to prepare the next frame while the current frame is shown.
    glfwSwapBuffers(window);
    // The glfwPollEvents() checks if any events are triggered (like keyboard input or mouse movement events),
    // updates the window state, and calls the corresponding functions (which we can register via callback methods)
    glfwPollEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

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
