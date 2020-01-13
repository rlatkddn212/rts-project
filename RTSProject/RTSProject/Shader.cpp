#include "Precompiled.h"
#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

GLuint Shader::BuildShader(std::vector<std::pair<std::string, int> >& shaderCodies)
{
	mProgramID = glCreateProgram();
	GLint Result = GL_FALSE;
	int InfoLogLength;

	std::vector<int> shaderIdVec;
	// 버텍스 쉐이더를 컴파일
	printf("Compiling shader\n");
	for (int i = 0; i < shaderCodies.size(); ++i)
	{
		GLuint shaderID = glCreateShader(shaderCodies[i].second);

		shaderIdVec.push_back(shaderID);

		char const * shaderPointer = shaderCodies[i].first.c_str();
		glShaderSource(shaderID, 1, &shaderPointer, NULL);
		glCompileShader(shaderID);

		// 버텍스 쉐이더를 검사
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0)
		{
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}

		glAttachShader(mProgramID, shaderID);
	}

	// 링크
	glLinkProgram(mProgramID);


	// 프로그램 검사
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(mProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	for (int i = 0; i < shaderIdVec.size(); ++i)
	{
		glDetachShader(mProgramID, shaderIdVec[i]);
		glDeleteShader(shaderIdVec[i]);
	}

	return mProgramID;
}

void Shader::SetActive()
{
	glUseProgram(mProgramID);
}

void Shader::SetMatrixUniform(const char* name, const glm::mat4& matrix)
{
	GLuint loc = glGetUniformLocation(mProgramID, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, (&matrix[0][0]));
}

void Shader::SetVectorUniform(const char* name, const glm::vec3& vector)
{
	GLuint loc = glGetUniformLocation(mProgramID, name);
	glUniform3fv(loc, 1, &vector[0]);
}

void Shader::SetVector4Uniform(const char* name, const glm::vec4& vector)
{
	GLuint loc = glGetUniformLocation(mProgramID, name);
	glUniform4fv(loc, 1, &vector[0]);
}

void Shader::SetFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(mProgramID, name);
	// Send the float data
	glUniform1f(loc, value);
}

std::string ReadShaderFile(const char* filePath)
{
	std::string shaderCode;
	std::ifstream shaderStream(filePath, std::ios::in);
	if (shaderStream.is_open()) {
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}
	else
	{
		printf("파일 %s 를 읽을 수 없음.\n", filePath);
	}

	return shaderCode;
}