#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void Shader::deleteProgram()
{
	glDeleteProgram(_program);
}

void Shader::loadFromString(GLenum whichShader, const std::string &source)
{
    GLuint shader = glCreateShader(whichShader);
	const char * ssource = source.c_str();
	glShaderSource(shader, 1, &ssource, NULL);
    glCompileShader(shader);

	GLint success;
    // Print compile errors if any
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint infoLogLength;		
		glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog= new GLchar[infoLogLength];
		glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog);
		std::cerr<<"Compile log: "<<infoLog<<std::endl;
		delete [] infoLog;
    }
	_shaders[_totalShaders++]=shader;
}

void Shader::loadFromFile(GLenum whichShader, const std::string &fileName)
{
	std::string vertexCode;
    std::string fragmentCode;
    try 
    {
        // Open files
        std::ifstream shaderFile(fileName);
        std::stringstream shaderStream, fShaderStream;
        // Read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();	
        // close file handlers
        shaderFile.close();

		loadFromString(whichShader, shaderStream.str());
	}
	catch(std::exception e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

void Shader::createAndLinkProgram()
{
	_program = glCreateProgram();

	if (_shaders[VERTEX_SHADER] != 0) {
		glAttachShader (_program, _shaders[VERTEX_SHADER]);
	}
	if (_shaders[FRAGMENT_SHADER] != 0) {
		glAttachShader (_program, _shaders[FRAGMENT_SHADER]);
	}
	if (_shaders[GEOMETRY_SHADER] != 0) {
		glAttachShader (_program, _shaders[GEOMETRY_SHADER]);
	}

	//link and check whether the program links fine
	GLint status;
	glLinkProgram (_program);
	glGetProgramiv (_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		
		glGetProgramiv (_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog= new GLchar[infoLogLength];
		glGetProgramInfoLog (_program, infoLogLength, NULL, infoLog);
		std::cerr<<"Link log: "<<infoLog<<std::endl;
		delete [] infoLog;
	}

	glDeleteShader(_shaders[VERTEX_SHADER]);
	glDeleteShader(_shaders[FRAGMENT_SHADER]);
	glDeleteShader(_shaders[GEOMETRY_SHADER]);
}

void Shader::addAttribute(const std::string &attribute)
{
	_attributeList[attribute] = glGetAttribLocation(_program, attribute.c_str());
}

void Shader::addUniform(const std::string &uniform)
{
	_uniformLocationList[uniform] = glGetUniformLocation(_program, uniform.c_str());
}

//An indexer that returns the location of the attribute
GLuint Shader::operator [](const std::string& attribute) {
	return _attributeList[attribute];
}

GLuint Shader::operator()(const std::string& uniform){
	return _uniformLocationList[uniform];
}