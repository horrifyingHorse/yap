cmake_minimum_required(VERSION 3.25)

project(
  yap
  LANGUAGES CXX
  VERSION 0.6.9)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME})
