
#include "voxc.h"

#define ON_TOP    0x01
#define PLUS_X    0x02
#define MINUS_X   0x04
#define PLUS_Y    0x08
#define MINUS_Y   0x10
#define ON_BOTTOM 0x20

void block_set_type(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z, int8_t type)
{
	lpctx->blockEntities[GRIDIDX(x, y, z)].type = type;
}

int8_t block_get_type(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z)
{
	return lpctx->blockEntities[GRIDIDX(x, y, z)].type;
}

int8_t block_get_type(VOXC_WINDOW_CONTEXT* lpctx, int64_t index)
{
	return lpctx->blockEntities[index].type;
}

void block_create_new_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t xc, int64_t yc, int64_t zc)
{
	lpctx->blockEntities[index].rigidStatic = lpctx->mPhysics->createRigidStatic(
		physx::PxTransform(xc + 0.5f, yc + 0.5f, zc + 0.5f));
	lpctx->blockEntities[index].rigidStatic->attachShape(*lpctx->mBlockShape);
}

physx::PxRigidStatic* block_get_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index)
{
	return lpctx->blockEntities[index].rigidStatic;
}

void block_release_all_actors(VOXC_WINDOW_CONTEXT* lpctx)
{
	std::vector<BLOCK_ENTITY>::iterator bi = lpctx->blockEntities.begin();
	for (; bi != lpctx->blockEntities.end(); ++bi)
	{
		if (bi->rigidStatic) {
			bi->rigidStatic->release();
			bi->rigidStatic = NULL;
		}
	}
}

void blocks_foreach(VOXC_WINDOW_CONTEXT* lpctx, CALLBACK_BLOCKS_FOREACH fn)
{
	std::vector<BLOCK_ENTITY>::iterator iter = lpctx->blockEntities.begin();
	for (; iter != lpctx->blockEntities.end(); ++iter)
	{
		fn(&*iter);
	}
}