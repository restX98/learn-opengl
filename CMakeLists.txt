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

add_subdirectory(thirdparty/glad)


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

target_link_libraries(1_rectangle PRIVATE glfw glad)
target_link_libraries(2_double_triangle PRIVATE glfw glad)
target_link_libraries(3_double_triangle_multiple_VAO PRIVATE glfw glad)
target_link_libraries(4_double_triangle_different_color PRIVATE glfw glad)
target_link_libraries(5_uniform_dynamic_color PRIVATE glfw glad)
target_link_libraries(6_shaders PRIVATE glfw glad)

# copy shader files to build directory
file(GLOB SHADERS
  "src/shaders/*.vert"
  "src/shaders/*.tesc"
  "src/shaders/*.tese"
  "src/shaders/*.geom"
  "src/shaders/*.frag"
  "src/shaders/*.comp"
)

foreach(SHADER ${SHADERS})
  add_custom_command(TARGET 6_shaders PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E copy
    ${SHADER} $<TARGET_FILE_DIR:6_shaders>
  )
endforeach(SHADER)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
