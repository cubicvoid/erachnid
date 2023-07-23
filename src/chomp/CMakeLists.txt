add_library(chomp OBJECT chomp.cc params.cc process.cc ports.cc bridge.cc)
target_include_directories(chomp PUBLIC include)
target_link_libraries(chomp PRIVATE clap)
