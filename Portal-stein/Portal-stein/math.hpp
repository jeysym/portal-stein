#ifndef PS_MATH_INCLUDED
#define PS_MATH_INCLUDED

#include <SFML\Graphics.hpp>
#include <cmath>
#include <cassert>

namespace ps {

	// *******************
	// DECLARATIONS
	// *******************

	/// Templated pi (3.1415...) constant.
	/// \tparam T Type of floating point number.
	template< typename T >
	const T PI = (T)(atan(1.0) * 4.0);

	/// Maps number value between two intervals (a0, a1) to (b0, b1).
	/// \param Value to be mapped.
	template< typename T >
	inline T mapIntervals(T a0, T a1, T b0, T b1, T x);

	/// Converts 3D vector to 2D vector by discarding the z cooridinate.
	template< typename T >
	inline sf::Vector2<T> toVector2(const sf::Vector3<T> & vector);

	/// Converts 2D vector to 3D vector by settig the z coordinate to 0.
	template< typename T >
	inline sf::Vector3<T> toVector3(const sf::Vector2<T> & vector);

	/// Rotates 2D vector by desired angle (in radians).
	template< typename T >
	inline void rotate(sf::Vector2<T> & vector, T angle);

	/// Dot product of two 2D vectors.
	template< typename T>
	inline T dot(const sf::Vector2<T> & a, const sf::Vector2<T> & b);

	/// Dot product of two 3D vectors.
	template< typename T>
	inline T dot(const sf::Vector3<T> & a, const sf::Vector3<T> & b);

	/// Computes the (euclidian) norm of the vector.
	template< typename T>
	inline T norm(const sf::Vector2<T> & vector);

	/// Computes the (euclidian) norm of the vector.
	template< typename T>
	inline T norm(const sf::Vector3<T> & vector);

	/// Computes the relative angle difference between two 2D vectors. Following invariant holds :
	/// rotate(x, angleBetween(x, y)) == y, where norm(x) == norm(y)
	template< typename T >
	inline T angleBetween(const sf::Vector2<T> & vectorA, const sf::Vector2<T> & vectorB);

	/// Returns normalized version of the 2D vector.
	template< typename T >
	inline sf::Vector2<T> normalized(const sf::Vector2<T> & vector);

	/// Normalizes the 2D vector.
	template< typename T >
	inline void normalize(sf::Vector2<T> & vector);

	/// Cross product of two 2D vectors.
	template< typename T >
	inline T cross(const sf::Vector2<T> & a, const sf::Vector2<T> & b);

	/// Cross product of two 3D vectors. 
	template< typename T>
	inline sf::Vector3<T> cross(const sf::Vector3<T> & a, const sf::Vector3<T> & b);

	/// Returns maximal out of two arguments.
	template< typename T >
	inline const T& getMax(const T & a, const T & b);

	/// Returns minimal out of two arguments.
	template< typename T >
	inline const T& getMin(const T & a, const T & b);

	// *********************
	// DEFINITIONS 
	// *********************

	template<typename T>
	inline sf::Vector3<T> cross(const sf::Vector3<T> & a, const sf::Vector3<T> & b) {
		sf::Vector3<T> result;

		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;

		return result;
	}

	template<typename T>
	const T& getMax(const T & a, const T & b)
	{
		return (a < b) ? b : a;
	}

	template<typename T>
	const T& getMin(const T & a, const T & b)
	{
		return (a < b) ? a : b;
	}

	template<typename T>
	T angleBetween(const sf::Vector2<T>& vectorA, const sf::Vector2<T>& vectorB)
	{
		T aAngle = atan2(vectorA.y, vectorA.x);
		T bAngle = atan2(vectorB.y, vectorB.x);

		return bAngle - aAngle;
	}

	template<typename T>
	T mapIntervals(T a0, T a1, T b0, T b1, T x)
	{
		T k = (b1 - b0) / (a1 - a0);
		return (x - a0) * k + b0;
	}

	template<typename T>
	sf::Vector2<T> toVector2(const sf::Vector3<T>& vector)
	{
		return sf::Vector2<T>(vector.x, vector.y);
	}

	template<typename T>
	sf::Vector3<T> toVector3(const sf::Vector2<T>& vector)
	{
		return sf::Vector3<T>(vector.x, vector.y, 0);
	}

	template<typename T>
	void rotate(sf::Vector2<T> & vector, T angle)
	{
		T x = vector.x;
		T y = vector.y;
		vector.x = cos(angle) * x - sin(angle) * y;
		vector.y = sin(angle) * x + cos(angle) * y;
	}

	template<typename T>
	inline T dot(const sf::Vector2<T> & a, const sf::Vector2<T> & b) {
		return a.x * b.x + a.y * b.y;
	}

	template<typename T>
	T dot(const sf::Vector3<T>& a, const sf::Vector3<T>& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	template<typename T>
	inline T norm(const sf::Vector2<T> & vector) {
		return sqrt(dot(vector, vector));
	}

	template<typename T>
	T norm(const sf::Vector3<T>& vector)
	{
		return sqrt(dot(vector, vector));
	}

	template<typename T>
	sf::Vector2<T> normalized(const sf::Vector2<T>& vector)
	{
		sf::Vector2<T> result;
		T normValue = norm(vector);
		
		if (normValue == 0) {
			// Zero vector cannot be normalized...
			// TODO : maybe exception would be better...
			assert(false);
		}
		
		result = (1 / normValue) * vector;
		return result;
	}

	template<typename T>
	void normalize(sf::Vector2<T>& vector)
	{
		T normValue = norm(vector);

		if (normValue == 0) {
			// TODO : add exception here
			assert(false);
		}

		vector *= (1 / normValue);
	}

	template<typename T>
	T cross(const sf::Vector2<T> & a, const sf::Vector2<T> & b)
	{
		return a.x * b.y - a.y * b.x;
	}

}

#endif // !PS_MATH_INCLUDED
