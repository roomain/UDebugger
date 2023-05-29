project(QtDebugProtocol VERSION 1.0.0 LANGUAGES CXX)
set(ProjectName QtDebugProtocol)

file(GLOB_RECURSE SRCS "*.cpp")
file(GLOB_RECURSE INCS "*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "header files" FILES ${INCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source files" FILES ${SRCS})



find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
add_library(${ProjectName} SHARED ${INCS} ${SRCS})
target_compile_definitions(${ProjectName} PRIVATE DEBUGPROTOCOL_LIB)

target_link_libraries(${ProjectName} PRIVATE Qt${QT_VERSION_MAJOR}::Core)

add_dependencies(${ProjectName} DebuggerInterfaces)
target_link_directories(${ProjectName} PRIVATE ${LIBRARY_OUTPUT_PATH})
target_link_libraries(${ProjectName} PRIVATE DebuggerInterfaces.lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../DebuggerInterfaces)