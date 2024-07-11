cmake_minimum_required(VERSION 3.10)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw # check how to specify a tag version
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
  FetchContent_Populate(glfw)

  set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
  set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
  set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
  set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

  add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/stb_image)


add_executable(1_rectangle
  src/1_rectangle.cpp
)

add_executable(2_double_triangle
  src/2_double_triangle.cpp
)

add_executable(3_double_triangle_multiple_VAO
  src/3_double_triangle_multiple_VAO.cpp
)

add_executable(4_double_triangle_different_color
  src/4_double_triangle_different_color.cpp
)

add_executable(5_uniform_dynamic_color
  src/5_uniform_dynamic_color.cpp
)

add_executable(6_shaders
  src/6_shaders.cpp
)

add_executable(7_textures
  src/7_textures.cpp
)

add_executable(8_transformations
  src/8_transformations.cpp
)

add_executable(9_coordinate_system
  src/9_coordinate_system.cpp
)

add_executable(10_camera
  src/10_camera.cpp
)

target_link_libraries(1_rectangle PRIVATE glfw glad )
target_link_libraries(2_double_triangle PRIVATE glfw glad )
target_link_libraries(3_double_triangle_multiple_VAO PRIVATE glfw glad )
target_link_libraries(4_double_triangle_different_color PRIVATE glfw glad )
target_link_libraries(5_uniform_dynamic_color PRIVATE glfw glad )
target_link_libraries(6_shaders PRIVATE glfw glad  glm::glm )
target_link_libraries(7_textures PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(8_transformations PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(9_coordinate_system PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(10_camera PRIVATE glfw glad stb_image glm::glm )

target_compile_definitions(6_shaders PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(7_textures PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(7_textures PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(8_transformations PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(8_transformations PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(9_coordinate_system PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(9_coordinate_system PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(10_camera PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(10_camera PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

include_directories(${PROJECT_SOURCE_DIR}/src/include)
