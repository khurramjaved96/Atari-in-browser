cmake_minimum_required(VERSION 3.14)

# Project specific target properties

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

project(ale-wasm LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
add_subdirectory( glfw )

set( IMGUI
        imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_demo.cpp
        imgui/imgui_widgets.cpp imgui/imconfig.h imgui/imgui.h
        imgui/imgui_internal.h imgui/imstb_rectpack.h imgui/imstb_textedit.h imgui/imstb_truetype.h
        imgui/imgui_impl_glfw.h imgui/imgui_impl_opengl3.h imgui/imgui_impl_glfw.cpp imgui/imgui_impl_opengl3.cpp
        )

add_executable(ale WIN32 desktop_demo.cpp ${IMGUI})
add_subdirectory(src)

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(ale
        PUBLIC
        ZLIB::ZLIB
        Threads::Threads dl
        ${OPENGL_LIBRARIES} glfw)
