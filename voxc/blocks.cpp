
#include "voxc.h"

uint32_t block_compute_block_id(uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	uint32_t bx = x % 256;
	uint32_t by = y % 256;
	uint32_t bz = z % 256;
	return (g << 24) + (bx << 16) + (by << 8) + bz;
}

// incoming coord is a regular coord
// when stored, it is reduced to range 0-15 for block id
uint32_t block_compute_block16_id(uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	uint32_t bx = (x % 256) / 16;
	uint32_t by = (y % 256) / 16;
	uint32_t bz = (z % 256) / 16;
	return (g << 24) + (bx << 16) + (by << 8) + bz;
}

// this is wrong
glm::u32vec3 block_compute_position(uint32_t blockId)
{
	BYTE g = (blockId & 0xff000000) >> 24;
	BYTE x = (blockId & 0xff0000) >> 16;
	BYTE y = (blockId & 0xff00) >> 8;
	BYTE z = blockId & 0xff;
	uint32_t gx = g % 16;
	uint32_t gy = g / 16;
	return glm::u32vec3((gx * 256) + x, (gy * 256) + y, z);
}

glm::u32vec3 block_compute_position16(uint32_t block16Id)
{
	BYTE g = (block16Id & 0xff000000) >> 24;
	BYTE x = ((block16Id & 0xff0000) >> 16) * 16;
	BYTE y = ((block16Id & 0xff00) >> 8) * 16;
	BYTE z = (block16Id & 0xff) * 16;
	uint32_t gx = g % 16;
	uint32_t gy = g / 16;
	return glm::u32vec3((gx * 256) + x, (gy * 256) + y, z);
}

/*
Contrary to most existing answers here, note that there are actually 4 methods related to finding an element in a map(ignoring lower_bound, upper_bound and equal_range, which are less precise) :

	operator[] only exist in non - const version, as noted it will create the element if it does not exist
	at(), introduced in C++11, returns a reference to the element if it existsand throws an exception otherwise
	find() returns an iterator to the element if it exists or an iterator to map::end() if it does not
	count() returns the number of such elements, in a map, this is 0 or 1
	Now that the semantics are clear, let us review when to use which :

- if you only wish to know whether an element is present in the map(or not), then use count().
- if you wish to access the element, and it shall be in the map, then use at().
- if you wish to access the element, and do not know whether it is in the map or not, 
then use find(); do not forget to check that the resulting iterator is not equal to the result of end().
- finally, if you wish to access the element if it exists or create it(and access it) 
if it does not, use operator[]; if you do not wish to call the type default constructor 
to create it, then use either insert or emplace appropriately
*/

//void block_allocate(VOXC_WINDOW_CONTEXT* lpctx, uint64_t x, uint64_t y, uint64_t z)
//{
	// no need to allocate mem
	// this is a vector now
//}

//void block_allocate(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z)
//{
//	lpctx->lpBlockEntities = new BLOCK_ENTITY[x * y * z];
//	if (lpctx->lpBlockEntities == nullptr)
//		throw new std::runtime_error("failed to allocate memory for blocks");
//	memset(lpctx->lpBlockEntities, 0, x * y * z * sizeof(BLOCK_ENTITY));
//	lpctx->numEntities = x * y * z;
//
//	//for (int64_t zc = 0; zc < Z_GRID_EXTENT; zc++) {
//	//	for (int64_t yc = 0; yc < Y_GRID_EXTENT; yc++) {
//	//		for (int64_t xc = 0; xc < X_GRID_EXTENT; xc++) {
//	//			int64_t index = GRIDIDX(xc, yc, zc);
//	//			lpctx->lpBlockEntities[index].gridLocation = glm::uvec3(xc, yc, zc);
//	//		}
//	//	}
//	//}
//}
//

//void block_cleanup(VOXC_WINDOW_CONTEXT* lpctx)
//{
	// no need to free mem
	// this is a vector now
//}

//void block_cleanup(VOXC_WINDOW_CONTEXT* lpctx)
//{
//	if(lpctx->lpBlockEntities)
//		delete[] lpctx->lpBlockEntities;
//}
//

