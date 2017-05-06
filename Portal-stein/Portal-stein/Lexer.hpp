#pragma once
#ifndef PS_LEXER_INCLUDED
#define PS_LEXER_INCLUDED
#include <exception>
#include <string>

namespace ps {

	enum class TokenType;

	//*********************************************
	// EXCEPTIONS
	//*********************************************

	/// This exception occurs when parser expects different token from actual token that is read.
	class UnexpectedTokenException : public std::exception {
	public:
		TokenType expected;
		TokenType actual;
		int lineNumber;

		UnexpectedTokenException(TokenType expected, TokenType actual, int lineNumber);
		virtual const char * what() const override;
	};

	class OpenStringLiteralException : public std::exception {
	public:
		int lineNumber;

		OpenStringLiteralException(int lineNumber);
		virtual const char * what() const override;
	};


	//*********************************************
	// TOKENS
	//*********************************************

	enum class TokenType {
		ASTERISK, COLON, COMMA, MINUS, ID, STRING, FLOAT, INT,
		LBRA, RBRA, LCBRA, RCBRA, LPAR, RPAR,
		END_OF_STREAM, ERROR
	};

	/// For each token there is a string that describes it, e.g. TokenType::LCBRA --> "Left curly brace".
	std::string getTokenString(TokenType type);
	/// Structure containing the value of token (string, int, or float).
	struct TokenValue {		// This could be a union, but I had problem with default constructor
		std::string	s;
		int i;
		float f;
	};

	/// Class representing lexical element.
	class Token {
	public:
		TokenType type;		///< Type of token.
		TokenValue value;	///< Value of token (string/int/float).
		int lineNumber;		///< Number of line the token was found on.

		/// Creates token of specidied type.
		/// \param type Type of the token.
		/// \param lineNumber Number of line the token is on.
		Token(TokenType type, int lineNumber);
		/// Creates token of FLOAT type.
		Token(float floatValue, int lineNumber);
		/// Creates token of INT type.
		Token(int intValue, int lineNumber);
		/// Creates token of STRING type.
		Token(std::string stringValue, int lineNumber);
	};



	//***********************************************
	// LEXER
	//***********************************************

	/// Lexical analyzer class.
	class Lexer {
	private:
		std::istream & input;	///< Input for the lexer.
		int lineNumber;			///< Line the lexer is on.
		Token lookahead;		///< Token that comes next (it will be returned in next getNextToken()).

		std::string readString();
		std::string readIdentifier();

	public:
		Lexer(std::istream & input_);

		/// Gets the token that will be read next.
		const Token & getLookahead();
		/// Gets next token from the input.
		Token getNextToken();
		/// Gets next token from input + checks whether its type matches the specified type. On failure throws UnexpectedTokenException.
		/// \sa UnexpectedTokenException
		Token eat(TokenType type);
		/// Same as eat(TokenType::ID) + returns directly the id string.
		std::string eatIdentifier();
		/// Same as eat(TokenType::STRING) + returns directly the actual string.
		std::string eatString();
		/// Same as eat(TokenType::INT) + returns directly the actual value.
		int eatInt();
		/// Eats value that can be converted to float i.e. INT or FLOAT.
		float eatFloatOrInt();

		// Give access to the parser.
		friend class LevelLoader;
	};

}

#endif // !PS_LEXER_INCLUDED
