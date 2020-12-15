#include "voxc.h"

char* readShaderSource(const char* filename)
{
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ,
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        throw new std::runtime_error("failed to open shader source file");

    DWORD sizeHigh = 0;
    DWORD fsize = GetFileSize(hFile, &sizeHigh);
    if (fsize == INVALID_FILE_SIZE)
        throw new std::runtime_error("failed to get file size");

    char* shaderSource = NULL;
    shaderSource = (char*)malloc(fsize + (DWORD)1);
    if (shaderSource == nullptr)
        throw new std::runtime_error("failed to allocate memory for shader source");
    memset(shaderSource, 0, fsize + (DWORD)1);

    DWORD nbr = 0;
    if (!ReadFile(hFile, shaderSource, fsize, &nbr, NULL))
        throw new std::runtime_error("failed to read file");

    if (false == CloseHandle(hFile))
        throw new std::runtime_error("failed to close shader source file");

    return shaderSource;
}

BOOL CreateAndCompile(const char* src, GLenum type, GLuint* pShader)
{
    GLuint shader = glCreateShader(type);
    HANDLE_GL_ERROR();

    GLint isCompiled = 0;
    GLint maxLength = 0;
    std::vector<GLchar> errorLog;

    glShaderSource(shader, 1, &src, NULL);
    HANDLE_GL_ERROR();

    glCompileShader(shader);
    HANDLE_GL_ERROR();

    // check for compilation errors as per normal here
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    HANDLE_GL_ERROR();

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
        HANDLE_GL_ERROR();

        // The maxLength includes the NULL character
        errorLog.resize(maxLength);

        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        HANDLE_GL_ERROR();

        for (size_t i = 0; i < errorLog.size(); i++) {
            printf("%c", errorLog[i]);
        }
        printf("\n");

        // Provide the infolog in whatever manor you deem best. 
        // Exit with failure .
        glDeleteShader(shader); // Don't leak the shader.
        HANDLE_GL_ERROR();

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
    HANDLE_GL_ERROR();

    if (shader2 > 0) {
        glAttachShader(prog, shader2);
        HANDLE_GL_ERROR();
    }

    glLinkProgram(prog);
    HANDLE_GL_ERROR();

    // check for linking errors and validate program as per normal here
    glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
    HANDLE_GL_ERROR();

    if (isLinked == GL_FALSE)
    {
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);
        HANDLE_GL_ERROR();

        // The maxLength includes the NULL character
        infoLog.resize(maxLength);
        glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);
        HANDLE_GL_ERROR();

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
        HANDLE_GL_ERROR();

        this->vsh = 0;
        free(fshaderSource);
        this->fsh = 0;
        return;
    }
    free(fshaderSource);

    this->prg = glCreateProgram();
    HANDLE_GL_ERROR();

    if (FALSE == LinkShader(this->prg, this->vsh, this->fsh))
    {
        glDeleteProgram(this->prg);
        HANDLE_GL_ERROR();

        glDeleteShader(this->fsh);
        HANDLE_GL_ERROR();

        glDeleteShader(this->vsh);
        HANDLE_GL_ERROR();

        this->vsh = 0;
        this->fsh = 0;
        this->prg = 0;
        return;
    }
}

void OpenGlProgram::Release()
{
    glDeleteShader(this->fsh);
    HANDLE_GL_ERROR();

    glDeleteShader(this->vsh);
    HANDLE_GL_ERROR();

    glDeleteProgram(this->prg);
    HANDLE_GL_ERROR();
}

OpenGlProgram::~OpenGlProgram()
{
}

void OpenGlProgram::Use()
{
    glUseProgram(this->prg);
    HANDLE_GL_ERROR();
}

void OpenGlProgram::SetUniformMatrix4fv(const char* name, const GLfloat* value)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    HANDLE_GL_ERROR();

    glUniformMatrix4fv(loc, 1, false, value);
    HANDLE_GL_ERROR();
}

void OpenGlProgram::SetUniform1i(const char* name, const GLint value)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    HANDLE_GL_ERROR();

    glUniform1i(loc, value);
    HANDLE_GL_ERROR();
}

void OpenGlProgram::SetUniform3fv(const char* name, const GLfloat* value)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    HANDLE_GL_ERROR();

    glUniform3fv(loc, 1, value);
    HANDLE_GL_ERROR();
}

void OpenGlProgram::SetUniform3f(const char* name, const GLfloat value1, const GLfloat value2, const GLfloat value3)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    HANDLE_GL_ERROR();

    glUniform3f(loc, value1, value2, value3);
    HANDLE_GL_ERROR();
}

void OpenGlProgram::SetUniform4fv(const char* name, const GLfloat* value)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    HANDLE_GL_ERROR();

    glUniform4fv(loc, 1, value);
    HANDLE_GL_ERROR();
}