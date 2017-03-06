#pragma once
#ifndef PS_SIMULATION_INCLUDED
#define PS_SIMULATION_INCLUDED
#include "SFML\Graphics.hpp"

namespace ps {

	class RigidBody {
	private:
		float mass;

		sf::Vector3f velocityEffect;
		float angularSpeedEffect;

		sf::Vector3f forceEffect;
		float torqueEffect;

	public:
		RigidBody(float mass_);

		void applyForce(sf::Vector3f force);
		void applyTorque(float torque);
		void Simulate(float secondsElapsed);
		sf::Vector3f getCurrentVelocity();
		float getAngularSpeed();
	};

}

#endif // !PS_SIMULATION_INCLUDED
