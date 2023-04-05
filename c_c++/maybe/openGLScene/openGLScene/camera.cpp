#include "camera.h"

#include<gtc/type_ptr.hpp>
#include<gtc/matrix_transform.hpp>

float fAspect;

void cameraInit(cameraStruct* data, float aspect)
{
	fAspect = aspect;
}

void cameraLoop(cameraStruct* data)
{
	data->projection = glm::mat4(1.f);
	data->projection = glm::perspective(glm::radians(45.f), fAspect, .1f, 100.f);
	glUniformMatrix4fv(data->program.projectionLocation, 1, GL_FALSE, glm::value_ptr(data->projection));

	data->model = glm::mat4(1.f);
	data->model = glm::rotate(data->model, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
	data->view = glm::mat4(1.f);
	data->view = glm::translate(data->view, glm::vec3(.0f, .0f, -3.f));
	glUniformMatrix4fv(data->program.modelLocation, 1, GL_FALSE, glm::value_ptr(data->model));
	glUniformMatrix4fv(data->program.viewLocation, 1, GL_FALSE, glm::value_ptr(data->view));
}