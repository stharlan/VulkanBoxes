#pragma once
class OpenGlProgram
{
public:
	OpenGlProgram(const char* vfn, const char* ffn);
	~OpenGlProgram();
	void Use();
	void SetUniform(const char* name, const GLfloat* value);
private:
	GLuint fsh = 0;
	GLuint vsh = 0;
	GLuint prg = 0;
};

