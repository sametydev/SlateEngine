#include "PhysicsFactory.h"
#include <SlateEngine/Engine/DXConfig.h>

PhysicsFactory::PhysicsFactory()
{
}

PhysicsFactory::~PhysicsFactory()
{
	
}

void PhysicsFactory::Init()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
	__CHECK(mFoundation, "PxCreateFoundation failed!");

	mPvd = InitializeDebugger(mFoundation);

	//abs(100 * -9.81f) == gravity * 1s
	mToleranceScale.speed = 981;

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());

	sceneDesc.gravity = physx::PxVec3(0.0f, GRAVITY_SCALE, 0.0f);
	mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	mScene = mPhysics->createScene(sceneDesc);
}

void PhysicsFactory::Update(float dt)
{
}

physx::PxPvd* PhysicsFactory::InitializeDebugger(physx::PxFoundation* foundation)
{
	physx::PxPvd* _pvd = nullptr;
	_pvd = PxCreatePvd(*foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PHYSX_VD_DEFAULT_HOST,
		PHYSX_VD_DEFAULT_PORT, 10);

	_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
	return _pvd;
}
