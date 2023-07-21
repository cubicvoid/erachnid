add_library(chomp STATIC chomp.hh chomp.cc)
target_link_libraries(chomp PRIVATE clap)
