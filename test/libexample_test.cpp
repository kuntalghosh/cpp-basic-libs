#include <libexample/libexample.h>
#include "gtest/gtest.h" 

namespace PROJECT_NAMESPACE {

TEST(MyComponentTest, initializationTest) {
  // Arrange
  MyClass<int> * mc = new MyClass<int>;    

  // Act
  mc->initialize(5);

  // Assert
  const int expected_value = 5;
  EXPECT_EQ(mc->get_initialized_value(), expected_value);

  // Don't forget to release resources
  delete mc;
}

}
