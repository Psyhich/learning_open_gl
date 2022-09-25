#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <vector>
#include <optional>

#include "glfw_wrapper.h"

namespace Shaders
{
	using ShaderProgramID = unsigned int;
	using ShaderID = unsigned int;


	struct Shader
	{
	public:
		enum ShaderType
		{
			VertexShader = GL_VERTEX_SHADER,
			FragmentShader = GL_FRAGMENT_SHADER
		};
		using CreationParams = std::pair<ShaderType, std::string>;

		Shader(ShaderType type);
		Shader(const Shader &copyValue) = delete;
		Shader(Shader &&moveValue);

		Shader &operator=(const Shader &copyValue) = delete;
		Shader &operator=(Shader &&moveValue);

		~Shader();

		inline ShaderID GetShaderID() const noexcept
		{
			return m_shaderID;
		}

		bool CompileShader(const std::string &source);
	private:
		ShaderID m_shaderID;
	};

	std::optional<std::vector<Shader>> CompileShaders(const std::vector<Shader::CreationParams> &typeSourcePairs);
	std::optional<std::vector<Shader>> LoadAndCompileShaders(const std::vector<Shader::CreationParams> &typeSourcePathPairs);

	struct ShaderProgram
	{
	public:
		ShaderProgram();

		ShaderProgram(const ShaderProgram &copyValue) = delete;
		ShaderProgram(ShaderProgram &&moveValue);

		ShaderProgram &operator=(const ShaderProgram &copyValue) = delete;
		ShaderProgram &operator=(ShaderProgram &&moveValue);


		~ShaderProgram();

		inline const ShaderProgramID GetProgramID() const noexcept
		{
			return m_programID;
		}

		void UseProgram() const;
		bool LinkToShaders(const std::vector<Shader> &shaders);
	private:
		ShaderProgramID m_programID;
	};
}

#endif // SHADERS_H
