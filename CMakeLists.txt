cmake_minimum_required(VERSION 3.27)
project(TerrainTileBuilder)

set(CMAKE_CXX_STANDARD 20)

# Variable
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/thirdParty")

# Configure assets header file
configure_file(${INCLUDE_DIR}/helpers/RootDir.h.in
        ${CMAKE_BINARY_DIR}/RootDir.h)

# quikGrid
set(QUIKGRID_DIR "${THIRD_PARTY_DIR}/quikgrid")
file(GLOB QUIKGRID_SRC_FILES "${QUIKGRID_DIR}/src/*.cpp")
add_library(quikgrid ${QUIKGRID_SRC_FILES})
target_include_directories(quikgrid
    PUBLIC
        "${QUIKGRID_DIR}/include"
    PRIVATE
        "${QUIKGRID_DIR}/src"
)
list(APPEND LIBS quikgrid)

# Executable
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "${CMAKE_SOURCE_DIR}/include"
        ${CMAKE_BINARY_DIR}
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${LIBS}
)
