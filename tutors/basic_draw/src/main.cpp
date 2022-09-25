#include <array>
#include <iostream>
#include <vector>

#include "fmt/format.h"

#include "glfw_wrapper.h"
#include "state.hpp"
#include "shaders.h"

using RenderAssets = std::vector<std::pair<State, Shaders::ShaderProgram>>;

void ProcessInput(GLFW::Window &window);
// Will return array of VAOs and their shaders
std::optional<RenderAssets> SetUpRender();
void Render(const RenderAssets &assets);

int main()
{
	GLFW::GLFWContext context;
	if(!context.Init())
	{
		fmt::print("Error during context creation!");
		return -1;
	}

	GLFW::Window window;
	if(!window.Init(640, 480, "Basic draw"))
	{
		fmt::print("Error during window creation!\n");
		return -1;
	}

	const auto assets = SetUpRender();
	if(!assets)
	{
		fmt::print("Render assets creation failed!\n");
		return -1;
	}

	// Running main loop
	while(!window.ShouldClose())
	{
		ProcessInput(window);

		Render(*assets);

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	return 0;
}

void ProcessInput(GLFW::Window &window)
{
	if(glfwGetKey(window.GetWindow(), GLFW_KEY_ESCAPE) ==
		GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window.GetWindow(), true);
	}
}

// Will return array of VAOs and their shaders
std::optional<RenderAssets> SetUpRender()
{
	RenderAssets assets;

	State state;
	state.RunInState([]()
		{
			std::array<float, 9> vertices
			{
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				0.0f, 0.5f, 0.0f
			};

			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// • GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
			// • GL_STATIC_DRAW: the data is set only once and used many times.
			// • GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * vertices.size(),
				vertices.data(), GL_STATIC_DRAW);

			// Passing pointer of vertices array in GPU 
			// to vertex shader as parameter with location
			// set to 0
			glVertexAttribPointer(0, 3, GL_FLOAT,
					GL_FALSE, 3 * sizeof(float),
					(void*)0);
			glEnableVertexAttribArray(0);
		});

	Shaders::ShaderProgram program;
	if(auto compiledShader =
		Shaders::LoadAndCompileShaders({
			{
				Shaders::Shader::ShaderType::VertexShader,
				SHADERS_PATH "vertex.glsl"
			},
			{
				Shaders::Shader::ShaderType::FragmentShader,
				SHADERS_PATH "fragment.glsl"
			}
		}))
	{
		program.LinkToShaders(*compiledShader);
	}
	else
	{
		fmt::print("Shader creation failed!\n");
		return std::nullopt;
	}

	assets.push_back({std::move(state), std::move(program)});
	return assets;
}

void Render(const RenderAssets &assets)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for(const auto &asset : assets)
	{
		asset.second.UseProgram();
		asset.first.UseState();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
