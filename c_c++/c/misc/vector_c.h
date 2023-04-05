#ifndef _VECTOR_C_H
#define _VECTOR_C_H

#include<stdint.h>

///simple vector

typedef struct
{
	size_t reserve;
	size_t size;
	void* buffer;
} vector_v;

typedef vector_v void_v;

typedef vector_v byte_v;
typedef vector_v char_v;
typedef vector_v int8_v;
typedef vector_v uint8_v;

typedef vector_v word_v;
typedef vector_v wchar_v;
typedef vector_v short_v;
typedef vector_v int16_v;
typedef vector_v uint16_v;

typedef vector_v dword_v;
typedef vector_v int_v;
typedef vector_v int32_v;
typedef vector_v uint32_v;
typedef vector_v float_v;

typedef vector_v qword_v;
typedef vector_v long_v;
typedef vector_v int64_v;
typedef vector_v uint64_v;
typedef vector_v double_v;
typedef vector_v size_v;
typedef vector_v ptr_v;

const vector_v NULL_v = {0};

#define insertValue(pVec, type, pValue) insertVoid(pVec, sizeof(type), pValue)
#define getLastValue(pVec, type, pValue) getLastVoid(pVec, sizeof(type), pValue)
#define getValueAt(pVec, type, index, pValue) getVoidAt(pVec, sizeof(type), index, pValue)
#define setValueAt(pVec, type, index, pValue) setVoidAt(pVec, sizeof(type), index, pValue)

void* insertVoid(void_v* vec, size_t dataSize, const void* value);
int getLastVoid(void_v* vec, size_t dataSize, void* value);
int getVoidAt(void_v* vec, size_t dataSize, size_t index, void* value);
int setVoidAt(void_v* vec, size_t dataSize, size_t index, void* value);
void unsupported(const char* command);
void* createTempArray(size_t argc, size_t typeSize, ...);

#define initVector(pVec, argc, type, ...) {void* DummyVectorVoidPointer__ = createTempArray((argc), sizeof(type), __VA_ARGS__); \
(pVec)->reserve = argc*sizeof(type); (pVec)->size = argc*sizeof(type); \
(pVec)->buffer = DummyVectorVoidPointer__;}
#define initVoidVector(pVec) {((vector_v*)pVec)->reserve = 0; ((vector_v*)pVec)->size = 0; ((vector_v*)pVec)->buffer = NULL;}

#define reserveVector(pVec, type, count) reserveVoid(pVec, sizeof(type), count)

void* reserveVoid(vector_v* vec, size_t dataSize, size_t count);
void destroyVector(void_v* vec);

///data vector

typedef struct
{
	vector_v data;
	int8_v dataType;
} data_vector_v, data_v;

#define insertData(pDataVec, type, pValue) insertDataVoid(pDataVec, sizeof(type), (void*)pValue)
#define initVoidData(pDataVec) {initVoidVector(&((pDataVec)->data)); initVoidVector(&((pDataVec)->dataType));}

data_v* _vinitData(data_v* data_vec, const char* format, va_list argv);
data_v* initData(data_v* data_vec, const char* format,...);
void* insertDataVoid(data_v* data_vec, size_t dataSize, void* value);
void destroyData(data_v* data_vec);

#endif // _VECTOR_C_H
