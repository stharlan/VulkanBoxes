
#include "voxc.h"

// 4 MB
//#define CHUNK_SIZE_BYTES 4194304ll
// 8 MB
//#define CHUNK_SIZE_BYTES 8388608ll
// 16 MB BUG!
//#define CHUNK_SIZE_BYTES 16777216ll
//#define CHUNK_SIZE_BYTES 16777188ll
// 32 MB
#define CHUNK_SIZE_BYTES 33554432ll
// 64 MB
//#define CHUNK_SIZE_BYTES 67108864ll
// 128 MB
//#define CHUNK_SIZE_BYTES 134217728ll


//int vmm_add_vertex(VOXC_WINDOW_CONTEXT* lpctx, int64_t textureConst, VERTEX3* vertex)
//{
//	uint32_t numBuffers = lpctx->vertex_buffers.size();
//	for (int i = 0; i < numBuffers; i++)
//	{
//		std::vector<VERTEX_BUFFER_CHUNK>::iterator vbchnk = std::find_if(
//			lpctx->vertex_buffers[i].chunks.begin(),
//			lpctx->vertex_buffers[i].chunks.end(),
//			[textureConst](const VERTEX_BUFFER_CHUNK& vbufch) {
//				return (vbufch.texture_const == textureConst
//					&& vbufch.bfree >= sizeof(VERTEX3));
//			}
//		);
//		if (vbchnk != lpctx->vertex_buffers[i].chunks.end())
//		{
//			// found a chunk with free memory
//			memcpy(
//				lpctx->vertex_buffers[i].mem + vbchnk->boffset + (vbchnk->blength - vbchnk->bfree),
//				vertex, 
//				sizeof(VERTEX3));
//			vbchnk->bfree -= sizeof(VERTEX3);
//			vbchnk->num_vertices++;
//			return 0;
//		}
//	}
//	return 1;
//}
//
//void vmm_allocate_buffer(VOXC_WINDOW_CONTEXT* lpctx, std::vector<GLuint> textureIds)
//{
//
//	VERTEX_BUFFER vbuf;
//
//	vbuf.mem = (BYTE*)malloc(sizeof(BYTE) * textureIds.size() * CHUNK_SIZE_BYTES);
//	if (vbuf.mem == nullptr) throw new std::runtime_error("failed to allocate memory for vertices");
//
//	vbuf.mem_size = sizeof(BYTE) * textureIds.size() * CHUNK_SIZE_BYTES;
//
//	memset(vbuf.mem, 0, sizeof(BYTE) * textureIds.size() * CHUNK_SIZE_BYTES);
//
//	printf("allocated %lli bytes for vertex buffers\n", sizeof(BYTE) * textureIds.size() * CHUNK_SIZE_BYTES);
//
//	glCreateBuffers(1, &vbuf.vbo);
//	HANDLE_GL_ERROR();
//
//	for (int32_t i = 0; i < textureIds.size(); i++)
//	{
//		VERTEX_BUFFER_CHUNK chunk;
//		chunk.bfree = CHUNK_SIZE_BYTES;
//		chunk.blength = CHUNK_SIZE_BYTES;
//		chunk.boffset = i * CHUNK_SIZE_BYTES;
//		chunk.num_vertices = 0;
//		chunk.texture_id = textureIds[i];
//		chunk.texture_const = i;
//		vbuf.chunks.push_back(chunk);
//	}
//
//	lpctx->vertex_buffers.push_back(vbuf);
//}
//
//void vmm_allocate_single_buffer(VOXC_WINDOW_CONTEXT* lpctx, GLuint textureId, int64_t texture_const)
//{
//
//	VERTEX_BUFFER vbuf;
//
//	vbuf.mem = (BYTE*)malloc(sizeof(BYTE) * CHUNK_SIZE_BYTES);
//	if (vbuf.mem == nullptr) throw new std::runtime_error("failed to allocate memory for vertices");
//
//	vbuf.mem_size = sizeof(BYTE) * CHUNK_SIZE_BYTES;
//
//	memset(vbuf.mem, 0, sizeof(BYTE) * CHUNK_SIZE_BYTES);
//
//	printf("allocated %lli bytes for vertex buffers\n", sizeof(BYTE) * CHUNK_SIZE_BYTES);
//
//	glCreateBuffers(1, &vbuf.vbo);
//	HANDLE_GL_ERROR();
//
//	VERTEX_BUFFER_CHUNK chunk;
//	chunk.bfree = CHUNK_SIZE_BYTES;
//	chunk.blength = CHUNK_SIZE_BYTES;
//	chunk.boffset = 0;
//	chunk.num_vertices = 0;
//	chunk.texture_id = textureId;
//	chunk.texture_const = texture_const;
//	vbuf.chunks.push_back(chunk);
//
//	lpctx->vertex_buffers.push_back(vbuf);
//}