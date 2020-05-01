add_library(regression STATIC
	regression.cpp
)

target_link_libraries(regression gtest)

# Add tests here

# 1. Test for libexample
add_executable(libexample_test libexample_test.cpp)
target_link_libraries(libexample_test libexample regression gtest)
add_test(NAME libexample_test COMMAND libexample_test)
