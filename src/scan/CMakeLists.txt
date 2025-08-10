set(PLUGIN_NAME "scan")

add_library(${PLUGIN_NAME} STATIC)
target_sources(${PLUGIN_NAME}
    PUBLIC
        desc.cc
        include/scan.hh
    PRIVATE
        plugin.cc
        plugin.hh
        process.cc
)

if(APPLE)
    target_sources(${PLUGIN_NAME} PRIVATE
        gui/gui_darwin.mm
        gui/gui_darwin.h)
    add_xib(gui/${PLUGIN_NAME}.xib)
endif()

target_include_directories(${PLUGIN_NAME} PUBLIC include)
target_include_directories(${PLUGIN_NAME} PRIVATE .)

target_link_libraries(${PLUGIN_NAME}
    PRIVATE
        clap
        reaper-sdk
    PUBLIC
        erachnid-common)
