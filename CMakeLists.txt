cmake_minimum_required(VERSION 3.7)

project(Renderer)


set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
file(GLOB src "include/*.h" "src/*.cpp")

find_package(glfw3)
find_package(OpenGL REQUIRED)


include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

include_directories(include)

add_executable(Renderer ${src})

target_link_libraries(Renderer glfw)

set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(Renderer PRIVATE "${GLAD_DIR}/include")
target_link_libraries(Renderer "glad" "${CMAKE_DL_LIBS}")
target_link_libraries(Renderer ${OPENGL_LIBRARIES})

install(TARGETS Renderer RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
