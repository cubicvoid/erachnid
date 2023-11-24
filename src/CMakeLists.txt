if(APPLE)
  include("${CMAKE_SOURCE_DIR}/cmake/xib_helpers.cmake")
endif()

add_library(erachnid MODULE
  clap-entry.cc
  reaper.c
)

add_subdirectory(common)
add_subdirectory(gui)
add_subdirectory(chomp)
add_subdirectory(starry)
add_subdirectory(gryphus)

target_link_libraries(erachnid PRIVATE
  #gui-shared
  common
  reaper-sdk
  clap
  chomp
	starry
  gryphus
¥)


if(APPLE)
    # strip out all symbols except the clap entry point
    target_link_options(erachnid PRIVATE -exported_symbols_list ${CMAKE_SOURCE_DIR}/cmake/macos-symbols.txt)

    # collect all xibs that appeared across the project and
    # attach them to the target
    finalize_xibs(XIBS)
    target_sources(erachnid PRIVATE "${XIBS}")
    set_target_properties(erachnid PROPERTIES RESOURCE "${XIBS}")

    set_target_properties(${PROJECT_NAME} PROPERTIES
      BUNDLE True
      BUNDLE_EXTENSION clap
      MACOSX_BUNDLE_GUI_IDENTIFIER "me.faec.erachnid"
      MACOSX_BUNDLE_BUNDLE_NAME "erachnid"
      MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
      MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/erachnid.plist.in
    )
    target_link_libraries(erachnid PRIVATE "-framework CoreFoundation" "-framework AppKit" "-framework CoreGraphics" "-framework Cocoa")

    target_compile_definitions(erachnid PRIVATE IS_MAC=1)

    if (${COPY_AFTER_BUILD})
      message(STATUS "Will copy plugin after every build" )
      set(build_file, $<TARGET_FILE:mylibrary>)
      #set(products_folder "${CMAKE_BINARY_DIR}/src")
      #COMMAND ${CMAKE_COMMAND} -E echo "Installing ${products_folder}/${PROJECT_NAME}.clap to ~/Library/Audio/Plug-Ins/CLAP/"
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND rm "-f" "~/Library/Audio/Plug-Ins/CLAP/erachnid.clap"
        COMMAND ln -s "${build_file}" "~/Library/Audio/Plug-Ins/CLAP/erachnid.clap")
    endif()
elseif(UNIX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE IS_LINUX=1)
    target_sources(${PROJECT_NAME} PRIVATE)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".clap" PREFIX "")
    if (${COPY_AFTER_BUILD})
        message(STATUS "Will copy plugin after every build" )
        set(products_folder ${CMAKE_BINARY_DIR})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Installing ${products_folder}/${PROJECT_NAME}.clap to ~/.clap"
                COMMAND ${CMAKE_COMMAND} -E make_directory "~/.clap"
                COMMAND ${CMAKE_COMMAND} -E copy "${products_folder}/${PROJECT_NAME}.clap" "~/.clap"
                )
    endif()

else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE IS_WIN=1)
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".clap" PREFIX "")
endif()