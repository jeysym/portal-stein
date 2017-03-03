#pragma once
#ifndef PS_TEST_COMMON_INCLUDED
#define PS_TEST_COMMON_INCLUDED
#include "SFML\Graphics.hpp"

template< typename T >
std::ostream & operator<<(std::ostream & os, sf::Vector2<T> vector) {
	os << "[" << vector.x << ", " << vector.y << "]";
	return os;
}

template< typename T >
std::ostream & operator<<(std::ostream & os, sf::Vector3<T> vector) {
	os << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
	return os;
}

// These macros are used to test sf::Vector2f and sf::Vector3f on equality.
#define EXPECT_VEC2NEAR(expected, actual, delta)	EXPECT_NEAR(expected.x, actual.x, delta); EXPECT_NEAR(expected.y, actual.y, delta)
#define EXPECT_VEC3NEAR(expected, actual, delta)	EXPECT_NEAR(expected.x, actual.x, delta); EXPECT_NEAR(expected.y, actual.y, delta); EXPECT_NEAR(expected.z, actual.z, delta)

#endif // !PS_TEST_COMMON_INCLUDED
