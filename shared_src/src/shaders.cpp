#include <algorithm>

#include "fmt/format.h"

#include "shaders.h"
#include "glfw_wrapper.h"
#include "filesystem_utils.h"

using namespace Shaders;

Shaders::Shader::Shader(ShaderType type)
{
	m_shaderID = glCreateShader(type);
}

Shaders::Shader::Shader(Shader &&moveValue)
{
	m_shaderID = moveValue.m_shaderID;
	moveValue.m_shaderID = -1;
}

Shaders::Shader &Shaders::Shader::operator=(Shaders::Shader &&moveValue)
{
	if(this == &moveValue)
	{
		return *this;
	}

	m_shaderID = moveValue.m_shaderID;
	moveValue.m_shaderID = -1;

	return *this;
}

Shaders::Shader::~Shader()
{
	if(m_shaderID != 0)
	{
		glDeleteShader(m_shaderID);
	}
}

bool Shaders::Shader::CompileShader(const std::string &source)
{
	if(m_shaderID == 0)
	{
		return false;
	}

	const char *textPtr = source.c_str();

	glShaderSource(m_shaderID, 1, &textPtr, NULL);
	glCompileShader(m_shaderID);

	int success{0};
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(m_shaderID, 512, NULL, infoLog);
		std::cerr << fmt::format("Failed to compile shader.\nErorr msg: {}\n", infoLog);
		return false;
	}
	return true;
}

std::optional<std::vector<Shader>> Shaders::CompileShaders(const std::vector<Shader::CreationParams> &typeSourcePairs)
{
	std::vector<Shader> shaders;
	shaders.reserve(typeSourcePairs.size());

	for(const auto &[type, source] : typeSourcePairs)
	{
		Shader &createdShader = shaders.emplace_back(type);
		if(!createdShader.CompileShader(source))
		{
			std::cerr << fmt::format("Error while compiling source:\n{}\n", source);
		}
	}

	return shaders;
}

std::optional<std::vector<Shader>> Shaders::LoadAndCompileShaders(const std::vector<Shader::CreationParams> &typeSourcePathPairs)
{
	std::vector<Shader> shaders;
	shaders.reserve(typeSourcePathPairs.size());

	for(const auto &[type, filePath] : typeSourcePathPairs)
	{
		auto source = Filesystem::LoadFile(filePath);
		if(!source)
		{
			return std::nullopt;
		}
		Shader &lastShader = shaders.emplace_back(type);
		if(!lastShader.CompileShader(*source))
		{
			std::cerr << fmt::format("In file: {}\n{}\n", filePath, *source);
			return std::nullopt;
		}
	}

	return shaders;
}

Shaders::ShaderProgram::ShaderProgram()
{
	m_programID = glCreateProgram();
}
ShaderProgram::ShaderProgram(ShaderProgram &&moveValue)
{
	m_programID = moveValue.m_programID;
	moveValue.m_programID = -1;
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&moveValue)
{
	if(this == &moveValue)
	{
		return *this;
	}

	m_programID = moveValue.m_programID;
	moveValue.m_programID = -1;

	return *this;
}
Shaders::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_programID);
}
void Shaders::ShaderProgram::UseProgram() const
{
	glUseProgram(m_programID);
}
bool Shaders::ShaderProgram::LinkToShaders(const std::vector<Shader> &shaders)
{
	for(const Shader &shader : shaders)
	{
		glAttachShader(m_programID, shader.GetShaderID());
	}

	glLinkProgram(m_programID);

	int success;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

	if(!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_programID, 512, NULL, infoLog);

		fmt::print("Failed to link shader program.\nErorr msg: {}\n", infoLog);
		return false;
	}

	return true;
}
