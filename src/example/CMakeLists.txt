add_library(example OBJECT example.c)
target_include_directories(example PUBLIC include)
target_link_libraries(example PRIVATE clap)
