cmake_minimum_required(VERSION 3.22)

project(element_buffer)

add_executable("${PROJECT_NAME}"
	src/main.cpp
)

link_open_gl("${PROJECT_NAME}")
add_copy_shaders_event("${PROJECT_NAME}"
	"${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)


