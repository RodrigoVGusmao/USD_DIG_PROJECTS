#pragma once

#include<glm.hpp>
#include"shaderProgram.h"

typedef struct {
	shaderStruct program;
	glm::mat4 model, view, projection;
} cameraStruct;

void cameraInit(cameraStruct* data, float aspect);
void cameraLoop(cameraStruct* data);