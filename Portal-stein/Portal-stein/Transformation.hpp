#pragma once
#ifndef PS_TRANSFORMATION_INCLUDED
#define PS_TRANSFORMATION_INCLUDED
#include <SFML\Graphics.hpp>
#include <array>

namespace ps {

	using float3x3array = std::array<std::array<float, 3>, 3>;

	class LinTransformation3 {
	public:
		LinTransformation3(float3x3array matrix_) : matrix(matrix_) {}

		sf::Vector3f transform(sf::Vector3f vector) {
			sf::Vector3f result;
			// simply a matrix multiplication with input vector.
			result.x = matrix[0][0] * vector.x + matrix[0][1] * vector.y + matrix[0][2] * vector.z;
			result.x = matrix[1][0] * vector.x + matrix[1][1] * vector.y + matrix[1][2] * vector.z;
			result.x = matrix[2][0] * vector.x + matrix[2][1] * vector.y + matrix[2][2] * vector.z;
		}

	private:
		// matrix[i][j] = element of matrix at i-th row, j-th column
		float3x3array matrix;
	};

	class AffineTransformation3 {
	public:
		AffineTransformation3(LinTransformation3 linearTransform_, sf::Vector3f offset_) : linearTransform(linearTransform_), offset(offset_) {}

		sf::Vector3f transform(sf::Vector3f vector) {
			sf::Vector3f result;
			// simply a matrix multiplication with input vector + moving result vector with offset.
			result = linearTransform.transform(vector) + offset;
			return result;
		}

	private:
		LinTransformation3 linearTransform;
		sf::Vector3f offset;
	};
}

#endif // !PS_TRANSFORMATION_INCLUDED
