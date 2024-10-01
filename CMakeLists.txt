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
add_subdirectory(thirdparty/imgui)

add_executable(1_1_rectangle
  src/1_getting_started/1_rectangle.cpp
)

add_executable(1_2_double_triangle
  src/1_getting_started/2_double_triangle.cpp
)

add_executable(1_3_double_triangle_multiple_VAO
  src/1_getting_started/3_double_triangle_multiple_VAO.cpp
)

add_executable(1_4_double_triangle_different_color
  src/1_getting_started/4_double_triangle_different_color.cpp
)

add_executable(1_5_uniform_dynamic_color
  src/1_getting_started/5_uniform_dynamic_color.cpp
)

add_executable(1_6_shaders
  src/1_getting_started/6_shaders.cpp
)

add_executable(1_7_textures
  src/1_getting_started/7_textures.cpp
)

add_executable(1_8_transformations
  src/1_getting_started/8_transformations.cpp
)

add_executable(1_9_coordinate_system
  src/1_getting_started/9_coordinate_system.cpp
)

add_executable(1_10_camera
  src/1_getting_started/10_camera.cpp
)

add_executable(2_1_colors
  src/2_lighting/1_colors.cpp
)

add_executable(2_2_lighting_maps
  src/2_lighting/2_lighting_maps.cpp
)

add_executable(2_3_lighting_caster__directional_light
  src/2_lighting/3_lighting_caster__directional_light.cpp
)

add_executable(2_3_lighting_caster__point_light
  src/2_lighting/3_lighting_caster__point_light.cpp
)

add_executable(2_3_lighting_caster__spotlight
  src/2_lighting/3_lighting_caster__spotlight.cpp
)

add_executable(2_4_multiple_lights
  src/2_lighting/4_multiple_lights.cpp
)

add_executable(3_1_assimp
  src/3_advanced_opengl/1_assimp.cpp
)

add_executable(3_2_depth_testing
  src/3_advanced_opengl/2_depth_testing.cpp
)

add_executable(3_3_stencil_testing
  src/3_advanced_opengl/3_stencil_testing.cpp
)

add_executable(3_4_discard_fragments
  src/3_advanced_opengl/4_discard_fragments.cpp
)

add_executable(3_5_blending
  src/3_advanced_opengl/5_blending.cpp
)

add_executable(3_6_face_culling
  src/3_advanced_opengl/6_face_culling.cpp
)

add_executable(3_7_framebuffers
  src/3_advanced_opengl/7_framebuffers.cpp
)

add_executable(3_8_0_cubemaps
  src/3_advanced_opengl/8_0_cubemaps.cpp
)

add_executable(3_8_1_cubemaps_reflection
  src/3_advanced_opengl/8_1_cubemaps_reflection.cpp
)

add_executable(3_8_2_cubemaps_refraction
  src/3_advanced_opengl/8_2_cubemaps_refraction.cpp
)

add_executable(3_9_advanced_glsl__uniform_buffers
  src/3_advanced_opengl/9_advanced_glsl__uniform_buffers.cpp
)

add_executable(3_10_0_geometry_shader
  src/3_advanced_opengl/10_0_geometry_shader.cpp
)

add_executable(3_10_1_geometry_shader__exploding_object
  src/3_advanced_opengl/10_1_geometry_shader__exploding_object.cpp
)

add_executable(3_10_2_geometry_shader__visualizing_normal_vec
  src/3_advanced_opengl/10_2_geometry_shader__visualizing_normal_vec.cpp
)

add_executable(3_0_imgui
  src/3_advanced_opengl/0_imgui.cpp
)

target_link_libraries(1_1_rectangle PRIVATE glfw glad )
target_link_libraries(1_2_double_triangle PRIVATE glfw glad )
target_link_libraries(1_3_double_triangle_multiple_VAO PRIVATE glfw glad )
target_link_libraries(1_4_double_triangle_different_color PRIVATE glfw glad )
target_link_libraries(1_5_uniform_dynamic_color PRIVATE glfw glad )
target_link_libraries(1_6_shaders PRIVATE glfw glad  glm::glm )
target_link_libraries(1_7_textures PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(1_8_transformations PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(1_9_coordinate_system PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(1_10_camera PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(2_1_colors PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(2_2_lighting_maps PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(2_3_lighting_caster__directional_light PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(2_3_lighting_caster__point_light PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(2_3_lighting_caster__spotlight PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(2_4_multiple_lights PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_0_imgui PRIVATE glfw glad stb_image glm::glm assimp imgui )
target_link_libraries(3_1_assimp PRIVATE glfw glad stb_image glm::glm assimp )
target_link_libraries(3_2_depth_testing PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_3_stencil_testing PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_4_discard_fragments PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_5_blending PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_6_face_culling PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_7_framebuffers PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_8_0_cubemaps PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_8_1_cubemaps_reflection PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_8_2_cubemaps_refraction PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_9_advanced_glsl__uniform_buffers PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_10_0_geometry_shader PRIVATE glfw glad stb_image glm::glm )
target_link_libraries(3_10_1_geometry_shader__exploding_object PRIVATE glfw glad stb_image glm::glm assimp )
target_link_libraries(3_10_2_geometry_shader__visualizing_normal_vec PRIVATE glfw glad stb_image glm::glm assimp )

target_compile_definitions(1_6_shaders PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(1_7_textures PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(1_7_textures PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(1_8_transformations PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(1_8_transformations PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(1_9_coordinate_system PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(1_9_coordinate_system PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(1_10_camera PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(1_10_camera PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(2_1_colors PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(2_1_colors PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(2_2_lighting_maps PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(2_2_lighting_maps PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(2_3_lighting_caster__directional_light PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(2_3_lighting_caster__directional_light PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(2_3_lighting_caster__point_light PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(2_3_lighting_caster__point_light PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(2_3_lighting_caster__spotlight PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(2_3_lighting_caster__spotlight PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(2_4_multiple_lights PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(2_4_multiple_lights PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_0_imgui PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_0_imgui PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_1_assimp PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_1_assimp PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_2_depth_testing PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_2_depth_testing PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_3_stencil_testing PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_3_stencil_testing PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_4_discard_fragments PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_4_discard_fragments PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_5_blending PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_5_blending PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_6_face_culling PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_6_face_culling PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_7_framebuffers PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_7_framebuffers PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_8_0_cubemaps PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_8_0_cubemaps PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_8_1_cubemaps_reflection PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_8_1_cubemaps_reflection PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_8_2_cubemaps_refraction PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_8_2_cubemaps_refraction PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_9_advanced_glsl__uniform_buffers PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_9_advanced_glsl__uniform_buffers PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_10_0_geometry_shader PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_10_0_geometry_shader PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_10_1_geometry_shader__exploding_object PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_10_1_geometry_shader__exploding_object PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
target_compile_definitions(3_10_2_geometry_shader__visualizing_normal_vec PUBLIC SHADERS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/")
target_compile_definitions(3_10_2_geometry_shader__visualizing_normal_vec PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

include_directories(${PROJECT_SOURCE_DIR}/src/include)
