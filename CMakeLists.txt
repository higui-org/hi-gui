cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME higui)
project(${PROJECT_NAME} VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_COMPILER /usr/bin/g++-8)
endif()

set(SOURCES
    src/main.cpp
    src/higui.h
    src/higui.cpp
    src/hitypes.h
    src/hitypes.cpp
    src/shader.cpp
    src/shader.h
    src/markup.cpp
    src/markup.h
    src/object.cpp
    src/object.h
    src/attribute.h
    src/attribute.cpp
    src/DOM.h
    src/DOM.cpp    
    src/component.cpp
    src/component.h
)

set(THIRD_PARTY
    3rd_party/stb/stb.cpp
    3rd_party/glad/glad.c
    3rd_party/glad/glad.h
)

source_group("Source Files" FILES ${SOURCES})
source_group("Elements Source Files" FILES ${TAG_CLASSES})
source_group("3rd Party" FILES ${THIRD_PARTY})

add_executable(${PROJECT_NAME} 
${SOURCES} 
${TAG_CLASSES} 
${THIRD_PARTY})

if(MSVC)
	if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3Rel64.lib)
	endif()

	if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3Rel32.lib)
	endif()
else()
	target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a)
	#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
	set(CMAKE_CXX_FLAGS "-std=c++17 -g")	
	target_link_libraries(${PROJECT_NAME} dl m GL GLU X11 pthread stdc++fs)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party)