include_directories(../fatfs)

add_library(sqlite sqliteReader.cpp cAPI.cpp)
target_link_libraries(sqlite PUBLIC fatfs)
