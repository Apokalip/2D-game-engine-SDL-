#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <vector>
#include <iostream>

namespace Engine2D {

	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		//vertex shader
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0) {
			fatalError("Vertex shader failed to be created!");
		}
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0) {
			fatalError("Fragment shader failed to be created!");
		}
		compileShader(vertexShaderFilePath, _vertexShaderID);
		compileShader(fragmentShaderFilePath, _fragmentShaderID);
	}

	void GLSLProgram::linkShaders() {


		//Attach shaders to program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link program
		glLinkProgram(_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		//errorcheck
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			//don't need program anymore.
			glDeleteProgram(_programID);
			//Don't leak shaders
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader failed to link");

			return;
		}

		//Always detach shaders after a successful link
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

	}
	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());

	}
	GLint GLSLProgram::getUniformLocation(const std::string uniformName) {
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return location;
	}

	void GLSLProgram::use() {

		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unUse() {
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}
	void GLSLProgram::compileShader(const std::string & filePath, GLuint id) {
		_programID = glCreateProgram();
		std::ifstream vertexFile(filePath);
		if (vertexFile.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open" + filePath);
		}
		std::string fileContents = "";
		std::string line;
		while (std::getline(vertexFile, line)) {
			fileContents += line + "\n";
		}
		//
		//std::cout << fileContents << std::endl;
		//
		vertexFile.close();
		const char* contentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &contentsPtr, nullptr);
		glCompileShader(id);

		//check if compiled

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + filePath + " failed to compile");



			return;
		}


	}

}