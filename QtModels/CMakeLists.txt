project(QtModels VERSION 1.0.0 LANGUAGES CXX)
set(ProjectName QtModels)

file(GLOB_RECURSE SRCS "*.cpp")
file(GLOB_RECURSE INCS "*.h")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "header files" FILES ${INCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "source files" FILES ${SRCS})


find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)


add_library(${ProjectName} SHARED ${INCS} ${SRCS})
target_compile_definitions(${ProjectName} PRIVATE QTMODELS_LIB)


target_link_libraries(${ProjectName} Qt${QT_VERSION_MAJOR}::Core)
