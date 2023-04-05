#include"vector_c.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>

void* insertVoid(void_v* vec, size_t dataSize, const void* value)
{
	if(value == NULL || vec == NULL) goto error;

	vec->size += dataSize;
	if(vec->size > vec->reserve)
	{
	    if(vec->reserve == 0 && vec->size > 0)
            vec->reserve += 1;
		while(vec->size >= (vec->reserve <<= 1));
		void* new_memory;
		if((new_memory = realloc(vec->buffer, vec->reserve)) == NULL) goto error;
		vec->buffer = new_memory;
	}

	for(size_t i = 0;dataSize > i; ++i)
	{
		((int8_t*)(vec->buffer))[vec->size-dataSize+i] = ((int8_t*)(value))[i];
	}

	return vec->buffer;

	error:
	return NULL;
}

int getLastVoid(void_v* vec, size_t dataSize, void* value)
{
	if(value == NULL || vec == NULL) return 1;
	if(vec->size == 0) return 1;

	vec->size -= dataSize;

	for(size_t i = 0;dataSize > i; ++i)
	{
		((int8_t*)(value))[i] = ((int8_t*)(vec->buffer))[vec->size+i];
	}
	return 0;
}

int getVoidAt(void_v* vec, size_t dataSize, size_t index, void* value)
{
	if(vec->size == 0 || value == NULL || vec == NULL || vec->size < (index+1)*dataSize) return 1;

	for(size_t i = 0;dataSize > i; ++i)
	{
		((int8_t*)(value))[i] = ((int8_t*)(vec->buffer))[index*dataSize+i];
	}

	return 0;
}

int setVoidAt(void_v* vec, size_t dataSize, size_t index, void* value)
{
	if(vec->size == 0 || value == NULL || vec == NULL || vec->size < (index+1)*dataSize) return 1;

	for(size_t i = 0;dataSize > i; ++i)
	{
		((int8_t*)(vec->buffer))[index*dataSize+i] = ((int8_t*)(value))[i];
	}

	return 0;
}

void unsupported(const char* command)
{
	fprintf(stderr, "%s is unsupported.\n", command);
}

void* createTempArray(size_t argc, size_t typeSize, ...)
{
	if(argc == 0 || typeSize == 0) return NULL;
	void* new_memory = malloc(argc*typeSize);
	if(new_memory == NULL) return NULL;
	va_list argv;
	va_start(argv, typeSize);
	for(size_t i = 0; argc > i; ++i)
	{
		switch(typeSize)
		{
		case 1:
			((int8_t*)(new_memory))[i] = va_arg(argv, int64_t);
			break;
		case 2:
			((int16_t*)(new_memory))[i] = va_arg(argv, int64_t);
			break;
		case 4:
			((int32_t*)(new_memory))[i] = va_arg(argv, int64_t);
			break;
		case 8:
			((int64_t*)(new_memory))[i] = va_arg(argv, int64_t);
			break;
		default:
			unsupported("type_v");
			free(new_memory);
			return NULL;
		}
	}
	return new_memory;
}

void* reserveVoid(vector_v* vec, size_t dataSize, size_t count)
{
	if(vec->reserve < dataSize*count)
	{
		void* new_memory;
		if((new_memory = realloc(vec->buffer, dataSize*count)) == NULL) goto error;
		vec->buffer = new_memory;
		vec->reserve = dataSize*count;
	}

	return vec->buffer;

	error:
		return NULL;
}

void destroyVector(void_v* vec)
{
	vec->reserve = 0;
	vec->size = 0;
	free(vec->buffer);
	vec->buffer = NULL;
}

data_v* _vinitData(data_v* data_vec, const char* format, va_list argv)
{
	initVoidVector(&data_vec->data);

	char* end = NULL;
	while(format != end)
	{
		long size = strtol(format, &end, 0);
		int64_t value;
		switch(size)
		{
		case 1:
			value = va_arg(argv, int64_t);
			break;
		case 2:
			value = va_arg(argv, int64_t);
			break;
		case 4:
			value = va_arg(argv, int64_t);
			break;
		case 8:
			value = va_arg(argv, int64_t);
			break;
        default:
            unsupported("type_v");
            destroyData(data_vec);
            return NULL;
		}
		//if(insertVoid(&(data_vec.data), size, value) == NULL) goto error;
		printf("testify");
		if(insertDataVoid(data_vec, size, &value) == NULL) goto error;
	}

	return data_vec;

	error:
	destroyData(data_vec);
	return data_vec;
}

