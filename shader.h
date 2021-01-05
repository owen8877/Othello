//
// Created by xdroid on 1/4/21.
//

#ifndef OTHELLO_SHADER_H
#define OTHELLO_SHADER_H

#include <GLES3/gl3.h>

/** Loads a vertex and fragment shader from disk and compiles (& links) them
 * into a shader program.
 * This will print any errors to the console.
 * @param vertFilename filename for the vertex shader
 * @param fragFilename the fragment shader's filename.
 * @return GLuint the shader program's ID, or 0 if failed. */
GLuint shaderProgLoad(const char *vertFilename, const char*fragFilename);

/** Destroys a shader program. */
void shaderProgDestroy(GLuint shaderProg);

#endif //OTHELLO_SHADER_H
