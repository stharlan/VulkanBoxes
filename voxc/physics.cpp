
#include "voxc.h"

using namespace physx;

void initPhysics(VOXC_WINDOW_CONTEXT* lpctx, glm::vec3 startingPosition)
{

    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;

    lpctx->mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
        gDefaultErrorCallback);
    if (!lpctx->mFoundation)
        throw new std::runtime_error("failed to create physx foundation");

    bool recordMemoryAllocations = true;

    lpctx->mPvd = PxCreatePvd(*lpctx->mFoundation);
    if (!lpctx->mPvd)
        throw new std::runtime_error("failed to create physx pvd");

    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    if (!transport) throw new std::runtime_error("failed to create physx transport");

    //if (false == lpctx->mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL))
        //throw new std::runtime_error("failed to connect physx pvd");

    lpctx->mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *lpctx->mFoundation,
        PxTolerancesScale(), recordMemoryAllocations, lpctx->mPvd);
    if (!lpctx->mPhysics)
        throw new std::runtime_error("failed to create physx physics");

    if (false == PxInitExtensions(*lpctx->mPhysics, lpctx->mPvd))
        throw new std::runtime_error("failed to init physx extensions");

    PxSceneDesc sceneDesc(lpctx->mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, 0.0f, -9.81f);
    lpctx->mDispatcher = PxDefaultCpuDispatcherCreate(2);
    if (!lpctx->mDispatcher)
        throw new std::runtime_error("failed to create physx default cpu dispatcher");

    sceneDesc.cpuDispatcher = lpctx->mDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    lpctx->mScene = lpctx->mPhysics->createScene(sceneDesc);
    if (!lpctx->mScene)
        throw new std::runtime_error("failed to create physx scene");

    // debugger
    if (!lpctx->mScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f))
        throw new std::runtime_error("failed to set physx scene vis parameter scale");

    if(!lpctx->mScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f))
        throw new std::runtime_error("failed to set physx scene vis parameter actor axes");

    //PxPvdSceneClient* pvdClient = lpctx->mScene->getScenePvdClient();
    //if (pvdClient)
    //{
    //    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
    //    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
    //    pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    //}
    //else {
    //    throw new std::runtime_error("failed to get scene pvd client");
    //}

    lpctx->pMaterial = lpctx->mPhysics->createMaterial(0.5f, 0.5f, 0.0f);
    if (!lpctx->pMaterial)
        throw new std::runtime_error("failed to create material");

    lpctx->mManager = PxCreateControllerManager(*lpctx->mScene);
    if (!lpctx->mManager)
        throw new std::runtime_error("failed to create physx controller manager");

    PxCapsuleControllerDesc cDesc;
    cDesc.material = lpctx->pMaterial;
    cDesc.position = physx::PxExtendedVec3(startingPosition.x, startingPosition.y, startingPosition.z);
    cDesc.height = 0.55f;
    cDesc.radius = 0.4f;
    cDesc.slopeLimit = 0.0f;
    cDesc.contactOffset = 0.1f;
    cDesc.stepOffset = 0.02f;
    cDesc.reportCallback = NULL;
    cDesc.behaviorCallback = NULL;

    lpctx->mController = static_cast<PxCapsuleController*>(lpctx->mManager->createController(cDesc));
    lpctx->mController->setUpDirection(PxVec3(0, 0, 1));

    // remove controller shape from scene query for standup overlap test
    lpctx->mPlayerCapsuleActor = lpctx->mController->getActor();
    if (!lpctx->mPlayerCapsuleActor)
        throw new std::runtime_error("failed to get physx controller actor");

    lpctx->mBlockShape = lpctx->mPhysics->createShape(PxBoxGeometry(0.5f, 0.5f, 0.5f), *lpctx->pMaterial);
    if (!lpctx->mBlockShape)
        throw new std::runtime_error("failed to create physx shape");
    lpctx->mBlockShape->setRestOffset(0.1f);
    lpctx->mBlockShape->setContactOffset(0.2f);

    lpctx->xblock = (int64_t)startingPosition.x;
    lpctx->yblock = (int64_t)startingPosition.y;
    lpctx->zblock = (int64_t)startingPosition.z;

}

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
