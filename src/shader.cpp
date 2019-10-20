#include <fmt/format.h>
#include <fstream>
#include <sstream>
#include <type_traits>

#include "shader.hpp"

namespace {

// Read whole file into a string
[[nodiscard]] std::string readFile(std::string_view path)
{
  std::ifstream file{path.data()};

  if (!file.is_open()) {
    fmt::print(stderr, "Cannot open file {}\n", path);
    std::fflush(stdout);
  }

  std::stringstream ss;
  // read file's buffer contents into streams
  ss << file.rdbuf();

  return ss.str();
}

void checkCompilingError(unsigned int shader_id)
{
  int success;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

  if (!success) {
    constexpr int max_log_size = 512;
    char info_log[max_log_size];
    glGetShaderInfoLog(shader_id, max_log_size, nullptr,
                       static_cast<char*>(info_log));
    throw std::runtime_error{fmt::format(
        "Shader compilation error for shader {}: {}", shader_id, info_log)};
  }
}

void checkLinkingError(unsigned int program_id)
{
  int success;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  GLchar info_log[1024];
  if (!success) {
    glGetProgramInfoLog(program_id, 1024, nullptr,
                        static_cast<GLchar*>(info_log));
    throw std::runtime_error{fmt::format("Shader linking error: {}", info_log)};
  }
}

} // anonymous namespace

Shader::Shader(const char* source, Shader::Type type)
    : type_{type}, id_{glCreateShader(std::underlying_type_t<Type>(type))}
{
  glShaderSource(id_, 1, &source, nullptr);
  glCompileShader(id_);
  checkCompilingError(id_);
}

Shader::~Shader()
{
  glDeleteShader(id_);
}

Shader::Shader(Shader&& other) noexcept
    : type_{other.type_}, id_{std::exchange(other.id_, 0)}
{
}

Shader& Shader::operator=(Shader&& other) noexcept
{
  id_ = std::exchange(other.id_, 0);
  type_ = other.type_;
  return *this;
}

ShaderProgram::ShaderProgram(const std::vector<Shader>& shaders)
    : id_{glCreateProgram()}
{
  for (const auto& shader : shaders) {
    glAttachShader(id_, shader.id_);
  }

  glLinkProgram(id_);
  checkLinkingError(id_);
}

auto ShaderBuilder::load(std::string_view filename, Shader::Type type)
    -> ShaderBuilder&
{
  const std::string src = readFile(filename);
  shaders_.emplace_back(src.c_str(), type);
  return *this;
}
