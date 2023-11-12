add_library(chomp OBJECT
    chomp.cc
    params.cc
    process.cc
    ports.cc
    bridge.cc
    gui/gui.mm
    gui/gui_darwin.mm)
set_property (TARGET chomp APPEND_STRING PROPERTY 
    COMPILE_FLAGS "-fno-objc-arc")
target_include_directories(chomp PUBLIC include)
target_include_directories(chomp PRIVATE .)
target_link_libraries(chomp PRIVATE clap)
