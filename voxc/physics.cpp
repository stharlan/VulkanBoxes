
#include "voxc.h"

using namespace physx;

void initPhysics(VOXC_WINDOW_CONTEXT* lpctx)
{

    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;

    lpctx->mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
        gDefaultErrorCallback);
    if (!lpctx->mFoundation) return;

    bool recordMemoryAllocations = true;

    lpctx->mPvd = PxCreatePvd(*lpctx->mFoundation);
    if (!lpctx->mPvd) return;

    //PxPvdTransport* transport = PxDefaultPvdFileTransportCreate("c:\\temp\\data2.pvd");
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    lpctx->mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    lpctx->mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *lpctx->mFoundation,
        PxTolerancesScale(), recordMemoryAllocations, lpctx->mPvd);
    if (!lpctx->mPhysics) return;

    PxInitExtensions(*lpctx->mPhysics, lpctx->mPvd);

    PxSceneDesc sceneDesc(lpctx->mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, 0.0f, -9.81f);
    lpctx->mDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = lpctx->mDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    lpctx->mScene = lpctx->mPhysics->createScene(sceneDesc);

    // debugger
    lpctx->mScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    lpctx->mScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);

    PxPvdSceneClient* pvdClient = lpctx->mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    lpctx->pMaterial = lpctx->mPhysics->createMaterial(0.5f, 0.5f, 0.0f);

    // create player
    //lpctx->mPlayerCapsuleActor = lpctx->mPhysics->createRigidDynamic(PxTransform(ex, ey, ez));    
    //PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 1, 0)));    
    //lpctx->mPlayerCapsuleShape = PxRigidActorExt::createExclusiveShape(*lpctx->mPlayerCapsuleActor,
    //    PxCapsuleGeometry(0.5f, 1.0f), *pMaterial);
    //lpctx->mPlayerCapsuleShape->setLocalPose(relativePose);
    //lpctx->mPlayerCapsuleShape->setRestOffset(0.1f);
    //lpctx->mPlayerCapsuleShape->setContactOffset(0.2f);
    //PxRigidBodyExt::updateMassAndInertia(*lpctx->mPlayerCapsuleActor, 1.0f);
    //lpctx->mScene->addActor(*lpctx->mPlayerCapsuleActor);

    lpctx->mManager = PxCreateControllerManager(*lpctx->mScene);

    PxCapsuleControllerDesc cDesc;
    cDesc.material = lpctx->pMaterial;
    cDesc.position = physx::PxExtendedVec3(lpctx->ex, lpctx->ey, lpctx->ez);
    cDesc.height = 2.0f;
    cDesc.radius = 1.0f;
    cDesc.slopeLimit = 0.0f;
    cDesc.contactOffset = 0.1f;
    cDesc.stepOffset = 0.02f;
    cDesc.reportCallback = NULL;
    cDesc.behaviorCallback = NULL;

    lpctx->mController = static_cast<PxCapsuleController*>(lpctx->mManager->createController(cDesc));
    lpctx->mController->setUpDirection(PxVec3(0, 0, 1));

    // remove controller shape from scene query for standup overlap test
    lpctx->mPlayerCapsuleActor = lpctx->mController->getActor();

    lpctx->mBlockShape = lpctx->mPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *lpctx->pMaterial);
    lpctx->mBlockShape->setRestOffset(0.1f);
    lpctx->mBlockShape->setContactOffset(0.2f);

    lpctx->xblock = (int64_t)lpctx->ex;
    lpctx->yblock = (int64_t)lpctx->ey;
    lpctx->zblock = (int64_t)lpctx->ez;

}

//void addBlockRigidBody(float bx, float by, float bz)
//{
//    PxRigidStatic* block = lpctx->mPhysics->createRigidStatic(PxTransform(bx + 0.5f, by + 0.5f, bz + 0.5f));
//    block->attachShape(*lpctx->mBlockShape);
//    lpctx->mScene->addActor(*block);
//    lpctx->blocks.push_back(block);
//}

void cleanupPhysics(VOXC_WINDOW_CONTEXT* lpctx)
{
    lpctx->mPvd->disconnect();
    if (lpctx->mManager) lpctx->mManager->release();
    if (lpctx->mBlockShape) lpctx->mBlockShape->release();
    if (lpctx->pMaterial) lpctx->pMaterial->release();
    if (lpctx->mScene) lpctx->mScene->release();
    if (lpctx->mDispatcher) lpctx->mDispatcher->release();
    if (lpctx->mPhysics) lpctx->mPhysics->release();
    if (lpctx->mPvd) lpctx->mPvd->release();
    if (lpctx->mFoundation) lpctx->mFoundation->release();
}

//void initPhysics()
//{
//	lpctx->world = lpctx->physicsCommon.createPhysicsWorld();
//    reactphysics3d::Vector3 gravity(0.0f, 0.0f, -9.8f);
//    lpctx->world->setGravity(gravity);
//
//    reactphysics3d::Vector3 position(ex, ey, ez);
//    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
//    reactphysics3d::Transform transform(position, orientation);
//    lpctx->player = world->createRigidBody(transform);
//
//    float radius = 0.5f;
//    float height = 2.0f;
//    lpctx->playerShape = lpctx->physicsCommon.createCapsuleShape(radius, height);
//
//    // rotate 90 deg around x
//    reactphysics3d::Vector3 colliderRotation(3.14159f / 2.0f, 0.0f, 0.0f);
//    reactphysics3d::Vector3 colliderPosition(0, 0, 0);
//    reactphysics3d::Quaternion colliderOrientation =
//        reactphysics3d::Quaternion::fromEulerAngles(colliderRotation);
//    reactphysics3d::Transform colliderTransform(colliderPosition, colliderOrientation);
//    lpctx->playerCollider = lpctx->player->addCollider(lpctx->playerShape, colliderTransform);
//    reactphysics3d::Material& mat = lpctx->playerCollider->getMaterial();
//    mat.setBounciness(0.0f);
//    lpctx->playerCollider->setMaterial(mat);
//
//    reactphysics3d::Vector3 blockExtent(0.5f, 0.5f, 0.5f);
//    lpctx->blockShape = lpctx->physicsCommon.createBoxShape(blockExtent);
//}
//
//void addBlockRigidBody(float bx, float by, float bz)
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
//        blockBody->addCollider(lpctx->blockShape, colliderTransform);
//    reactphysics3d::Material& mat = pBlockCollider->getMaterial();
//    mat.setBounciness(0.0f);
//    pBlockCollider->setMaterial(mat);
//
//    lpctx->blocks.push_back(blockBody);
//}
//
//void cleanupPhysics()
//{
//    std::vector<reactphysics3d::RigidBody*>::iterator iter = lpctx->blocks.begin();
//    for (; iter != lpctx->blocks.end(); ++iter)
//    {
//        lpctx->world->destroyRigidBody(*iter);
//    }
//    lpctx->world->destroyRigidBody(lpctx->player);
//    lpctx->physicsCommon.destroyBoxShape(lpctx->blockShape);
//    lpctx->physicsCommon.destroyCapsuleShape(lpctx->playerShape);
//    lpctx->physicsCommon.destroyPhysicsWorld(lpctx->world);
//}