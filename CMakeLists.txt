cmake_minimum_required(VERSION 3.7)

project(Renderer)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11 -g")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
        if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
                string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        else()
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        endif()
endif()



set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
file(GLOB src "include/*.h" "src/*.cpp")

find_package(glfw3)
find_package(OpenGL REQUIRED)
find_package(DevIL REQUIRED)

include_directories(${LIB_DIR}/jsoncpp/include)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})
include_directories(${LIB_DIR}/eigen)
include_directories(include)
include_directories(${DEVIL_INCLUDE_DIRS})

message(STATUS ${DEVIL_INCLUDE_DIRS})

add_executable(Renderer ${src})

target_link_libraries(Renderer glfw)
target_link_libraries(Renderer ${IL_LIBRARIES} ${ILU_LIBRARIES} ${ILUT_LIBRARIES})

set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(Renderer PRIVATE "${GLAD_DIR}/include")
target_link_libraries(Renderer "glad" "${CMAKE_DL_LIBS}")

set(JSONCPP_DIR "${LIB_DIR}/jsoncpp")
execute_process(COMMAND python ${JSONCPP_DIR}/amalgamate.py WORKING_DIRECTORY ${JSONCPP_DIR})
add_library("jsoncpp" "${JSONCPP_DIR}/dist/jsoncpp.cpp")
target_include_directories("jsoncpp" PRIVATE "${JSONCPP_DIR}/dist")
target_include_directories(Renderer PRIVATE "${JSONCPP_DIR}/dist")
target_link_libraries(Renderer "jsoncpp" "${CMAKE_DL_LIBS}")

target_link_libraries(Renderer ${OPENGL_LIBRARIES})

install(TARGETS Renderer RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
