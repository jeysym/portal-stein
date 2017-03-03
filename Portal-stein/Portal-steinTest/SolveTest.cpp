#include "gtest\gtest.h"
#include "..\Portal-stein\Solve.hpp"

using namespace ps;

class SolveTest : public ::testing::Test {
public:

	SolveTest() : 
		identity(1.0f, 0.0f,
				 0.0f, 1.0f),
		zeroMatrix(),
		shearMatrix(1.0f, 1.0f,
		            0.0f, 1.0f),
		anotherMatrix(6.0f, 5.0f,
		              3.0f, 1.0f) {
	}

	virtual void SetUp() {

	}

	Matrix2<float> identity;
	Matrix2<float> zeroMatrix;
	Matrix2<float> shearMatrix;
	Matrix2<float> anotherMatrix;
};

TEST_F(SolveTest, GetElementTest) {
	for (int i = 0; i < 2; ++i) 
		for (int j = 0; j < 2; ++j) {
			if (i == j) {
				EXPECT_EQ(1.0f, identity.getElement(i, j));
			}
			else {
				EXPECT_EQ(0.0f, identity.getElement(i, j));
			}
			EXPECT_EQ(0.0f, zeroMatrix.getElement(i, j));
		}
}

TEST_F(SolveTest, SetElementTest) {
	identity.setElement(0, 1, 4.0f);
	EXPECT_EQ(4.0f, identity.getElement(0, 1));

	identity.setElement(0, 0, -6.0f);
	EXPECT_EQ(-6.0f, identity.getElement(0, 0));
}

TEST_F(SolveTest, DeterminantTest) {
	EXPECT_EQ(1.0f, identity.determinant()) << "Identity matrix should have determinant 1!";
	EXPECT_EQ(0.0f, zeroMatrix.determinant()) << "Zero matrix should have determinant 0!";
	EXPECT_EQ(1.0f, shearMatrix.determinant()) << "This shear matrix should have determinant 1!";
	EXPECT_EQ(-9.0f, anotherMatrix.determinant()) << "Bad determinant!";
}

TEST_F(SolveTest, MatrixMulitplyTest) {
	sf::Vector2f vector{ 1.0f, 2.0f };
	sf::Vector2f zero{ 0.0f, 0.0f };

	EXPECT_EQ(vector, matrixMultiply(identity, vector));
	EXPECT_EQ(zero, matrixMultiply(identity, zero));

	EXPECT_EQ(zero, matrixMultiply(zeroMatrix, vector)) << "Zero matrix should map every vector to zero vector";
	EXPECT_EQ(zero, matrixMultiply(zeroMatrix, zero)) << "Zero matrix should map every vector to zero vector";

	EXPECT_EQ(zero, matrixMultiply(shearMatrix, zero)) << "Matrix multiplied with zero vector should result in zero vector!";
	EXPECT_EQ(zero, matrixMultiply(anotherMatrix, zero)) << "Matrix multiplied with zero vector should result in zero vector!";

	EXPECT_EQ(sf::Vector2f( 3.0f, 2.0f ), matrixMultiply(shearMatrix, vector));
	EXPECT_EQ(sf::Vector2f( 16.0f, 5.0f), matrixMultiply(anotherMatrix, vector));
}

TEST_F(SolveTest, LinearSolveTest0) {
	sf::Vector2f a{ 1.0f, 1.0f };
	sf::Vector2f b{ 7.0f, 4.0f };

	auto solution0 = linearSolve(identity, a);
	ASSERT_TRUE(solution0.solveable) << "Identity matrix should be solveable!";
	EXPECT_EQ(a, solution0.solution);

	auto solution1 = linearSolve(identity, b);
	ASSERT_TRUE(solution1.solveable) << "Identity matrix should be solveable";
	EXPECT_EQ(b, solution1.solution);
}

TEST_F(SolveTest, LinearSolveTest1) {
	sf::Vector2f a{ 1.0f, 1.0f };
	sf::Vector2f b{ 7.0f, 4.0f };

	auto solution0 = linearSolve(zeroMatrix, a);
	EXPECT_FALSE(solution0.solveable) << "Zero matrix should be solveable!";

	auto solution1 = linearSolve(zeroMatrix, b);
	EXPECT_FALSE(solution1.solveable) << "Zero matrix should be solveable!";
}

TEST_F(SolveTest, LinearSolveTest2) {
	sf::Vector2f a{ 7.0f, 4.0f };

	auto solution0 = linearSolve(shearMatrix, a);
	ASSERT_TRUE(solution0.solveable);
	EXPECT_EQ(sf::Vector2f(3.0f, 4.0f), solution0.solution);
}

TEST_F(SolveTest, LinearSolveTest3) {
	sf::Vector2f a{ 4.0f, -1.0f };

	auto solution0 = linearSolve(anotherMatrix, a);
	ASSERT_TRUE(solution0.solveable);
	EXPECT_EQ(sf::Vector2f(-1.0f, 2.0f), solution0.solution);
}