void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z, uint8_t type)
{
	//=INT((INT(Y/256)*16)+(X/256))
	//int32_t g = ((y / 256) * 16) + (x / 256);
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	//uint32_t blockId = COMPUTE_BLOCK_ID(g, x % 256, y % 256, z % 256);
	if (x == 191 && y == 494 && z == 18)
	{
		printf("pause");
	}
	uint32_t blockId = block_compute_block_id(x, y, z);
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		b->second.regType = type;
	}
	else {
		// if not there, add it
		BLOCK_ENTITY newb;
		newb.flags = 0;
		newb.regType = type;
		newb.rigidStatic = nullptr;
		lpctx->blockVector.insert(BLOCK_MAP_ENTRY(blockId, newb));
	}
}

//void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z, int8_t type)
//{
//	lpctx->lpBlockEntities[GRIDIDX(x, y, z)].regType = type;
//}
//
//void block_set_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int8_t type)
//{
//	lpctx->lpBlockEntities[index].regType = type;
//}
//

uint8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	//uint32_t blockId = COMPUTE_BLOCK_ID(g, x % 256, y % 256, z % 256);
	uint32_t blockId = block_compute_block_id(x, y, z);
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		return b->second.regType;
	}
	return REG_AIR;
}

uint8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, uint32_t blockId)
{
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		return b->second.regType;
	}
	return REG_AIR;
}

//int8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t x, int64_t y, int64_t z, bool debug=false)
//{
//	if (debug == true)
//	{
//		BLOCK_ENTITY be = lpctx->lpBlockEntities[GRIDIDX(x, y, z)];
//		//printf("block_get_regtype: hash: %lli %lli %lli %lli\n", x, y, z, be.hashCode);
//		//printf("block_get_regtype: rigid: %lli %lli %lli %i\n", x, y, z, be.rigidStatic);		
//	}
//	return lpctx->lpBlockEntities[GRIDIDX(x, y, z)].regType;
//}
//
//int8_t block_get_regtype(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, bool debug=false)
//{
//	if (debug == true)
//	{
//		BLOCK_ENTITY be = lpctx->lpBlockEntities[index];
//		//printf("block_get_regtype: hash: %lli %lli\n", index, be.hashCode);
//		//printf("block_get_regtype: rigid: %lli %i\n", index, be.rigidStatic);
//	}
//	return lpctx->lpBlockEntities[index].regType;
//}
//

void block_create_new_actor(VOXC_WINDOW_CONTEXT* lpctx, 
	uint32_t x, uint32_t y, uint32_t z, 
	float xc, float yc, float zc)
{
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	//uint32_t blockId = COMPUTE_BLOCK_ID(g, x % 256, y % 256, z % 256);
	uint32_t blockId = block_compute_block_id(x, y, z);
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{		
		b->second.rigidStatic = lpctx->mPhysics->createRigidStatic(
			physx::PxTransform(xc + 0.5f, yc + 0.5f, zc + 0.5f));
		
		if (b->second.rigidStatic == nullptr)
			throw new std::runtime_error("failed to create rigid static");
		
		if (FALSE == b->second.rigidStatic->attachShape(*lpctx->mBlockShape))
			throw new std::runtime_error("failed to attach shape to rigid static");
		
		b->second.rigidStatic->userData = (void*)blockId;
	}
	else {
		throw new std::runtime_error("block doesn't exist");
	}
}

//void block_create_new_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t xc, int64_t yc, int64_t zc)
//{
//	lpctx->lpBlockEntities[index].rigidStatic = lpctx->mPhysics->createRigidStatic(
//		physx::PxTransform(xc + 0.5f, yc + 0.5f, zc + 0.5f));
//
//	if (lpctx->lpBlockEntities[index].rigidStatic == nullptr)
//		throw new std::runtime_error("failed to create rigid static");
//
//	if (FALSE == lpctx->lpBlockEntities[index].rigidStatic->attachShape(*lpctx->mBlockShape))
//		throw new std::runtime_error("failed to attach shape to rigid static");
//
//	lpctx->lpBlockEntities[index].rigidStatic->userData = (void*)&lpctx->lpBlockEntities[GRIDIDX(xc, yc, zc)];
//}
//

