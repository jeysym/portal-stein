#include "LevelLoader.hpp"
#include "SegmentBuilder.hpp"
#include <assert.h>

namespace ps {

	IdentifierException::IdentifierException(const std::string & id_, const std::string & type_, const std::string & message_, int lineNumber_) :
		id(id_), type(type_), message(message_), lineNumber(lineNumber_)
	{
	}

	const char * IdentifierException::what() const
	{
		return message.c_str();
	}

	TexureLoadFailedException::TexureLoadFailedException(const std::string & path_, int lineNumber_) :
		path(path_), lineNumber(lineNumber_) {
	}

	const char * TexureLoadFailedException::what() const
	{
		return "Error occured while trying to load texture from file!";
	}


	//********************************************
	// PARSER
	//********************************************

	LevelLoader::LevelLoader(std::istream & input_) : 
		lexer(input_), 
		namedTextures("Texture"), namedColors("Color"), namedVertices("Vertex"), namedSegments("Segment"), 
		initialPlayer(sf::Vector3f{ 0.0f, 0.0f, 0.0f }, sf::Vector2f{ 1.0f, 0.0f }, 0),
		defaultFloor(sf::Color::Blue), defaultCeiling(sf::Color::Green)
	{
	}

	std::shared_ptr<sf::Texture> LevelLoader::texture()
	{
		// Texture entry format:
		//     "path_to_texture"
		//     id			// named texture

		if (lexer.getLookahead().type == TokenType::ID)
			return namedTextures.get(lexer.eat(TokenType::ID));

		Token pathToken = lexer.eat(TokenType::STRING);

		std::shared_ptr<sf::Texture> texPtr = std::make_shared<sf::Texture>();
		if (!texPtr->loadFromFile(pathToken.value.s))		
			throw TexureLoadFailedException(pathToken.value.s, pathToken.lineNumber);
			
		return texPtr;
	}

	void LevelLoader::textures()
	{
		while (lexer.lookahead.type == TokenType::ID) {
			Token idToken = lexer.eat(TokenType::ID);
			lexer.eat(TokenType::COLON);
			auto texturePtr = texture();

			namedTextures.insertNew(idToken, texturePtr);
		}
	}

	sf::Color LevelLoader::color()
	{
		// Color entry format:
		//    (r_value, g_value, b_value)
		//    id	// color has name

		if (lexer.lookahead.type == TokenType::ID)
			return namedColors.get(lexer.eat(TokenType::ID));

		lexer.eat(TokenType::LPAR);
		auto r = lexer.eat(TokenType::INT);
		lexer.eat(TokenType::COMMA);
		auto g = lexer.eat(TokenType::INT);
		lexer.eat(TokenType::COMMA);
		auto b = lexer.eat(TokenType::INT);
		lexer.eat(TokenType::RPAR);

		return sf::Color((sf::Uint8)r.value.i, (sf::Uint8)g.value.i, (sf::Uint8)b.value.i);
	}

	void LevelLoader::colors()
	{
		while (lexer.lookahead.type == TokenType::ID) {
			// Color entry format:
			//     identifier : color
			Token idToken = lexer.eat(TokenType::ID);
			lexer.eat(TokenType::COLON);
			sf::Color assignedColor = color();
			
			namedColors.insertNew(idToken, assignedColor);
		}
	}

	sf::Vector2f LevelLoader::vertex()
	{
		// Vertex entry format:
		//    (x_coordinate, y_coordinate)
		//    id		// named vertex

		if (lexer.lookahead.type == TokenType::ID)
			return namedVertices.get(lexer.eat(TokenType::ID));

		lexer.eat(TokenType::LPAR);
		float x = lexer.eatFloatOrInt();
		lexer.eat(TokenType::COMMA);
		float y = lexer.eatFloatOrInt();
		lexer.eat(TokenType::RPAR);

		return sf::Vector2f(x, y);
	}

	void LevelLoader::vertices()
	{
		while (lexer.lookahead.type == TokenType::ID) {
			// Vertex entry format:
			//     identifier : vertex
			Token idToken = lexer.eat(TokenType::ID);
			lexer.eat(TokenType::COLON);
			sf::Vector2f assignedVertex = vertex();

			namedVertices.insertNew(idToken, assignedVertex);
		}
	}

	void LevelLoader::loadMap()
	{
		std::istream & input = lexer.input;
		char c;

		// skip all characters to first line-feed
		do {
			c = input.get();
		} while (input.good() && c != '\n');
		lexer.lineNumber++;

		int x = 0;
		int y = 0;
		while (true) {
			c = input.get();

			if (input.good() == false) {
				// end of stream => end of map
				lexer.lookahead = Token(TokenType::END_OF_STREAM, lexer.lineNumber);
				return;
			}
			
			if (c == '*') {
				// * means beginning of next sectino => end of map
				lexer.lookahead = Token(TokenType::ASTERISK, lexer.lineNumber);
				return;
			}

			if (c == '\n') {
				lexer.lineNumber++;
				--y;
				x = -1;
			}
			else if (c != ' ') {
				Token idToken(TokenType::ID, lexer.lineNumber);
				idToken.value.s = std::string(1, c);	// string of only one character

				namedVertices.insertNew(idToken, sf::Vector2f((float)x, (float)y));
			}

			++x;
		}
	}

