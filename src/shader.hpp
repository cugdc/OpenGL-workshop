#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

class ShaderProgram;

struct Shader {
public:
  enum class Type : GLenum {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    TessControl = GL_TESS_CONTROL_SHADER,
    TessEval = GL_TESS_EVALUATION_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    Compute = GL_COMPUTE_SHADER
  };

  Shader(const char* source, Type type);
  ~Shader();

  Shader(const Shader& other) = delete;
  Shader& operator=(const Shader& other) = delete;
  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

private:
  Type type_;
  unsigned int id_;

  friend ShaderProgram;
};

class ShaderProgram {
public:
  ShaderProgram() = default;
  explicit ShaderProgram(const std::vector<Shader>& shaders);

  void use() const
  {
    glUseProgram(id_);
  }

  [[nodiscard]] unsigned int id() const
  {
    return id_;
  }

  auto setBool(const std::string& name, bool value) const
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()),
                static_cast<int>(value));
  }
  auto setInt(const std::string& name, int value) const
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
  }
  auto setFloat(const std::string& name, float value) const
  {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
  }
  auto setVec2(const std::string& name, const glm::vec2& value) const
  {
    glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }
  auto setVec2(const std::string& name, float x, float y) const
  {
    glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
  }
  auto setVec3(const std::string& name, const glm::vec3& value) const
  {
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }
  auto setVec3(const std::string& name, float x, float y, float z) const
  {
    glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
  }
  auto setVec4(const std::string& name, const glm::vec4& value) const
  {
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }
  auto setVec4(const std::string& name, float x, float y, float z,
               float w) const
  {
    glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
  }
  auto setMat2(const std::string& name, const glm::mat2& mat) const
  {
    glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  auto setMat3(const std::string& name, const glm::mat3& mat) const
  {
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  auto setMat4(const std::string& name, const glm::mat4& mat) const
  {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }

private:
  unsigned int id_;
};

class ShaderBuilder {
public:
  ShaderBuilder() = default;

  auto load(std::string_view filename, Shader::Type type) -> ShaderBuilder&;

  [[nodiscard]] auto build() const -> ShaderProgram
  {
    return ShaderProgram{shaders_};
  }

private:
  std::vector<Shader> shaders_;
};

#endif // SHADER_HPP
