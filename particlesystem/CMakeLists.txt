cmake_minimum_required(VERSION 3.21...3.31 FATAL_ERROR)

project(particlesystem LANGUAGES CXX)

enable_testing()
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMake)

set(CMAKE_XCODE_SCHEME_ADDRESS_SANITIZER_USE_AFTER_RETURN ON)
set(CMAKE_XCODE_SCHEME_ADDRESS_SANITIZER ON)
set(CMAKE_XCODE_SCHEME_MAIN_THREAD_CHECKER_STOP ON)
set(CMAKE_XCODE_SCHEME_UNDEFINED_BEHAVIOUR_SANITIZER ON)
set(CMAKE_XCODE_SCHEME_UNDEFINED_BEHAVIOUR_SANITIZER_STOP ON)

add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE 
  $<$<CXX_COMPILER_ID:MSVC>:
    /W4                 # Enable the highest warning level
    /w44388             # eneble 'signed/unsigned mismatch' '(off by default)
    /we4715             # turn 'not all control paths return a value' into a compile error
    /permissive-        # Stick to the standard
    >
  $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:
    -Wall               # Enable the highest warning level
    -Wextra 
    -Wconversion
    >
)

# Enable the Address Sanatizer, helps finding bugs at runtime
add_library(project_sanitize INTERFACE)
target_compile_options(project_sanitize INTERFACE 
  $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-fsanitize=address>
)
target_link_options(project_sanitize INTERFACE 
  $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-fsanitize=address>
)

# External libraries
find_package(Catch2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Rendering
add_library(rendering)
add_library(rendering::rendering ALIAS rendering)
target_sources(rendering
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS include
    FILES
        include/rendering/window.h
    PRIVATE
        src/rendering/window.cpp
)
target_link_libraries(rendering 
  PUBLIC 
    glm::glm
    fmt::fmt 
    project_warnings
    project_sanitize
  PRIVATE 
    glad::glad 
    glfw
    imgui::imgui 
)

# ExampleLibrary
add_library(example)
add_library(example::example ALIAS example)
target_sources(example
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS include
    FILES
        include/example/randomsystem.h
    PRIVATE
        src/example/randomsystem.cpp
)
target_link_libraries(example
  PUBLIC
    glm::glm
    fmt::fmt
    project_warnings
    project_sanitize
)

# Particle System Library
add_library(particlesystem)
add_library(particlesystem::particlesystem ALIAS particlesystem)
target_sources(particlesystem
    PUBLIC
    FILE_SET HEADERS
    TYPE HEADERS
    BASE_DIRS include
    FILES
        include/particlesystem/particlesystem.h
	include/particlesystem/particle.h
	include/particlesystem/emitter.h
	include/particlesystem/effect.h
        # ADD PATICLE SYSTEM HEADER FILES HERE
    PRIVATE
        src/particlesystem/particlesystem.cpp
	src/particlesystem/particle.cpp
	src/particlesystem/emitter.cpp
	src/particlesystem/effect.cpp
        # ADD PATICLE SYSTEM SOURCE FILES HERE
)
target_link_libraries(particlesystem 
  PUBLIC
    glm::glm
    fmt::fmt
    project_warnings
    project_sanitize
)

# Unit tests
add_executable(unittest ${TEST_FILES})
target_include_directories(unittest PUBLIC "include")
target_sources(unittest
    PRIVATE
        unittest/randomsystem-tests.cpp
        unittest/particlesystem-tests.cpp
        # ADD MORE TEST FILES HERE
)
target_link_libraries(unittest 
  PUBLIC 
    Catch2::Catch2WithMain 
    particlesystem::particlesystem
    example::example
    project_warnings
    project_sanitize
)
add_test(NAME unittest COMMAND $<TARGET_FILE:unittest>)

# Application
add_executable(application ${APP_SOURCE_FILES} ${APP_HEADER_FILES})
target_sources(application
    PRIVATE
        src/application/main.cpp
)
target_link_libraries(application
  PRIVATE 
    rendering::rendering
    example::example
    particlesystem::particlesystem
    project_warnings
    project_sanitize
)

if(MSVC)
  set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT application)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang") 
  set_target_properties(application unittest PROPERTIES XCODE_GENERATE_SCHEME YES)
endif()


