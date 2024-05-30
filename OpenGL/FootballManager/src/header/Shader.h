#pragma once
#include <string>
#include "DebugRenderer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
private:
	unsigned int id;
	std::string filepath;
	std::map<std::string, int> cache_location;

	ShaderProgramSource parseShader();
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	int getUniformLocation(const std::string& name);

public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unBind() const;

	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void setUniform1i(const std::string& name, int value);
};