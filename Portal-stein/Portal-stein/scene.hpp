#pragma once
#ifndef PS_SCENE_INCLUDED
#define PS_SCENE_INCLUDED
#include <vector>
#include <memory>
#include <string>
#include <exception>
#include <SFML\Graphics.hpp>
#include "FloorCeiling.hpp"
#include "Wall.hpp"
#include "ObjectInScene.hpp"

namespace ps {

	//********************************************************************
	// EXCEPTIONS
	//********************************************************************

	// Exception that is thrown when trying to get segment from scene with invalid segment ID.
	class SegmentNotFound : public std::exception {
	private:
		std::string description;

	public:
		SegmentNotFound(std::size_t segmentId_);

		virtual const char * what() const;
	};



	//********************************************************************
	// SEGMENT CLASS
	//********************************************************************

	const float DEFAULT_FLOOR_HEIGHT = 0.0f;
	const float DEFAULT_WALL_HEIGHT = 1.0f;

	// Segment represents a convex room bounded by walls.
	class Segment {
	private:
		std::vector<PortalWall> walls;

		Segment(const Floor & floor, const Ceiling & ceiling);
		Segment(float floorHeight, float wallHeight, const Floor & floor, const Ceiling & ceiling);

	public:
		bool finish;
		float segmentFloorHeight;
		float segmentWallHeight;
		Floor floor;
		Ceiling ceiling;

		Segment(const Segment & obj) = default;
		Segment(Segment && obj) = default;
		Segment& operator=(const Segment & obj) = default;
		Segment& operator=(Segment && obj) = default;

		const std::vector<PortalWall> & getWalls() const;
		
		friend class SegmentBuilder;
		friend class LevelLoader;
	};



	//*******************************************************************
	// FLOATING OBJECT IN SCENE
	//*******************************************************************

	class Scene;

	class FloatingObjInScene : public ObjectInScene {
	private:
		sf::Vector3f force;
		float torque;

		sf::Vector3f speed;
		float angularSpeed;

		float mass;
		Scene * scene;

	public:
		virtual void rotate(float angle) override;
		virtual void move(sf::Vector3f offset) override;

		void applyForce(const sf::Vector3f & force_);
		void applyTorque(float torque);
		void simulate(float deltaTime);

		sf::Vector3f getSpeed() const;
		float getAngularSpeed() const;

		FloatingObjInScene(const ObjectInScene & obj, float mass_, Scene & scene_);

		friend class Scene;
	};



	//********************************************************************
	// CAMERA CLASS
	//********************************************************************

	class Camera : public FloatingObjInScene {
	private:
		float hFOV;
		float vFOV;

		sf::Vector2f viewPlaneDirection;
		float viewPlaneHeight;

	public:
		Camera(const FloatingObjInScene & obj);

		// Rotates camera by desired angle (counterclockwise).
		virtual void rotate(float angle) override;
		// Sets horizontal and vertical field-of-view from horizontal FOV and aspect ratio (width : height).
		void setFOV(float horizontalFOV, float aspectRatio);

		// Camera is part of ray-caster. This lets RayCaster fully access Camera's private members.
		friend class RayCaster;
	};



	//********************************************************************
	// SCENE CLASS
	//********************************************************************
	
	// Scene is a collection of segments + camera. Scene can be rendered with RayCaster.
	class Scene {
	private:
		std::vector<Segment> segments;

	public:
		Camera camera;

		Scene(const ObjectInScene & camera_);
		Scene(const Scene & rhs);
		Scene & operator=(const Scene & rhs);

		/*Scene(const Scene & rs) = default;
		Scene(Scene && rs) = default;
		Scene & operator=(const Scene & rs) = default;
		Scene & operator=(Scene && rs) = default;*/

		// Adds a new segment. Its unique id is returned.
		std::size_t addSegment(Segment && segment_);
		// Gets segment by its id.
		Segment& getSegment(std::size_t segmentId);

		const Segment& getSegment(std::size_t segmentId) const;

		friend class Level;
	};


	using scenePtr = std::shared_ptr<Scene>;
}

#endif // !PS_SCENE_INCLUDED
