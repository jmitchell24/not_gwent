cmake_minimum_required(VERSION 3.15)

project(not_gwent
		DESCRIPTION "Definitely Not Gwent"
        LANGUAGES CXX C
		VERSION 0.2.1
)

set(GAME_EXECUTABLE ${PROJECT_NAME})
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${GAME_EXECUTABLE})

#
# Sources needed to build the game
#
file(GLOB_RECURSE GAME_SOURCES
    "src/*.cpp"
    "src/*.hpp"
    "src/*.c"
    "src/*.h"
)

target_sources(${GAME_EXECUTABLE}
    PRIVATE
        ${GAME_SOURCES}
)

target_include_directories(${GAME_EXECUTABLE}
    PRIVATE
        src
)

include(FetchContent)

#
# raylib
#

# add_compile_definitions(SUPPORT_CUSTOM_FRAME_CONTROL)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 4.5.0)
FetchContent_MakeAvailable(raylib)
target_link_libraries(${GAME_EXECUTABLE} PRIVATE raylib)

#
# ut
#
FetchContent_Declare(
    ut
    GIT_REPOSITORY https://gitlab.com/james-mitchell-dev/ut.git)
FetchContent_MakeAvailable(ut)
target_link_libraries(${GAME_EXECUTABLE} PRIVATE ut)

#
# yaml-cpp
#
option(YAML_CPP_BUILD_CONTRIB   OFF)
option(YAML_CPP_BUILD_TOOLS     OFF)
option(YAML_BUILD_SHARED_LIBS   OFF)
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG yaml-cpp-0.7.0)
FetchContent_MakeAvailable(yaml-cpp)
target_link_libraries(${GAME_EXECUTABLE} PRIVATE yaml-cpp)

#
# sqlite3
#
FetchContent_Declare(
		sqlite3
		GIT_REPOSITORY https://github.com/tamaskenez/sqlite3-cmake.git)
FetchContent_MakeAvailable(sqlite3)
target_link_libraries(${GAME_EXECUTABLE} PRIVATE sqlite3)


#
# lua
#
FetchContent_Declare(
		lua
		GIT_REPOSITORY https://github.com/walterschell/Lua.git
		GIT_TAG v5.4.5)
FetchContent_MakeAvailable(lua)
target_link_libraries(${GAME_EXECUTABLE} PRIVATE lua_static)


#find_package(SQLite3 REQUIRED)
#target_include_directories(${GAME_EXECUTABLE} PRIVATE ${SQLite3_INCLUDE_DIRS})
#target_link_directories(${GAME_EXECUTABLE} PRIVATE ${SQLite3_LIBRARY_DIR})
#target_link_libraries(${GAME_EXECUTABLE} ${SQLite3_LIBRARIES})


#
# The "Headers" source group for IDEs
#
get_target_property(EXECUTABLE_SOURCES ${GAME_EXECUTABLE} SOURCES)
set(EXECUTABLE_HEADERS "${EXECUTABLE_SOURCES}")
list(FILTER EXECUTABLE_HEADERS INCLUDE REGEX .hpp$)
list(JOIN EXECUTABLE_HEADERS " " EXECUTABLE_HEADERS)
source_group("Headers" FILES "${EXECUTABLE_HEADERS}")

#target_link_libraries(${GAME_EXECUTABLE} PRIVATE
#    m
#    ${CMAKE_DL_LIBS}
#)



target_link_libraries(${GAME_EXECUTABLE} PRIVATE
    ${CMAKE_DL_LIBS}
)

