#include "voxc.h"

char* readShaderSource(const char* filename)
{
    char* shaderSource = NULL;
    FILE* f = NULL;
    long l = 0;
    fopen_s(&f, filename, "r");
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        l = ftell(f);
        fseek(f, 0, SEEK_SET);
        shaderSource = (char*)malloc(l);
        if (shaderSource != NULL) {
            memset(shaderSource, 0, l);
            fread(shaderSource, 1, l, f);
            fclose(f);
            f = NULL;
        }
        else {
            printf("Failed to allocate memory for shader source code\n");
            fclose(f);
            f = NULL;
            return NULL;
        }
    }
    else {
        printf("Failed to open shader source file\n");
        return NULL;
    }
    return shaderSource;
}

BOOL CreateAndCompile(const char* src, GLenum type, GLuint* pShader)
{
    GLuint shader = glCreateShader(type);
    GLint isCompiled = 0;
    GLint maxLength = 0;
    std::vector<GLchar> errorLog;

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    // check for compilation errors as per normal here
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        if (type == GL_VERTEX_SHADER) {
            printf("vertex shader\n");
        }
        else if(type == GL_FRAGMENT_SHADER) {
            printf("fragment shader\n");
        }
        printf("Error compiling shader\n");

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        errorLog.resize(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

        for (size_t i = 0; i < errorLog.size(); i++) {
            printf("%c", errorLog[i]);
        }
        printf("\n");

        // Provide the infolog in whatever manor you deem best. 
        // Exit with failure .
        glDeleteShader(shader); // Don't leak the shader.
        *pShader = 0;
        return FALSE;
    }
    else {
        *pShader = shader;
        return TRUE;
    }

}

BOOL LinkShader(GLuint prog, GLuint shader1, GLuint shader2)
{
    GLint isLinked = 0;
    GLint maxLength = 0;
    std::vector<GLchar> infoLog;

    glAttachShader(prog, shader1);
    if (shader2 > 0) glAttachShader(prog, shader2);
    glLinkProgram(prog);
    // check for linking errors and validate program as per normal here
    glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        infoLog.resize(maxLength);
        glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);

        for (size_t i = 0; i < infoLog.size(); i++) {
            printf("%c", infoLog[i]);
        }
        printf("\n");
        return FALSE;
    }
    else {
        return TRUE;
    }

}

OpenGlProgram::OpenGlProgram(const char* vfn, const char* ffn)
{
    this->vsh = 0;
    this->prg = 0;
    this->fsh = 0;

    char* vshaderSource = readShaderSource(vfn);
    if (FALSE == CreateAndCompile(vshaderSource, GL_VERTEX_SHADER, &this->vsh))
    {
        free(vshaderSource);
        this->vsh = 0;
        return;
    }
    free(vshaderSource);

    char* fshaderSource = readShaderSource(ffn);
    if (FALSE == CreateAndCompile(fshaderSource, GL_FRAGMENT_SHADER, &this->fsh))
    {
        glDeleteShader(this->vsh);
        this->vsh = 0;
        free(fshaderSource);
        this->fsh = 0;
        return;
    }
    free(fshaderSource);

    this->prg = glCreateProgram();
    if (FALSE == LinkShader(this->prg, this->vsh, this->fsh))
    {
        glDeleteProgram(this->prg);
        glDeleteShader(this->fsh);
        glDeleteShader(this->vsh);
        this->vsh = 0;
        this->fsh = 0;
        this->prg = 0;
        return;
    }
}

OpenGlProgram::~OpenGlProgram()
{
    glDeleteShader(this->fsh);
    glDeleteShader(this->vsh);
    glDeleteProgram(this->prg);
}

void OpenGlProgram::Use()
{
    glUseProgram(this->prg);
}

void OpenGlProgram::SetUniformMatrix4fv(const char* name, const GLfloat* value)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    glUniformMatrix4fv(loc, 1, false, value);
}

void OpenGlProgram::SetUniform1i(const char* name, const GLint value)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    glUniform1i(loc, value);
}
