#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <map>

class Shader
{
public:
  	
	// Constructor reads and builds the shader
	Shader(): _totalShaders(0)
	{
		_shaders[VERTEX_SHADER]=0;
		_shaders[FRAGMENT_SHADER]=0;
		_shaders[GEOMETRY_SHADER]=0;
		_attributeList.clear();
		_uniformLocationList.clear();
	}
	~Shader()
	{
		_attributeList.clear();	
		_uniformLocationList.clear();
	}
	void loadFromString(GLenum whichShader, const std::string &source);
	void loadFromFile(GLenum whichShader, const std::string &fileName);
	void createAndLinkProgram();
  	// Use the program
	void use(){glUseProgram(_program);}
	void unUse(){glUseProgram(0);}
	void addAttribute(const std::string &attribute);
	void addUniform(const std::string &uniform);

	GLuint operator[](const std::string &attribute);
	GLuint operator()(const std::string &uniform);

	void deleteProgram();
private:
	enum ShaderType{VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
	// The program ID
	GLuint _program;

	int _totalShaders;
	GLuint _shaders[3];
	std::map<std::string, GLuint> _attributeList;
	std::map<std::string, GLuint> _uniformLocationList;
};

#endif