#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "application.hpp"

namespace {
// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void glfw_framebuffer_size_callback(GLFWwindow* /*window*/, int width,
                                    int height)
{
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
  auto* app_ptr =
      reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app_ptr->cursor_pos_callback(xpos, ypos);
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action,
                                int mods)
{
  auto* app_ptr =
      reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app_ptr->mouse_button_callback(window, button, action, mods);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  auto* app_ptr =
      reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app_ptr->scroll_callback(xoffset, yoffset);
}

void glfw_key_callback(GLFWwindow* window)
{
  auto* app_ptr =
      reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
  app_ptr->key_callback(window);
}
} // namespace

Application::Application(int width, int height, std::string_view title)
    : width_{width}, height_{height}
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width_, height_, title.data(), nullptr, nullptr);
  if (window_ == nullptr) {
    fmt::print(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    std::exit(1);
  }
  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, glfw_framebuffer_size_callback);
  glfwSetMouseButtonCallback(window_, glfw_mouse_button_callback);
  glfwSetCursorPosCallback(window_, glfw_cursor_pos_callback);
  glfwSetScrollCallback(window_, glfw_scroll_callback);

  glfwSetWindowUserPointer(window_, this);

  // tell GLFW to capture our mouse
  // glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) ==
      0) {
    fmt::print(stderr, "Failed to initialize GLAD\n");
    glfwTerminate();
    std::exit(1);
  }

  // Imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 450");

  // Load Fonts
  io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 30.0f);

  glEnable(GL_DEPTH_TEST);

  starting_time_ = std::chrono::high_resolution_clock::now();
}

Application::~Application()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window_);
  glfwTerminate();
}

auto Application::render() -> void
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  render_impl();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

auto Application::run() -> void
{
  last_frame_ = std::chrono::high_resolution_clock::now();
  while (glfwWindowShouldClose(window_) == 0) {
    const auto current_time = std::chrono::high_resolution_clock::now();
    delta_time_ = current_time - last_frame_;
    last_frame_ = current_time;

    glfw_key_callback(window_);
    render();

    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
}

auto Application::cursor_pos_callback(double /*xpos*/, double /*ypos*/) -> void
{
}
auto Application::key_callback(GLFWwindow * /*window*/) -> void {}
auto Application::mouse_button_callback(GLFWwindow* /*window*/, int /*button*/,
                                        int /*action*/, int /*mods*/) -> void
{
}
auto Application::scroll_callback(double /*xoffset*/, double /*yoffset*/)
    -> void
{
}
