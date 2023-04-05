#ifndef RENDER_H
#define RENDER_H

#include<vector>
#include<string>

#include<SDL.h>
#include<glew.h>
#include<SDL_opengl.h>

#include"window.h"

#define VBO_SIZE 1
#define VAO_SIZE 1
#define VBO_STATIC 0
#define VAO_STATIC 0

#include"object.h"

//renders scene
namespace degine
{
	namespace render
	{
		class render
		{
			public:
			render(object::objects& objects, int imgFlags);
			virtual ~render();
			virtual void renderLoop(double) {};

			SDL_Surface* loadImage(std::string filePath);

			protected:
			object::objects& objects;

			private:
			static unsigned int refCount;
		};

		class GLRender : public render
		{
			public:
			GLRender(object::GLObjects& objects, int imgFlags,
						 window::GLWindow& GLWindow);
			virtual ~GLRender();

			void createStaticObject(GLfloat* vertices, size_t verticesSize,
									int count = 1);
			void createProgram(std::string& vertexShaderSource,
								std::string& fragmentShaderSource);
			void createTexture(std::string filePath);

			virtual void renderLoop(double timeSpan);
			virtual void renderStaticLoop(double);


			protected:
			SDL_Window* glWindow;
			std::vector<GLuint> programs;
			GLuint VBO[VBO_SIZE];
			GLuint VAO[VAO_SIZE];

			std::vector<GLuint> textures;
		};
	}
}

#endif // RENDER_H
