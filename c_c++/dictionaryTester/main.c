#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<inttypes.h>
#include<string.h>

#include"include/get5Word.h"

const wchar_t path[] = L"dictionary.txt";
typedef struct {
    uint64_t* directory;
    uint64_t* folders;
} directory;

directory CreateDirectory(size_t folders);
void InsertInFolder(directory directory, size_t folderIndex, uint64_t data);

int main()
{
    size_t size;
    dictionary wordList = getDictionary(path, &size);
    if(wordList == NULL) goto USD_ERR_GET_DICTIONARY;

    dictionary iter = wordList;
    dictionary end = size + wordList;

    directory directory = CreateDirectory(1<<26);

    int count = 0;
    while(iter < end)
    {
        regString word = get5Word(&iter);
        if(word != 0)
        {
            char* string = ((char*)(&word));
            printf("%"PRIx64"\t\t%d\t\t%s\n", word, ++count, string);
            int32_t wordMap = 0;
            for(int i = 0; i < 5; ++i)
            {
                int32_t bit = 1 << string[i] - 'a'; //assumes dictionary is lower case
                if(bit & wordMap)
                {
                    wordMap = ~0;
                    break;
                }
                wordMap |= bit;
            }
            /*printf("\n%"PRIx64"\n\n", wordMap);
            getchar();*/
            if(wordMap > 0)
            {
                InsertInFolder(directory, wordMap, word);
            }
        }
    }

    freeDictionary(wordList);



    return 0;

    USD_ERR_GET_DICTIONARY:
    return 1;
}

directory CreateDirectory(size_t folders)
{
    directory dir;
    dir.directory = calloc(sizeof(uint64_t*), folders);
    dir.folders = malloc(8*folders*sizeof(uint64_t));

    for(size_t i = 0; i < folders; ++i)
    {
        dir.directory[i] = i*8;
    }

    return dir;
}

typedef struct {
    uint32_t index;
    uint32_t size;
} dirData;

void InsertInFolder(directory directory, size_t folderIndex, uint64_t data)
{
    uint32_t index = folderIndex;
    if((((dirData*)directory.directory)[folderIndex].size % 8 == 0 && ((dirData*)directory.directory)[folderIndex].size >= 8) ||  directory.directory[folderIndex] == -1)
    {
        if(directory.directory[folderIndex+1] == -1)
        {
            printf("warning! overlap\n");
            getchar();
            return;
        }
        for(;((dirData*)directory.directory)[index].size != 0; ++index)
        {
            ((dirData*)directory.directory)[index].index += 8;
        }
        memmove(&directory.folders[((dirData*)directory.directory)[folderIndex+1].index], &directory.folders[((dirData*)directory.directory)[folderIndex+2].index], (index-folderIndex)*8);
        directory.directory[index] = -1;
    }
    directory.folders[((dirData*)directory.directory)[folderIndex].index + (((dirData*)directory.directory)[folderIndex].size)++] = data;

    return;
}
void DestroyDirectory(directory directory)
{
    free(directory.directory);
    free(directory.folders);
}
