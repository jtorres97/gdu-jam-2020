find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SQLite3 REQUIRED CONFIG)

set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES "*.cpp")

set(SOURCES ${SOURCES})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

add_executable(${BINARY}_game ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

include_directories(${SDL2_INCLUDE_DIRS} 
                    ${SDL2_IMAGE_INCLUDE_DIR} 
                    ${SDL2TTF_INCLUDE_DIR} 
                    ${SDL2_MIXER_INCLUDE_DIRS})

target_link_libraries(${BINARY}_game 
                      ${SDL2_LIBRARIES}
                      ${SDL2_IMAGE_LIBRARY} 
                      ${SDL2TTF_LIBRARY} 
                      ${SDL2_MIXER_LIBRARIES}
                      SQLite::SQLite3)