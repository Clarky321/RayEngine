# Установка минимальной версии CMake
cmake_minimum_required(VERSION 3.12)

# Определение названия проекта
set(PROJECT_NAME RayEngine)
project(${PROJECT_NAME})

# Добавления поддиректории для модуля RayEngineCore (ядро движка)
add_subdirectory(RayEngineCore)

# Добавления поддиректории для модуля RayEngineEditor (запускаем файл приложения)
add_subdirectory(RayEngineEditor)

# Установка RayEngineEditor в качестве стартового проекта для Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT RayEngineEditor)