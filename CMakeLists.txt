cmake_minimum_required(VERSION 3.24)

project("UniversalDebugger" CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(CMAKE_CXX_STANDARD 20 CACHE STRING "c++ stantard" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#option(QT_VERSION_MAJOR "Qt major version" "6")
set(QT_VERSION_MAJOR CACHE STRING "6")

#option(QT_LIB_DIR "Path where Qt library are located" "C:/")
set(QT_LIB_DIR CACHE PATH "C:/Qt/6.5.0/msvc2019_64")

find_file(QT_EXIST "Qt${QT_VERSION_MAJOR}Config.cmake" "${QT_LIB_DIR}/lib/cmake/Qt${QT_VERSION_MAJOR}")


if(EXISTS ${QT_EXIST})
    MESSAGE("File exist: ${QT_LIB_DIR}/lib/cmake/Qt${QT_VERSION_MAJOR}/Qt${QT_VERSION_MAJOR}Config.cmake")
    set(Qt${QT_VERSION_MAJOR}_DIR CACHE PATH "${QT_LIB_DIR}/lib/cmake/Qt${QT_VERSION_MAJOR}")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/${CMAKE_GENERATOR_PLATFORM}")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/${CMAKE_GENERATOR_PLATFORM}")
    set(ARCHIVE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/${CMAKE_GENERATOR_PLATFORM}")

    set(CMAKE_SOLUTION_DIR "${CMAKE_SOURCE_DIR}")
    set(Qt${QT_VERSION_MAJOR}_DIR "${QT_LIB_DIR}/lib/cmake/Qt${QT_VERSION_MAJOR}")

    add_subdirectory("QtModels")
    add_subdirectory("DebuggerInterfaces")
    add_subdirectory("QtDebuggerServer")
    add_subdirectory("QtDebugProtocol")
    add_subdirectory("UniversalDebugger")
    add_subdirectory("TestApp")
    set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "UniversalDebugger" )
else()    
    MESSAGE("File NOT exist: ${QT_LIB_DIR}/lib/cmake/Qt${QT_VERSION_MAJOR}/Qt${QT_VERSION_MAJOR}Config.cmake")
endif()
