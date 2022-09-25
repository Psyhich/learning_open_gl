#include <array>
#include <iostream>
#include <vector>

#include "fmt/format.h"

#include "glfw_wrapper.h"
#include "state.hpp"
#include "shaders.h"

using RenderAssets =
	std::vector<std::tuple<State, Shaders::ShaderProgram>>;

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
			std::array<float, 12> vertices
			{
				0.5f, 0.5f, 0.0f, // top right
				0.5f, -0.5f, 0.0f, // bottom right
				-0.5f, -0.5f, 0.0f, // bottom left
				-0.5f, 0.5f, 0.0f // top left
			};

			GLuint VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * vertices.size(),
				vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT,
					GL_FALSE, 3 * sizeof(float),
					(void*)0);
			glEnableVertexAttribArray(0);

			std::array<unsigned int, 6> indices
			{ // note that we start from 0!
				0, 1, 3, // first triangle
				1, 2, 3 // second triangle
			};
			GLuint EBO;
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(unsigned int) * indices.size(),
				indices.data(), GL_STATIC_DRAW);
			// We can bind EBO to VAO the same as VBO, 
			// but we should remember count of values 
			// in indices array
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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


	assets.push_back(
		{
			std::move(state), 
			std::move(program)
		});
	return assets;
}

void Render(const RenderAssets &assets)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for(const auto &asset : assets)
	{
		std::get<1>(asset).UseProgram();
		std::get<0>(asset).UseState();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
