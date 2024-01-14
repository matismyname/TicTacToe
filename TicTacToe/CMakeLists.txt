# Set CMake version
cmake_minimum_required(VERSION 3.10)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TicTacToe)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Find SDL_image
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# Create a list of header files
file(GLOB_RECURSE HEADER_FILES src/headers/*.h)

# Create a list of source files
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*/*.cpp)

# Create executable and link against SDL2 and SDL_image
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# Include directories for the target
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/headers/render
    ${CMAKE_CURRENT_SOURCE_DIR}/src/headers/menu
    ${CMAKE_CURRENT_SOURCE_DIR}/src/headers/log
    ${CMAKE_CURRENT_SOURCE_DIR}/src/headers/helperfuncs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/headers/game
    ${CMAKE_CURRENT_SOURCE_DIR}/src/headers/config
)

# Link against SDL2 and SDL_image libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    -lSDL2_image  # Add explicit linking for SDL_image
)

# Fixing the order of linking (SDL_image after SDL2)
set_property(TARGET ${PROJECT_NAME} PROPERTY
    COMPILE_OPTIONS -D_REENTRANT
)

