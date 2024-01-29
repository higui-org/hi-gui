cmake_minimum_required(VERSION 3.20)

# higui 24.0
set(PROJECT_NAME higui)
project(${PROJECT_NAME} VERSION 24.0)
set(CMAKE_CXX_STANDARD 17)
set(GLAD_NAME glad_gl_compat_33)
set(GLAD_GENERATE_PATH ${CMAKE_CURRENT_BINARY_DIR}/gladsources/${GLAD_NAME})

# -----------------------------
#           BUILDS
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE) # Set Debug and Release configurations

if (CMAKE_BUILD_TYPE MATCHES Debug) # debug
    add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE MATCHES Release) #release
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

# Set specific flags for MSVC
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

# -----------------------------

# Sources
set(SOURCES
    src/main.cpp
    ${GLAD_GENERATE_PATH}/src/gl.c
    src/parser/parser.cpp

    #explicit MARKUP parser: HIML
    src/parser/himl/himl.cpp
    src/parser/himl/indent.cpp

    #explicit STYLE parser: HIST
    src/parser/hist/hist.cpp
)

# -----------------------------
#          INCLUDE
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glm)
include_directories(${GLAD_GENERATE_PATH}/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glad/cmake)
# -----------------------------

# GLAD
glad_add_library(${GLAD_NAME} SHARED API gl:compatibility=3.3)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})


# -----------------------------
#          LINKING
if(MSVC)
    target_link_libraries(${PROJECT_NAME} glfw ${GLAD_NAME})
elseif(APPLE)
    # Specific libraries for macOS
    target_link_libraries(${PROJECT_NAME} glfw ${GLAD_NAME} "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
else()
    # For Linux and other Unix-like systems
    target_link_libraries(${PROJECT_NAME} glfw ${GLAD_NAME} dl m GL X11 pthread stdc++fs)
endif()
# -----------------------------
