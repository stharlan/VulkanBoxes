#pragma once
template<class T>
class OpenGlVertexBuffer
{
public:
	OpenGlVertexBuffer(const GLuint pvao) 
		: vao(pvao), texture(0), target(0), textureId(0), hasTexture(false)
	{
		glCreateBuffers(1, &this->vbo);
		HANDLE_GL_ERROR();
	}
	OpenGlVertexBuffer(const GLuint pvao, const GLsizei numVertices) 
		: vao(pvao), texture(0), target(0), textureId(0), hasTexture(false)
	{
		glCreateBuffers(1, &this->vbo);
		HANDLE_GL_ERROR();

		this->vertices.resize(numVertices);
	}
	OpenGlVertexBuffer(const GLuint pvao, const GLsizei pNumVertices, const T* varray) 
		: vao(pvao), texture(0), target(0), textureId(0), hasTexture(false)
	{
		glCreateBuffers(1, &this->vbo);
		HANDLE_GL_ERROR();

		glNamedBufferStorage(this->vbo, pNumVertices * sizeof(T), varray, 0);
		HANDLE_GL_ERROR();

		this->numVertices = pNumVertices;
	}
	~OpenGlVertexBuffer()
	{
	}
	void Release()
	{
		glDeleteBuffers(1, &this->vbo);
		HANDLE_GL_ERROR();
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
			HANDLE_GL_ERROR();

			glBindTexture(this->target, this->textureId);
			HANDLE_GL_ERROR();
		}
		glBindVertexArray(this->vao);
		HANDLE_GL_ERROR();

		glVertexArrayVertexBuffer(this->vao, 0, this->vbo, 0, sizeof(T));
		HANDLE_GL_ERROR();

		glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
		HANDLE_GL_ERROR();

		glBindVertexArray(0);
		HANDLE_GL_ERROR();

		if (hasTexture) {
			glBindTexture(this->target, 0);
			HANDLE_GL_ERROR();
		}
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
