#include "gtest\gtest.h"
#include "Common.hpp"
#include "..\Portal-stein\Geometry.hpp"
#include "..\Portal-stein\Math.hpp"

using namespace ps;

class GeometryTest : public ::testing::Test {
public:
	GeometryTest() :
		lsA(sf::Vector2f{ 0.0f, 1.0f }, sf::Vector2f{ 2.0f, 1.0f }),
		lsB(sf::Vector2f{ 1.0f, 2.0f }, sf::Vector2f{ 1.0f, 0.0f }),
		lsC(sf::Vector2f{ 3.0f, 0.0f }, sf::Vector2f{ 4.0f, 1.0f }),
		ray(sf::Vector3f{ 0.0f, 0.5f, 0.0f }, sf::Vector2f{ 1.0f, 0.0f }, 0),
		ray2(sf::Vector3f{ 0.0f, 0.5f, 0.0f }, sf::Vector2f{ -1.0f, 0.0f }, 0) {
	}

	LineSegment lsA;
	LineSegment lsB;
	LineSegment lsC;

	Ray ray;
	Ray ray2;
};

TEST_F(GeometryTest, LSLSIntersectionTest0) {
	auto intersection = intersect(lsA, lsB);
	ASSERT_TRUE(intersection.theyIntersect) << "These line segments should intersect each other!";
	EXPECT_EQ(0.5f, intersection.firstParameter);
	EXPECT_EQ(0.5f, intersection.secondParameter);
}

TEST_F(GeometryTest, LSLSIntersectionTest1) {
	auto intersection = intersect(lsA, lsC);
	ASSERT_FALSE(intersection.theyIntersect) << "These line segments should not intersect each other!";
}

TEST_F(GeometryTest, LSLSIntersectionTest2) {
	auto intersection = intersect(lsB, lsC);
	ASSERT_FALSE(intersection.theyIntersect) << "These line segments should not intersect each other!";
}

TEST_F(GeometryTest, RayLSIntersection0) {
	auto intersection = intersect(ray, lsB);
	ASSERT_TRUE(intersection.theyIntersect);
	EXPECT_EQ(1.0f, intersection.rayParameter);
	EXPECT_EQ(1.5f / 2.0f, intersection.lineSegmentParameter);
}

TEST_F(GeometryTest, RayLSIntersection1) {
	auto intersection = intersect(ray, lsA);
	ASSERT_FALSE(intersection.theyIntersect);
}

TEST_F(GeometryTest, RayLSIntersection2) {
	auto intersection = intersect(ray, lsC);
	ASSERT_TRUE(intersection.theyIntersect);
	EXPECT_EQ(3.5f, intersection.rayParameter);
	EXPECT_EQ(0.5f, intersection.lineSegmentParameter);
}

TEST_F(GeometryTest, RayLSIntersection3) {
	auto intersection = intersect(ray2, lsB);
	ASSERT_FALSE(intersection.theyIntersect) << "Ray intersects line segment opposite of the ray direction!";
}

TEST_F(GeometryTest, MapLSTest0) {
	ObjectInScene x{ sf::Vector3f{ 0.0f, 1.0f, 0.0f }, sf::Vector2f{ 0.0f, 1.0f }, 0 };
	ObjectInScene y{ sf::Vector3f{ 1.0f, 0.0f, 1.0f }, sf::Vector2f{ 0.0f, -1.0f }, 1 };
	ObjectInScene z{ sf::Vector3f{ 2.0f, 2.0f, 3.0f }, sf::Vector2f{ 1.0f, 0.0f }, 2 };

	LineSegment::mapLineSegments(lsA, lsB, x);
	EXPECT_VEC3NEAR(sf::Vector3f( 1.0f, 2.0f, 0.0f ), x.getPosition(), 0.01);
	EXPECT_VEC2NEAR(sf::Vector2f( 1.0f, 0.0f ), x.getDirection(), 0.01);
	EXPECT_EQ(0, x.getSegmentId());

	LineSegment::mapLineSegments(lsA, lsB, y);
	EXPECT_VEC3NEAR(sf::Vector3f( 0.0f, 1.0f, 1.0f ), y.getPosition(), 0.01);
	EXPECT_VEC2NEAR(sf::Vector2f( -1.0f, 0.0f ), y.getDirection(), 0.01);
	EXPECT_EQ(1, y.getSegmentId());

	LineSegment::mapLineSegments(lsA, lsB, z);
	EXPECT_VEC3NEAR(sf::Vector3f( 2.0f, 0.0f, 3.0f ), z.getPosition(), 0.01);
	EXPECT_VEC2NEAR(sf::Vector2f( 0.0f, -1.0f ), z.getDirection(), 0.01);
	EXPECT_EQ(2, z.getSegmentId());
}

TEST_F(GeometryTest, MapLSTest1) {
	ObjectInScene x{ sf::Vector3f{ 0.0f, 0.0f, 0.0f }, sf::Vector2f{ 1.0f, 0.0f }, 0 };

	LineSegment from{ sf::Vector2f{1.0f, 0.0f}, sf::Vector2f{1.0f, 4.0f} };
	LineSegment to{ sf::Vector2f{5.0f, 1.0f}, sf::Vector2f{5.0, 5.0} };

	LineSegment::mapLineSegments(from, to, x);
	EXPECT_EQ(sf::Vector3f(4.0f, 1.0f, 0.0f), x.getPosition());
	EXPECT_EQ(sf::Vector2f(1.0f, 0.0f), x.getDirection());
	EXPECT_EQ(0, x.getSegmentId());
}

TEST_F(GeometryTest, MapLSTestScaled) {
	ObjectInScene x{ sf::Vector3f{ 0.0f, 2.0f, 0.0f }, sf::Vector2f{ 0.0f, 1.0f }, 0 };

	LineSegment from{ sf::Vector2f{ 1.0f, 0.0f }, sf::Vector2f{ 1.0f, 4.0f } };
	LineSegment to{ sf::Vector2f{ 5.0f, 1.0f }, sf::Vector2f{ 5.0, 3.0 } };

	LineSegment::mapLineSegments(from, to, x);
	EXPECT_EQ(sf::Vector3f(4.0f, 2.0f, 0.0f), x.getPosition());
	EXPECT_EQ(sf::Vector2f(0.0f, 1.0f), x.getDirection());
	EXPECT_EQ(0, x.getSegmentId());
}