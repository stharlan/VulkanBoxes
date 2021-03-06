#include "voxc.h"

// ISSUE: when jump and hit a ceiling, set upward velocity to zero
// ISSUE: jump and hit ceiling, head goes out of map
// ISSUE: select block ray is too low, raise it up

const VERTEX4 quadVerts[] = {
    { { 0.5, 0.5, 0 },{ 0, 0 }, {0,0,0} },
    { { 1, 0.5, 0 },{ 1, 0 }, {0,0,0} },
    { { 1, 1, 0 },{ 1, 1 }, {0,0,0} },
    { { 0.5, 0.5, 0 },{ 0, 0 }, {0,0,0} },
    { { 1, 1, 0 },{ 1, 1 }, {0,0,0} },
    { { 0.5, 1, 0 },{ 0, 1 }, {0,0,0} }
};

void addActorsForCurrentLocation(VOXC_WINDOW_CONTEXT* lpctx, int64_t xint, int64_t yint, int64_t zint)
{

    int64_t idx = 0;

    // create all the rigid static actors
    // as an array in the crate vertex buffer algo
    // then, just pull them out of the array in this method

    // remove current actors
    if (lpctx->blocksAroundMe.size() > (size_t)0) {
        lpctx->mScene->removeActors(lpctx->blocksAroundMe.data(), (unsigned int)lpctx->blocksAroundMe.size());
    }

    // clear array
    lpctx->blocksAroundMe.clear();

    int64_t radius = 10;

    for (int64_t xbl = xint - radius; xbl < xint + (radius + 1); xbl++)
    {
        if (xbl >= 0 && xbl < X_GRID_EXTENT)
        {
            for (int64_t ybl = yint - radius; ybl < yint + (radius + 1); ybl++)
            {
                if (ybl >= 0 && ybl < Y_GRID_EXTENT)
                {
                    for (int64_t zbl = zint - radius; zbl < zint + (radius + 1); zbl++)
                    {
                        if (zbl >= 0 && zbl < Z_GRID_EXTENT)
                        {
                            //idx = GRIDIDX(xbl, ybl, zbl);
                            //int64_t blockType = block_get_regtype(lpctx, idx, false);
                            uint8_t regType = block_get_regtype(lpctx, xbl, ybl, zbl);
                            if (regType == REG_DIRT || regType == REG_DIRTGRASS)
                            {
                                //physx::PxRigidStatic* lpActor = block_get_actor(lpctx, idx);
                                physx::PxRigidStatic* lpActor = block_get_actor(lpctx, xbl, ybl, zbl);
                                if (lpActor != NULL)
                                {
                                    lpctx->blocksAroundMe.push_back(lpActor);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    printf("adding %lli actors\n", lpctx->blocksAroundMe.size());
    lpctx->mScene->addActors(lpctx->blocksAroundMe.data(), (unsigned int)lpctx->blocksAroundMe.size());

    lpctx->xblock = xint;
    lpctx->yblock = yint;
    lpctx->zblock = zint;
}

void createRenderingContext2(HDC hdc, VOXC_WINDOW_CONTEXT* lpctx)
{
    int attrs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLE_BUFFERS_ARB, 1, // Number of buffers (must be 1 at time of writing)
        WGL_SAMPLES_ARB, 8,        // Number of samples
        0
    };

    int pf = 0;
    UINT numfmts = 0;
    if (FALSE == wglChoosePixelFormatARB(hdc, attrs, 0, 1, &pf, &numfmts))
        throw new std::runtime_error("failed to choose pixel format");

    PIXELFORMATDESCRIPTOR pfd = { 0 };
    if (!DescribePixelFormat(hdc, pf, sizeof(pfd), &pfd))
        throw new std::runtime_error("failed to describe pixel format");

    if (FALSE == SetPixelFormat(hdc, pf, &pfd))
        throw new std::runtime_error("failed to set pixel format");


    int glverattribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        0
    };

    lpctx->hglrc = wglCreateContextAttribsARB(hdc, 0, glverattribs);
    if (lpctx->hglrc == NULL) {
        throw new std::runtime_error("failed to create gl context");
    }
    else {
        printf("Rendering context...ok.\n");
    }

    lpctx->hglrcAlt = wglCreateContextAttribsARB(hdc, 0, glverattribs);
    if (lpctx->hglrcAlt == NULL)
        throw new std::runtime_error("failed to create gl alt context");

    BOOL success = wglShareLists(lpctx->hglrc, lpctx->hglrcAlt);
    if (!success)
        throw new std::runtime_error("failed to share context lists");

    if (FALSE == wglMakeCurrent(hdc, lpctx->hglrc))
        throw new std::runtime_error("failed to make gl context current");

}

//#define noiseWidth 64
//#define noiseHeight 64
//double noise[noiseHeight][noiseWidth]; //the noise array
//
//void generateNoise()
//{
//    for (int y = 0; y < noiseHeight; y++)
//        for (int x = 0; x < noiseWidth; x++)
//        {
//            noise[y][x] = (rand() % 32768) / 32768.0;
//        }
//}
//
//double smoothNoise(double x, double y)
//{
//    //get fractional part of x and y
//    double fractX = x - int(x);
//    double fractY = y - int(y);
//
//    //wrap around
//    int x1 = (int(x) + noiseWidth) % noiseWidth;
//    int y1 = (int(y) + noiseHeight) % noiseHeight;
//
//    //neighbor values
//    int x2 = (x1 + noiseWidth - 1) % noiseWidth;
//    int y2 = (y1 + noiseHeight - 1) % noiseHeight;
//
//    //smooth the noise with bilinear interpolation
//    double value = 0.0;
//    value += fractX * fractY * noise[y1][x1];
//    value += (1 - fractX) * fractY * noise[y1][x2];
//    value += fractX * (1 - fractY) * noise[y2][x1];
//    value += (1 - fractX) * (1 - fractY) * noise[y2][x2];
//
//    return value;
//}
//
//double turbulence(double x, double y, double size)
//{
//    double value = 0.0, initialSize = size;
//
//    while (size >= 1)
//    {
//        value += smoothNoise(x / size, y / size) * size;
//        size /= 2.0;
//    }
//
//    return(128.0 * value / initialSize);
//}
//

void load_textures_2(TEXTURE_SPEC tsArray[], int numFilenames, VOXC_WINDOW_CONTEXT* lpctx)
{
    std::vector<GLuint> texids(numFilenames);
    //lpctx->groups.resize(numFilenames);

    glGenTextures(numFilenames, texids.data());
    HANDLE_GL_ERROR();

    int texWidth, texHeight, texChannels;
    for (int i = 0; i < numFilenames; i++) {

        stbi_uc* pixels = stbi_load(tsArray[i].name, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        if (!pixels)
            throw new std::runtime_error("failed to load texture");

        glActiveTexture(GL_TEXTURE0);
        HANDLE_GL_ERROR();

        glBindTexture(GL_TEXTURE_2D, texids[i]);
        HANDLE_GL_ERROR();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        HANDLE_GL_ERROR();

        if (TRUE == tsArray[i].isTransparent) { // this is a transparent texture, no mipmaps
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            HANDLE_GL_ERROR();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            HANDLE_GL_ERROR();
        }
        else {
            glGenerateMipmap(GL_TEXTURE_2D);
            HANDLE_GL_ERROR();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            HANDLE_GL_ERROR();

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            HANDLE_GL_ERROR();
        }
        stbi_image_free(pixels);

        //lpctx->groups[i].tid = texids[i];
        //lpctx->groups[i].vertexBlocks.clear();
        //lpctx->groups[i].vbo = 0;
        //lpctx->groups[i].vertices.clear();
        //lpctx->groups[i].vsize = 0;

        lpctx->tex_const_id_map.insert(std::pair<int, int>(i, texids[i]));
    }
    
    //vmm_allocate_buffer(lpctx, texids);

}

void RenderText(VOXC_WINDOW_CONTEXT* lpctx, OpenGlProgram& prog, std::string text, float x, float y, float scale, glm::vec3 color,
    GLuint VAO, GLuint VBO, std::map<char, Character>& Characters)
{
    // activate corresponding render state	
    prog.Use();
    glm::mat4 fontProjection = glm::ortho(0.0f, (float)lpctx->screenWidth, 0.0f, (float)lpctx->screenHeight);
    prog.SetUniform3f("textColor", color.x, color.y, color.z);
    prog.SetUniformMatrix4fv("projection", &fontProjection[0][0]);

    glActiveTexture(GL_TEXTURE0);
    HANDLE_GL_ERROR();

    glBindVertexArray(VAO);
    HANDLE_GL_ERROR();

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 4 * sizeof(GLfloat));
    HANDLE_GL_ERROR();

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        HANDLE_GL_ERROR();
        // update content of VBO memory
        glNamedBufferSubData(VBO, 0, sizeof(vertices), vertices);
        HANDLE_GL_ERROR();
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        HANDLE_GL_ERROR();
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    HANDLE_GL_ERROR();

    glBindTexture(GL_TEXTURE_2D, 0);
    HANDLE_GL_ERROR();

    glUseProgram(0);
    HANDLE_GL_ERROR();
}

void RenderScene(VOXC_WINDOW_CONTEXT* lpctx, const glm::vec3& pos, const glm::vec3& look)
{

    glActiveTexture(GL_TEXTURE0);
    HANDLE_GL_ERROR();

    for (const auto& dv : lpctx->drawVector)
    {
        int64_t voffset = 0;
        glm::vec2 cen2pos = dv.centroid - glm::vec2(pos);
        float cen2pos_look_dotp = glm::dot(cen2pos, glm::vec2(look));
        float dist = glm::length(cen2pos);

        if (dist < 256.0f && cen2pos_look_dotp > -8.0f)
        {
            for (const auto& sv : dv.subverts)
            {
                glBindTexture(GL_TEXTURE_2D, sv.tex_id);

                glVertexArrayVertexBuffer(
                    lpctx->vao,
                    0,
                    dv.vbo,
                    voffset * sizeof(VERTEX4),
                    sizeof(VERTEX4));

                glDrawArrays(GL_TRIANGLES, 0, sv.num_vertices);

                voffset += sv.num_vertices;
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    HANDLE_GL_ERROR();


    //glActiveTexture(GL_TEXTURE0);
    //HANDLE_GL_ERROR();

    //for (const auto& vertex_buffer : lpctx->vertex_buffers)    
    //{
    //    for (const auto& chunk : vertex_buffer.chunks)
    //    {
    //        if (chunk.num_vertices > 0) {

    //            glBindTexture(GL_TEXTURE_2D, chunk.texture_id);
    //            HANDLE_GL_ERROR();

    //            glVertexArrayVertexBuffer(
    //                lpctx->vao,
    //                0,
    //                vertex_buffer.vbo,
    //                chunk.boffset,
    //                sizeof(VERTEX3));
    //            HANDLE_GL_ERROR();

    //            glDrawArrays(GL_TRIANGLES, 0, chunk.num_vertices);
    //            HANDLE_GL_ERROR();
    //        }
    //    }
    //}

    //glBindTexture(GL_TEXTURE_2D, 0);
    //HANDLE_GL_ERROR();

    //glActiveTexture(GL_TEXTURE0);
    //HANDLE_GL_ERROR();

    //for (const auto& group : lpctx->groups)
    //{
    //    if (group.vertexBlocks.size() > 0) {
    //        glBindTexture(GL_TEXTURE_2D, group.tid);
    //        HANDLE_GL_ERROR();
    //        for (const auto& vblk : group.vertexBlocks)
    //        {
    //            if (vblk.second.vsize > 0) {
    //                glVertexArrayVertexBuffer(lpctx->vao, 0, vblk.second.vbo, 0, 12 * sizeof(GLfloat));
    //                HANDLE_GL_ERROR();
    //                glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vblk.second.vsize);
    //                HANDLE_GL_ERROR();
    //            }
    //        }
    //    }
    //}

    //glBindTexture(GL_TEXTURE_2D, 0);
    //HANDLE_GL_ERROR();

    //for (; iter != lpctx->groups.end(); ++iter)
    //{
    //    if(iter->vsize > 0) 
    //    {
    //        glVertexArrayVertexBuffer(lpctx->vao, 0, iter->vbo, 0, 12 * sizeof(GLfloat));
    //        HANDLE_GL_ERROR();

    //        glBindTexture(GL_TEXTURE_2D, iter->tid);
    //        HANDLE_GL_ERROR();

    //        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)iter->vsize);
    //        HANDLE_GL_ERROR();
    //    }
    //}
    //glBindTexture(GL_TEXTURE_2D, 0);
    //HANDLE_GL_ERROR();
}

void setupFreeType(std::map<char, Character>& Characters, GLuint* pfontVAO, GLuint* pfontVBO)
{

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        throw new std::runtime_error("failed to init freetype");

    FT_Face face;
    if (FT_New_Face(ft, "FiraCode-Regular.ttf", 0, &face))
        throw new std::runtime_error("failed to load font");

    if (FT_Set_Pixel_Sizes(face, 0, 48))
        throw new std::runtime_error("failed to set pixel sizes");

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    HANDLE_GL_ERROR();

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            continue;
        }
        // generate texture
        GLuint texId;
        glGenTextures(1, &texId);
        HANDLE_GL_ERROR();

        glBindTexture(GL_TEXTURE_2D, texId);
        HANDLE_GL_ERROR();

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        HANDLE_GL_ERROR();

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        HANDLE_GL_ERROR();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        HANDLE_GL_ERROR();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        HANDLE_GL_ERROR();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        HANDLE_GL_ERROR();

        // now store character for later use
        Character character = {
            texId,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    if (FT_Done_Face(face))
        throw new std::runtime_error("failed to destroy font face");

    if (FT_Done_FreeType(ft))
        throw new std::runtime_error("failed to destroy freetype");

    glCreateVertexArrays(1, pfontVAO);
    HANDLE_GL_ERROR();

    glVertexArrayAttribBinding(*pfontVAO, 0, 0);
    HANDLE_GL_ERROR();

    glVertexArrayAttribFormat(*pfontVAO, 0, 4, GL_FLOAT, GL_FALSE, 0);
    HANDLE_GL_ERROR();

    glVertexArrayBindingDivisor(*pfontVAO, 0, 0);
    HANDLE_GL_ERROR();

    glEnableVertexArrayAttrib(*pfontVAO, 0);
    HANDLE_GL_ERROR();

    glCreateBuffers(1, pfontVBO);
    HANDLE_GL_ERROR();

    glNamedBufferStorage(*pfontVBO, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_STORAGE_BIT);
    HANDLE_GL_ERROR();
}

void LoadModel(std::vector<VBO_DATA>& vboData)
{

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "tree.model"))
        throw new std::runtime_error("failed to load obj file");

    // find model bounds
    float xmin = attrib.vertices[0], 
        xmax = attrib.vertices[0], 
        ymin = attrib.vertices[1], 
        ymax = attrib.vertices[1], 
        zmin = attrib.vertices[2], 
        zmax = attrib.vertices[2];

    size_t nverts = attrib.vertices.size() / 3;
    for (int vi = 0; vi < nverts; vi++)
    {
        if (attrib.vertices[vi * 3] < xmin) xmin = attrib.vertices[vi * 3];
        if (attrib.vertices[vi * 3] < xmax) xmax = attrib.vertices[vi * 3];
        if (attrib.vertices[(vi * 3) + 1] < ymin) ymin = attrib.vertices[(vi * 3) + 1];
        if (attrib.vertices[(vi * 3) + 1] < ymax) ymax = attrib.vertices[(vi * 3) + 1];
        if (attrib.vertices[(vi * 3) + 2] < zmin) zmin = attrib.vertices[(vi * 3) + 2];
        if (attrib.vertices[(vi * 3) + 2] < zmax) zmax = attrib.vertices[(vi * 3) + 2];
    }

    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(20, 20, -ymin + 1.0f));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), 3.14159f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 tr = trans * rot;

    for(int i=0; i<materials.size(); i++)
    {
        std::vector<VERTEX4> vertices;
        for (const auto& shape : shapes)
        {
            int64_t f = 0;
            for (const auto& index : shape.mesh.indices)
            {

                int64_t face = f / 3;
                int current_material_id = shape.mesh.material_ids[face];
                if (current_material_id == i) {

                    VERTEX4 v;
                    v.vertex = tr * glm::vec4(
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2],
                        1.0f
                    );
                    v.texc = glm::vec2(
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    );
                    v.norm = rot * glm::vec4(
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2],
                        1.0f
                    );
                    //v.userData[0] = 0;
                    //v.userData[1] = 0;
                    //v.blockId = glm::u8vec4(0, 0, 0, 0);
                    vertices.push_back(v);
                }
                f++;
            }
        }
        if (vertices.size() > 0) {
            VBO_DATA vboObject;

            glCreateBuffers(1, &vboObject.vboId);
            HANDLE_GL_ERROR();

            glNamedBufferStorage(vboObject.vboId, sizeof(VERTEX4) * vertices.size(), vertices.data(), 0);
            HANDLE_GL_ERROR();

            vboObject.numVerts = (GLsizei)vertices.size();
            vboObject.diffuseColor = glm::vec4(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2], 1.0f);

            vboData.push_back(vboObject);
        }
    }
}

void render_loop(VOXC_WINDOW_CONTEXT* lpctx, RENDER_LOOP_CONTEXT* rctx)
{
    std::future<void> isDoneProcessing;
    std::map<GLuint, GLuint> vbosToUpdate;
    LARGE_INTEGER perfCount = { 0 };
    LARGE_INTEGER perfFreq = { 0 };
    LARGE_INTEGER lastCount = { 0 };
    // performance monitoring
    int64_t elapsedTicks = 0;
    // 0.016f is 60 fps
    float elps[10] = {
        0.016f, 0.016f, 0.016f, 0.016f, 0.016f,
        0.016f, 0.016f, 0.016f, 0.016f, 0.016f
    };
    int64_t elpsCtr = 0;
    physx::PxControllerFilters mCCFilters = { 0 };
    physx::PxExtendedVec3 pos;
    char* textBuffer = (char*)malloc(256);

    if (false == QueryPerformanceCounter(&perfCount))
        throw new std::runtime_error("failed to query perf ctr");
    lastCount = perfCount;
    if (false == QueryPerformanceFrequency(&perfFreq)) // counts per second
        throw new std::runtime_error("failed to query perf freq");

    PROCESS_MEMORY_COUNTERS pmc = { 0 };

    // GO!
    while (TRUE)
    {
        // check if done
        DWORD wfsoResult = WaitForSingleObject(lpctx->hQuitEvent, 0);
        if (WAIT_FAILED == wfsoResult) throw new std::runtime_error("failed to wait for quit event");
        if (WAIT_OBJECT_0 == wfsoResult) break;

        // check to see if vertices need to be refreshed
        // this will switch out the VBOs for the 
        // block vertex arrays with new ones
        //if (isDoneProcessing.valid())
        //{
        //    if (std::future_status::ready == isDoneProcessing.wait_for(std::chrono::milliseconds(0)))
        //    {
        //        if (!vbosToUpdate.empty())
        //        {
        //            std::vector<VERTEX_BUFFER_GROUP1>::iterator giter = lpctx->groups.begin();
        //            for (; giter != lpctx->groups.end(); ++giter)
        //            {
        //                GLuint oldVbo = giter->vbo;
        //                GLuint newVbo = vbosToUpdate.at(oldVbo);
        //                giter->vbo = newVbo;
        //                giter->vsize = giter->vertices.size();
        //                glDeleteBuffers(1, &oldVbo);
        //            }
        //            vbosToUpdate.clear();
        //        }
        //    }
        //}

        // calculate elapsed seconds
        // use an average of the last ten values
        // to smooth out the elapsed time
        if (false == QueryPerformanceCounter(&perfCount))
            throw new std::runtime_error("failed to query perf counter");
        elapsedTicks = perfCount.QuadPart - lastCount.QuadPart;
        lastCount = perfCount;
        elps[elpsCtr] = (float)elapsedTicks / (float)perfFreq.QuadPart;
        elpsCtr++;
        if (elpsCtr > 9) elpsCtr = 0;
        float elapsed = (elps[0] + elps[1] + elps[2] + elps[3] + elps[4]
            + elps[5] + elps[6] + elps[7] + elps[8] + elps[9]) / 10.0f;

        // apply gravity to vz
        if (lpctx->keys[4] == 1)
        {
            lpctx->vz = 8.0f;
            lpctx->keys[4] = 0;
        }
        else {
            lpctx->vz += (float)elapsed * -15.0f; // accel due to gravity
        }

        // process walking/running
        float speed = elapsed * 10.0f;
        if (lpctx->keys[5] == 1) speed *= 2.0f;
        float mx = 0.0f, my = 0.0f;
        if (lpctx->keys[0] == 1) { // w
            mx += cosf(DEG2RAD(lpctx->azimuth)) * speed;
            my += sinf(DEG2RAD(lpctx->azimuth)) * speed;
        }
        else if (lpctx->keys[2] == 1) // s
        {
            mx += cosf(DEG2RAD(lpctx->azimuth)) * -speed;
            my += sinf(DEG2RAD(lpctx->azimuth)) * -speed;
        }
        if (lpctx->keys[1] == 1) // a
        {
            mx += sinf(DEG2RAD(lpctx->azimuth)) * -speed;
            my += cosf(DEG2RAD(lpctx->azimuth)) * speed;
        }
        else if (lpctx->keys[3] == 1) // d
        {
            mx += sinf(DEG2RAD(lpctx->azimuth)) * speed;
            my += cosf(DEG2RAD(lpctx->azimuth)) * -speed;
        }

        // move player
        physx::PxControllerCollisionFlags cflags = lpctx->mController->move(
            physx::PxVec3(mx, my, lpctx->vz * elapsed),
            0.00f, // min dist
            elapsed, mCCFilters);


        // if on ground, stop falling
        if (cflags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN))
        {
            lpctx->vz = 0.0f;
        }

        // simulate physics and get results
        lpctx->mScene->simulate(elapsed);
        if (false == lpctx->mScene->fetchResults(true))
            throw new std::runtime_error("failed to fetch physx results");
        pos = lpctx->mController->getPosition();

        // process elevation to radians
        float sinfElevation = sinf(DEG2RAD(lpctx->elevation));
        sinfElevation = FCLAMP(sinfElevation, -0.99f, 0.99f);
        float invSinfElevation = 1.0f - fabs(sinfElevation);

        // cast ray for selected cube
        bool hitStatus = FALSE;
        physx::PxTransform gp;
        BLOCK_ENTITY* hitBlock = NULL;
        bool checkAddActors = true;
        //{
        //    physx::PxVec3 origin = physx::PxVec3((float)pos.x, (float)pos.y, (float)pos.z);
        //    physx::PxVec3 unitDir = physx::PxVec3(
        //        cosf(DEG2RAD(lpctx->azimuth)) * invSinfElevation,
        //        (sinf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
        //        sinfElevation);
        //    unitDir.normalize();
        //    physx::PxRaycastBuffer hit;
        //    hitStatus = lpctx->mScene->raycast(origin + unitDir, unitDir, 5, hit);
        //    if (hitStatus) {

        //        gp = hit.block.actor->getGlobalPose();
        //        hitBlock = (BLOCK_ENTITY*)hit.block.actor->userData;

        //        // keys 6 left button destroys block
        //        // hitBlock is the block to update
        //        if (lpctx->keys[6] == 1)
        //        {
        //            lpctx->keys[6] = 0;
        //            if (vbosToUpdate.empty())
        //            {

        //                int64_t hashCode = hitBlock->hashCode;

        //                //clock_t t = clock();

        //                // remove faces
        //                for (int gi = 0; gi < lpctx->groups.size(); gi++)
        //                {
        //                    lpctx->groups[gi].vertices.erase(
        //                        std::remove_if(lpctx->groups[gi].vertices.begin(), lpctx->groups[gi].vertices.end(),
        //                            [hashCode](const VERTEX2& item) { return item.userData[0] == hashCode;  }),
        //                        lpctx->groups[gi].vertices.end());
        //                }

        //                int64_t hitBlockIndex = GRIDIDX(hitBlock->gridLocation.x, hitBlock->gridLocation.y, hitBlock->gridLocation.z);

        //                // setting block to air
        //                block_set_regtype(lpctx, hitBlockIndex, REG_AIR);

        //                // since this block is now air, it must be removed from the scene
        //                physx::PxRigidStatic* actor = block_get_actor(lpctx, hitBlockIndex);
        //                lpctx->mScene->removeActor(*actor);
        //                lpctx->blocksAroundMe.erase(
        //                    std::find(
        //                        lpctx->blocksAroundMe.begin(),
        //                        lpctx->blocksAroundMe.end(),
        //                        actor));

        //                // reset the block
        //                block_release_actor(lpctx, hitBlockIndex);
        //                block_set_flags(lpctx, hitBlockIndex, 0);

        //                // update the masks for the block removed
        //                // and update the masks for the surrounding blocks
        //                // and update the face data for the surrounding blocks
        //                update_surrounding_blocks(lpctx, hitBlock->gridLocation.x, hitBlock->gridLocation.y, hitBlock->gridLocation.z);

        //                addActorsForCurrentLocation(lpctx, (int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
        //                checkAddActors = false;

        //                isDoneProcessing = std::async([lpctx, &vbosToUpdate, rctx]()
        //                    {
        //                        printf("starting async proc\n");
        //                        if (FALSE == wglMakeCurrent(rctx->hdc, lpctx->hglrcAlt))
        //                            throw new std::runtime_error("failed to make rc current on async proc");
        //                        for (const auto& vbg : lpctx->groups)
        //                        {
        //                            GLuint newVbo = 0;
        //                            
        //                            glCreateBuffers(1, &newVbo);

        //                            glNamedBufferStorage(
        //                                newVbo,
        //                                sizeof(VERTEX2) * vbg.vertices.size(),
        //                                vbg.vertices.data(), 0);

        //                            vbosToUpdate.insert(std::pair<int, GLuint>(vbg.vbo, newVbo));
        //                        }
        //                        if (false == wglMakeCurrent(rctx->hdc, nullptr))
        //                            throw new std::runtime_error("failed to release rc on async proc");
        //                        printf("async proc done\n");
        //                    }
        //                );

        //                //t = clock() - t;
        //                //double time_taken = ((double)t) / CLOCKS_PER_SEC; // calculate the elapsed time
        //                //printf("Elapsed %f seconds", time_taken);

        //                // the hit block is invalid now - it's gone
        //                hitBlock = NULL;
        //            }
        //        }

        //        // keys 7 right button

        //    }
        //}

        // refresh physics actors based on location
        if (true == checkAddActors)
        {
            if (abs(lpctx->xblock - (int64_t)pos.x) > 5) {
                addActorsForCurrentLocation(lpctx, (int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
            }
            else if (abs(lpctx->yblock - (int64_t)pos.y) > 5) {
                addActorsForCurrentLocation(lpctx, (int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
            }
            else if (abs(lpctx->zblock - (int64_t)pos.z) > 5)
            {
                addActorsForCurrentLocation(lpctx, (int64_t)pos.x, (int64_t)pos.y, (int64_t)pos.z);
            }
        }

        glm::vec4 diffuseColor = glm::vec4(0.3, 0.5, 0.7, 1.0);

        // render the shadow buffer
        glm::vec3 lightLooking = glm::vec3(pos.x - 100, pos.y - 100, pos.z + 100);
        glm::vec3 posLocation3 = glm::vec3(pos.x, pos.y, pos.z + 1.0f);
        glm::vec3 lookVector(
            (cosf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
            (sinf(DEG2RAD(lpctx->azimuth)) * invSinfElevation),
            sinfElevation);
        glm::vec3 lightDir = glm::normalize(posLocation3 - lightLooking);
        glm::mat4 lightView = glm::lookAt(lightLooking, posLocation3, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 lightSpaceMatrix = rctx->lightProjection * lightView;
        {
            glViewport(0, 0, rctx->shadowWidth, rctx->shadowHeight);

            glBindFramebuffer(GL_FRAMEBUFFER, rctx->depthMapFBO);

            glClear(GL_DEPTH_BUFFER_BIT);

            rctx->shadowProg.Use();
            rctx->shadowProg.SetUniformMatrix4fv("lightSpaceMatrix", &lightSpaceMatrix[0][0]);
            rctx->shadowProg.SetUniformMatrix4fv("model", &rctx->modelMatrix[0][0]);

            glBindVertexArray(lpctx->vao);

            RenderScene(lpctx, posLocation3, lookVector);

            // render model
            for (const auto& modelVboItem : rctx->modelVboData)
            {
                glVertexArrayVertexBuffer(lpctx->vao, 0, modelVboItem.vboId, 0, sizeof(VERTEX4));

                glDrawArrays(GL_TRIANGLES, 0, modelVboItem.numVerts);
            }

            glBindVertexArray(0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        // configure player matrices (view and projection)
        // based on current location
        glm::mat4 viewMatrix = glm::lookAt(
            posLocation3,
            posLocation3 + lookVector,
            glm::vec3(0, 0, 1)
        );
        glm::mat4 projMatrix = glm::perspective(
            glm::radians(45.0f),
            lpctx->viewportRatio,
            0.01f,
            500.0f);

        // render the main scene (the voxels)
        {
            glViewport(0, 0, lpctx->screenWidth, lpctx->screenHeight);

            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            rctx->voxcProgram.Use();
            rctx->voxcProgram.SetUniformMatrix4fv("model", &rctx->modelMatrix[0][0]);
            rctx->voxcProgram.SetUniformMatrix4fv("view", &viewMatrix[0][0]);
            rctx->voxcProgram.SetUniformMatrix4fv("proj", &projMatrix[0][0]);
            rctx->voxcProgram.SetUniformMatrix4fv("lightSpaceMatrix", &lightSpaceMatrix[0][0]);
            rctx->voxcProgram.SetUniform3fv("lightDir", &lightDir[0]);
            rctx->voxcProgram.SetUniform1i("useDiffuseColor", 0);

            glActiveTexture(GL_TEXTURE1);

            glBindTexture(GL_TEXTURE_2D, rctx->depthMap);

            glBindVertexArray(lpctx->vao);

            RenderScene(lpctx, posLocation3, lookVector);

            // render model
            rctx->voxcProgram.SetUniform1i("useDiffuseColor", 1);
            // render model
            for (const auto& modelVboItem : rctx->modelVboData)
            {
                rctx->voxcProgram.SetUniform4fv("diffuseColor", &modelVboItem.diffuseColor[0]);

                glVertexArrayVertexBuffer(lpctx->vao, 0, modelVboItem.vboId, 0, sizeof(VERTEX4));

                glDrawArrays(GL_TRIANGLES, 0, modelVboItem.numVerts);
            }

            glBindVertexArray(0);

            glActiveTexture(GL_TEXTURE1);

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // need alpha blending for next two items
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // render selection cube
        // player is as pos.xyz
        // direction is azimuth
        if (hitStatus == true)
        {
            // raycast
            glm::mat4 zeroCubeModelt = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(
                    floorf(gp.p[0]) + 0.5f,
                    floorf(gp.p[1]) + 0.5f,
                    floorf(gp.p[2]) + 0.5f));

            // draw selection cube
            rctx->selCubeProg.Use();
            rctx->selCubeProg.SetUniformMatrix4fv("model", &zeroCubeModelt[0][0]);
            rctx->selCubeProg.SetUniformMatrix4fv("view", &viewMatrix[0][0]);
            rctx->selCubeProg.SetUniformMatrix4fv("proj", &projMatrix[0][0]);
            rctx->zeroCubeBuffer.draw();
            // end sel cube

        }

        // render text
        {
            SYSTEMTIME systime = { 0 };
            GetSystemTime(&systime);
            memset(textBuffer, 0, 256);
            sprintf_s(textBuffer, 256, "%02i:%02i:%02i %03i", systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
            RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - 14.0f, 0.3f, glm::vec3(0.5, 0.8f, 0.2f), 
                rctx->fontVAO, rctx->fontVBO, rctx->Characters);
            memset(textBuffer, 0, 256);
            sprintf_s(textBuffer, 256, "FPS: %i", (int)floorf(1 / elapsed));
            RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (1.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);
            memset(textBuffer, 0, 256);
            sprintf_s(textBuffer, 256, "POS: %.1f %.1f %.1f  AZ %.1f  EL %.1f", pos.x, pos.y, pos.z, lpctx->azimuth, lpctx->elevation);
            RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (2.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);
            if (hitStatus)
            {
                memset(textBuffer, 0, 256);
                sprintf_s(textBuffer, 256, "HIT POS: %.1f %.1f %.1f", gp.p[0], gp.p[1], gp.p[2]);
                RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (3.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);
                //if (hitBlock != NULL)
                //{
                    //memset(textBuffer, 0, 256);
                    //sprintf_s(textBuffer, 256, "T: %i F: %i H: 0x%08x", hitBlock->regType, hitBlock->flags, (unsigned int)hitBlock->hashCode);
                    //RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (4.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);
                    //memset(textBuffer, 0, 256);
                    //sprintf_s(textBuffer, 256, "GRIDLOC: %i %i %i",
                    //    hitBlock->gridLocation.x,
                    //    hitBlock->gridLocation.y,
                    //    hitBlock->gridLocation.z);
                    //RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (5.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);

                    //uint8_t regType = block_get_regtype(lpctx, hitBlock->gridLocation.x, hitBlock->gridLocation.y, hitBlock->gridLocation.z, false);
                    //memset(textBuffer, 0, 256);
                    //sprintf_s(textBuffer, 256, "REGTYPE: %i", regType);
                    //RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (6.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);
                //}
            }
        }

        memset(textBuffer, 0, 256);
        sprintf_s(textBuffer, 256, "draw vector has %zi items", lpctx->drawVector.size());
        RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (7.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);

        MEMORYSTATUSEX memstat = { 0 };
        memstat.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memstat);
        memset(textBuffer, 0, 256);
        sprintf_s(textBuffer, 256, "memory load %i%%", memstat.dwMemoryLoad);
        RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (8.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);

        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        memset(textBuffer, 0, 256);
        sprintf_s(textBuffer, 256, "process mem %lli bytes", pmc.WorkingSetSize);
        RenderText(lpctx, rctx->fontProg, textBuffer, 0.0f, lpctx->screenHeight - (14.0f + (9.0f * 14.0f)), 0.3f, glm::vec3(0.5, 0.8f, 0.2f), rctx->fontVAO, rctx->fontVBO, rctx->Characters);

        // render depth map
        rctx->ddProg.Use();
        rctx->quadBuffer.draw();

        // disable blend
        glDisable(GL_BLEND);

        glFlush();

        if (false == SwapBuffers(rctx->hdc))
            throw new std::runtime_error("failed to swap buffers");

        glUseProgram(0);

        HANDLE_GL_ERROR();

    }

    free(textBuffer);

}

void load_textures(VOXC_WINDOW_CONTEXT* lpctx)
{
    // if adding a new texture
    // increase the count in load textures
    // also, go to voxc.h and add another define
    // also, in vertex buffer creation below, increase count
    // also, createbuffers and loop below it
    TEXTURE_SPEC tsArray[6] = { 0 };
    tsArray[0].name = "vocxdirt.png"; tsArray[0].isTransparent = FALSE;
    tsArray[1].name = "vocxdirtgrass.png"; tsArray[1].isTransparent = FALSE;
    tsArray[2].name = "vocxgrass.png"; tsArray[2].isTransparent = FALSE;
    tsArray[3].name = "vocxleaves_t.png"; tsArray[3].isTransparent = TRUE;
    tsArray[4].name = "vocxwoodbark.png"; tsArray[4].isTransparent = FALSE;
    tsArray[5].name = "vocxwoodrings.png"; tsArray[5].isTransparent = FALSE;
    // after this, there will be one block group
    // for each texture 
    load_textures_2(tsArray, 6, lpctx);
}

void render_message_on_screen(MESSAGE_CONTEXT* mctx, const char* msg, float prog)
{
    char textBuffer[256];
    glViewport(0, 0, mctx->lpctx->screenWidth, mctx->lpctx->screenHeight);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    memset(textBuffer, 0, 256);
    sprintf_s(textBuffer, 256, "%s", msg);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    RenderText(
        mctx->lpctx,
        *mctx->prog,
        textBuffer, 
        0.0f, 
        mctx->lpctx->screenHeight - (14.0f + (1.0f * 14.0f)),
        0.3f, 
        glm::vec3(0.5, 0.8f, 0.2f), 
        mctx->fontVAO,
        mctx->fontVBO,
        *mctx->Characters);
    glDisable(GL_BLEND);

    mctx->prog2d->Use();
    glm::vec2 vs[6] = { {-1,0},{(prog * 2.0f) - 1.0f,0},{(prog * 2.0f) - 1.0f,0.1},
        {-1,0},{(prog * 2.0f) - 1.0f,0.1},{-1,0.1} };
    GLuint vao2d;
    glCreateVertexArrays(1, &vao2d);
    glVertexArrayAttribBinding(vao2d, 0, 0);
    glVertexArrayAttribFormat(vao2d, 0, 2, GL_FLOAT, FALSE, 0);
    glVertexArrayBindingDivisor(vao2d, 0, 0);
    glEnableVertexArrayAttrib(vao2d, 0);
    glBindVertexArray(vao2d);
    GLuint vbo2d;
    glCreateBuffers(1, &vbo2d);
    glNamedBufferStorage(vbo2d, 12 * sizeof(GLfloat), vs, 0);
    glVertexArrayVertexBuffer(vao2d, 0, vbo2d, 0, 2 * sizeof(GLfloat));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glFlush();
    if (false == SwapBuffers(mctx->hdc))
        throw new std::runtime_error("failed to swap buffers");

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo2d);
    glDeleteVertexArrays(1, &vao2d);
}

DWORD WINAPI RenderThread(LPVOID parm)
{
    // create the rendering contxt
    HWND hwnd = (HWND)parm;
    VOXC_WINDOW_CONTEXT* lpctx = (VOXC_WINDOW_CONTEXT*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    HDC hdc = GetDC(hwnd); 

    PROCESS_MEMORY_COUNTERS pmc = { 0 };
    std::ofstream memStatsFile;
    memStatsFile.open("memstats.txt");
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    createRenderingContext2(hdc, lpctx);

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // load all the xtension functions
    loadExtensionFunctions();

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // opengl configuration
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, lpctx->screenWidth, lpctx->screenHeight);
    glFrontFace(GL_CCW);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    HANDLE_GL_ERROR();

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // load programs
    OpenGlProgram voxcProgram("vshader.txt", "fshader.txt");
    voxcProgram.Use();
    voxcProgram.SetUniform1i("texs", 0);        // one-time setup
    voxcProgram.SetUniform1i("shadowMap", 1);   // one-time setup
    OpenGlProgram shadowProg("vshadow.txt", "fshadow.txt");
    OpenGlProgram ddProg("vsh2d.txt", "fsh2d.txt");
    OpenGlProgram fontProg("vfont.txt", "ffont.txt");
    OpenGlProgram selCubeProg("selcube.vert", "selcube.frag");
    OpenGlProgram shader2d("vert2d.txt", "frag2d.txt");

    // freetype init
    GLuint fontVAO = 0;
    GLuint fontVBO = 0;
    std::map<char, Character> Characters;
    setupFreeType(Characters, &fontVAO, &fontVBO);

    MESSAGE_CONTEXT mctx = { 0 };
    mctx.Characters = &Characters;
    mctx.fontVAO = fontVAO;
    mctx.fontVBO = fontVBO;
    mctx.hdc = hdc;
    mctx.lpctx = lpctx;
    mctx.prog = &fontProg;
    mctx.prog2d = &shader2d;

    // now we can render messages
    render_message_on_screen(&mctx, "Preparing...", 0.0f);

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    render_message_on_screen(&mctx, "Loading textures...", 0.0f);
    load_textures(lpctx);

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // physics: must be done before create vertex buffer
    render_message_on_screen(&mctx, "Initializing physics...", 0.0f);
    glm::vec3 startingPosition(8, 8, 260);
    initPhysics(lpctx, startingPosition);
    // end physics

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // vertex buffer
    render_message_on_screen(&mctx, "Creating vertex buffer...", 0.0f);
    {
        create_vertex_buffer(lpctx, render_message_on_screen, &mctx);

        //for (const auto& vertex_buffer : lpctx->vertex_buffers)
        //{
        //    glNamedBufferStorage(
        //        vertex_buffer.vbo,
        //        vertex_buffer.mem_size,
        //        vertex_buffer.mem,
        //        GL_DYNAMIC_STORAGE_BIT);
        //    HANDLE_GL_ERROR();
        //}

        //std::vector<GLuint> vbos(6);
        //glCreateBuffers(6, vbos.data());
        //HANDLE_GL_ERROR();

        //std::vector<VERTEX_BUFFER_GROUP1>::iterator vbgIter = lpctx->groups.begin();
        //for(;vbgIter != lpctx->groups.end(); ++vbgIter)
        //{
        //    std::map<uint16_t, VERTEX_BLOCK>::iterator blockIter = vbgIter->vertexBlocks.begin();
        //    for (; blockIter != vbgIter->vertexBlocks.end(); ++blockIter)
        //    {
        //        if (blockIter->second.vertices.size() > 0) {
        //            GLuint vbo = 0;
        //            glCreateBuffers(1, &vbo);

        //            blockIter->second.vsize = blockIter->second.vertices.size();
        //            glNamedBufferStorage(vbo, sizeof(VERTEX2) * blockIter->second.vsize,
        //                blockIter->second.vertices.data(), 0);
        //            HANDLE_GL_ERROR();

        //            blockIter->second.vbo = vbo;
        //        }
        //        else {
        //            blockIter->second.vsize = 0;
        //        }
        //    }

        //    //if (lpctx->groups[i].vertices.size() > 0) {
        //    //    lpctx->groups[i].vsize = lpctx->groups[i].vertices.size();
        //    //    glNamedBufferStorage(vbos[i], sizeof(VERTEX2) * lpctx->groups[i].vertices.size(),
        //    //        lpctx->groups[i].vertices.data(), 0);
        //    //    HANDLE_GL_ERROR();
        //    //}
        //    //else {
        //    //    lpctx->groups[i].vsize = 0;
        //    //}
        //    //lpctx->groups[i].vbo = vbos[i];
        //}
    }

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // model
    render_message_on_screen(&mctx, "Loading model...", 0.0f);
    std::vector<VBO_DATA> modelVboData;
    LoadModel(modelVboData);

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // create the vertex array
    render_message_on_screen(&mctx, "Creating vertex array specs...", 0.0f);
    glCreateVertexArrays(1, &lpctx->vao);
    glVertexArrayAttribBinding(lpctx->vao, 0, 0);
    glVertexArrayAttribBinding(lpctx->vao, 1, 0);
    glVertexArrayAttribBinding(lpctx->vao, 2, 0);
    glVertexArrayAttribFormat(lpctx->vao, 0, 3, GL_FLOAT, FALSE, 0);
    glVertexArrayAttribFormat(lpctx->vao, 1, 2, GL_FLOAT, FALSE, 3 * sizeof(GLfloat));
    glVertexArrayAttribFormat(lpctx->vao, 2, 3, GL_FLOAT, FALSE, 5 * sizeof(GLfloat));
    glVertexArrayBindingDivisor(lpctx->vao, 0, 0);
    glEnableVertexArrayAttrib(lpctx->vao, 0);
    glEnableVertexArrayAttrib(lpctx->vao, 1);
    glEnableVertexArrayAttrib(lpctx->vao, 2);
    HANDLE_GL_ERROR();
    // end vertex array 

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // create a frame buffer for shadows 
    render_message_on_screen(&mctx, "Creating shadow buffer...", 0.0f);
    GLuint depthMapFBO = 0;
    glGenFramebuffers(1, &depthMapFBO);
    HANDLE_GL_ERROR();

    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    GLuint depthMap = 0;
    glGenTextures(1, &depthMap);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    HANDLE_GL_ERROR();

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    HANDLE_GL_ERROR();

    float near_plane = 0.1f, far_plane = 500.0f;
    glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
    glm::vec3 eye1 = glm::vec3(0, 0, 200);
    glm::vec3 cntr1 = glm::vec3(X_GRID_EXTENT / 3, Y_GRID_EXTENT / 3, 50);
    glm::mat4 lightView1 = glm::lookAt(eye1, cntr1, glm::vec3(0.0f, 0.0f, 1.0f));
    // end shadows

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // vbo for 2d quad
    render_message_on_screen(&mctx, "Creating quad buffer for shadow thumbnail...", 0.0f);
    OpenGlVertexBuffer<VERTEX4> quadBuffer(lpctx->vao, (GLsizei)6, &quadVerts[0]);
    quadBuffer.setTextureInfo(GL_TEXTURE0, GL_TEXTURE_2D, depthMap);

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // selection cube vertex buffer
    render_message_on_screen(&mctx, "Creating selection cube...", 0.0f);
    std::vector<VERTEX4> zeroCubeVertices;
    getZeroCubeVertices(zeroCubeVertices);
    OpenGlVertexBuffer<VERTEX4> zeroCubeBuffer(lpctx->vao, (GLsizei)zeroCubeVertices.size(), zeroCubeVertices.data());
    zeroCubeVertices.clear();

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // add initial set of actors based on starting location
    render_message_on_screen(&mctx, "Adding initial actors...", 0.0f);
    addActorsForCurrentLocation(lpctx,
        (int64_t)startingPosition.x,
        (int64_t)startingPosition.y,
        (int64_t)startingPosition.z);

    // final prep
    glClearColor(0.9f, 0.9f, 1.0f, 1.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    HANDLE_GL_ERROR();

    memStatsFile << "start render loop" << std::endl;

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // ***********
    // render loop
    // ***********
    RENDER_LOOP_CONTEXT rctx = {
        hdc, lightProjection, SHADOW_WIDTH, SHADOW_HEIGHT,
        depthMapFBO, shadowProg, modelMatrix, modelVboData, voxcProgram,
        depthMap, selCubeProg, zeroCubeBuffer, fontProg, fontVAO, fontVBO,
        Characters, ddProg, quadBuffer
    };
    render_loop(lpctx, &rctx);

    memStatsFile << "end render loop" << std::endl;

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    // *******
    // cleanup
    // *******
    glUseProgram(0);
   
    glDeleteBuffers(1, &fontVBO);
    glDeleteVertexArrays(1, &fontVAO);

    for (const auto& c : Characters)
    {
        glDeleteTextures(1, &c.second.TextureID);
    }

    for (const auto& modelVboItem : modelVboData)
    {
        glDeleteBuffers(1, &modelVboItem.vboId);
    }

    glDeleteTextures(1, &depthMap);
    glDeleteFramebuffers(1, &depthMapFBO);

    //for (const auto& gp : lpctx->groups)
    //{
    //    glDeleteTextures(1, &gp.tid);
    //    for (const auto& vblk : gp.vertexBlocks)
    //    {
    //        glDeleteBuffers(1, &vblk.second.vbo);
    //    }
    //}

    //for (const auto& vertex_buffer : lpctx->vertex_buffers)
    //{
    //    glDeleteBuffers(1, &vertex_buffer.vbo);
    //    free(vertex_buffer.mem);
    //}

    for (const auto& dv : lpctx->drawVector)
    {
        glDeleteBuffers(1, &dv.vbo);
    }

    glDeleteVertexArrays(1, &lpctx->vao);

    voxcProgram.Release();
    shadowProg.Release();
    ddProg.Release();
    fontProg.Release();
    selCubeProg.Release();

    quadBuffer.Release();
    zeroCubeBuffer.Release();

    HANDLE_GL_ERROR();

    wglMakeCurrent(hdc, nullptr);
    ReleaseDC(hwnd, hdc);
    wglDeleteContext(lpctx->hglrc);
    if (lpctx->hglrcAlt) wglDeleteContext(lpctx->hglrcAlt);

    block_release_all_actors(lpctx);

    cleanupPhysics(lpctx);

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    memStatsFile << (double)clock() / CLOCKS_PER_SEC << " s " << pmc.WorkingSetSize << " bytes " << std::endl;

    memStatsFile.close();

    return 0;
}

