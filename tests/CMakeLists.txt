enable_testing ()
include_directories (${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable (runUnitTests
	Simple.cc
)

target_link_libraries(runUnitTests gtest gtest_main)
target_link_libraries(runUnitTests slotty)