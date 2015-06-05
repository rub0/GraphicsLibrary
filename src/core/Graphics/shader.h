#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class Shader
{
public:
  	
	// Constructor reads and builds the shader
	Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
  	// Use the program
	void Use(){glUseProgram(_program);}
	GLuint getProgram(){return _program;}

private:

	GLuint compileShader(unsigned int typeShader, const GLchar* shaderCode);

	// The program ID
	GLuint _program;
};

#endif