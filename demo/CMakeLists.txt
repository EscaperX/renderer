file(GLOB_RECURSE raster_sources
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.c")

add_executable(raster ${raster_sources})
target_link_libraries(raster PUBLIC core)
target_include_directories(raster PUBLIC ${CMAKE_CURRENT_LIST_DIR})