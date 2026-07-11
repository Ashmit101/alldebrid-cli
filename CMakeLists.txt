cmake_minimum_required(VERSION 3.30.0)
project(AllDebridCLI VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(CPR_USE_SYSTEM_LIB_PSL ON CACHE BOOL "" FORCE)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.14.2)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpr)

find_package(nlohmann_json  REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS program_options)

add_subdirectory(src bin)
