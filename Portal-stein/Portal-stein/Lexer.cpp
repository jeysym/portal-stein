#include "Lexer.hpp"

namespace ps {

	UnexpectedTokenException::UnexpectedTokenException(TokenType expected_, TokenType actual_, int lineNumber_) :
		expected(expected_), actual(actual_), lineNumber(lineNumber_) {
	}

	const char * UnexpectedTokenException::what() const
	{
		return "Unexpected token encountered while reading the input!";
	}

	inline bool isWhite(char c) {
		return (c == '\n' || c == ' ' || c == '\t' || c == '\r' || c == '\v');
	}

	inline bool isAlpha(char c) {
		bool isSmallLetter = ('a' <= c && c <= 'z');
		bool isCapsLetter = ('A' <= c && c <= 'Z');
		return isSmallLetter || isCapsLetter;
	}

	inline bool isNum(char c) {
		return ('0' <= c && c <= '9');
	}

	inline bool isAlnum(char c) {
		return isAlpha(c) || isNum(c);
	}

	inline bool isIDLetter(char c) {
		return isAlnum(c) || c == '_';
	}


	//**************************************
	// TOKENS
	//**************************************

	std::string getTokenString(TokenType type)
	{
		switch (type) {
		case TokenType::ASTERISK: return "Asterisk";
		case TokenType::COLON: return "Colon";
		case TokenType::COMMA: return "Comma";
		case TokenType::MINUS: return "Minus";
		case TokenType::ID: return "Identifier";
		case TokenType::STRING: return "String";
		case TokenType::FLOAT: return "Float";
		case TokenType::INT: return "Int";
		case TokenType::LBRA: return "Left bracket";
		case TokenType::RBRA: return "Right bracket";
		case TokenType::LCBRA: return "Left curly brace";
		case TokenType::RCBRA: return "Right curyly brace";
		case TokenType::LPAR: return "Left parenthesis";
		case TokenType::RPAR: return "Right parethesis";
		case TokenType::END_OF_STREAM: return "End of stream";
		case TokenType::ERROR: return "Error";
		}
		return "Unnamed type";
	}

	Token::Token(TokenType type_, int lineNumber_) : type(type_), lineNumber(lineNumber_) {
	}

	Token::Token(float value_, int lineNumber_) : Token(TokenType::FLOAT, lineNumber_) {
		value.f = value_;
	}

	Token::Token(int value_, int lineNumber_) : Token(TokenType::INT, lineNumber_) {
		value.i = value_;
	}

	Token::Token(std::string value_, int lineNumber_) : Token(TokenType::STRING, lineNumber_) {
		value.s = value_;
	}


	//**********************************
	// LEXER
	//**********************************

	std::string Lexer::readString()
	{
		std::string result;
		bool escape = false;
		char c;
		while (c = input.get()) {
			if (c == '\n')
				lineNumber++;

			if (escape) {
				result.push_back(c);
				escape = false;
			}
			else {
				if (c == '\\') {
					escape = true;
				}
				else if (c == '"') {
					return result;
				}
				else {
					result.push_back(c);
				}
			}
		}

		// This case happens when end-of-file (or some error) is encountered while searching for right "
		// TODO : maybe throw exception...
		return "";
	}

	std::string Lexer::readIdentifier()
	{
		std::string result;

		char c;
		while (c = input.get()) {
			if (isIDLetter(c)) {
				result.push_back(c);
			}
			else {
				input.putback(c);
				return result;
			}
		}
	}

	Lexer::Lexer(std::istream & input_) : input(input_), lineNumber(1), lookahead(-1, -1)
	{
		lookahead = getNextToken();
	}

	const Token & Lexer::getLookahead()
	{
		return lookahead;
	}

	void discardThisLine(std::istream & input) {
		char c;
		do {
			c = input.get();
		} while (c != '\n' && input.good());
	}

	Token Lexer::getNextToken()
	{
		char c = input.get();
		while (isWhite(c) || c == '#') {
			// skips white characters
			while (isWhite(c)) {
				if (c == '\n')
					lineNumber++;

				c = input.get();
			}

			if (c == '#') {
				// '#' means start of the comment => discard the rest of the line
				discardThisLine(input);
				lineNumber++;
				c = input.get();
			}
		}

		if (input.good() == false)
			return Token(TokenType::END_OF_STREAM, lineNumber);

		switch (c) {
		case '*': return Token(TokenType::ASTERISK, lineNumber);
		case ':': return Token(TokenType::COLON, lineNumber);
		case ',': return Token(TokenType::COMMA, lineNumber);
		case '[': return Token(TokenType::LBRA, lineNumber);
		case ']': return Token(TokenType::RBRA, lineNumber);
		case '{': return Token(TokenType::LCBRA, lineNumber);
		case '}': return Token(TokenType::RCBRA, lineNumber);
		case '(': return Token(TokenType::LPAR, lineNumber);
		case ')': return Token(TokenType::RPAR, lineNumber);
		}

		int oldLineNumber = lineNumber;	// this is necessary beacause string can span multiple lines
		if (c == '"')	// reading string
			return Token(readString(), oldLineNumber);

		if (isAlpha(c)) { // reading identifier
			input.putback(c);
			Token result(TokenType::ID, lineNumber);
			result.value.s = readIdentifier();
			return result;
		}

		bool negativeNumber = false;
		if (c == '-') {
			c = input.get();
			if (isNum(c) == false) {
				// this was just ordinary minus with not-number char afterwards, e.g. "-hello"
				input.putback(c);
				return Token(TokenType::MINUS, lineNumber);
			}
			else
				// this is minus in front of number, e.g. "-234"
				negativeNumber = true;
		}

		if (isNum(c)) { // reading number
			std::string number;
			number.push_back(c);
			bool floatNum = false;

			while (c = input.get()) {
				if (isNum(c)) {
					number.push_back(c);
				}
				else if (c == '.') {
					number.push_back(c);
					floatNum = true;
				}
				else {
					input.putback(c);

					if (floatNum)
						return Token((negativeNumber ? -1.0f : 1.0f) * std::stof(number), lineNumber);
					else
						return Token((negativeNumber ? -1 : 1) * std::stoi(number), lineNumber);
				}
			}
		}

		return Token(TokenType::ERROR, lineNumber);
	}

	Token Lexer::eat(TokenType expectedType)
	{
		if (lookahead.type != expectedType) {
			throw UnexpectedTokenException(expectedType, lookahead.type, lineNumber);
		}

		Token oldLookahead = lookahead;
		lookahead = getNextToken();
		return oldLookahead;
	}

	std::string Lexer::eatIdentifier()
	{
		Token token = eat(TokenType::ID);
		return token.value.s;
	}

	std::string Lexer::eatString()
	{
		Token token = eat(TokenType::STRING);
		return token.value.s;
	}

	int Lexer::eatInt()
	{
		Token token = eat(TokenType::INT);
		return token.value.i;
	}

	float Lexer::eatFloatOrInt()
	{
		if (lookahead.type == TokenType::INT) {
			auto token = eat(TokenType::INT);
			return (float)token.value.i;
		}

		auto token = eat(TokenType::FLOAT);
		return token.value.f;
	}

}