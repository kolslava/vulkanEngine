cmake_minimum_required(VERSION 3.16)
project(VulkanEngine)

message(STATUS "Configuring ${PROJECT_NAME}...")

# Встановлюємо стандарт C++17 для всього проекту
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Вказуємо, щоб усі виконувані файли зберігалися в папку 'bin'
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

message(STATUS "Adding subdirectories: EngineCore and EngineEditor")
add_subdirectory(EngineCore)
add_subdirectory(EngineEditor)

# Встановлюємо EngineEditor як проект для запуску у Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT EngineEditor)
message(STATUS "Set EngineEditor as startup project for Visual Studio")

message(STATUS "Configuration of ${PROJECT_NAME} is complete!")