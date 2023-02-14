
#include "angle/shader_util.h"

namespace angle {

GLuint CompileProgram(const char *vsSource, const char *fsSource) {
    return CompileProgramInternal(vsSource, "", "", "", fsSource);
}

GLuint CompileProgramInternal(const char *vsSource,
                              const char *tcsSource,
                              const char *tesSource,
                              const char *gsSource,
                              const char *fsSource) {
    GLuint program = glCreateProgram();

    GLuint vs = CompileShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsSource);

    if (vs == 0 || fs == 0)
    {
        glDeleteShader(fs);
        glDeleteShader(vs);
        glDeleteProgram(program);
        return 0;
    }

    glAttachShader(program, vs);
    glDeleteShader(vs);

    glAttachShader(program, fs);
    glDeleteShader(fs);

    GLuint tcs = 0;
    GLuint tes = 0;
    GLuint gs  = 0;

    if (strlen(tcsSource) > 0)
    {
        tcs = CompileShader(GL_TESS_CONTROL_SHADER_EXT, tcsSource);
        if (tcs == 0)
        {
            glDeleteShader(vs);
            glDeleteShader(fs);
            glDeleteProgram(program);
            return 0;
        }

        glAttachShader(program, tcs);
        glDeleteShader(tcs);
    }

    if (strlen(tesSource) > 0)
    {
        tes = CompileShader(GL_TESS_EVALUATION_SHADER_EXT, tesSource);
        if (tes == 0)
        {
            glDeleteShader(vs);
            glDeleteShader(fs);
            glDeleteShader(tcs);
            glDeleteProgram(program);
            return 0;
        }

        glAttachShader(program, tes);
        glDeleteShader(tes);
    }

    if (strlen(gsSource) > 0)
    {
        gs = CompileShader(GL_GEOMETRY_SHADER_EXT, gsSource);
        if (gs == 0)
        {
            glDeleteShader(vs);
            glDeleteShader(fs);
            glDeleteShader(tcs);
            glDeleteShader(tes);
            glDeleteProgram(program);
            return 0;
        }

        glAttachShader(program, gs);
        glDeleteShader(gs);
    }

    glLinkProgram(program);

    return CheckLinkStatusAndReturnProgram(program, true);
}

GLuint CheckLinkStatusAndReturnProgram(GLuint program, bool outputErrorMessages) {
    if (glGetError() != GL_NO_ERROR)
        return 0;

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == 0)
    {
        if (outputErrorMessages)
        {
            GLint infoLogLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

            // Info log length includes the null terminator, so 1 means that the info log is an
            // empty string.
            if (infoLogLength > 1)
            {
                std::vector<GLchar> infoLog(infoLogLength);
                glGetProgramInfoLog(program, static_cast<GLsizei>(infoLog.size()), nullptr,
                                    &infoLog[0]);

                std::cerr << "program link failed: " << &infoLog[0];
            }
            else
            {
                std::cerr << "program link failed. <Empty log message>";
            }
        }

        glDeleteProgram(program);
        return 0;
    }

    return program;
}

GLuint CompileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);

    const char *sourceArray[1] = {source};
    glShaderSource(shader, 1, sourceArray, nullptr);
    glCompileShader(shader);

    GLint compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == 0)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        // Info log length includes the null terminator, so 1 means that the info log is an empty
        // string.
        if (infoLogLength > 1)
        {
            std::vector<GLchar> infoLog(infoLogLength);
            glGetShaderInfoLog(shader, static_cast<GLsizei>(infoLog.size()), nullptr, &infoLog[0]);
            std::cerr << "shader compilation failed: " << &infoLog[0];
        }
        else
        {
            std::cerr << "shader compilation failed. <Empty log message>";
        }

        std::cerr << std::endl;

        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

} // namespace angle