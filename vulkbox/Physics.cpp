
#include "Common.h"

using namespace physx;

void HelloTriangleApplication::initPhysics()
{
    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;

    this->mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
        gDefaultErrorCallback);
    if (!this->mFoundation) return;

    bool recordMemoryAllocations = true;

    this->mPvd = PxCreatePvd(*this->mFoundation);
    if (!this->mPvd) return;

    //PxPvdTransport* transport = PxDefaultPvdFileTransportCreate("c:\\temp\\data2.pvd");
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
        PxTolerancesScale(), recordMemoryAllocations, mPvd);
    if (!mPhysics) return;

    PxInitExtensions(*this->mPhysics, this->mPvd);

    PxSceneDesc sceneDesc(this->mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, 0.0f, -9.81f);
    this->mDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = this->mDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    this->mScene = this->mPhysics->createScene(sceneDesc);

    // debugger
    this->mScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    this->mScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);

    PxPvdSceneClient* pvdClient = this->mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    pMaterial = this->mPhysics->createMaterial(0.5f, 0.5f, 0.0f);

    // create player
    //this->mPlayerCapsuleActor = this->mPhysics->createRigidDynamic(PxTransform(ex, ey, ez));    
    //PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 1, 0)));    
    //this->mPlayerCapsuleShape = PxRigidActorExt::createExclusiveShape(*this->mPlayerCapsuleActor,
    //    PxCapsuleGeometry(0.5f, 1.0f), *pMaterial);
    //this->mPlayerCapsuleShape->setLocalPose(relativePose);
    //this->mPlayerCapsuleShape->setRestOffset(0.1f);
    //this->mPlayerCapsuleShape->setContactOffset(0.2f);
    //PxRigidBodyExt::updateMassAndInertia(*this->mPlayerCapsuleActor, 1.0f);
    //this->mScene->addActor(*this->mPlayerCapsuleActor);

    this->mManager = PxCreateControllerManager(*this->mScene);

    PxCapsuleControllerDesc cDesc;
    cDesc.material = pMaterial;
    cDesc.position = physx::PxExtendedVec3(ex, ey, ez);
    cDesc.height = 2.0f;
    cDesc.radius = 1.0f;
    cDesc.slopeLimit = 0.0f;
    cDesc.contactOffset = 0.1f;
    cDesc.stepOffset = 0.02f;
    cDesc.reportCallback = NULL;
    cDesc.behaviorCallback = NULL;

    this->mController = static_cast<PxCapsuleController*>(this->mManager->createController(cDesc));
    this->mController->setUpDirection(PxVec3(0, 0, 1));

    // remove controller shape from scene query for standup overlap test
    this->mPlayerCapsuleActor = this->mController->getActor();

    this->mBlockShape = this->mPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *pMaterial);
    this->mBlockShape->setRestOffset(0.1f);
    this->mBlockShape->setContactOffset(0.2f);

}

void HelloTriangleApplication::addBlockRigidBody(float bx, float by, float bz)
{
    PxRigidStatic* block = this->mPhysics->createRigidStatic(PxTransform(bx + 0.5f, by + 0.5f, bz + 0.5f));
    block->attachShape(*this->mBlockShape);
    this->mScene->addActor(*block);
    this->blocks.push_back(block);
}

void HelloTriangleApplication::cleanupPhysics()
{
    mPvd->disconnect();
    std::vector<physx::PxRigidStatic*>::iterator iter = blocks.begin();
    for (; iter != blocks.end(); ++iter)
    {
        (*iter)->release();
    }
    if (this->mBlockShape) this->mBlockShape->release();
    if (this->mManager) this->mManager->release();
    //if (this->mPlayerCapsuleShape) this->mPlayerCapsuleShape->release();
    //if (this->mPlayerCapsuleActor) this->mPlayerCapsuleActor->release();
    if (this->pMaterial) pMaterial->release();
    if (this->mScene) this->mScene->release();
    if (this->mDispatcher) this->mDispatcher->release();
    if (this->mPhysics) this->mPhysics->release();
    if (this->mPvd) this->mPvd->release();
    if (this->mFoundation) this->mFoundation->release();
}

//void HelloTriangleApplication::initPhysics()
//{
//	this->world = this->physicsCommon.createPhysicsWorld();
//    reactphysics3d::Vector3 gravity(0.0f, 0.0f, -9.8f);
//    this->world->setGravity(gravity);
//
//    reactphysics3d::Vector3 position(ex, ey, ez);
//    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
//    reactphysics3d::Transform transform(position, orientation);
//    this->player = world->createRigidBody(transform);
//
//    float radius = 0.5f;
//    float height = 2.0f;
//    this->playerShape = this->physicsCommon.createCapsuleShape(radius, height);
//
//    // rotate 90 deg around x
//    reactphysics3d::Vector3 colliderRotation(3.14159f / 2.0f, 0.0f, 0.0f);
//    reactphysics3d::Vector3 colliderPosition(0, 0, 0);
//    reactphysics3d::Quaternion colliderOrientation =
//        reactphysics3d::Quaternion::fromEulerAngles(colliderRotation);
//    reactphysics3d::Transform colliderTransform(colliderPosition, colliderOrientation);
//    this->playerCollider = this->player->addCollider(this->playerShape, colliderTransform);
//    reactphysics3d::Material& mat = this->playerCollider->getMaterial();
//    mat.setBounciness(0.0f);
//    this->playerCollider->setMaterial(mat);
//
//    reactphysics3d::Vector3 blockExtent(0.5f, 0.5f, 0.5f);
//    this->blockShape = this->physicsCommon.createBoxShape(blockExtent);
//}
//
//void HelloTriangleApplication::addBlockRigidBody(float bx, float by, float bz)
//{
//    // Initial position and orientation of the collision body 
//    reactphysics3d::Vector3 position(bx + 0.5f, by + 0.5f, bz + 0.5f);
//    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
//    reactphysics3d::Transform transform(position, orientation);
//
//    // Create a collision body in the world 
//    reactphysics3d::RigidBody* blockBody = world->createRigidBody(transform);
//    blockBody->setType(reactphysics3d::BodyType::STATIC);
//
//    reactphysics3d::Transform colliderTransform = reactphysics3d::Transform::identity();
//    reactphysics3d::Collider* pBlockCollider = 
//        blockBody->addCollider(this->blockShape, colliderTransform);
//    reactphysics3d::Material& mat = pBlockCollider->getMaterial();
//    mat.setBounciness(0.0f);
//    pBlockCollider->setMaterial(mat);
//
//    this->blocks.push_back(blockBody);
//}
//
//void HelloTriangleApplication::cleanupPhysics()
//{
//    std::vector<reactphysics3d::RigidBody*>::iterator iter = this->blocks.begin();
//    for (; iter != this->blocks.end(); ++iter)
//    {
//        this->world->destroyRigidBody(*iter);
//    }
//    this->world->destroyRigidBody(this->player);
//    this->physicsCommon.destroyBoxShape(this->blockShape);
//    this->physicsCommon.destroyCapsuleShape(this->playerShape);
//    this->physicsCommon.destroyPhysicsWorld(this->world);
//}