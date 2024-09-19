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

FetchContent_Declare(
	assimp
	GIT_REPOSITORY	https://github.com/assimp/assimp.git
	GIT_TAG 	ddb74c2bbdee1565dda667e85f0c82a0588c8053 #refs/tags/5.4.2
)
FetchContent_MakeAvailable(assimp)

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

add_executable(11_colors
  src/11_colors.cpp
)

add_executable(12_lighting_maps
  src/12_lighting_maps.cpp
)

add_executable(13_lighting_caster__directional_light
  src/13_lighting_caster__directional_light.cpp
)

add_executable(13_lighting_caster__point_light
  src/13_lighting_caster__point_light.cpp
)

add_executable(13_lighting_caster__spotlight
  src/13_lighting_caster__spotlight.cpp
)

add_executable(14_multiple_lights
  src/14_multiple_lights.cpp
)

add_executable(15_assimp
  src/15_assimp.cpp
)

add_executable(16_depth_testing
  src/16_depth_testing.cpp
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
target_link_libraries(11_colors PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(12_lighting_maps PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(13_lighting_caster__directional_light PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(13_lighting_caster__point_light PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(13_lighting_caster__spotlight PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(14_multiple_lights PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(15_assimp PRIVATE glfw glad stb_image glm::glm assimp )
target_link_libraries(16_depth_testing PRIVATE glfw glad stb_image glm::glm )

target_compile_definitions(6_shaders PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(7_textures PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(7_textures PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(8_transformations PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(8_transformations PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(9_coordinate_system PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(9_coordinate_system PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(10_camera PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(10_camera PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(11_colors PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(11_colors PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(12_lighting_maps PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(12_lighting_maps PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(13_lighting_caster__directional_light PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(13_lighting_caster__directional_light PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(13_lighting_caster__point_light PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(13_lighting_caster__point_light PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(13_lighting_caster__spotlight PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(13_lighting_caster__spotlight PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(14_multiple_lights PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(14_multiple_lights PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(15_assimp PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(15_assimp PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(16_depth_testing PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(16_depth_testing PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

include_directories(${PROJECT_SOURCE_DIR}/src/include)
