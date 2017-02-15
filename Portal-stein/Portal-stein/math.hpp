#ifndef PS_MATH_INCLUDED
#define PS_MATH_INCLUDED

#include <SFML\Graphics.hpp>
#include <cmath>

namespace ps {
	const double PI = atan(1.0) * 4;

	template<typename T>
	inline void rotateVec2(sf::Vector2<T> & vector, float alpha) {
		vector.x = cos(alpha) * vector.x - sin(alpha) * vector.y;
		vector.y = sin(alpha) * vector.x + cos(alpha) * vector.y;
	}

	template<typename T>
	inline sf::Vector3<T> cross(sf::Vector3<T> a, sf::Vector3<T> b) {
		sf::Vector3<T> result;

		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;

		return result;
	}

	template<typename T>
	inline T dot(sf::Vector2<T> & a, sf::Vector2<T> & b) {
		return a.x * b.x + a.y * b.y;
	}

	template<typename T>
	inline T norm(sf::Vector2<T> & vector) {
		return sqrt(dot(vector, vector));
	}

	template<typename T>
	inline sf::Vector2<T> normalize(sf::Vector2<T> & vector) {
		T n = norm(vector);
		vector.x /= n;
		vector.y /= n;
		return vector;
	}

	template<typename T>
	inline T angle(sf::Vector2<T> & a, sf::Vector2<T> & b) {
		return acos(dot(a, b) / (norm(a) * norm(b)));
	}

	template<typename T>
	inline T determinant(sf::Vector2<T> & a, sf::Vector2<T> & b) {
		return a.x * b.y - a.y * b.x;
	}

	// Solves equation of type (a|b)*solution = c
	template<typename T>
	inline bool solveLinEq(sf::Vector2<T> & a, sf::Vector2<T> & b, sf::Vector2<T> & c, sf::Vector2<T> & solution) {
		// using Cramer's rule
		T det = determinant(a, b);
		if (det == 0) {
			// no solution (or infinitely many)
			return false;
		}
		else {
			solution.x = determinant(c, b) / det;
			solution.y = determinant(a, c) / det;
			return true;
		}
	}

	template<typename T>
	inline sf::Vector2<T> getPerpendicular(sf::Vector2<T> & vector) {
		return sf::Vector2<T> {vector.y, -vector.x};
	}

	template<typename T>
	inline T minimum(T a, T b) {
		return (a < b) ? a : b;
	}

	template<typename T>
	inline T maximum(T a, T b) {
		return (a > b) ? a : b;
	}
}

#endif // !PS_MATH_INCLUDED
