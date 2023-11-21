add_library(chomp OBJECT
    chomp.cc
    params.cc
    process.cc
    ports.cc
    bridge.cc
    gui/gui.mm
    gui/gui_darwin.mm)

add_xib(gui/Stuff.xib)

target_include_directories(chomp PUBLIC include)
target_include_directories(chomp PRIVATE .)
target_link_libraries(chomp PRIVATE clap)
