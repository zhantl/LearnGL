#include <iostream>
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader& Shader::Use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::Compile(const char* vData, const char* fData, const char* gData)
{
	unsigned int vertex, fragment, geometry;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vData, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fData, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	if (gData != "")
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gData, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	if (gData != "")
	{
		glAttachShader(this->ID, geometry);
	}
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (gData != "")
	{
		glDeleteShader(geometry);
	}
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

// --------------------------------glm----------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value, int count) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), count, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value, int count) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value, int count) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), count, glm::value_ptr(value));
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat, int count) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), count, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat, int count) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), count, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat, int count) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, GL_FALSE, glm::value_ptr(mat));
}

void Shader::checkCompileErrors(GLuint Shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(Shader, 1024, NULL, infoLog);
			std::cout << "ERROR::Shader_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(Shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(Shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}