physx::PxRigidStatic* block_get_actor(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	//uint32_t blockId = COMPUTE_BLOCK_ID(g, x % 256, y % 256, z % 256);
	uint32_t blockId = block_compute_block_id(x, y, z);
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		return b->second.rigidStatic;
	}
	return nullptr;
}

//physx::PxRigidStatic* block_get_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index)
//{
//	return lpctx->lpBlockEntities[index].rigidStatic;
//}
//

void block_release_actor(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	//uint32_t blockId = COMPUTE_BLOCK_ID(g, x % 256, y % 256, z % 256);
	uint32_t blockId = block_compute_block_id(x, y, z);
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		if (b->second.rigidStatic) b->second.rigidStatic->release();
		b->second.rigidStatic = nullptr;
	}
}

//void block_release_actor(VOXC_WINDOW_CONTEXT* lpctx, int64_t index)
//{
//	if (lpctx->lpBlockEntities[index].rigidStatic)
//		lpctx->lpBlockEntities[index].rigidStatic->release();
//	lpctx->lpBlockEntities[index].rigidStatic = NULL;
//}
//

void block_release_all_actors(VOXC_WINDOW_CONTEXT* lpctx)
{
	std::unordered_map<uint32_t, BLOCK_ENTITY>::iterator i = lpctx->blockVector.begin();
	for (; i != lpctx->blockVector.end(); ++i)
	{
		if (i->second.rigidStatic) i->second.rigidStatic->release();
		i->second.rigidStatic = nullptr;
	}
}

//void block_release_all_actors(VOXC_WINDOW_CONTEXT* lpctx)
//{
//	for (int64_t i = 0; i < lpctx->numEntities; i++) {
//		if (lpctx->lpBlockEntities[i].rigidStatic)
//		{
//			lpctx->lpBlockEntities[i].rigidStatic->release();
//			lpctx->lpBlockEntities[i].rigidStatic = NULL;
//		}
//	}
//}
//

uint16_t block_get_flags(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	//uint32_t blockId = COMPUTE_BLOCK_ID(g, x % 256, y % 256, z % 256);
	//((g << 24) + (x << 16) + (y << 8) + z)
	//uint32_t blockId = (g << 24) + ((x % 256) << 16) + ((y % 256) << 8) + (z % 256);
	uint32_t blockId = block_compute_block_id(x, y, z);
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		return b->second.flags;
	}
	return 0;
}

uint16_t block_get_flags(VOXC_WINDOW_CONTEXT* lpctx, uint32_t blockId)
{
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		return b->second.flags;
	}
	return 0;
}

//uint64_t block_get_flags(VOXC_WINDOW_CONTEXT* lpctx, uint64_t index)
//{
//	return lpctx->lpBlockEntities[index].flags;
//}
//

void block_set_flags(VOXC_WINDOW_CONTEXT* lpctx, uint32_t x, uint32_t y, uint32_t z, uint16_t flags)
{
	uint32_t g = ((y >> 8) << 4) + (x >> 8);
	//uint32_t blockId = COMPUTE_BLOCK_ID(g, x % 256, y % 256, z % 256);
	uint32_t blockId = block_compute_block_id(x, y, z);
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		b->second.flags = flags;
	}
	else {
		throw new std::runtime_error("block can't be found");
	}
}

void block_set_flags(VOXC_WINDOW_CONTEXT* lpctx, uint32_t blockId, uint16_t flags)
{
	auto b = lpctx->blockVector.find(blockId);
	if (b != lpctx->blockVector.end())
	{
		b->second.flags = flags;
	}
	else {
		throw new std::runtime_error("block can't be found");
	}
}

//void block_set_flags(VOXC_WINDOW_CONTEXT* lpctx, uint64_t index, uint16_t value)
//{
//	lpctx->lpBlockEntities[index].flags = value;
//}
//
////void block_set_hash_code(VOXC_WINDOW_CONTEXT* lpctx, int64_t index, int64_t hashCode)
////{
////	lpctx->lpBlockEntities[index].hashCode = hashCode;
////}
