#include "Simulation.hpp"

namespace ps {
	RigidBody::RigidBody(float mass_) : mass(mass_), velocityEffect(0.0f, 0.0f, 0.0f), angularSpeedEffect(0.0f), forceEffect(0.0f, 0.0f, 0.0f), torqueEffect(0.0f)
	{
	}

	void RigidBody::applyForce(sf::Vector3f force)
	{
		forceEffect += force;
	}
	void RigidBody::applyTorque(float torque)
	{
		torqueEffect += torque;
	}

	void RigidBody::Simulate(float secondsElapsed)
	{
		velocityEffect += secondsElapsed * (forceEffect / mass);
		angularSpeedEffect += secondsElapsed * (torqueEffect / mass);

		// reset force effects
		forceEffect = sf::Vector3f{ 0.0f,0.0f,0.0f };
		torqueEffect = 0.0f;
	}

	sf::Vector3f RigidBody::getCurrentVelocity()
	{
		return velocityEffect;
	}

	float RigidBody::getAngularSpeed()
	{
		return angularSpeedEffect;
	}

}