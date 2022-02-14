cmake_minimum_required(VERSION 3.11)
project(glmaze VERSION "1.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(WIN32)
	set(SDL2_PATH ${SDL2_PATH} "${CMAKE_SOURCE_DIR}/ext/SDL2")
	set(SDL2_IMAGE_PATH ${SDL2_IMAGE_PATH} "${CMAKE_SOURCE_DIR}/ext/SDL2_image")
endif()

if(WIN32 OR APPLE)
        set(GLM_ROOT_DIR ${GLM_ROOT_DIR} "${CMAKE_SOURCE_DIR}/ext/glm")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${GLM_INCLUDE_DIR})

add_executable(glmaze WIN32 MACOSX_BUNDLE
	src/glad.cpp 
	src/ShaderManager.cpp 
	src/MazeGenerator.cpp 
	src/MazeGeneratorDFS.cpp 
	src/MazeGeneratorRD.cpp 
	src/glmaze.cpp)

target_include_directories(glmaze PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_link_libraries(glmaze ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${OPENGL_LIBRARIES})

if(APPLE)
	file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}/glmaze.app/Contents/Resources")
	file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_BINARY_DIR}/glmaze.app/Contents/Resources")

	set(MACOSX_BUNDLE_BUNDLE_NAME glmaze)
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.github.DragonSWDev.glmaze")
	set(MACOSX_BUNDLE_LONG_VERSION_STRING ${CMAKE_PROJECT_VERSION})
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${CMAKE_PROJECT_VERSION})
	set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2022, DragonSWDev")
endif()
