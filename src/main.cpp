#include <optional>

#include "application.hpp"

#include <imgui.h>

struct MousePos {
  double x;
  double y;
};

class TriangleApp : public Application {
public:
  TriangleApp() : Application{1920, 1080, "Triangle App"} {}

private:
  std::optional<MousePos> last_mouse_{};

  auto render_impl() -> void override
  {
    glClearColor(0.188f, 0.039f, 0.141f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw_gui();
    draw_scene();
  }

  auto draw_gui() -> void
  {
    ImGui::Begin("Info");

    ImGui::Text("%.3f ms/frame (%.1f FPS)", this->delta_time().count(),
                1000 / this->delta_time().count());
    ImGui::Text("Mouse at %.1f, %.1f", last_mouse_->x, last_mouse_->y);

    ImGui::End();
    ImGui::Render();
  }

  auto draw_scene() -> void
  {
    // TODO
  }

  auto cursor_pos_callback(double xpos, double ypos) -> void override
  {
    if (!last_mouse_) {
      last_mouse_ = MousePos{xpos, ypos};
    }

    // reversed since y-coordinates go from bottom to top
    const auto xoffset = xpos - last_mouse_->x;
    const auto yoffset = last_mouse_->y - ypos;

    last_mouse_ = MousePos{xpos, ypos};

    // Do something with this information
  }

  auto key_callback(GLFWwindow* window) -> void override
  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, 1);
    }
  }
};

int main()
try {
  TriangleApp app;
  app.run();
} catch (const std::exception& e) {
  fmt::print(stderr, "Error: {}\n", e.what());
} catch (...) {
  fmt::print(stderr, "Unknown exception!\n");
}
