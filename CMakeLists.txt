cmake_minimum_required(VERSION 3.22)
project(SaunaClicker)

set(CMAKE_CXX_STANDARD 20)

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
add_executable(${PROJECT_NAME} sauna_clicker/main.cpp
        sauna_clicker/sauna.h
        sauna_clicker/saunaclicker.cpp)


# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
set(SDL2_PATH "C:/Users/Kaspar/CLionProjects/SaunaClicker/SDL2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})

# --- SDL_ttf SETUP ---
set(SDL_TTF_PATH "C:/Users/Kaspar/CLionProjects/SaunaClicker/SDL2TTF")
include_directories(${SDL_TTF_PATH}/include)  # Include SDL_ttf header files
target_link_libraries(${PROJECT_NAME} ${SDL_TTF_PATH}/lib/SDL2_ttf.lib)  # Link with SDL_ttf library