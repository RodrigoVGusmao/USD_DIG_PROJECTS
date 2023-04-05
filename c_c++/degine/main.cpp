#include<iostream>
#include<stdexcept>
#include<vector>
#include<string>
#include<cmath>

#include<glew.h>
#include<SDL.h>
#include<SDL_opengl.h>
#include<SDL_image.h>

#include"thread.h"
#include"window.h"
#include"render.h"
#include"physics.h"

class objects : public degine::object::GLObjects
{
	public:
	objects()
	{
		position.assign({{-0.5, 0.5}});
		speed.assign({{0.0, 0.0}});
		texture.assign({0});
		program.assign({0});
	}
};

class render : public degine::render::GLRender
{
	public:
	render(degine::object::GLObjects& objects, int imgFlags,
						 degine::window::GLWindow& GLWindow) :
	GLRender(objects, imgFlags, GLWindow)
	{
		createStaticObject(&vertices[0], vertices.size()
							* sizeof(decltype(vertices)::value_type));
		createProgram(vertexShaderSource, fragmentShaderSource);
		createTexture("assets\\wall.jpg");
	}

	protected:
	std::string vertexShaderSource =
	"#version 410 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec3 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	"out vec3 ourColor;\n"
	"out vec2 TexCoord;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"	ourColor = aColor;\n"
	"	TexCoord = aTexCoord;\n"
	"}";
	std::string fragmentShaderSource =
	"#version 410 core\n"
	"in vec3 ourColor;\n"
	"in vec2 TexCoord;\n"
	"out vec4 FragColor;\n"
	"uniform sampler2D ourTexture;\n"
	"void main()\n"
	"{\n"
	"	FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n"
	"}";
	std::vector<float> vertices = {
		-0.5f, -0.5f, 0.0f, 	1.0f, 0.0f, 0.0f, 	0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f, 	1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 	0.0f, 0.0f, 1.0f, 	0.5f, 1.0f
	};
};

int main(int, char*[])
{
	try
	{
		degine::window::GLWindow window;
		objects objects;
		render render(objects, IMG_INIT_JPG, window);
		degine::physics::physics2D physics(objects);
		degine::thread::singleThread thread(0.0, window, render, physics);
		thread.mainLoop();
	}catch(std::exception& e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), NULL);
	}
	return 0;
}
