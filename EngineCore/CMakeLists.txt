message(STATUS "[EngineCore] Configuring dependencies...")

# Знаходимо Vulkan SDK на вашій системі (має бути встановлений)
find_package(Vulkan REQUIRED)
message(STATUS "[EngineCore] Found Vulkan SDK: ${Vulkan_INCLUDE_DIRS}")

# Підключаємо бібліотеки з папки 'external'
message(STATUS "[EngineCore] Configuring submodules: GLFW, spdlog, GLM")
add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)

# --- Налаштування ImGui для роботи з Vulkan та GLFW ---
message(STATUS "[EngineCore] Configuring ImGui for Vulkan backend")
file(GLOB IMGUI_SOURCES
    ../external/imgui/*.cpp
    ../external/imgui/backends/imgui_impl_glfw.cpp
    ../external/imgui/backends/imgui_impl_vulkan.cpp
)
add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PUBLIC Vulkan::Vulkan PRIVATE glfw)

# --- Автоматичний пошук файлів ядра двигуна ---
message(STATUS "[EngineCore] Searching for source files...")
# Публічні заголовки (доступні для EngineEditor)
file(GLOB_RECURSE ENGINE_PUBLIC_HEADERS "include/EngineCore/*.hpp")
# Приватні файли (реалізація та внутрішні заголовки)
file(GLOB_RECURSE ENGINE_PRIVATE_SOURCES "src/EngineCore/*.cpp")

# --- Створення статичної бібліотеки EngineCore ---
message(STATUS "[EngineCore] Creating static library 'EngineCore'")
add_library(EngineCore STATIC
    ${ENGINE_PUBLIC_HEADERS}
    ${ENGINE_PRIVATE_SOURCES}
)

# Вказуємо шляхи до заголовків:
# PUBLIC: папка 'includes' стає доступною для тих, хто підключає EngineCore
# PRIVATE: папка 'src' доступна тільки всередині самого EngineCore
target_include_directories(EngineCore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Лінкуємо залежності до ядра двигуна
message(STATUS "[EngineCore] Linking dependencies...")
target_link_libraries(EngineCore
    PUBLIC
        Vulkan::Vulkan
        spdlog::spdlog
        glm::glm
    PRIVATE
        ImGui
        glfw
        Pdh 
)

message(STATUS "[EngineCore] Configuration finished successfully!")