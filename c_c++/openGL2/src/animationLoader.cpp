#include "animationLoader.h"

#include<stdio.h>
#include<stdlib.h>

void loadAnim(keyFrame** keyFrames, subHeader** header, uint32_t* count, const char* filename, double currentTime)
{
	mainHeader mh;

	FILE* file = fopen(filename, "rb");
	if(file == NULL)
		return;
	fread(&mh, sizeof(mainHeader), 1, file);

	*header = (subHeader*)malloc(mh.objectCount*sizeof(subHeader));
	fread(*header, sizeof(subHeader), mh.objectCount, file);

	size_t size = 0;
	for(size_t i = 0; i < mh.objectCount; ++i)
	{
		size += (*header)[i].size+1;
	}

	*keyFrames = (keyFrame*)malloc(size*sizeof(keyFrame));
	fread(*keyFrames, size*sizeof(keyFrame), 1, file);

	for(size_t i = 0, k = 0; i < mh.objectCount; ++i)
	{
		double time = currentTime;
		for(size_t j = 0; j < (*header)[i].size;++j, ++k)
		{
			double oldTime = ((*keyFrames)[k]).timeSec;
			((*keyFrames)[k]).timeSec += time;
			time +=oldTime;
		}
	}

	*count = mh.objectCount;
}
