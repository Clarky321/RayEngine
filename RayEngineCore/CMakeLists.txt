# Установка минимальной версии CMake
cmake_minimum_required(VERSION 3.12)

# Определение названия проекта RayEngineCore
set(ENGINE_PROJECT_NAME RayEngineCore)

# Список публичных заголовочных файлов
set(ENGINE_PUBLIC_INCLUDES
	includes/RayEngineCore/cube.h
	includes/RayEngineCore/cameraController.h
	includes/RayEngineCore/mainMenu.h	
	includes/RayEngineCore/mapEditor.h
)

# Список приватных исходных файлов
set(ENGINE_PRIVATE_SOURCES	
	src/RayEngineCore/cube.cpp
	src/RayEngineCore/cameraController.cpp
	src/RayEngineCore/mainMenu.cpp
	src/RayEngineCore/mapEditor.cpp
)

# Общий список исходных файлов
set(ENGINE_ALL_SOURCE
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_SOURCES}
)

# Создание статической библиотеки с названием RayEngineCore
add_library(${ENGINE_PROJECT_NAME} STATIC ${ENGINE_ALL_SOURCE})

# Группировка файлов в структуру проекта в IDE
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_ALL_SOURCE})

# Добавление путей к заголовочным файлам для внешних пользователей и для компиляции
target_include_directories(${ENGINE_PROJECT_NAME}
	PUBLIC
		includes
	PRIVATE
		src
)

# Установка стандарта C++ для компиляции
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

# ------------------------------------------------------------------------------------- #

add_subdirectory(../external/raylib ${CMAKE_CURRENT_BINARY_DIR}/raylib)
add_subdirectory(../external/raygui/projects/CMake ${CMAKE_CURRENT_BINARY_DIR}/raygui)
target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC raylib raygui)

# ------------------------------------------------------------------------------------- #

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	
	../external/imgui/imconfig.h
	../external/imgui/imgui_internal.h
	
	../external/imgui/imstb_rectpack.h
	../external/imgui/imstb_textedit.h
	../external/imgui/imstb_truetype.h
	
)

set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	
	../external/imgui/imgui_draw.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_tables.cpp
	
	../external/imgui/imgui_demo.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)

#target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC ImGui rlImGui)

# ------------------------------------------------------------------------------------- #

set(RLLMGUI_INCLUDES
	../external/rllmgui/imgui_impl_raylib.h
	../external/rllmgui/rlImGui.h
	../external/rllmgui/rlImGuiColors.h
	
	../external/rllmgui/extras/FA6FreeSolidFontData.h
	../external/rllmgui/extras/IconsFontAwesome6.h
)

set(RLLMGUI_SOURCES
	../external/rllmgui/rlImGui.cpp
)

add_library(rlImGui STATIC
	${RLLMGUI_INCLUDES}
	${RLLMGUI_SOURCES}
)

target_include_directories(rlImGui PUBLIC ../external/rllmgui)

target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC ImGui rlImGui)