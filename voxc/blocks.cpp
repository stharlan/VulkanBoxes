
#include "voxc.h"

void block_allocate(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z)
{
	lpctx->lpBlockEntities = new BLOCK_ENTITY[x * y * z];
	if (lpctx->lpBlockEntities == nullptr)
		throw new std::runtime_error("failed to allocate memory for blocks");
	memset(lpctx->lpBlockEntities, 0, x * y * z * sizeof(BLOCK_ENTITY));
	lpctx->numEntities = x * y * z;

	for (int64_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
		for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
			for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {
				int64_t index = GRIDIDX(xc, yc, zc);
				//lpctx->lpBlockEntities[index].gridLocation = glm::uvec3(xc, yc, zc);
			}
		}
	}
}

void block_cleanup(VOXC_WINDOW_CONTEXT* lpctx)
{
	if(lpctx->lpBlockEntities)
		delete[] lpctx->lpBlockEntities;
}

void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z, int8_t type)
{
	lpctx->lpBlockEntities[GRIDIDX(x, y, z)].regType = type;
}

void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int8_t type)
{
	lpctx->lpBlockEntities[index].regType = type;
}

int8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z, bool debug=false)
{
	if (debug == true)
	{
		BLOCK_ENTITY be = lpctx->lpBlockEntities[GRIDIDX(x, y, z)];
		//printf("block_get_regtype: hash: %lli %lli %lli %lli\n", x, y, z, be.hashCode);
		//printf("block_get_regtype: rigid: %lli %lli %lli %i\n", x, y, z, be.rigidStatic);		
	}
	return lpctx->lpBlockEntities[GRIDIDX(x, y, z)].regType;
}

int8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, bool debug=false)
{
	if (debug == true)
	{
		BLOCK_ENTITY be = lpctx->lpBlockEntities[index];
		//printf("block_get_regtype: hash: %lli %lli\n", index, be.hashCode);
		//printf("block_get_regtype: rigid: %lli %i\n", index, be.rigidStatic);
	}
	return lpctx->lpBlockEntities[index].regType;
}

void block_create_new_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t xc, int64_t yc, int64_t zc)
{
	lpctx->lpBlockEntities[index].rigidStatic = lpctx->mPhysics->createRigidStatic(
		physx::PxTransform(xc + 0.5f, yc + 0.5f, zc + 0.5f));

	if (lpctx->lpBlockEntities[index].rigidStatic == nullptr)
		throw new std::runtime_error("failed to create rigid static");

	if (FALSE == lpctx->lpBlockEntities[index].rigidStatic->attachShape(*lpctx->mBlockShape))
		throw new std::runtime_error("failed to attach shape to rigid static");

	lpctx->lpBlockEntities[index].rigidStatic->userData = (void*)&lpctx->lpBlockEntities[GRIDIDX(xc, yc, zc)];
}

physx::PxRigidStatic* block_get_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index)
{
	return lpctx->lpBlockEntities[index].rigidStatic;
}

void block_release_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index)
{
	if (lpctx->lpBlockEntities[index].rigidStatic)
		lpctx->lpBlockEntities[index].rigidStatic->release();
	lpctx->lpBlockEntities[index].rigidStatic = NULL;
}

void block_release_all_actors(VOXC_WINDOW_CONTEXT* lpctx)
{
	for (int64_t i = 0; i < lpctx->numEntities; i++) {
		if (lpctx->lpBlockEntities[i].rigidStatic)
		{
			lpctx->lpBlockEntities[i].rigidStatic->release();
			lpctx->lpBlockEntities[i].rigidStatic = NULL;
		}
	}
}

uint64_t block_get_flags(VOXC_WINDOW_CONTEXT* lpctx, uint64_t index)
{
	return lpctx->lpBlockEntities[index].flags;
}

void block_set_flags(VOXC_WINDOW_CONTEXT* lpctx, uint64_t index, uint64_t value)
{
	lpctx->lpBlockEntities[index].flags = value;
}

//void block_set_hash_code(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t hashCode)
//{
//	lpctx->lpBlockEntities[index].hashCode = hashCode;
//}
