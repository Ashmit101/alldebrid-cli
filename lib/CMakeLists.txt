add_library(alldebrid alldebrid.cpp)

# Expose the include directory to anyone linking this library
target_include_directories(alldebrid PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link dependencies privately so they don't leak unnecessarily
target_link_libraries(alldebrid PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
