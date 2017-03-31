#include "Scene.hpp"
#include "Math.hpp"

namespace ps {

	SegmentNotFound::SegmentNotFound(std::size_t segmentId_) {
		description = "Segment with id = " + std::to_string(segmentId_) + " was not found!";
	}

	const char * SegmentNotFound::what() const {
		return description.c_str();
	}

	Segment::Segment(const Floor & floor, const Ceiling & ceiling) : 
		Segment(DEFAULT_FLOOR_HEIGHT, DEFAULT_WALL_HEIGHT, floor, ceiling) {

	}

	Segment::Segment(float floorHeight, float wallHeight, const Floor & floor_, const Ceiling & ceiling_) :
		segmentFloorHeight(floorHeight), segmentWallHeight(wallHeight), floor(floor_), ceiling(ceiling_), walls() {

	}

	const std::vector<PortalWall>& Segment::getWalls() const {
		return walls;
	}

	Camera::Camera(const FloatingObjInScene & obj) : viewPlaneDirection(), FloatingObjInScene(obj)
	{
		float defaultHFOV = 0.4f * PI<float>;		// default horizontal fov is approx. 72 degrees
		float defaultAspectRatio = 16.0f / 9.0f;	// expect widescreen monitor by default
		setFOV(defaultHFOV, defaultAspectRatio);
	}

	void Camera::rotate(float angle)
	{
		FloatingObjInScene::rotate(angle);
		ps::rotate(viewPlaneDirection, angle);
	}

	void Camera::setFOV(float horizontalFOV, float aspectRatio)
	{
		if (0.0f < horizontalFOV && 0.0f < aspectRatio) {
			hFOV = horizontalFOV;
			float viewPlaneNorm = tan(horizontalFOV / 2.0f);	// view plane length (corresponds to horizontal FOV)

																// sets viewPlane to be perpendicuar to direction
			viewPlaneDirection.x = direction.y;
			viewPlaneDirection.y = -1.0f * direction.x;

			viewPlaneDirection *= viewPlaneNorm;
			viewPlaneHeight = viewPlaneNorm / aspectRatio;		// this is height of view plane (corresponds to vertical FOV (define by aspect ratio))
		}
	}

	Scene::Scene(const ObjectInScene & camera_) : 
		segments(), camera(FloatingObjInScene(camera_, 50.0f, *this)) {
	}

	std::size_t Scene::addSegment(Segment && segment_) {
		segments.push_back(std::move(segment_));
		return segments.size() - 1;
	}

	Segment& Scene::getSegment(std::size_t segmentId) {
		if (0 <= segmentId && segmentId < segments.size()) {
			return segments[segmentId];
		}
		else {
			// this id corresponds to no segment => throw expection
			throw SegmentNotFound(segmentId);
		}
	}

	const Segment & Scene::getSegment(std::size_t segmentId) const {
		if (0 <= segmentId && segmentId < segments.size()) {
			return segments[segmentId];
		}
		else {
			// this id corresponds to no segment => throw expection
			throw SegmentNotFound(segmentId);
		}
	}

	void FloatingObjInScene::rotate(float angle)
	{
		ObjectInScene::rotate(angle);

		sf::Vector2f newForce = toVector2(force);
		ps::rotate(newForce, angle);
		force.x = newForce.x;
		force.y = newForce.y;

		sf::Vector2f newSpeed = toVector2(speed);
		ps::rotate(newSpeed, angle);
		speed.x = newSpeed.x;
		speed.y = newSpeed.y;
	}

	void FloatingObjInScene::move(sf::Vector3f offset)
	{
		// check here passing through wall
		sf::Vector3f from = position;
		sf::Vector3f to = from + offset;

		position = to;

		LineSegment cameraLS{ toVector2(from), toVector2(to) };

		auto & segment = scene.getSegment(segmentId);

		bool cameraUnderFloor = (to.z <= (segment.segmentFloorHeight + 0.1f));
		bool cameraAboveCeiling = (to.z >= (segment.segmentFloorHeight + segment.segmentWallHeight - 0.1f));
		if (cameraUnderFloor || cameraAboveCeiling) {
			// camera wants either to go through the floor or the ceiling
			position = from;
			return;
		}

		// check whether the camera didn't pass through wall
		for (auto & wall : segment.getWalls()) {
			if (wall.intersect(cameraLS)) {
				// camera would move through the wall
				if (wall.isPortal()) {
					// camera steps through the portal
					wall.stepThrough(*this);
					return;
				}
				else {
					// camera wants to pass solid wall => put it back
					position = from;
					return;
				}
			}
		}
	}

	void FloatingObjInScene::applyForce(const sf::Vector3f & force_)
	{
		this->force += force_;
	}

	void FloatingObjInScene::applyTorque(float torque_)
	{
		this->torque += torque_;
	}

	void FloatingObjInScene::simulate(float deltaTime)
	{
		sf::Vector3f acceleration = force * (1.0f / mass);
		float angularAcceleration = torque / mass;

		speed += acceleration * deltaTime;
		angularSpeed += angularAcceleration * deltaTime;

		move(speed * deltaTime);
		rotate(angularSpeed * deltaTime);

		// reset force and torque
		force = sf::Vector3f(0.0f, 0.0f, 0.0f);
		torque = 0.0f;
	}

	sf::Vector3f FloatingObjInScene::getSpeed() const
	{
		return speed;
	}

	float FloatingObjInScene::getAngularSpeed() const
	{
		return angularSpeed;
	}

	FloatingObjInScene::FloatingObjInScene(const ObjectInScene & obj, float mass_, Scene & scene_) :
		ObjectInScene(obj), scene(scene_), mass(mass_), force(0.0f, 0.0f, 0.0f), torque(0.0f),
		speed(0.0f, 0.0f, 0.0f), angularSpeed(0.0f)
	{
	}
}