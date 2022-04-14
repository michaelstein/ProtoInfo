cmake_minimum_required(VERSION 3.22)
project(ProtoInfo)

# basic cmake settings
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# compiler settings
if(WIN32)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W4")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Conan build files
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")

# global include settings
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CONAN_INCLUDE_DIRS_MS-GSL})

# Git version information
include(git-version.cmake)

#
# DEPENDENCIES
#

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

#
# SOURCES
#

file(GLOB_RECURSE headers src/*.h)
file(GLOB_RECURSE sources src/*.cpp)
file(GLOB_RECURSE qtforms res/forms/*.ui)
set(qtres res/app.qrc)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	PREFIX src
	FILES ${headers} ${sources}
)

QT6_WRAP_UI(qtforms_generated ${qtforms})

#
# BINARIES
#

set(GUI_TYPE "")
if (WIN32)
	set(GUI_TYPE WIN32)
endif()

add_executable(${PROJECT_NAME}
	${GUI_TYPE}
	${headers}
	${sources}
	${qtforms}
	${qtforms_generated}
	${qtres}
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE CONAN_PKG::humblelogging
	PRIVATE CONAN_PKG::protobuf
	PUBLIC Qt6::Widgets
)

if(WIN32)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE Ws2_32.lib
	)
endif(WIN32)

if(UNIX)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE pthread
	)
endif(UNIX)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/res/qt.conf" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
