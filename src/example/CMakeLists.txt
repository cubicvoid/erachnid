add_library(example STATIC example.h example.c)
target_link_libraries(example PRIVATE clap)
