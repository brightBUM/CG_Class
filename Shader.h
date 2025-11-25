#pragma once
#include <glad/glad.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
private:
	void checkCompileErrors(unsigned int shader, std::string type);
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, glm::mat4 matrix) const;
};

