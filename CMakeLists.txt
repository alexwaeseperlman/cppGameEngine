cmake_minimum_required(VERSION 3.7)

project(Renderer)

# Bump up warning levels appropriately for clang, gcc & msvc
# Also set debug/optimization flags depending on the build type. IDE users choose this when
# selecting the build mode in their IDE
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++14 -g")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
        if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
                string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        else()
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
        endif()
endif()


if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
file(GLOB src "include/*.h" "src/*.cpp")

find_package(glfw3)
find_package(OpenGL REQUIRED)
find_package(DevIL REQUIRED)

include_directories(${LIB_DIR}/jsoncpp/include)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})
message(STATUS ${GLFW3_INCLUDE_DIRS})
include_directories(${LIB_DIR}/ultralight/deps)
include_directories(include)
include_directories(${DEVIL_INCLUDE_DIRS})
include_directories(${LIB_DIR}/glm)

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

# Iinclude ultralight
set(ULTRALIGHT_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/libraries/ultralight/include")
if (APPLE)
	set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/libraries/ultralight/bin/mac")
	set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/libraries/ultralight/bin/mac")
elseif(UNIX)
	set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/libraries/ultralight/bin/linux")
	set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/libraries/ultralight/bin/linux")
else()
	set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/libraries/ultralight/bin/win")
	set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/libraries/ultralight/bin/win")
endif()

message(STATUS ${ULTRALIGHT_LIBRARY_DIRS})

link_directories("${ULTRALIGHT_LIBRARY_DIRS}")

if (NOT APPLE)
    link_libraries(UltralightCore Ultralight WebCore)
else ()
    # On macOS the last two libraries are Frameworks so we must pass special flags to the linker
    link_libraries(${ULTRALIGHT_LIBRARY_DIRS}/libUltralightCore.dylib "-framework Ultralight -framework WebCore -Wl,-F${ULTRALIGHT_LIBRARY_DIRS}")
endif()

include_directories("${ULTRALIGHT_INCLUDE_DIRS}")

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()
set_target_properties(Renderer PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")


set(ULTRALIGHT_INCLUDE_DIRS "$${PROJECT_SOURCE_DIR}/libraries/ultralight/include")
set(FRAMEWORK_SOURCE_DIR "${PROJECT_SOURCE_DIR}/libraries/ultralight/deps/Framework")
set(FRAMEWORK_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/libraries/ultralight/deps")
		 
set(FRAMEWORK_SRC "${FRAMEWORK_SOURCE_DIR}/Application.h"
                  "${FRAMEWORK_SOURCE_DIR}/GPUContext.h"
                  "${FRAMEWORK_SOURCE_DIR}/JSHelpers.h"
                  "${FRAMEWORK_SOURCE_DIR}/JSHelpers.cpp"
                  "${FRAMEWORK_SOURCE_DIR}/Overlay.h"
                  "${FRAMEWORK_SOURCE_DIR}/Overlay.cpp"
                  "${FRAMEWORK_SOURCE_DIR}/Platform.h"
                  "${FRAMEWORK_SOURCE_DIR}/Window.h"
                  "${FRAMEWORK_SOURCE_DIR}/platform/glfw/main.cpp"
                  "${FRAMEWORK_SOURCE_DIR}/platform/glfw/PlatformGLFW.h"
                  "${FRAMEWORK_SOURCE_DIR}/platform/glfw/WindowGLFW.cpp"
                  "${FRAMEWORK_SOURCE_DIR}/platform/glfw/gl/GPUContextGL.cpp"
                  "${FRAMEWORK_SOURCE_DIR}/platform/glfw/gl/GPUDriverGL.h"
                  "${FRAMEWORK_SOURCE_DIR}/platform/glfw/gl/GPUDriverGL.cpp")
                  
set(FRAMEWORK_SHADER_SRC "${FRAMEWORK_SOURCE_DIR}/shaders/glsl/ps/fill.frag"
                         "${FRAMEWORK_SOURCE_DIR}/shaders/glsl/ps/fill_path.frag"
                         "${FRAMEWORK_SOURCE_DIR}/shaders/glsl/vs/v2f_c4f_t2f.vert"
                         "${FRAMEWORK_SOURCE_DIR}/shaders/glsl/vs/v2f_c4f_t2f_t2f_d28f.vert")
				  
if (WIN32)
    list(APPEND FRAMEWORK_SRC "${FRAMEWORK_SOURCE_DIR}/platform/common/FileSystemBasic.h"
                              "${FRAMEWORK_SOURCE_DIR}/platform/common/FileSystemBasic.cpp"
                              "${FRAMEWORK_SOURCE_DIR}/platform/win/FontLoaderWin.h"
                              "${FRAMEWORK_SOURCE_DIR}/platform/win/FontLoaderWin.cpp")
                              
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/lib/win/x64")
        set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/bin/win/x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/lib/win/x86")
        set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/bin/win/x86")
    endif()
    
elseif (APPLE)
    list(APPEND FRAMEWORK_SRC "${FRAMEWORK_SOURCE_DIR}/platform/mac/FileSystemMac.h"
                              "${FRAMEWORK_SOURCE_DIR}/platform/mac/FileSystemMac.mm"
                              "${FRAMEWORK_SOURCE_DIR}/platform/mac/FontLoaderMac.h"
                              "${FRAMEWORK_SOURCE_DIR}/platform/mac/FontLoaderMac.mm")
                              
    set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/bin/mac")
    set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/bin/mac")
    
elseif (UNIX)
    list(APPEND FRAMEWORK_SRC "${FRAMEWORK_SOURCE_DIR}/platform/common/FileSystemBasic.h"
                              "${FRAMEWORK_SOURCE_DIR}/platform/common/FileSystemBasic.cpp"
                              "${FRAMEWORK_SOURCE_DIR}/platform/common/FontLoaderRoboto.h"
                              "${FRAMEWORK_SOURCE_DIR}/platform/common/FontLoaderRoboto.cpp")
                              
    set(ULTRALIGHT_LIBRARY_DIRS "${PROJECT_SOURCE_DIR}/bin/linux")
    set(ULTRALIGHT_BINARY_DIRS "${PROJECT_SOURCE_DIR}/bin/linux")
endif ()

source_group(Framework FILES ${FRAMEWORK_SRC})
source_group(Shaders FILES ${FRAMEWORK_SHADER_SRC})

target_link_libraries(Renderer "ultralight" "${CMAKE_DL_LIBS}")
install(TARGETS Renderer RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
