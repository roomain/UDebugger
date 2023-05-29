project(DebuggerInterfaces VERSION 1.0.0 LANGUAGES CXX)
set(ProjectName DebuggerInterfaces)

file(GLOB_RECURSE SRCS "*.cpp")
file(GLOB_RECURSE INCS "*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "header files" FILES ${INCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source files" FILES ${SRCS})

add_library(${ProjectName} SHARED ${INCS} ${SRCS})
target_compile_definitions(${ProjectName} PRIVATE DEBUGGER_INTERFACE_LIB)