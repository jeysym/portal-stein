#include "gtest\gtest.h"
#include "Common.hpp"
#include "..\Portal-stein\Math.hpp"

using namespace ps;

TEST(MathTest, RoatateTest) {
	sf::Vector2f x{ 1.0f, 0.0f };
	sf::Vector2f expected0{ -1.0f, 0.0f };
	sf::Vector2f expected1{ 0.0f, -1.0f };
	
	rotate(x, PI<float>);
	EXPECT_VEC2NEAR(expected0, x, 0.01);

	rotate(x, PI<float> / 2.0f);
	EXPECT_VEC2NEAR(expected1, x, 0.01);
}

TEST(MathTest, AngleBetweenTest) {
	sf::Vector2f x{ 2.0f, 3.0f };
	sf::Vector2f y{ -3.0f, -2.0f };

	rotate(x, angleBetween(x, y));

	EXPECT_VEC2NEAR(y, x, 0.01);
}

TEST(MathTest, DotTest) {
	sf::Vector2f zero{ 0.0f, 0.0f };
	sf::Vector2f a{ 1.0f, 2.0f };
	sf::Vector2f b{ 3.0f, 2.5f };

	EXPECT_EQ( 0.0f, dot( zero, zero ) );
	EXPECT_EQ( 0.0f, dot( zero, a ) );
	EXPECT_EQ( 0.0f, dot( b, zero ) );

	EXPECT_EQ( 8.0f, dot( a, b ) ) << "Unexpected result of dot product!";
	EXPECT_EQ( 8.0f, dot( b, a ) ) << "Unexpected result of dot product!";
}

TEST(MathTest, NormTest) {
	sf::Vector2f a{ 1.0f, 0.0f };
	sf::Vector2f b{ 3.0f, 4.0f };
	sf::Vector2f c{ -4.0f, 3.0f };

	EXPECT_EQ( 1.0f, norm(a) ) << "Unexpected norm result!";
	EXPECT_EQ( 5.0f, norm(b) ) << "Unexpected norm result!";
	EXPECT_EQ( 5.0f, norm(c) ) << "Unexpected norm result!";
}

TEST(MathTest, NormalizedTest) {
	sf::Vector2f a{ 1.0f, 1.0f };
	sf::Vector2f b{ -13.0f, 0.0f };

	sf::Vector2f na = (1 / sqrtf(2)) * a;
	sf::Vector2f nb{ -1.0f, 0.0f };

	EXPECT_EQ( na, normalized(a) );
	EXPECT_EQ( nb, normalized(b) );
}

TEST(MathTest, NormalizeTest) {
	sf::Vector2f a{ 14.0f, 0.0f };
	sf::Vector2f na{ 1.0f, 0.0f };
	normalize(a);
	 
	EXPECT_EQ(na, a) << "Vector was not normalized!";
}

TEST(MathTest, CrossProduct2DTest) {
	sf::Vector2f zero{ 0.0f, 0.0f };
	sf::Vector2f a{ 1.0f, 2.0f };
	sf::Vector2f b{ -2.0f, 1.0f };

	EXPECT_EQ(0.0f, cross(zero, zero));
	EXPECT_EQ(0.0f, cross(zero, a));
	EXPECT_EQ(0.0f, cross(a, zero));
	EXPECT_EQ(0.0f, cross(a, a));
	EXPECT_EQ(5.0f, cross(a, b));
	EXPECT_EQ(-5.0f, cross(b, a));
}

TEST(MathTest, CrossProduct3DTest) {
	sf::Vector3f zero{ 0.0f, 0.0f, 0.0f };
	sf::Vector3f a{ 0.0f, 1.0f, 0.0f };
	sf::Vector3f b{ 0.0f, 0.0f, 2.0f };
	sf::Vector3f crossAB{ 2.0f, 0.0f, 0.0f };

	EXPECT_EQ(zero, cross(zero, zero));
	EXPECT_EQ(zero, cross(zero, a));
	EXPECT_EQ(zero, cross(a, zero));
	EXPECT_EQ(crossAB, cross(a, b));
	EXPECT_EQ(-crossAB, cross(b, a));
}

TEST(MathTest, GetMaxTest) {
	EXPECT_EQ(6, getMax(6, -109));
	EXPECT_EQ(-56, getMax(-456, -56));
	EXPECT_EQ(567, getMax(567, 0));
}

TEST(MathTest, GetMinTest) {
	EXPECT_EQ(4, getMin(4, 5));
	EXPECT_EQ(-56, getMin(-56, 552));
	EXPECT_EQ(0, getMin(654, 0));
}

