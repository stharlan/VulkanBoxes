
#include "voxc.h"

//extern const GLuint topVertexIndices[];
//extern const GLuint plusxVertexIndices[];
//extern const GLuint minusxVertexIndices[];
//extern const GLuint plusyVertexIndices[];
//extern const GLuint minusyVertexIndices[];
//extern const GLuint bottomVertexIndices[];
//
//extern const glm::vec4 locs[];
//extern const glm::vec2 texcrds[];
//extern const glm::vec3 normals[];

//void block_add_block_geometry(VOXC_WINDOW_CONTEXT* lpctx, int64_t regType, int64_t x, int64_t y, int64_t z)
//{
//	glm::mat4 xlate = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
//	int64_t idx = GRIDIDX(x, y, z);
//	for (int i = 0; i < 4; i++) {
//		if (blockRegistry[i].regType == regType) {
//
//			uint64_t facesAdded = 0;
//
//			if ((lpctx->blockEntities[idx].surround & SURR_ON_TOP) == 0)
//			{
//				int64_t groupId = blockRegistry[i].textureIndex[TEXTURE_INDEX_TOP];
//				for (int64_t v = 0; v < 6; v++) {
//					lpctx->groups[groupId].vertices.push_back({
//						glm::vec3(xlate * locs[topVertexIndices[v]]),
//						texcrds[topVertexIndices[v]],
//						normals[topVertexIndices[v]]
//					});
//					facesAdded++;
//				}
//			}
//			if ((lpctx->blockEntities[idx].surround & SURR_ON_BOTTOM) == 0)
//			{
//				int64_t groupId = blockRegistry[i].textureIndex[TEXTURE_INDEX_BOTTOM];
//				for (int64_t v = 0; v < 6; v++) {
//					lpctx->groups[groupId].vertices.push_back({
//						glm::vec3(xlate * locs[bottomVertexIndices[v]]),
//						texcrds[bottomVertexIndices[v]],
//						normals[bottomVertexIndices[v]]
//					});
//					facesAdded++;
//				}
//			}
//
//			return;
//		}
//	}
//}

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
	for (; iter != lpctx->blockEntities.end(); ++iter) fn(&*iter);
}

uint8_t block_get_surround_mask(VOXC_WINDOW_CONTEXT* lpctx, int64_t index)
{
	return lpctx->blockEntities[index].surround;
}