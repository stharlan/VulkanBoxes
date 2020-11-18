
#include "Common.h"

void HelloTriangleApplication::initPhysics()
{

	this->world = this->physicsCommon.createPhysicsWorld();
    reactphysics3d::Vector3 gravity(0.0f, 0.0f, -9.8f);
    this->world->setGravity(gravity);

    reactphysics3d::Vector3 position(ex, ey, ez);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform(position, orientation);
    this->player = world->createRigidBody(transform);

    float radius = 2.0f;
    this->playerShape = this->physicsCommon.createSphereShape(radius);
    reactphysics3d::Transform colliderTransform = reactphysics3d::Transform::identity();
    this->playerCollider = this->player->addCollider(this->playerShape, colliderTransform);

    reactphysics3d::Vector3 blockExtent(0.5f, 0.5f, 0.5f);
    this->blockShape = this->physicsCommon.createBoxShape(blockExtent);
}

void HelloTriangleApplication::addBlockRigidBody(float bx, float by, float bz)
{
    // Initial position and orientation of the collision body 
    reactphysics3d::Vector3 position(bx + 0.5f, by + 0.5f, bz + 0.5f);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform(position, orientation);

    // Create a collision body in the world 
    reactphysics3d::RigidBody* blockBody = world->createRigidBody(transform);
    blockBody->setType(reactphysics3d::BodyType::STATIC);

    reactphysics3d::Transform colliderTransform = reactphysics3d::Transform::identity();
    reactphysics3d::Collider* pBlockCollider = 
        blockBody->addCollider(this->blockShape, colliderTransform);

    this->blocks.push_back(blockBody);
}

void HelloTriangleApplication::cleanupPhysics()
{
    std::vector<reactphysics3d::RigidBody*>::iterator iter = this->blocks.begin();
    for (; iter != this->blocks.end(); ++iter)
    {
        this->world->destroyRigidBody(*iter);
    }
    this->world->destroyRigidBody(this->player);
    this->physicsCommon.destroyBoxShape(this->blockShape);
    this->physicsCommon.destroySphereShape(this->playerShape);
    this->physicsCommon.destroyPhysicsWorld(this->world);
}