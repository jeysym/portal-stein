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

	// Class for storing values tied to some identifiers. This class handles errors as multiple defined identifier, or undefined identifier that is used, by throwing exception. 
	template<typename ValueT>
	class NamedValues {
		std::map<std::string, ValueT> data;
		std::string typeDescription;

	public:
		NamedValues(const std::string & typeDescription_) : typeDescription(typeDescription_) {
		}
		
		// Inserts value to the database. If this identifier was already defined IdentifierException will be thrown.
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

		// Returns true if such identifier was defined previously.
		bool contains(const Token & idToken) {
			assert(idToken.type == TokenType::ID);
			return (data.find(idToken.value.s) != data.end());
		}

		// Returns value tied to this identifier. If this identifier was not defined, IdentifierException will be thrown.
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

	// Class that parses the level file, and reads all the information from it, constructing Level object.
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
		ObjectInScene initialPlayer;

		SegmentWithId & getSegment(const Token & idToken);
		// Loads vertex map. This procedure bypasses the lexer, and reads the input directly.
		void loadMap();

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
		// colorAndTexture : "(" color ")" | "(" texture ")" | "(" color "," texture ")"
		void colorAndTexture(sf::Color & color, std::shared_ptr<sf::Texture> & texture);
		// floorAttribute : colorAndTexture
		void floorAttribute(SegmentBuilder & builder);
		// ceilingAttribute : colorAndTexture
		void ceilingAttribute(SegmentBuilder & builder);

		enum class PortalType {
			NONE, DOOR, WALL_PORTAL
		};

		struct LoadedPortal {
			PortalType type;
			sf::Vector2f to0;
			sf::Vector2f to1;
			std::size_t targetSegment;

			LoadedPortal();
			std::shared_ptr<Portal> makePortal(sf::Vector2f from0, sf::Vector2f from1);
		};

		// portal : ("[" id "]") | ("[" id "-" vertex "-" vertex "]") 
		LoadedPortal portal();
		// wallModifier : ("-" | (colorAndTexture? portal?))
		void wallModifier(sf::Color & color, std::shared_ptr<sf::Texture> & texture, LoadedPortal & portal);
		// wallsAttribute : colorAndTexture "{" vertex (wallModifier vertex)* wallModifier "}"
		void wallsAttribute(SegmentBuilder & builder);
		// segment : "{" floorAttribute? ceilingAttribute? wallsAttribute? "}"
		Segment segment();
		// segments : (id ":" segment)*
		void segments();
		// player : vertex "-" vertex "-" id
		void player();

	public:
		LevelLoader(std::istream & input);

		Level loadLevel();
	};
}

#endif // !PS_LEVEL_LOADER_INCLUDED