add_library(regression STATIC
	regression.cpp
)

target_link_libraries(regression gtest)

# Add tests here

SET(TESTS
	libexample_test.cpp
	libalgos/concurrent_queue_test.cpp)

foreach(testfile ${TESTS})
	get_filename_component(test ${testfile} NAME_WE)
	add_executable(${test} ${testfile})
	target_link_libraries(${test} libexample regression gtest)
	add_test(NAME ${test} COMMAND ${test})
endforeach(testfile ${TESTS})
