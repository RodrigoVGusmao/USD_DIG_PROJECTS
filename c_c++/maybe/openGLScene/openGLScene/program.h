#pragma once

#include "scene.h"
#include"shaderProgram.h"

typedef GLFWwindow* Window;

typedef struct {
	sceneStruct scene;
	Window window;
} programStruct;

void debugMessage(const char* msg, ...);
bool startup(programStruct* data);
bool loop(programStruct data);
void cleanup(programStruct data);
