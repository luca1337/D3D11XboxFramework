#pragma once

#include <PxPhysicsAPI.h>
#include <string>
#include <iostream>

namespace d3d_engine
{
	static physx::PxFilterFlags FooShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		return physx::PxFilterFlag::eDEFAULT;
	}

	class DummySimulationEvent : public physx::PxSimulationEventCallback
	{

		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
		{
			std::cout << "callback event !!!" << std::endl;
		}

		// Inherited via PxSimulationEventCallback
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override
		{
		}
		void onWake(physx::PxActor** actors, physx::PxU32 count) override
		{
		}
		void onSleep(physx::PxActor** actors, physx::PxU32 count) override
		{
		}
		void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override
		{
		}
		void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override
		{
		}
	};

	class DummyAllocator : public physx::PxAllocatorCallback
	{
		// Inherited via PxAllocatorCallback
		void* allocate(size_t size, const char* typeName, const char* filename, int line) override
		{
			//std::cout << "typeName: " << typeName << " filename: " << filename << " line: " << line << std::endl;
			return std::malloc(size);
		}

		void deallocate(void* ptr) override
		{
			std::free(ptr);
		}
	};

	class DummyErrorCallback : public physx::PxErrorCallback
	{
		// Inherited via PxErrorCallback
		void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
		{
			std::cerr << message << std::endl;
		}
	};

	class DummyDispatcher : public physx::PxCpuDispatcher
	{
	public:
		DummyDispatcher()
		{
			// spawn the thread
		}
		// Inherited via PxCpuDispatcher
		virtual void submitTask(physx::PxBaseTask& task) override
		{
			task.run();
			task.release();
		}

		virtual uint32_t getWorkerCount() const override
		{
			return 0;
		}

	};

	class Object
	{
		virtual void OnTriggerEnter()
		{

		}
	};

	class Actor
	{

	};

	class Physix
	{
		void InitPhysix()
		{
			DummyAllocator dummy_allocator;
			DummyErrorCallback dummy_error_callback;

			physx::PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, dummy_allocator, dummy_error_callback);
			if (!foundation)
			{
				throw std::exception("unable to create foundation");
			}

			physx::PxPhysics* physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());
			if (!physics)
			{
				throw std::exception("unable to create physics engine");
			}

			DummyDispatcher dispatcher;
			DummySimulationEvent evt;

			physx::PxSceneDesc scene_desc(physics->getTolerancesScale());
			scene_desc.gravity = physx::PxVec3(0, -9.81, 0);
			scene_desc.cpuDispatcher = &dispatcher;
			scene_desc.simulationEventCallback = &evt;
			scene_desc.filterShader = FooShader;
			physx::PxScene* scene = physics->createScene(scene_desc);
			if (!scene)
			{
				throw std::exception("unable to create scene");
			}

			physx::PxMaterial* material = physics->createMaterial(1, 0.1, 0.1);
			if (!material)
			{
				throw std::exception("unable to create material");
			}

			physx::PxSphereGeometry sphere_geometry(10);

			physx::PxShape* shape = physics->createShape(sphere_geometry, *material, true);
			if (!shape)
			{
				throw std::exception("unable to create shape");
			}

			physx::PxRigidDynamic* body = physics->createRigidDynamic(physx::PxTransform(0, 30, 0));
			body->setMass(1);
			body->attachShape(*shape);

			scene->addActor(*body);

			physx::PxBoxGeometry box(20, 20, 20);
			physx::PxShape* shape2 = physics->createShape(box, *material, true);
			if (!shape2)
			{
				throw std::exception("unable to create shape");
			}

			physx::PxRigidStatic* body2 = physics->createRigidStatic(physx::PxTransform(0, -10, 0));
			body2->attachShape(*shape2);
			scene->addActor(*body2);

			if (!body)
			{
				throw std::exception("unable to create body");
			}

			for (;;)
			{
				std::cout << body->getGlobalPose().p.y << std::endl;
				scene->simulate(1.0 / 60);
				scene->fetchResults(true);

				//Sleep(100);
			}
		}
	};
}