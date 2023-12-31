#pragma once
#include<unordered_map>
#include<string>
#include"glm/glm.hpp"


struct ShaderSources
{
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader
{
private:
	std::string m_filepath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int num);
	void SetUniform1iv(const std::string& name, int count, int* value);
	void SetUniform1f(const std::string& name, float num);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, glm::mat4 matrix);
private:
	int GetUniformLocation(const std::string& name);
	ShaderSources parseShader(const std::string& filepath);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};