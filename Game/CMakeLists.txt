cmake_minimum_required(VERSION 3.22)

set(PROJ_TYPE EXECUTABLE) # EXECUTABLE or LIBRARY
set(PROJ_NAME "Game")
set(PROJ_LIBS ${GLOBAL_LIB})
set(SRC_DIR "src")
set(INC_DIR "include" ${GLOBAL_INC})

set(
    SRC_FILES

    "main.cpp"
)

project(
    ${PROJ_NAME}
    VERSION 0.1
    DESCRIPTION "A project"
    LANGUAGES C CXX 
)

###################

list(TRANSFORM SRC_FILES PREPEND "${SRC_DIR}/")

###################

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if(UNIX)
    message(STATUS "STATUS - ${PROJ_NAME}: Building for UNIX.")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a")

    # Override to newer compiler usually needed on ubuntu cause shits old
    set(CMAKE_C_COMPILER gcc-13)
    set(CMAKE_CXX_COMPILER g++-13)

    add_compile_definitions(__unix__)
elseif(WIN32)
    message(STATUS "STATUS - ${PROJ_NAME}: Building for Windows.")

    add_compile_definitions(__win32__)
elseif(APPLE)
    message(STATUS "STATUS - ${PROJ_NAME}: Building for Apple.")

    add_compile_definitions(__apple__)
endif()

if(${PROJ_TYPE} MATCHES "EXECUTABLE")
    message(STATUS "STATUS - ${PROJ_NAME}: Is built as EXECUTABLE.")

    add_executable(${PROJ_NAME} ${SRC_FILES})

    set(${PROJ_NAME}_BIN ${PROJ_NAME} PARENT_SCOPE) # Expose binary executable
elseif(${PROJ_TYPE} MATCHES "LIBRARY")
    message(STATUS "STATUS - ${PROJ_NAME}: Is built as LIBRARY.")

    add_library(${PROJ_NAME} ${PROJ_LIB_TYPE} ${SRC_FILES})

    set(${PROJ_NAME}_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/${INC_DIR} PARENT_SCOPE) # TODO: Add "current dir" 
    set(${PROJ_NAME}_LIB ${PROJ_NAME} PARENT_SCOPE) # Expose library
else()
    message(FATAL_ERROR "ERROR - ${PROJ_NAME}: PROJ_TYPE in CMakeLists.txt wrongly set. Supported types are EXECUTABLE or LIBRARY.")
endif()

if(TARGET SDL2::SDL2main)
    message(STATUS "STATUS - ${PROJ_NAME} => SDL2main found, using.")
    target_link_libraries(${PROJ_NAME} PUBLIC SDL2::SDL2main)
endif()

target_include_directories(${PROJ_NAME} PUBLIC ${INC_DIR})
target_link_libraries(${PROJ_NAME} PUBLIC ${PROJ_LIBS} SDL2::SDL2)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    target_compile_definitions(${PROJ_NAME} PRIVATE _DEBUG)
else()
    target_compile_definitions(${PROJ_NAME} PRIVATE _RELEASE)
endif()
