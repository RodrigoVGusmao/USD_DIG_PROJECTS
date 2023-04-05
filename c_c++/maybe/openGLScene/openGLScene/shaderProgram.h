#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>

typedef struct {
	unsigned int basicShader;
	int modelLocation, viewLocation, projectionLocation;
} shaderStruct;

bool createShaderPrograms(shaderStruct* data);
void deleteShaderPrograms(shaderStruct data);