#pragma once
class OpenGlProgram
{
public:
	OpenGlProgram(const char* vfn, const char* ffn);
	~OpenGlProgram();
	void Use();
	void SetUniformMatrix4fv(const char* name, const GLfloat* value);
	void SetUniform1i(const char* name, const GLint value);
private:
	GLuint fsh = 0;
	GLuint vsh = 0;
	GLuint prg = 0;
};

