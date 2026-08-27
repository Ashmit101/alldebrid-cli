cmake_minimum_required(VERSION 3.30.0)
project(AllDebridCLI VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CPR_USE_SYSTEM_LIB_PSL ON CACHE BOOL "" FORCE)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.14.2)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpr)

find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
  FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(nlohmann_json)
endif()

find_package(Boost CONFIG COMPONENTS program_options)
if(NOT Boost_FOUND)
  FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost
    GIT_TAG boost-1.92.0
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(Boost)
endif()

add_subdirectory(src bin)
