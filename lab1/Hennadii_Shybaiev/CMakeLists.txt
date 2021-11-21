cmake_minimum_required(VERSION 3.18.4)

project(cryptomech)

get_filename_component(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH)

set(CMECH ${ROOT_DIR}/Hennadii_Shybaiev)
message(${CMECH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -fconcepts")

install (DIRECTORY "bin" DESTINATION ${CMECH} )

include_directories(${CMECH}/include)

include(${CMECH}/etc/cmake/spdlog.cmake)

set(main "${PROJECT_SOURCE_DIR}/src/main.cpp")
set(EXECUTABLE_OUTPUT_PATH ${CMECH}/bin)

add_executable( ${PROJECT_NAME} ${main} )
target_link_libraries(${PROJECT_NAME}
                     pthread
                     spdlog
                     gmp
                     gmpxx )