add_library(weave STATIC)
target_sources(weave
    PUBLIC
        weave.cc
        include/weave.hh
    PRIVATE
        weave_plugin.cc
        weave_plugin.hh
)

#if(APPLE)
#    target_sources(weave PRIVATE
#        gui/weave_gui_darwin.mm
#        gui/weave_gui_darwin.h)
#    add_xib(gui/weave.xib)
#endif()

target_include_directories(weave PUBLIC include)
target_include_directories(weave PRIVATE .)

target_link_libraries(weave
    PRIVATE
        clap
        reaper-sdk
    PUBLIC
        erachnid-common)
