project(UniversalDebugger VERSION 1.0.0 LANGUAGES CXX)
set(ProjectName UniversalDebugger)
file(GLOB_RECURSE SRCS "*.cpp")
file(GLOB_RECURSE INCS "*.h")


file(GLOB_RECURSE UI "*.ui")
file(GLOB_RECURSE QRC "*.qrc")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "header files" FILES ${INCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source files" FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "form files" FILES ${UI})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "resources files" FILES ${QRC})


find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Gui Widgets Xml REQUIRED)

add_executable(${ProjectName} ${INCS} ${SRCS} ${UI} ${QRC})

target_link_libraries(${ProjectName} PRIVATE  Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Xml)

add_dependencies(${ProjectName} DebuggerInterfaces)
add_dependencies(${ProjectName} QtDebuggerServer)
add_dependencies(${ProjectName} QtDebugProtocol)
add_dependencies(${ProjectName} QtModels)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../DebuggerInterfaces)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QtDebuggerServer)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QtDebugProtocol)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../QtModels)

target_link_directories(${ProjectName} PRIVATE ${LIBRARY_OUTPUT_PATH})
target_link_libraries(${ProjectName} PRIVATE DebuggerInterfaces.lib)
target_link_libraries(${ProjectName} PRIVATE QtDebugProtocol.lib)
target_link_libraries(${ProjectName} PRIVATE QtDebuggerServer.lib)
target_link_libraries(${ProjectName} PRIVATE QtModels.lib)
