#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * Shader convention name:
 * .vert - a vertex shader
 * .tesc - a tessellation control shader
 * .tese - a tessellation evaluation shader
 * .geom - a geometry shader
 * .frag - a fragment shader
 * .comp - a compute shader
 */

class Shader {
public:
  unsigned int ID;

  Shader(const char* vertexPath, const char* geometryPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string geometryCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream gShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      vShaderFile.open(vertexPath);
      gShaderFile.open(geometryPath);
      fShaderFile.open(fragmentPath);

      std::stringstream vShaderStream, gShaderStream, fShaderStream;

      vShaderStream << vShaderFile.rdbuf();
      gShaderStream << gShaderFile.rdbuf();
      gShaderFile.close();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      geometryCode = gShaderStream.str();
      fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, geometry, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    this->checkCompileErrors(vertex, "VERTEX");

    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, nullptr);
    glCompileShader(geometry);
    this->checkCompileErrors(geometry, "GEOMETRY");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    this->checkCompileErrors(fragment, "FRAGMENT");

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, geometry);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    this->checkCompileErrors(this->ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
  }

  Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);

      std::stringstream vShaderStream, fShaderStream;

      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    this->checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    this->checkCompileErrors(fragment, "FRAGMENT");

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    this->checkCompileErrors(this->ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  void use() {
    glUseProgram(this->ID);
  }

  void setBool(const std::string& name, bool value) {
    int attributeLocation = glGetUniformLocation(this->ID, name.c_str());
    glUniform1i(attributeLocation, (int)value);
  }

  void setInt(const std::string& name, int value) {
    int attributeLocation = glGetUniformLocation(this->ID, name.c_str());
    glUniform1i(attributeLocation, value);
  }

  void setFloat(const std::string& name, float value) {
    int attributeLocation = glGetUniformLocation(this->ID, name.c_str());
    glUniform1f(attributeLocation, value);
  }

  void setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }
  void setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  void setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }

  void setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  void setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  void setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

private:
  void checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[512];

    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n";
        std::cout << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n";
        std::cout << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
  }
};
#endif
