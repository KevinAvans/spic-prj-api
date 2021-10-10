#include "PhysicsSystem.hpp"

#include "Gravity.hpp"
#include "../RigidBody.hpp"
#include "Components/Thrust.hpp"
#include "Components/Transform.hpp"
#include "Coordinator.hpp"


extern Coordinator gCoordinator;


void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(float dt)
{
	for (auto const& entity : mEntities)
	{
		auto& rigidBody = gCoordinator.GetComponent<RigidBodyECS>(entity);
		auto& transform = gCoordinator.GetComponent<Transform>(entity);
		auto const& gravity = gCoordinator.GetComponent<Gravity>(entity);

		// Forces
		auto const& gravity = gCoordinator.GetComponent<Gravity>(entity);

		transform.position += rigidBody.velocity * dt;

		rigidBody.velocity += gravity.force * dt;
	}
}