cmake_minimum_required(VERSION 3.22)

add_subdirectory(dependencies)

add_subdirectory(GameEngine)
message(STATUS "STATUS - Lib: Built as '${GameEngine_LIB} with includes at '${GameEnding_INCLUDE}'")

set(
    GLOBAL_LIB 
    ${GameEngine_LIB}
)

set(
    GLOBAL_INC
    ${GameEngine_INCLUDE}
)

add_subdirectory(Game)

message(STATUS "STATUS - Exec: Built as '${Game_BIN}'")
