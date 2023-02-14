#ifndef ANGLE_SHADER_UTILS_H
#define ANGLE_SHADER_UTILS_H

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "angle/egl_loader_autogen.h"
#include "angle/gles_loader_autogen.h"

namespace angle {

GLuint CompileProgram(const char *vsSource, const char *fsSource);

GLuint CompileProgramInternal(const char *vsSource,
                              const char *tcsSource,
                              const char *tesSource,
                              const char *gsSource,
                              const char *fsSource);


GLuint CheckLinkStatusAndReturnProgram(GLuint program, bool outputErrorMessages);


GLuint CompileShader(GLenum type, const char *source);

} // namespace angle
#endif // ANGLE_SHADER_UTILS_H