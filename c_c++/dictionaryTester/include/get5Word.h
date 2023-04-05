#ifndef GET_5_WORD_H
#define GET_5_WORD_H

#include<stdint.h>

typedef void* dictionary;
typedef uint64_t regString;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
extern __declspec(dllexport) dictionary getDictionary(const wchar_t* __restrict__ filename, size_t* size);
extern __declspec(dllexport) regString get5Word(dictionary* dictionaryH);
extern __declspec(dllexport) int freeDictionary(dictionary dictionaryStart);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif
