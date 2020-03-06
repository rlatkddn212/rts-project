/**
 * Shader
 * 쉐이더 클래스
 */

#pragma once
#include <vector>
#include <map>

class Shader
{
public:
											Shader();
										   ~Shader();

	void									SetActive();

	void									SetMatrixUniform(const char* name, const glm::mat4& matrix);
	void									SetVectorUniform(const char* name, const glm::vec3& vector);
	void									SetVector4Uniform(const char* name, const glm::vec4& vector);
	void									SetFloatUniform(const char* name, float value);
	void									SetIntUniform(const char* name, int value);

	GLuint									BuildShader(std::vector<std::pair<std::string, int> >& shaderCodies);

	GLuint									mProgramID;
};

std::string ReadShaderFile(const char* filePath);