#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "FileUtils.hpp"

class Shader
{
public:
	unsigned int ID;
	Shader(const char *vertexFile, const char *fragmentFile, const char *geometryFile = nullptr);
	void use();
	GLint getAtrributeLocation(const std::string &name);
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);

	// --------------------------------glm----------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value, int count=1) const;
	void setVec3(const std::string &name, const glm::vec3 &value, int count=1) const;
	void setVec4(const std::string &name, const glm::vec4 &value, int count=1) const;
	void setMat2(const std::string &name, const glm::mat2 &mat, int count=1) const;
	void setMat3(const std::string &name, const glm::mat3 &mat, int count=1) const;
	void setMat4(const std::string &name, const glm::mat4 &mat, int count=1) const;

private:
	void checkCompileErrors(GLuint shader, std::string type);
};
