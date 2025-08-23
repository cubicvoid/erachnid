add_library(erachnid-common OBJECT
	clap_bridge.cc
	clap_bridge.hh
	clap_gui.hh
	clap_plugin.cc
	clap_plugin.hh
	clap_param.cc
	clap_param.hh
)

target_include_directories(erachnid-common PUBLIC .)
target_link_libraries(erachnid-common
	PRIVATE
		clap
		reaper-sdk
	PUBLIC
		nlohmann_json::nlohmann_json)
