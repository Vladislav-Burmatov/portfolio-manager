set(TEST_SOURCES
	BoundedIntTest.cpp
) 

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests
	PRIVATE
	finance_core
	GTest::gtest
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)