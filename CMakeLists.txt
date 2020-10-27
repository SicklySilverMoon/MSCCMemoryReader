cmake_minimum_required(VERSION 3.17)
project(CCMemoryReader)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(PNG REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${PNG_INCLUDE_DIR})

add_executable(CCMemoryReader main.cpp MemoryReader.cpp MemoryReader.h GameState.cpp GameState.h Renderer.cpp Renderer.h)
target_link_libraries(CCMemoryReader ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} -mconsole -mwin32 -mwindows)
#target_link_libraries(CCMemoryReader PRIVATE )
