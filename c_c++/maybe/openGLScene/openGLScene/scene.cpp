#include "scene.h"

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<stdlib.h>
#include<string.h>

bool startupScene(sceneStruct* data, float aspect)
{
	float vertices[] = {
		-.5f, -.5f, .0f, 1.0f, 1.0f, 0.0f,
		 .5f, -.5f, .0f, 0.0f, 1.0f, 1.0f,
		-.5f,  .5f, .0f, 1.0f, 0.0f, 1.0f,
		 .5f,  .5f, .0f, 1.0f, 1.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};
	
	glGenVertexArrays(1, &data->rectangleVAO);
	glGenBuffers(1, &data->rectangleVBO);
	glGenBuffers(1, &data->rectangleEBO);

	glBindVertexArray(data->rectangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, data->rectangleVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->rectangleEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0*sizeof(float)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	glUseProgram(data->camera.program.basicShader);

	cameraInit(&data->camera, aspect);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}

void renderScene(sceneStruct data)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	cameraLoop(&data.camera);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void cleanupScene(sceneStruct data)
{
	glDeleteBuffers(1, &data.rectangleEBO);
	glDeleteBuffers(1, &data.rectangleVBO);
	glDeleteVertexArrays(1, &data.rectangleVAO);
}