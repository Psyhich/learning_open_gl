cmake_minimum_required(VERSION 3.22)

project(blocks)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_REQUIRED TRUE)
set(CMAKE_CXX_COMPILE_FEATURES TRUE)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(lib/glad)

add_library(shared_lib OBJECT
	shared_src/src/glfw_wrapper.cpp
	shared_src/src/shaders.cpp
	shared_src/src/filesystem_utils.cpp
)
target_include_directories(shared_lib PUBLIC 
	shared_src/include/
)
target_link_libraries(shared_lib PUBLIC
	glfw
	glad
	${CMAKE_DL_LIBS}
	${CONAN_LIBS}
	OpenGL::GL
)

function(add_copy_shaders_event TARGET_NAME SHADERS_DIR)
	file(GLOB "${TARGET_NAME}_SHADERS" CONFIGURE_DEPENDS "${SHADERS_DIR}/*.glsl")

	set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders-${TARGET_NAME}/")

	add_custom_command(
		OUTPUT "${SHADER_OUTPUT_DIR}"
		COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${SHADER_OUTPUT_DIR}"
	)

	foreach(SHADER_FILE ${${TARGET_NAME}_SHADERS})
		cmake_path(GET SHADER_FILE FILENAME filename)
		set(COPIED_SHADER "${SHADER_OUTPUT_DIR}/${filename}")
		list(APPEND "${TARGET_NAME}_COPIED_SHADERS" "${COPIED_SHADER}")

		add_custom_command(
			OUTPUT "${COPIED_SHADER}"
			COMMAND
				"${CMAKE_COMMAND}" "-E" "copy_if_different"
				"${SHADER_FILE}" "${COPIED_SHADER}"
			COMMENT "Copying ${SHADER_FILE}"
			DEPENDS "${SHADER_OUTPUT_DIR}/"
			DEPENDS "${SHADER_FILE}"
		)
	endforeach(SHADER_FILE)

	add_custom_target("BUILD_AND_COPY_FOR_${TARGET_NAME}" ALL
		DEPENDS "${TARGET_NAME}" "${${TARGET_NAME}_COPIED_SHADERS}"
	)

	target_compile_definitions("${TARGET_NAME}" PRIVATE
		"SHADERS_PATH=\"${SHADER_OUTPUT_DIR}\""
	)
	message(STATUS "Added shader copy event for ${TARGET_NAME}")
endfunction(add_copy_shaders_event)

function(link_open_gl TARGET_NAME)
	target_link_libraries("${TARGET_NAME}" PUBLIC
		shared_lib
	)
	message(STATUS "Linked ${TARGET_NAME} to OpenGL libraries")
endfunction(link_open_gl)

add_subdirectory(./tutors/basic_draw/)
add_subdirectory(./tutors/element_buffer/)
add_subdirectory(./tutors/advanced_shaders/)
