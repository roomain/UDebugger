cmake_minimum_required(VERSION 3.24)
include(CMakeDependentOption)
project("UniversalDebugger" CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(CMAKE_CXX_STANDARD 20 CACHE STRING "c++ stantard" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(QT_VERSION_MAJOR "Qt major version")
set(QT_VERSION_MAJOR 6 CACHE STRING "Qt major version" FORCE)

#option(QT_LIB_DIR "Path where Qt library are located")
#set(QT_LIB_DIR "C:/Qt" PATH "" FORCE)
#set(QT_LIB_DIR "C:/Qt/6.5.0/msvc2019_64/lib" CACHE PATH "Path where Qt library are located" FORCE)

#set(QT_VERSION_MAJOR 6)
set(Qt${QT_VERSION_MAJOR}_DIR "C:/Qt/6.5.0/msvc2019_64/lib/cmake/Qt6" CACHE PATH "<Qt library dir>/cmake/Qt6" FORCE)
#cmake_dependent_option(Qt${QT_VERSION_MAJOR}_DIR "Qt cmake" ${QT_LIB_DIR}/cmake/Qt6  "QT_VERSION_MAJOR" FORCE)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/${CMAKE_GENERATOR_PLATFORM}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/${CMAKE_GENERATOR_PLATFORM}")
set(ARCHIVE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/${CMAKE_GENERATOR_PLATFORM}")

set(CMAKE_SOLUTION_DIR "${CMAKE_SOURCE_DIR}")

add_subdirectory("QtModels")
add_subdirectory("DebuggerInterfaces")
add_subdirectory("QtDebuggerServer")
add_subdirectory("QtDebugProtocol")
add_subdirectory("UniversalDebugger")
add_subdirectory("TestApp")

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "UniversalDebugger" )