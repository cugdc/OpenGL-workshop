#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <chrono>
#include <string_view>

#include <fmt/format.h>

class Application {
public:
  using DeltaDuration = std::chrono::duration<double, std::milli>;

  Application(int width, int height, std::string_view title);
  virtual ~Application();

  Application(const Application& app) = delete;
  Application& operator=(const Application& app) = delete;
  Application(Application&& app) = delete;
  Application& operator=(Application&& app) = delete;

  auto render() -> void;
  auto run() -> void;

  [[nodiscard]] auto delta_time() const noexcept -> DeltaDuration
  {
    return delta_time_;
  }

  [[nodiscard]] auto width() const noexcept -> int
  {
    return width_;
  }

  [[nodiscard]] auto height() const noexcept -> int
  {
    return height_;
  }

  [[nodiscard]] auto window() noexcept -> GLFWwindow&
  {
    return *window_;
  }

  virtual auto cursor_pos_callback(double xpos, double ypos) -> void;
  virtual auto key_callback(GLFWwindow* window) -> void;
  virtual auto mouse_button_callback(GLFWwindow* window, int button, int action,
                                     int mods) -> void;
  virtual auto scroll_callback(double xoffset, double yoffset) -> void;

private:
  GLFWwindow* window_{};

  int width_{};
  int height_{};

  DeltaDuration delta_time_{};
  std::chrono::high_resolution_clock::time_point last_frame_;

  virtual auto render_impl() -> void = 0;
};

#endif // APPLICATION_HPP
