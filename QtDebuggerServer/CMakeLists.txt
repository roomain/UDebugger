project(QtDebuggerServer VERSION 1.0.0 LANGUAGES CXX)
set(ProjectName QtDebuggerServer)

file(GLOB_RECURSE SRCS "*.cpp")
file(GLOB_RECURSE INCS "*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "header files" FILES ${INCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source files" FILES ${SRCS})

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)
add_library(${ProjectName} SHARED ${INCS} ${SRCS})
target_compile_definitions(${ProjectName} PRIVATE QDEBUGGERSERVER_LIB)

target_link_libraries(${ProjectName} PRIVATE  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

add_dependencies(${ProjectName} DebuggerInterfaces)
add_dependencies(${ProjectName} QtDebugProtocol)

target_link_directories(${ProjectName} PRIVATE ${LIBRARY_OUTPUT_PATH})
target_link_libraries(${ProjectName} PRIVATE DebuggerInterfaces.lib)
target_link_libraries(${ProjectName} PRIVATE QtDebugProtocol.lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../DebuggerInterfaces)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QtDebugProtocol)

