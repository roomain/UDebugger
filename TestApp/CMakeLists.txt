project(TestApp VERSION 1.0.0 LANGUAGES CXX)
set(ProjectName TestApp)
file(GLOB_RECURSE SRCS "*.cpp")
file(GLOB_RECURSE INCS "*.h")


file(GLOB_RECURSE UI "*.ui")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "header files" FILES ${INCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source files" FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "form files" FILES ${UI})


find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Gui Widgets REQUIRED)
add_executable(${ProjectName} ${INCS} ${SRCS} ${UI})

target_link_libraries(${ProjectName} PRIVATE  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui)

add_dependencies(${ProjectName} DebuggerInterfaces)
add_dependencies(${ProjectName} QtDebuggerServer)
add_dependencies(${ProjectName} QtDebugProtocol)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../DebuggerInterfaces)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QtDebuggerServer)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QtDebugProtocol)

target_link_directories(${ProjectName} PRIVATE ${LIBRARY_OUTPUT_PATH})
target_link_libraries(${ProjectName} PRIVATE DebuggerInterfaces.lib)
target_link_libraries(${ProjectName} PRIVATE QtDebugProtocol.lib)
target_link_libraries(${ProjectName} PRIVATE QtDebuggerServer.lib)