	void LevelLoader::colorAndTexture(sf::Color & color_, std::shared_ptr<sf::Texture> & texture_)
	{
		// default values
		color_ = sf::Color::White;
		texture_ = nullptr;

		lexer.eat(TokenType::LPAR);

		const Token & lookahead = lexer.getLookahead();

		bool nextIsIdTexture = lookahead.type == TokenType::ID && namedTextures.contains(lookahead);
		bool nextIsStringTexture = lookahead.type == TokenType::STRING;
		if (nextIsIdTexture || nextIsStringTexture) {
			// this id represents texture
			//     "(" texture ")"
			texture_ = texture();
		}
		else {
			color_ = color();

			if (lexer.lookahead.type == TokenType::COMMA) {
				// texture can follow
				//    "(" color "," texture ")"
				lexer.eat(TokenType::COMMA);
				texture_ = texture();
			}
			// else it is the case 
			//     "(" color ")"
		}

		lexer.eat(TokenType::RPAR);
	}

	void LevelLoader::floorAttribute(SegmentBuilder & builder)
	{
		sf::Color color;
		std::shared_ptr<sf::Texture> texture;
		colorAndTexture(color, texture);
		builder.setFloor(Floor(color, texture));
	}

	void LevelLoader::ceilingAttribute(SegmentBuilder & builder)
	{
		sf::Color color;
		std::shared_ptr<sf::Texture> texture;
		colorAndTexture(color, texture);
		builder.setCeiling(Ceiling(color, texture));
	}

	LevelLoader::LoadedPortal LevelLoader::portal()
	{
		LoadedPortal portal;

		lexer.eat(TokenType::LBRA);
		Token idToken = lexer.eat(TokenType::ID);

		portal.type = PortalType::DOOR;
		portal.targetSegment = getSegment(idToken).id;

		if (lexer.lookahead.type == TokenType::MINUS) {
			// portal transports the object to some wall
			lexer.eat(TokenType::MINUS);
			portal.to0 = vertex();
			lexer.eat(TokenType::MINUS);
			portal.to1 = vertex();
			portal.type = PortalType::WALL_PORTAL;
		}

		lexer.eat(TokenType::RBRA);
		return portal;
	}

	void LevelLoader::wallModifier(sf::Color & color, std::shared_ptr<sf::Texture>& texture, LoadedPortal & portal_)
	{
		if (lexer.lookahead.type == TokenType::MINUS) {
			// wall has no modifiers
			lexer.eat(TokenType::MINUS);
		}
		else {
			// wall has some modifiers in this format:
			//       (color, texture)[portal]

			if (lexer.lookahead.type == TokenType::LPAR) {
				// wall has appearance modifiers
				colorAndTexture(color, texture);
			}

			if (lexer.lookahead.type == TokenType::LBRA) {
				// wall has portal modifier		
				portal_ = portal();
			}
		}
	}

	void LevelLoader::wallsAttribute(SegmentBuilder & builder)
	{
		sf::Color defaultColor = sf::Color::Red;
		std::shared_ptr<sf::Texture> defaultTexture = nullptr;

		// Optional default wall parameters in format:
		//      (color, texture) 
		//      (color)
		//      (texture)
		if (lexer.lookahead.type == TokenType::LPAR) {
			colorAndTexture(defaultColor, defaultTexture);
		}

		// Walls format:
		//     { a-b-c(color,texture)d-e(red)[segment1]-f-g- }
		lexer.eat(TokenType::LCBRA);
		
		sf::Vector2f firstVertex = vertex();
		sf::Vector2f lastVertex = firstVertex;

		while (lexer.lookahead.type != TokenType::RCBRA) {
			sf::Color currentColor = defaultColor;
			std::shared_ptr<sf::Texture> currentTexture = defaultTexture;
			portalPtr currentPortal = nullptr;

			LoadedPortal portal;
			wallModifier(currentColor, currentTexture, portal);

			sf::Vector2f from0;
			sf::Vector2f from1;

			bool end = false;
			if (lexer.lookahead.type == TokenType::RCBRA) {
				from0 = lastVertex;
				from1 = firstVertex;
				end = true;
			}
			else {
				sf::Vector2f vertex_ = vertex();
				from0 = lastVertex;
				from1 = vertex_;
			}

			currentPortal = portal.makePortal(from0, from1);

			PortalWall wall(from0, from1, currentColor, currentTexture);
			wall.setPortal(currentPortal);
			builder.addWall(std::move(wall));

			if (end)
				break;

			lastVertex = from1;
		}

		lexer.eat(TokenType::RCBRA);
	}

