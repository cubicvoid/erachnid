add_library(chomp OBJECT
    chomp.cc
    chomp_impl.hh
    params.cc
    process.cc
    ports.cc
    bridge.cc
    gui/gui.cc)

if(APPLE)
    target_sources(chomp PRIVATE gui/gui_darwin.mm)
    add_xib(gui/Stuff.xib)
endif()


target_include_directories(chomp PUBLIC include)
target_include_directories(chomp PRIVATE .)
target_link_libraries(chomp PRIVATE clap reaper-sdk)
