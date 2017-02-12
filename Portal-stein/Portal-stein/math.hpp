#ifndef PS_MATH_INCLUDED
#define PS_MATH_INCLUDED

#include <SFML\Graphics.hpp>

namespace ps {
	template<typename T>
	sf::Vector3<T> cross(sf::Vector3<T> a, sf::Vector3<T> b) {
		sf::Vector3<T> result;

		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;

		return result;
	}
}

#endif // !PS_MATH_INCLUDED
