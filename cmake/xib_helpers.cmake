
function(add_xib)
  get_property(is_defined GLOBAL PROPERTY XIB_LIST DEFINED)
  if(NOT is_defined)
    define_property(GLOBAL PROPERTY XIB_LIST
      BRIEF_DOCS "List of .xib file resources")
  endif()
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

macro(finalize_xibs target output)
get_property(${output} GLOBAL PROPERTY XIB_LIST)
#message("adding sources ${NIBS}")
target_sources(${target} PRIVATE ${${output}})
endmacro()

