#ifndef GET_CACHE_LINE_SIZE_H
#define GET_CACHE_LINE_SIZE_H

#include<stdlib.h>

#if defined(_WIN32)
#include<windows.h>
size_t getCacheLine()
{
	DWORD bufferSize = 0;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION *processorInfoBuffer = NULL;
	GetLogicalProcessorInformation(processorInfoBuffer, &bufferSize);
	if(GetLastError() != ERROR_INSUFFICIENT_BUFFER) return 0;

	processorInfoBuffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(bufferSize);
	if(processorInfoBuffer == NULL) return 0;

	size_t cacheLineSize = 0;
	if(GetLogicalProcessorInformation(processorInfoBuffer, &bufferSize) == FALSE) goto GET_LOGICAL_PROCESSOR_INFO_ERROR;

	for(DWORD i = 0; i < (bufferSize/sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION)); ++i)
	{
		if(processorInfoBuffer[i].Relationship == RelationCache && processorInfoBuffer[i].Cache.Level == 1)
		{
			cacheLineSize = processorInfoBuffer[i].Cache.LineSize;
			break;
		}
	}

	GET_LOGICAL_PROCESSOR_INFO_ERROR:
	free(processorInfoBuffer);
	return cacheLineSize;
}
#else
size_t getCacheLine()
{
	return 0;
}
#endif

#endif
