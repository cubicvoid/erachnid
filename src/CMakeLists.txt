set(COCOA_UI_XIBS Stuff)

add_library(${PROJECT_NAME} MODULE
        clap-entry.cc
        Stuff.xib
        )

find_program(IBTOOL ibtool REQUIRED)
foreach(XIBFILE ${COCOA_UI_XIBS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${IBTOOL} --compile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.clap/Contents/Resources/${XIBFILE}.nib ${CMAKE_CURRENT_SOURCE_DIR}/${XIBFILE}.xib
            COMMENT "Compiling NIB file ${XIBFILE}.nib")
endforeach()

#set(RESOURCES Stuff.nib)


add_subdirectory(chomp)
add_subdirectory(starry)
add_subdirectory(example)

target_link_libraries(${PROJECT_NAME} PRIVATE
    clap
    chomp
	starry
	example
)


if(APPLE)
    target_link_options(${PROJECT_NAME} PRIVATE -exported_symbols_list ${CMAKE_CURRENT_SOURCE_DIR}/macos-symbols.txt)

    set_target_properties(${PROJECT_NAME} PROPERTIES
            BUNDLE True
            BUNDLE_EXTENSION clap
            MACOSX_BUNDLE_GUI_IDENTIFIER org.surge-synth-team.${PROJECT_NAME}
            MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
            MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}.plist.in
            RESOURCE "${RESOURCES}"
            )
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreFoundation" "-framework AppKit" "-framework CoreGraphics" "-framework Cocoa")

    target_compile_definitions(${PROJECT_NAME} PRIVATE IS_MAC=1)
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror)

    if (${COPY_AFTER_BUILD})
        message(STATUS "Will copy plugin after every build" )
        set(products_folder "${CMAKE_BINARY_DIR}/src")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Installing ${products_folder}/${PROJECT_NAME}.clap to ~/Library/Audio/Plug-Ins/CLAP/"
                COMMAND ${CMAKE_COMMAND} -E make_directory "~/Library/Audio/Plug-Ins/CLAP"
                COMMAND ${CMAKE_COMMAND} -E rm -rf "~/Library/Audio/Plug-Ins/CLAP/${PROJECT_NAME}.clap"
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${products_folder}/${PROJECT_NAME}.clap" "~/Library/Audio/Plug-Ins/CLAP/${PROJECT_NAME}.clap"
                )
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