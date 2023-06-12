add_executable(
${PROJECT_NAME}
main.cpp
../include/stb/stb.cpp
higui.h

../include/glad/glad.c
../include/glad/glad.h

shader.cpp
shader.h

markup_parser.cpp
markup_parser.h

style_parser.cpp
style_parser.h

gui_object.cpp
gui_object.h

any_map.h

)

if(MSVC)
	if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../lib/glfw3Rel64.lib)
	endif()

	if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../lib/glfw3Rel32.lib)
	endif()
else()
	target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libglfw3.a)
	#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
	set(CMAKE_CXX_FLAGS "-std=c++17 -g")	
	target_link_libraries(${PROJECT_NAME} dl m GL GLU X11 pthread stdc++fs)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ../src)
target_include_directories(${PROJECT_NAME} PRIVATE ../include)
target_include_directories(${PROJECT_NAME} PRIVATE ../include/imgui)
