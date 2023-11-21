
#add_library(gui-shared OBJECT)

add_xib(asdf.xib)

# if(APPLE)
#   set(NIBS Stuff)
#   find_program(IBTOOL ibtool REQUIRED)

#   foreach(FILENAME ${NIBS})
#     add_custom_command(OUTPUT ${FILENAME}.nib
#         COMMAND ${IBTOOL} --compile "${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}.nib" "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}.xib"
#         DEPENDS ${FILENAME}.xib
#         VERBATIM
#     )
#     #target_sources(gui-shared PUBLIC ${FILENAME}.nib)
#     list(APPEND GENERATED_NIBS "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}.nib")
#   endforeach()
# endif()

# list(APPEND faeTestList "in gui dir")

# set(GUI_RESOURCES ${GENERATED_NIBS} PARENT_SCOPE)


# add_custom_command(OUTPUT Stuff.nib
#     COMMAND ${IBTOOL} --compile Stuff.nib ${CMAKE_CURRENT_SOURCE_DIR}/Stuff.xib
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Stuff.xib
#     VERBATIM
# )


#add_library(gui-shared OBJECT)

#target_sources(gui-shared
#  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Stuff.nib)

