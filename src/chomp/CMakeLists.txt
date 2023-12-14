add_library(chomp STATIC)
target_sources(chomp
    PUBLIC
        chomp.cc
        include/chomp.hh
    PRIVATE
        chomp_plugin.cc
        chomp_plugin.hh
        process.cc
)

if(APPLE)
    target_sources(chomp PRIVATE
        gui/chomp_gui_darwin.mm
        gui/chomp_gui_darwin.h)
    add_xib(gui/Stuff.xib)
endif()

target_include_directories(chomp PUBLIC include)
target_include_directories(chomp PRIVATE .)

target_link_libraries(chomp
    PRIVATE
        clap
        reaper-sdk
    PUBLIC
        erachnid-common)
