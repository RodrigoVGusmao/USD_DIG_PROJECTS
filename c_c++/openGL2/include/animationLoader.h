#ifndef ANIMATIONLOADER_H
#define ANIMATIONLOADER_H

#include<stdint.h>

typedef struct {
	uint32_t header;
	uint32_t objectCount;
	uint32_t objectAnimOffset;
	uint32_t reserved;
} mainHeader;

typedef struct
{
	float timeSec, radius;
    float angleX, angleY, angleZ;
    float transX, transY, transZ;
    float transAngX, transAngY, transAngZ;
    float reserved;
} keyFrame;

typedef struct {
	uint32_t offset;
	uint32_t size;
	int32_t index;
	float angleX, angleY, angleZ;
    float transAngleX, transAngleY, transAngleZ;
    float transX, transY, transZ;
}subHeader;

typedef struct {
	keyFrame* key;
    subHeader* sh;
    uint32_t count;
}animation;


void loadAnim(keyFrame** keyFrames, subHeader** header, uint32_t* count, const char* filename, double currentTime);

#endif // ANIMATIONLOADER_H
