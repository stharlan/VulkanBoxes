#pragma once
class OpenGlProgram
{
public:
	OpenGlProgram(const char* vfn, const char* ffn);
	~OpenGlProgram();
	void Use();
	void SetUniformMatrix4fv(const char* name, const GLfloat* value);
	void SetUniform1i(const char* name, const GLint value);
	void SetUniform3fv(const char* name, const GLfloat* value);
	void SetUniform3f(const char* name, const GLfloat value1, const GLfloat value2, const GLfloat value3);
	void SetUniform4fv(const char* name, const GLfloat* value);
private:
	GLuint fsh = 0;
	GLuint vsh = 0;
	GLuint prg = 0;
};

