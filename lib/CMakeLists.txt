add_subdirectory(clap EXCLUDE_FROM_ALL)

add_library(reaper-sdk INTERFACE)

target_include_directories(reaper-sdk INTERFACE
	WDL/WDL
	reaper-sdk/sdk
)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
set(JSON_ImplicitConversions OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)