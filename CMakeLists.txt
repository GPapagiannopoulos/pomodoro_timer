cmake_minimum_required(VERSION 3.20)
project(pomodoro_timer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pomodoro
	main.cpp
 "Session.h" "Session.cpp")