	LevelLoader::SegmentWithId & LevelLoader::getSegment(const Token & idToken) {
		assert(idToken.type == TokenType::ID);

		if (namedSegments.contains(idToken) == false) {
			LevelLoader::SegmentWithId newSegment(nullptr, namedSegments.size());
			namedSegments.insertNew(idToken, newSegment);
		}

		return namedSegments.get(idToken);
	}

	Segment LevelLoader::segment()
	{
		SegmentBuilder builder(defaultFloor, defaultCeiling);

		lexer.eat(TokenType::LCBRA);
		while (lexer.lookahead.type == TokenType::ID) {
			auto idToken = lexer.eat(TokenType::ID);
			auto & attribute = idToken.value.s;

			if (attribute == "floor") {
				floorAttribute(builder);
			}
			else if (attribute == "ceiling") {
				ceilingAttribute(builder);
			}
			else if (attribute == "walls") {
				wallsAttribute(builder);
			}
			else if (attribute == "finish") {
				builder.setFinish(true);
			}
			else {
				throw IdentifierException(attribute, "Attribute", "Unknown segment attribute!", idToken.lineNumber);
			}
		}

		lexer.eat(TokenType::RCBRA);
		return builder.finalize();
	}

	void LevelLoader::segments()
	{
		while (lexer.lookahead.type == TokenType::ID) {
			// Segment entry format:
			//     identifier : segment
			Token idToken = lexer.eat(TokenType::ID);
			lexer.eat(TokenType::COLON);
			auto segment_ = segment();

			getSegment(idToken).segment = std::make_shared<Segment>(segment_);
			getSegment(idToken).defined = true;		// segment was defined now
		}
	}

	void LevelLoader::player()
	{
		sf::Vector2f playerPosition2D = vertex();
		sf::Vector3f playerPosition(playerPosition2D.x, playerPosition2D.y, 0.5f);
		lexer.eat(TokenType::MINUS);
		sf::Vector2f playerDirection = vertex();
		lexer.eat(TokenType::MINUS);
		Token idToken = lexer.eat(TokenType::ID);
		std::size_t playerSegment = getSegment(idToken).id;
	
		initialPlayer = ObjectInScene(playerPosition, playerDirection, playerSegment);
	}

	Level LevelLoader::loadLevel()
	{
		while (lexer.lookahead.type == TokenType::ASTERISK) {
			lexer.eat(TokenType::ASTERISK);
			Token idToken = lexer.lookahead;
			std::string keyword = idToken.value.s;

			if (keyword != "MAP")
				// map is handled differently so it needs that the "MAP" token is not eaten when loadMap() starts
				lexer.eat(TokenType::ID);

			if (keyword == "TEXTURES") {
				textures();
			}
			else if (keyword == "COLORS") {
				colors();
			}
			else if (keyword == "VERTICES") {
				vertices();
			}
			else if (keyword == "MAP") {
				loadMap();
			}
			else if (keyword == "SEGMENTS") {
				segments();
			}
			else if (keyword == "PLAYER") {
				player();
			}
			else {
				throw IdentifierException(keyword, "Section", "Unknown section!", idToken.lineNumber);
			}
		}

		lexer.eat(TokenType::END_OF_STREAM);

		std::vector<Segment> segmentsVector(namedSegments.size(), Segment(defaultFloor, defaultCeiling));
		for (auto & pair : namedSegments.data) {
			SegmentWithId & segment = pair.second;

			if (segment.defined == false)
				// segment was reference (in portal definition) but it was not defined
				throw IdentifierException(pair.first, "Segment", "Segment with this id was referenced, but it was not defined!", lexer.lineNumber);

			segmentsVector[segment.id] = *(segment.segment);	// segment is coppied
		}

		return Level(std::move(segmentsVector), initialPlayer);
	}

	LevelLoader::SegmentWithId::SegmentWithId() : 
		segment(nullptr), id(-1), defined(false)
	{
	}

	LevelLoader::SegmentWithId::SegmentWithId(const std::shared_ptr<Segment> & segment_, std::size_t id_) : 
		segment(segment_), id(id_), defined(false)
	{
	}

	LevelLoader::LoadedPortal::LoadedPortal() : type(PortalType::NONE)
	{
	}

	std::shared_ptr<Portal> LevelLoader::LoadedPortal::makePortal(sf::Vector2f from0, sf::Vector2f from1)
	{
		switch (type) {
		case PortalType::NONE: return nullptr;
			break;
		case PortalType::DOOR: return std::make_shared<Door>(targetSegment);
			break;
		case PortalType::WALL_PORTAL: return std::make_shared<WallPortal>(LineSegment(from0, from1), LineSegment(to0, to1), targetSegment);
			break;
		}
		
		assert(false);
		return nullptr;
	}

}