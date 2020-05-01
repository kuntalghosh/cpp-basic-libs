#pragma once

/* This is a simple library interface. */

namespace PROJECT_NAMESPACE {

template <typename T>
class MyClass {
	T a;

	public:

	void initialize(T a_val) {
		a = a_val;
	}

	T get_initialized_value() {
		return a;
	}
};

}
