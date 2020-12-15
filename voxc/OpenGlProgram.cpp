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
    shaderSource = (char*)malloc(fsize+1);
    if (shaderSource == nullptr)
        throw new std::runtime_error("failed to allocate memory for shader source");
    memset(shaderSource, 0, fsize+1);

    DWORD nbr = 0;
    if (!ReadFile(hFile, shaderSource, fsize, &nbr, NULL))
        throw new std::runtime_error("failed to read file");

    if (false == CloseHandle(hFile))
        throw new std::runtime_error("failed to close shader source file");

    //FILE* f = NULL;
    //long l = 0;
    //errno_t err = fopen_s(&f, filename, "r");
    //if (err != 0)
        //throw new std::runtime_error("failed to open shader source file");
    //if (f == nullptr)
        //throw new std::runtime_error("failed to open shader source file");

    //int res = fseek(f, 0, SEEK_END);
    //if (res != 0)
        //throw new std::runtime_error("failed to seek end of file");

    //l = ftell(f);

    //res = fseek(f, 0, SEEK_SET);
    //if (res != 0)
        //throw new std::runtime_error("failed to seek start of file");


    //size_t nread = fread(shaderSource, 1, l, f);
    //if (nread != l) {
        //if (!feof(f))
        //{
            //throw new std::runtime_error("failed to read shader source from file");
        //}
    //}

    //res = fclose(f);
    //if(res != 0)
        //throw new std::runtime_error("failed to seek start of file");

    //f = NULL;

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

void OpenGlProgram::SetUniform3fv(const char* name, const GLfloat* value)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    glUniform3fv(loc, 1, value);
}

void OpenGlProgram::SetUniform3f(const char* name, const GLfloat value1, const GLfloat value2, const GLfloat value3)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    glUniform3f(loc, value1, value2, value3);
}

void OpenGlProgram::SetUniform4fv(const char* name, const GLfloat* value)
{
    GLuint loc = glGetUniformLocation(this->prg, name);
    glUniform4fv(loc, 1, value);
}