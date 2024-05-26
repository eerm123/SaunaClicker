cmake_minimum_required(VERSION 3.22)
project(SaunaClicker)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_PATH "/home/eero/Documents/SaunaClicker/SDL2/x86_64-w64-mingw32")
set(SDL_TTF_PATH "/home/eero/Documents/SaunaClicker/SDL2TTF")
set(SDL_IMAGE_PATH "/home/eero/Documents/SaunaClicker/SDL2_image")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)

add_executable(${PROJECT_NAME} 
        sauna_clicker/main.cpp
        sauna_clicker/sauna.h
        sauna_clicker/saunaclicker.cpp
)

# --- SDL2 SETUP ---
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})


# --- SDL_ttf SETUP ---
include_directories(${SDL_TTF_PATH}/include)
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf PATHS ${SDL_TTF_PATH}/lib) 
target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARY}) 


# --- SDL_image SETUP ---
include_directories(${SDL_IMAGE_PATH}/include)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARY})
