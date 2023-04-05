#include"render.h"

#include<string>
#include<algorithm>
#include<stdexcept>

#include"ctrycatch.h"

degine::render::GLRender::GLRender(object::GLObjects& objects, int imgFlags,
								window::GLWindow& GLWindow)
: render(objects, imgFlags),
glWindow(GLWindow.getWindow())
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);

	glGenVertexArrays(VAO_SIZE, VAO);
	glGenBuffers(VBO_SIZE, VBO);
}

degine::render::GLRender::~GLRender()
{
	glDeleteVertexArrays(VAO_SIZE, VAO);
	glDeleteBuffers(VBO_SIZE, VBO);
	std::for_each(programs.begin(), programs.end(), glDeleteProgram);
}

void degine::render::GLRender::createStaticObject(GLfloat* vertices,
												size_t verticesSize,
												int count)
{
	glBindVertexArray(VAO[VAO_STATIC]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[VBO_STATIC]);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat),
						  reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat),
						  reinterpret_cast<void*>(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat),
						  reinterpret_cast<void*>(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

void degine::render::GLRender::createProgram(std::string& vertexShaderSource,
											std::string& fragmentShaderSource)
{
	GLuint shader;
	TRY
	{
		shader = glCreateShader(GL_VERTEX_SHADER);
		const char* source = vertexShaderSource.c_str();
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		THROW_ON_FAIL(!success, GLRD_ERR_SHADER_COMPILE_ERROR);

		GLuint vertexShader = shader;

		shader = glCreateShader(GL_FRAGMENT_SHADER);
		source = fragmentShaderSource.c_str();
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		THROW_ON_FAIL(!success, GLRD_ERR_SHADER_COMPILE_ERROR);

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, shader);

		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		THROW_ON_FAIL(!success, GLRD_ERR_SHADER_LINK_ERROR);

		glDeleteShader(vertexShader);
		glDeleteShader(shader);

		programs.push_back(shaderProgram);

		return;
	}CATCH(GLRD_ERR_SHADER_LINK_ERROR, NULL)
	{
		char infoLog[512];
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		WHAT = (std::string("Failed to link shader program:\n")
				+ infoLog).c_str();
	}CATCH(GLRD_ERR_SHADER_COMPILE_ERROR, NULL)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		WHAT = (std::string("Failed to compile shader:\n")
				+ infoLog).c_str();

		throw std::runtime_error(WHAT);
	}
}

void degine::render::GLRender::createTexture(std::string filePath)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	SDL_Surface* surface = loadImage(filePath);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	textures.push_back(texture);
}

void degine::render::GLRender::renderStaticLoop(double)
{
	glClear(GL_COLOR_BUFFER_BIT);
	for(size_t i = 0; i < objects.position.size(); ++i)
	{
		glUseProgram(programs[
					reinterpret_cast<object::GLObjects&>(objects).program[i]]);
		glBindTexture(GL_TEXTURE_2D, textures[objects.texture[i]]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	SDL_GL_SwapWindow(glWindow);
}

void degine::render::GLRender::renderLoop(double timeSpan)
{
	renderStaticLoop(timeSpan);
}
