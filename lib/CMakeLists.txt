add_subdirectory(clap EXCLUDE_FROM_ALL)

add_library(reaper-sdk INTERFACE)

target_include_directories(reaper-sdk INTERFACE
	WDL/WDL
	reaper-sdk/sdk
)