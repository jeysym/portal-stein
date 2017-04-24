#pragma once
#ifndef PS_LEVEL_LOADER_INCLUDED
#define PS_LEVEL_LOADER_INCLUDED
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <assert.h>

#include "SFML\Graphics.hpp"
#include "Lexer.hpp"
#include "Scene.hpp"
#include "Level.hpp"

namespace ps {

	enum class TokenType;

	//*********************************************
	// EXCEPTIONS
	//*********************************************

	// This exception is thrown when identifier that was not defined is encountered while parsing.
	class IdentifierException : public std::exception {
	public:
		int lineNumber;
		std::string id;
		std::string type;
		std::string message;

		IdentifierException(const std::string & id, const std::string & type, const std::string & message, int lineNumber);
		virtual const char * what() const override;
	};

	class TexureLoadFailedException : public std::exception {
	public:
		int lineNumber;
		std::string path;

		TexureLoadFailedException(const std::string & path, int lineNumber);
		virtual const char * what() const override;
	};


	//**************************************************
	// LEVEL LOADER (PARSER)
	//**************************************************

	template<typename ValueT>
	class NamedValues {
		std::map<std::string, ValueT> data;
		std::string typeDescription;

	public:
		NamedValues(const std::string & typeDescription_) : typeDescription(typeDescription_) {
		}

		void insertNew(const Token & idToken, const ValueT & value) {
			assert(idToken.type == TokenType::ID);

			auto it = data.find(idToken.value.s);
			if (it == data.end()) {
				data.insert(it, std::make_pair(idToken.value.s, value));
			}
			else {
				throw IdentifierException(idToken.value.s, typeDescription, "Identifier cannot be defined twice!", idToken.lineNumber);
			}
		}

		bool contains(const Token & idToken) {
			assert(idToken.type == TokenType::ID);
			return (data.find(idToken.value.s) != data.end());
		}

		ValueT & get(const Token & idToken) {
			auto it = data.find(idToken.value.s);
			if (it != data.end())
				return it->second;
			else
				throw IdentifierException(idToken.value.s, typeDescription, "Identifier was not defined prior to the use!", idToken.lineNumber);
		}

		std::size_t size() {
			return data.size();
		}

		friend class LevelLoader;
	};

	class LevelLoader {
	private:
		Lexer lexer;

		Floor defaultFloor;
		Ceiling defaultCeiling;

		struct SegmentWithId {
			std::size_t id;
			bool defined;
			std::shared_ptr<Segment> segment;

			SegmentWithId();
			SegmentWithId(const std::shared_ptr<Segment> & segment, std::size_t id);
		};
		
		NamedValues<std::shared_ptr<sf::Texture>>	namedTextures;
		NamedValues<sf::Color>						namedColors;
		NamedValues<sf::Vector2f>					namedVertices;
		NamedValues<SegmentWithId>					namedSegments;
		ObjectInScene player;


		// texture : "path_to_texture" | id
		std::shared_ptr<sf::Texture> texture();
		// textures : (id ":" texture)*
		void textures();
		// color : "(" int "," int "," int ")" | id
		sf::Color color();
		// colors : (id ":" color)*
		void colors();
		// vertex : "(" float "," float ")" | id
		sf::Vector2f vertex();
		// vertices : (id ":" vertex)*
		void vertices();
		// loads vertex map
		void loadMap();

		void loadColorTexture(sf::Color & color, std::shared_ptr<sf::Texture> & texture);

		void loadFloorAttribute(SegmentBuilder & builder);
		void loadCeilingAttribute(SegmentBuilder & builder);
		void loadWallsAttribute(SegmentBuilder & builder);
		SegmentWithId & getSegment(const Token & idToken);
		Segment loadSegment();
		void loadSegments();

		void loadPlayer();

	public:
		LevelLoader(std::istream & input);

		Level loadLevel();
	};
}

#endif // !PS_LEVEL_LOADER_INCLUDED