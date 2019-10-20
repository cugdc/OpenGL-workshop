add_executable(app
    "application.hpp"
    "application.cpp"
    "main.cpp"
    "shader.hpp"
    "shader.cpp"
    "texture.hpp"
    "texture.cpp"
    )
target_link_libraries(app
    PRIVATE compiler_warnings
    CONAN_PKG::glm CONAN_PKG::glfw CONAN_PKG::fmt CONAN_PKG::stb glad imgui
    )
add_dependencies(app assets)
