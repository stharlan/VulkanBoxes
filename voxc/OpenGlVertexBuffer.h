#pragma once
template<class T>
class OpenGlVertexBuffer
{
public:
	OpenGlVertexBuffer(const GLuint pvao) 
		: vao(pvao), texture(0), target(0), textureId(0), hasTexture(false)
	{
		glCreateBuffers(1, &this->vbo);
	}
	OpenGlVertexBuffer(const GLuint pvao, const GLsizei numVertices) 
		: vao(pvao), texture(0), target(0), textureId(0), hasTexture(false)
	{
		glCreateBuffers(1, &this->vbo);
		this->vertices.resize(numVertices);
	}
	OpenGlVertexBuffer(const GLuint pvao, const GLsizei pNumVertices, const T* varray) 
		: vao(pvao), texture(0), target(0), textureId(0), hasTexture(false)
	{
		glCreateBuffers(1, &this->vbo);
		glNamedBufferStorage(this->vbo, pNumVertices * sizeof(T), varray, 0);
		this->numVertices = pNumVertices;
	}
	~OpenGlVertexBuffer()
	{
		glDeleteBuffers(1, &this->vbo);
	}
	void addVertex(T& v)
	{
		this->vertices.push_back(v);
	}
	void draw()
	{
		if (this->hasTexture)
		{
			glActiveTexture(this->texture);
			glBindTexture(this->target, this->textureId);
		}
		glBindVertexArray(this->vao);
		glVertexArrayVertexBuffer(this->vao, 0, this->vbo, 0, sizeof(T));
		glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
		glBindVertexArray(0);
		if (hasTexture) glBindTexture(this->target, 0);
	}
	void setTextureInfo(GLenum ptexture, GLenum ptarget, GLuint ptextureId)
	{
		this->texture = ptexture;
		this->target = ptarget;
		this->textureId = ptextureId;
		hasTexture = true;
	}
private:
	std::vector<T> vertices;
	GLuint numVertices;
	GLuint vbo;
	GLuint vao;
	bool hasTexture;
	GLenum texture;
	GLenum target;
	GLint textureId;
};
