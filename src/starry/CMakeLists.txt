add_library(starry STATIC)
target_sources(starry
    PUBLIC
        starry.cc
        include/starry.hh
    PRIVATE
        starry_plugin.cc
        starry_plugin.hh
)

#if(APPLE)
#    target_sources(starry PRIVATE
#        gui/starry_gui_darwin.mm
#        gui/starry_gui_darwin.h)
#    add_xib(gui/starry.xib)
#endif()

target_include_directories(starry PUBLIC include)
target_include_directories(starry PRIVATE .)

target_link_libraries(starry
    PRIVATE
        clap
        reaper-sdk
    PUBLIC
        erachnid-common)
