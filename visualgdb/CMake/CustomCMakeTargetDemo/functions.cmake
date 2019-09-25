function(register_static_library NAME FILES)
	add_library(Static${NAME} ${FILES})
endfunction(register_static_library)

function(set_static_library_cflags NAME FLAGS)
	target_compile_options(Static${NAME} PRIVATE ${FLAGS})
endfunction(set_static_library_cflags)

function(register_shared_library)
	cmake_parse_arguments(_ "" "NAME" "SOURCES;CFLAGS" ${ARGN})

	add_library(Shared${__NAME} SHARED ${__SOURCES})
	target_compile_options(Shared${__NAME} PRIVATE ${__CFLAGS})
endfunction(register_shared_library)