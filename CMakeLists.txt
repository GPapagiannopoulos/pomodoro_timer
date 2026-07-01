cmake_minimum_required(VERSION 3.20)
project(pomodoro_timer CXX C)

include(FetchContent)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw
	GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579
)
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui
	GIT_TAG 8936b58fe26e8c3da834b8f60b06511d537b4c63
)
FetchContent_Declare(
	miniaudio
	GIT_REPOSITORY https://github.com/mackron/miniaudio
	GIT_TAG 4a5b74bef029b3592c54b6048650ee5f972c1a48
)

FetchContent_MakeAvailable(glfw imgui miniaudio)

find_package(OpenGL REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pomodoro
	main.cpp
	"Session.h" 
	"Session.cpp"
	"${imgui_SOURCE_DIR}/imgui.cpp"
	"${imgui_SOURCE_DIR}/imgui_draw.cpp"
	"${imgui_SOURCE_DIR}/imgui_tables.cpp"
	"${imgui_SOURCE_DIR}/imgui_widgets.cpp"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
 "AudioPlayer.h" "AudioPlayer.cpp" "sqlite3/sqlite3.h" "sqlite3/sqlite3.c" "SessionRecord.h" "SessionRecord.cpp" "SessionStore.h" "SessionStore.cpp" "Paths.h" "Paths.cpp")

target_include_directories(
	pomodoro PRIVATE
	"${imgui_SOURCE_DIR}"
	"${imgui_SOURCE_DIR}/backends"
	"${miniaudio_SOURCE_DIR}"
	"C:/Users/georg/source/repos/pomodoro_timer/sqlite3"
)
target_link_libraries(
	pomodoro PRIVATE
	glfw
	OpenGL::GL
)

add_custom_command(TARGET pomodoro POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/sounds"
		"$<TARGET_FILE_DIR:pomodoro>/sounds"
)