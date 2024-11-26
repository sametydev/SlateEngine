#pragma once
#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>
#include <SlateEngine/Engine/Utils.h>


class ENGINE_API PhysicsFactory
{
public:
	PhysicsFactory();
	~PhysicsFactory();

	void Init();
	void Update(float dt = 0.0f);


	physx::PxPhysics* GetPXPhysics() { return mPhysics; };
	physx::PxScene* GetPXScene() { return mScene; };
	physx::PxFoundation* GetPXFoundation() { return mFoundation; };

private:
	physx::PxPvd* InitializeDebugger(physx::PxFoundation* foundation);


	physx::PxDefaultAllocator		mDefaultAllocatorCallback;
	physx::PxDefaultErrorCallback	mDefaultErrorCallback;
	physx::PxDefaultCpuDispatcher*	mDispatcher = nullptr;
	physx::PxTolerancesScale		mToleranceScale;
	physx::PxFoundation*			mFoundation = nullptr;
	physx::PxPhysics*				mPhysics = nullptr;
	physx::PxScene*					mScene = nullptr;
	physx::PxMaterial*				mMaterial = nullptr;
	physx::PxPvd*					mPvd = nullptr;
};

