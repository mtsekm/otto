cmake_minimum_required(VERSION 3.10)
project(otto)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(ENABLE_UINPUT "Enable direct uinput support for key events" ON)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)

set(SOURCES
    ${SOURCE_DIR}/AppExecutor.cpp
    ${SOURCE_DIR}/CommandExecutor.cpp
    ${SOURCE_DIR}/CommandHandler.cpp
    ${SOURCE_DIR}/EventManager.cpp
    ${SOURCE_DIR}/KeyManager.cpp
    ${SOURCE_DIR}/KeyMap.cpp
    ${SOURCE_DIR}/KeyPressExecutor.cpp
    ${SOURCE_DIR}/Logger.cpp
    ${SOURCE_DIR}/LoopExecutor.cpp
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/VariableExecutor.cpp
    ${SOURCE_DIR}/WaitExecutor.cpp
)

if (ENABLE_UINPUT)
    add_definitions(-DENABLE_UINPUT)
    include_directories(${INCLUDE_DIR})
else()
    find_package(IARM REQUIRED)
    find_library(UINPUT_LIBRARIES NAMES uinput PATH_SUFFIXES lib usr/lib)
    if (NOT UINPUT_LIBRARIES)
        message(FATAL_ERROR "libuinput not found. Please install it or provide its location.")
    endif()
    list(APPEND SOURCES ${SOURCE_DIR}/IARMUtils.cpp)
    include_directories(${INCLUDE_DIR} ${IARMBUS_INCLUDE_DIRS} ${IRMGR_INCLUDE_DIRS} ${IRMGR_INTERNAL_INCLUDE_DIRS})
endif()

add_executable(otto ${SOURCES})

if (ENABLE_UINPUT)
    target_link_libraries(otto pthread)
else()
    target_link_libraries(otto pthread IARMBus ${IARMBUS_LIBRARIES} ${UINPUT_LIBRARIES})
endif()

target_compile_options(otto PRIVATE -Wall -Wextra -Wpedantic -Werror -ffunction-sections -fdata-sections -Os)
target_link_options(otto PRIVATE -Wl,--gc-sections)

install(TARGETS otto DESTINATION bin)
