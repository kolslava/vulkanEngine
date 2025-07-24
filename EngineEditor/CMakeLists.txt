message(STATUS "[EngineEditor] Configuring executable...")

# Автоматично знаходимо всі файли .cpp у папці 'src'
file(GLOB_RECURSE EDITOR_SOURCES "src/*.cpp")

# Створюємо виконуваний файл редактора
add_executable(EngineEditor ${EDITOR_SOURCES})
message(STATUS "[EngineEditor] Created executable 'EngineEditor'")

# Лінкуємо наш редактор з ядром двигуна.
# Усі PUBLIC залежності з EngineCore (Vulkan, spdlog, glm) будуть автоматично підключені.
target_link_libraries(EngineEditor PRIVATE EngineCore ImGui)

message(STATUS "[EngineEditor] Linked with EngineCore library")