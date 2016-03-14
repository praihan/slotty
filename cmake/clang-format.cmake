find_program (	CLANG_FORMAT clang-format
				PATHS "/usr/bin")

file (GLOB_RECURSE ALL_SOURCE_FILES tests/*.cc src/*.cc include/*.h)

add_custom_target (	clang-format
					COMMAND ${CLANG_FORMAT}
					-style=file
					-i
					${ALL_SOURCE_FILES})
