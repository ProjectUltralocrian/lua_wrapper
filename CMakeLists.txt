cmake_minimum_required(VERSION 3.16)

project(LuaWrapper)
set(CMAKE_CXX_STANDARD 20)

add_executable(sandbox ${CMAKE_CURRENT_SOURCE_DIR}/src/sandbox.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/lua_wrapper.cpp)

#set_target_properties(TARGET sandbox PROPERTY CXX_STANDARD 20)


#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/lua-5.4.6/src)

target_link_directories(sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lua-5.4.6/src)
target_link_libraries(sandbox lua)