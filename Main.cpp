#include "SDL.h" 
#include "ECS/PhysicsSystem.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/Vec3.hpp"
#include "ECS/Types.hpp"
#include "ECS/System.hpp"
#include <bitset>
#include <vector>
#include <random>
#include <bitset>
#include <bitset>
#include <random>
#include <chrono>
#include <iostream>

// A simple type alias
using Entity = std::uint32_t;
using Signature = std::bitset<MAX_COMPONENTS>;
using ComponentType = std::uint8_t;

struct Gravity
{
    Vec3 force;
};

struct RigidBody
{
    Vec3 velocity;
    Vec3 acceleration;
};

struct Transform
{
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
};

struct RigidBodyECS
{
	Vec3 velocity;
	Vec3 acceleration;
};


Coordinator gCoordinator;

void PhysicsSystem::Update(float dt)
{
    for (auto const& entity : mEntities)
    {
        auto& rigidBody = gCoordinator.GetComponent<RigidBodyECS>(entity);
        auto& transform = gCoordinator.GetComponent<Transform>(entity);
        auto const& gravity = gCoordinator.GetComponent<Gravity>(entity);

        transform.position += rigidBody.velocity * dt;

        rigidBody.velocity += gravity.force * dt;
		// std::cout << "updating " + entity;
    }
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "SDL2Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(200);











	gCoordinator.Init();

	gCoordinator.RegisterComponent<Gravity>();
	gCoordinator.RegisterComponent<RigidBodyECS>();
	gCoordinator.RegisterComponent<Transform>();

	auto physicsSystem = gCoordinator.RegisterSystem<PhysicsSystem>();

	Signature signature;
	signature.set(gCoordinator.GetComponentType<Gravity>());
	signature.set(gCoordinator.GetComponentType<RigidBodyECS>());
	signature.set(gCoordinator.GetComponentType<Transform>());
	gCoordinator.SetSystemSignature<PhysicsSystem>(signature);

	std::vector<Entity> entities(MAX_ENTITIES);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
	std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

	float scale = randScale(generator);

	for (auto& entity : entities)
	{
		entity = gCoordinator.CreateEntity();
		std::cout << entity << std::endl;

		gCoordinator.AddComponent(
			entity,
			Gravity{ Vec3(0.0f, randGravity(generator), 0.0f) });


		RigidBodyECS rigidBody = RigidBodyECS();
		rigidBody.velocity = Vec3(0.0f, 0.0f, 0.0f);
		rigidBody.acceleration = Vec3(0.0f, 0.0f, 0.0f);

		gCoordinator.AddComponent(
			entity,
			rigidBody);


		Transform transform = Transform();
		transform.position = Vec3(randPosition(generator), randPosition(generator), randPosition(generator));
		transform.rotation = Vec3(randRotation(generator), randRotation(generator), randRotation(generator));
		transform.scale = Vec3(scale, scale, scale);
		gCoordinator.AddComponent(
			entity,
			transform);
	}

	float dt = 0.0f;

	bool quit = false;
	while (!quit)
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		physicsSystem->Update(dt);

		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();



		for (auto& entity : entities) {


			auto& rigidBody = gCoordinator.GetComponent<RigidBodyECS>(entity);
			auto& transform = gCoordinator.GetComponent<Transform>(entity);
			auto const& gravity = gCoordinator.GetComponent<Gravity>(entity);

			int scale = 5;
			SDL_Rect rect;
			rect.x = transform.position.x * scale;
			rect.y = transform.position.y * scale;
			rect.w = transform.scale.x * scale;
			rect.h = transform.scale.y * scale;
			SDL_SetRenderDrawColor(renderer, entity / 2, 255, 255, entity / 2);
			SDL_RenderDrawRect(renderer, &rect);
			//// SDL_RenderDraw


			//Event handler
			SDL_Event e;
			 //Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			

		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);

	}











    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}