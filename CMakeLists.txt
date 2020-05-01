# Only Linux build is supported. C++11 is required to build googletest.
#
# 1. mkdir build; cd build
# 2. cmake ..
# 3. make -j

cmake_minimum_required(VERSION 2.8.2)
project (googletestexample)

set(CMAKE_CXX_STANDARD 11)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
	include_directories("${gtest_SOURCE_DIR}/include")
	include_directories("${gmock_SOURCE_DIR}/include")
endif()

# Add libraries here
add_library(libexample INTERFACE)
target_include_directories(libexample
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

OPTION(WITH_TESTS "build with tests" ON)
if (WITH_TESTS)
	enable_testing()
	# Add a 'make check' command for the regression tests
	add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
	add_subdirectory(test)
endif()
