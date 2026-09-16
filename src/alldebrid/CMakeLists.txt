add_library(alldebrid_obj OBJECT alldebrid.cpp)

target_link_libraries(alldebrid_obj PUBLIC cpr::cpr nlohmann_json::nlohmann_json)

target_include_directories(alldebrid_obj PUBLIC
				 "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
				 "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
set_target_properties(alldebrid_obj PROPERTIES
				    PUBLIC_HEADER src/alldebrid/include/alldebrid/alldebrid.h
				    POSITION_INDEPENDENT_CODE 1
				    )

add_library(alldebrid_shared SHARED)
set_target_properties(alldebrid_shared PROPERTIES
  MACOSX_RPATH ON
  INSTALL_RPATH "/usr/local/lib"
)

target_link_libraries(alldebrid_shared PUBLIC alldebrid_obj)

add_library(alldebrid_static STATIC)
target_link_libraries(alldebrid_static PUBLIC alldebrid_obj)

