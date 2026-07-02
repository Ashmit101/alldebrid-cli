add_executable(adcli main.cpp)
target_link_libraries(adcli PRIVATE alldebrid Boost::program_options)
install(TARGETS adcli)
