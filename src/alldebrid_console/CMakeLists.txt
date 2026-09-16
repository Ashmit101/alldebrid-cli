add_executable(adcli main.cpp)

target_link_libraries(adcli PRIVATE alldebrid_static Boost::program_options)
target_include_directories(adcli PRIVATE ../alldebrid/include)

install(TARGETS adcli)

