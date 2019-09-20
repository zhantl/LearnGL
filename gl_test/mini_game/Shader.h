#ifndef Shader_H
#define Shader_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int ID;
	Shader() {};
	Shader& Use();
	void Compile(const char* vData, const char* fData, const char* gData = nullptr);

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);
	// --------------------------------glm----------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
private:
	void checkCompileErrors(GLuint Shader, std::string type);
};

#endif //Shader_H