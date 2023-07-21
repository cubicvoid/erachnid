add_library(chomp OBJECT chomp.cc)
target_include_directories(chomp PUBLIC include)
target_link_libraries(chomp PRIVATE clap)
