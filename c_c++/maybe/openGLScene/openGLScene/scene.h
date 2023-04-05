#pragma once

#include"shaderProgram.h"
#include"camera.h"

typedef struct {
	cameraStruct camera;
	unsigned int rectangleVBO, rectangleVAO, rectangleEBO;
} sceneStruct;

bool startupScene(sceneStruct* data, float aspect);
void renderScene(sceneStruct data);
void cleanupScene(sceneStruct data);