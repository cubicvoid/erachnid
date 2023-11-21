
find_program(IBTOOL ibtool REQUIRED)

# this isn't pretty but it's the only way to avoid including all
# xibs for all modules explicitly in the main target cmake file
function(add_xib)
  # get_property(is_defined GLOBAL PROPERTY XIB_LIST DEFINED)
  # if(NOT is_defined)
  #   define_property(GLOBAL PROPERTY XIB_LIST
  #     BRIEF_DOCS "List of .xib file resources")
  # endif()
  # make absolute paths
  set(XIBS)
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(s "${s}" ABSOLUTE)
    endif()
    message("adding xib ${s}")
    list(APPEND XIBS "${s}")
  endforeach()
  # append to global list
  set_property(GLOBAL APPEND PROPERTY XIB_LIST "${XIBS}")
endfunction()

# call this in the same file that applies the RESOURCE tag, and after
# any subdirectories/subtargets that might have xib files
function(finalize_xibs output)
  get_property(xibs GLOBAL PROPERTY XIB_LIST)
  # Xcode handles xib -> nib compilation, for other generators we must
  # do it ourself
  if("${CMAKE_GENERATOR}" STREQUAL "Xcode")
    set("${output}" "${xibs}" PARENT_SCOPE)
  else()
    foreach(fullpath ${xibs})
      get_filename_component(name "${fullpath}" NAME_WE)
      message("processing name ${name} path ${fullpath}")
      add_custom_command(OUTPUT "${name}.nib"
        COMMAND ${IBTOOL} --compile "${name}.nib" "${fullpath}"
        DEPENDS "${fullpath}"
        VERBATIM
      )
      list(APPEND nibs "${name}.nib")
    endforeach()
    message("setting final nibs ${nibs}")
    set("${output}" "${nibs}" PARENT_SCOPE)
  endif()
endfunction()

function(testset output)
  set("${output}" "before if" PARENT_SCOPE)
  if("${output}" STREQUAL "somevar")
    set("${output}" "in if" PARENT_SCOPE)
  else()
    set("${output}" "in else" PARENT_SCOPE)
  endif()
endfunction()