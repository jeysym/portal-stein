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

	/// Exception that is thrown when trying to get segment from scene with invalid segment ID.
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

	/// Segment represents a convex room bounded by walls.
	class Segment {
	private:
		std::vector<PortalWall> walls;

		Segment(const Floor & floor, const Ceiling & ceiling);
		Segment(float floorHeight, float wallHeight, const Floor & floor, const Ceiling & ceiling);

	public:
		bool finish;				///< Segments marked with finish flag move the player into next level.
		float segmentFloorHeight;	///< Height level the floor is on.
		float segmentWallHeight;	///< Height of the wall in segment.
		Floor floor;				///< Floor of the segment.
		Ceiling ceiling;			///< Ceiling of the segment.

		Segment(const Segment & obj) = default;
		Segment(Segment && obj) = default;
		Segment& operator=(const Segment & obj) = default;
		Segment& operator=(Segment && obj) = default;

		/// Gets walls of the segment. The walls cannot be modified.
		const std::vector<PortalWall> & getWalls() const;
		
		friend class SegmentBuilder;
		friend class LevelLoader;
	};



	//*******************************************************************
	// FLOATING OBJECT IN SCENE
	//*******************************************************************

	class Scene;

	/// Object in scene, that is aware of the scene (meaning it will not pass through walls), and it can be manipulated by applying force on it.
	/// User must first apply all forces to this object, and the call simulate() to update object speed and angular speed.
	class FloatingObjInScene : public ObjectInScene {
	private:
		sf::Vector3f force;
		float torque;

		sf::Vector3f speed;
		float angularSpeed;

		float mass;
		Scene * scene;

	public:
		/// Creates a new floating object in scene.
		/// \param mass_ Mass of the object (in kg).
		/// \param scene_ Scene the object is in.
		FloatingObjInScene(const ObjectInScene & obj, float mass_, Scene & scene_);

		virtual void rotate(float angle) override;
		virtual void move(sf::Vector3f offset) override;

		/// Applies force on the object.
		void applyForce(const sf::Vector3f & force_);
		/// Applies torque on the object.
		void applyTorque(float torque);
		/// Simulates all the applied forces and torques on the object, which changes the speed and angular speed. After this call all applied forces and torques
		/// on this object are forgot.
		void simulate(float deltaTime);
		/// Gets speed of the object (in unit/s).
		sf::Vector3f getSpeed() const;
		/// Gets angular speed of the object (in rad/s).
		float getAngularSpeed() const;

		friend class Scene;
	};



	//********************************************************************
	// CAMERA CLASS
	//********************************************************************

	/// Represents camera of the ray-caster.
	class Camera : public FloatingObjInScene {
	private:
		float hFOV;	///< Horizontal field-of-view.
		float vFOV;	///< Vertical field-of-view.

		sf::Vector2f viewPlaneDirection;
		float viewPlaneHeight;

	public:
		Camera(const FloatingObjInScene & obj);

		/// Rotates camera by desired angle (counterclockwise).
		virtual void rotate(float angle) override;
		/// Sets horizontal and vertical field-of-view from horizontal FOV and aspect ratio (width : height).
		void setFOV(float horizontalFOV, float aspectRatio);

		/// Camera is part of ray-caster. This lets RayCaster fully access Camera's private members.
		friend class RayCaster;
	};



	//********************************************************************
	// SCENE CLASS
	//********************************************************************
	
	/// Scene is a collection of segments + camera. Scene can be rendered with RayCaster.
	class Scene {
	private:
		std::vector<Segment> segments;

	public:
		Camera camera;

		Scene(const ObjectInScene & camera_);
		Scene(const Scene & rhs);
		Scene & operator=(const Scene & rhs);

		/// Adds a new segment. Its unique id is returned.
		std::size_t addSegment(Segment && segment_);
		/// Gets segment by its id. If no such segment exists SegmentNotFound is thrown.
		/// \sa SegmentNotFound
		Segment& getSegment(std::size_t segmentId);
		/// Gets segment by its id. If no such segment exists SegmentNotFound is thrown.
		/// \sa SegmentNotFound
		const Segment& getSegment(std::size_t segmentId) const;

		friend class Level;
	};


	using scenePtr = std::shared_ptr<Scene>;
}

#endif // !PS_SCENE_INCLUDED
