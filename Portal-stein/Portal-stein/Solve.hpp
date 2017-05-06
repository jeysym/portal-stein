#pragma once
#ifndef PS_SOLVE_INCLUDED
#define PS_SOLVE_INCLUDED
#include "SFML\Graphics.hpp"
#include "Math.hpp"

namespace ps {

	// *******************
	// DECLARATIONS
	// *******************
	
	/// 2x2 matrix.
	template< typename T >
	class Matrix2 {
	private:
		T elements[2][2];

	public:
		/// Initializes the matrix in following way:
		///		| element00  element01 |
		///		| element10  element11 |
		Matrix2(T element00, T element01, T element10, T element11);
		/// Initializes the matrix in following way:
		Matrix2(sf::Vector2<T> a, sf::Vector2<T> b);
		/// Cretes the zero matrix.
		Matrix2();


		/// Sets the matrix element at specific position.
		void setElement(std::size_t rowIndex, std::size_t columnIndex, T value);
		/// Gets the matrix element at the specific position.
		T getElement(std::size_t rowIndex, std::size_t columnIndex) const;
		/// Calculates the determinant of the matrix.
		T determinant() const;
	};

	/// Multiplies a 2D vector with a 2x2 matrix.
	template< typename T >
	inline sf::Vector2<T> matrixMultiply(const Matrix2<T> & matrix, const sf::Vector2<T> & vector);

	template < typename T >
	struct Solution {
		Solution();

		bool solveable;
		sf::Vector2<T> solution;
	};

	/// Solves linear equations in form matrix * x = b. (By using Cramer's rule)
	template< typename T >
	inline Solution<T> linearSolve(const Matrix2<T> & matrix, const sf::Vector2<T> & b);

	// *****************
	// DEFINITIONS 
	// *****************

	template<typename T>
	inline Matrix2<T>::Matrix2(T element00, T element01, T element10, T element11)
	{
		elements[0][0] = element00;
		elements[0][1] = element01;
		elements[1][0] = element10;
		elements[1][1] = element11;
	}

	template<typename T>
	inline Matrix2<T>::Matrix2(sf::Vector2<T> a, sf::Vector2<T> b) : Matrix2(a.x, b.x, a.y, b.y)
	{
	}

	template<typename T>
	inline Matrix2<T>::Matrix2() : Matrix2(0, 0, 0, 0)
	{
	}

	template<typename T>
	inline void Matrix2<T>::setElement(std::size_t rowIndex, std::size_t columnIndex, T value) {
		elements[rowIndex][columnIndex] = value;
	}

	template<typename T>
	inline T Matrix2<T>::getElement(std::size_t rowIndex, std::size_t columnIndex) const
	{
		return elements[rowIndex][columnIndex];
	}

	template<typename T>
	inline T Matrix2<T>::determinant() const
	{
		return elements[0][0] * elements[1][1] - elements[1][0] * elements[0][1];
	}

	template<typename T>
	sf::Vector2<T> matrixMultiply(const Matrix2<T>& matrix, const sf::Vector2<T>& vector)
	{
		sf::Vector2<T> result;

		result.x = matrix.getElement(0, 0) * vector.x + matrix.getElement(0, 1) * vector.y;
		result.y = matrix.getElement(1, 0) * vector.x + matrix.getElement(1, 1) * vector.y;

		return result;
	}

	template<typename T>
	Solution<T> linearSolve(const Matrix2<T>& matrix, const sf::Vector2<T>& b)
	{
		Solution<T> result;
		
		T matrixDeterminant = matrix.determinant();
		
		if (matrixDeterminant == 0) {
			result.solveable = false;
			return result;
		}
		else {
			result.solveable = true;
			Matrix2<T> myMatrix = matrix;
			
			myMatrix.setElement(0, 0, b.x);
			myMatrix.setElement(1, 0, b.y);
			result.solution.x = myMatrix.determinant() / matrixDeterminant;

			myMatrix = matrix;
			myMatrix.setElement(0, 1, b.x);
			myMatrix.setElement(1, 1, b.y);
			result.solution.y = myMatrix.determinant() / matrixDeterminant;

			return result;
		}
	}

	template<typename T>
	inline Solution<T>::Solution() : solveable(false), solution() {	
	}

}

#endif // !PS_SOLVE_INCLUDED
