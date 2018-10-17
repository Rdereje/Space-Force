#ifndef INITSHADER_H_
#define INITSHADER_H_

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#ifdef __APPLE__  // include Mac OS X verions of headers
#include <OpenGL/OpenGL.h>
//~ #include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#include <GL/glew.h>
// #include <GL/freeglut.h>
// #include <GL/freeglut_ext.h>
//~ #include <GL/glew.h>
//~ #include <GL/glut.h>
#endif  // __APPLE__


struct Shader
{
    const char*  filename;
    GLenum       type;
    GLchar*      source;
};

GLuint initShader(const char* vertexShaderFile, const char* fragmentShaderFile);


#endif /* INITSHADER_H_ */