data_v* initData(data_v* data_vec, const char* format,...)
{
	va_list argv;
	va_start(argv, format);
	data_v* return_value = _vinitData(data_vec, format, argv);
	va_end(argv);

	return return_value;
}

void* insertDataVoid(data_v* data_vec, size_t dataSize, void* value)
{
	if(insertValue(&(data_vec->dataType), int8_t, &dataSize) == NULL) return NULL;
	if(insertVoid(&data_vec->data, dataSize, value) == NULL) return NULL;

	return data_vec;
}

void destroyData(data_v* data_vec)
{
	destroyVector(&data_vec->data);
	destroyVector(&data_vec->dataType);
}

#ifdef __DEBUG_VECTOR_C__

void printfVector(const char* msg, vector_v vec)
{
    puts(msg);
    printf("buffer: %p\nsize: %d\nreserve: %d\nvalues:\n", vec.buffer, vec.size, vec.reserve);
    for(size_t i = 0; i < vec.size/sizeof(int) && vec.buffer != NULL; ++i)
    {
        printf("\t[%d]: %d\n", i, ((int*)(vec.buffer))[i]);
    }
    puts("finished...\n");
}

void printfData(const char* msg, data_v data)
{
    puts(msg);
    printf("data:\n\tbuffer: %p\n\tsize: %d\n\treserve: %d\ndata type:\n\tbuffer: %p\n\tsize: %d\n\treserve: %d\nvalues:\n",
           data.data.buffer, data.data.size, data.data.reserve, data.dataType.buffer, data.dataType.size, data.dataType.reserve);
    int8_t* buffer = data.data.buffer;
    for(size_t i = 0; i < data.dataType.size/sizeof(int8_t) && data.dataType.buffer != NULL; ++i)
    {
        printf("\t[%d]: %d\t", i, (int)((int8_t*)(data.dataType.buffer))[i]);
        switch(((int8_t*)(data.dataType.buffer))[i])
        {
        case 1:
            printf("%d\n", i, (int)*((int8_t*)buffer));
            buffer += 1;
            break;
        case 2:
            printf("%d\n", i, (int)*((int16_t*)buffer));
            buffer += 2;
            break;
        case 4:
            printf("%d\n", i, (int)*((int32_t*)buffer));
            buffer += 4;
            break;
        case 8:
            printf("%d\n", i, (int)*((int64_t*)buffer));
            buffer += 8;
            break;
        }
    }
    puts("finished...\n");
}

int main()
{
    vector_v vec;
    initVoidVector(&vec);
    printfVector("creating vector...", vec);

    for(size_t k = 0; k < 2; ++k)
    {
        int value = 58;
        insertValue(&vec, int, &value);
        printfVector("inserting value...", vec);

        int valueArr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        insertVoid(&vec, sizeof(valueArr), &valueArr);
        printfVector("inserting array...", vec);

        getLastValue(&vec, int, &value);
        printf("getting last value...\nlast value: %d\n", value);
        printfVector(NULL, vec);

        getLastVoid(&vec, sizeof(valueArr), &valueArr);
        printf("getting last array...\nlast array: \n");
        for(size_t i = 0; i < sizeof(valueArr)/sizeof(int); ++i)
        {
            printf("\t[%d]: %d\n", i, valueArr[i]);
        }
        printfVector(NULL, vec);

        insertVoid(&vec, sizeof(valueArr), &valueArr);
        printfVector("inserting array...", vec);

        getValueAt(&vec, int, 3, &value);
        printf("getting 3rd value...\nlast value: %d\n", value);
        printfVector(NULL, vec);

        value = 25;
        setValueAt(&vec, int, 3, &value);
        printfVector("setting 3rd value...", vec);

        reserveVector(&vec, int, 100);
        printfVector("reserving 100 ints...", vec);

        reserveVoid(&vec, sizeof(int), 200);
        printfVector("reserving 200 ints...", vec);

        destroyVector(&vec);
        printfVector("deleting vector...", vec);
        printf("------------------------------------------------------------------\n\n");

        if(k == 0)
        {
            initVector(&vec, 5, int, 0, 1, 2, 3, 4);
            printfVector("creating vector...", vec);
        }
    }

    initVector(&vec, 5, vector_v, 0, 1, 2, 3, 4);
    printfVector("creating vector...", vec);

    destroyVector(&vec);
    printfVector("deleting vector...", vec);

    return 0;
}

#endif
