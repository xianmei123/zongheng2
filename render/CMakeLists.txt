cmake_minimum_required (VERSION 3.27)

SET_PROPERTY( GLOBAL PROPERTY USE_FOLDERS ON )


# 在CMakeLists.txt中定义项目根目录，并传递给代码
add_definitions(-DPROJECT_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")


set(CMAKE_CXX_STANDARD 11)


set(GLFW_LIB ${CMAKE_SOURCE_DIR}/render/lib/glfw3.lib)

set(ASSIMP_LIB ${CMAKE_SOURCE_DIR}/render/lib/assimp-vc140-mt.lib)


include_directories(${CMAKE_SOURCE_DIR}/render/include)

file(GLOB RENDER_SOURCES ${CMAKE_SOURCE_DIR}/render/*.cpp ${CMAKE_SOURCE_DIR}/render/*.c ${CMAKE_SOURCE_DIR}/render/jsonlib/*.c)

file(GLOB RENDER_HEADERS ${CMAKE_SOURCE_DIR}/render/*.h ${CMAKE_SOURCE_DIR}/render/jsonlib/*.h)

add_library(renderLib STATIC ${RENDER_SOURCES})

target_link_libraries(renderLib PRIVATE ${GLFW_LIB} ${ASSIMP_LIB})
