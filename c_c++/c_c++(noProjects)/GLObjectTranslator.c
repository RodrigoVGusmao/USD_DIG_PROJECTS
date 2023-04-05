#include<stdio.h>
#include<stdlib.h>

//defines
#define uint (unsigned int)
#define GL_FLOAT 0x1406
#define GL_UNSIGNED_INT 0x1405
#define GLObjectEnd ~0
#define vertices 0
#define indices 1
#define verticesMode 1
#define colorMode 2
#define textureMode 4
#define bit 8

void fileIntAsChar(FILE *filePointer, unsigned int value)
{
    fprintf(filePointer, "%c%c%c%c", value >> bit*0, value >> bit*1, value >> bit*2, value >> bit*3);
}

int main()
{
    unsigned int fileWriteArray[] = {(uint vertices << bit*3) + verticesMode, 12, GL_FLOAT};
    unsigned int fileWriteArray2[] = {(uint indices << bit*3), 6, GL_UNSIGNED_INT};
    float verticesArray[] = {0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
    unsigned int indicesArray[] = {0, 1, 2, 0, 2, 3};
    unsigned int end[] = {GLObjectEnd};

    unsigned int fileReadArray[3];
    float* fData;
    unsigned int* uiData;
    unsigned int num, i;
    char read = 0;
    FILE *fptr;

    printf("creating file...\n");

    if(read == 0)
        fptr = fopen("archive.data", "wb");
    else
        fptr = fopen("archive.data", "rb");
    if(fptr == NULL)
    {
        printf("couldn't create/open archive...");
        return 1;
    }

    if(read != 0)
    {
        fread(fileReadArray, sizeof(fileReadArray), 1, fptr);
        while(fileReadArray[0] != GLObjectEnd)
        {
            for(i=0;i<3;i++)
            {
                printf("%d ", fileReadArray[i]);
            }
            if(fileReadArray[2] == GL_FLOAT)
            {
                fData = (float*)calloc(sizeof(float), fileReadArray[1]);
                fread(fData, sizeof(float), fileReadArray[1], fptr);
                for(i=0;i<fileReadArray[1];i++)
                {
                    printf("%f ", fData[i]);
                }
            }
            if(fileReadArray[2] == GL_UNSIGNED_INT)
            {
                uiData = (unsigned int*)calloc(sizeof(unsigned int), fileReadArray[1]);
                fread(uiData, sizeof(unsigned int), fileReadArray[1], fptr);
                for(i=0;i<fileReadArray[1];i++)
                {
                    printf("%d ", uiData[i]);
                }
            }
            fread(fileReadArray, sizeof(fileReadArray), 1, fptr);
        }
        printf("%d", GLObjectEnd);
        return 0;
    }

    printf("converting data..\n");

    fwrite(fileWriteArray, sizeof(fileWriteArray), 1, fptr);
    fwrite(verticesArray, sizeof(verticesArray), 1, fptr);
    fwrite(fileWriteArray2, sizeof(fileWriteArray2), 1, fptr);
    fwrite(indicesArray, sizeof(indicesArray), 1, fptr);
    fwrite(end, sizeof(end), 1, fptr);
    /*fileIntAsChar(fptr, GL_FLOAT);
    fileIntAsChar(fptr, (unsigned int)vertices >> bit*3 + verticesMode);*/

    //fprintf(fptr, "%c%c%c%c%c%c%c%c%c%c", GL_FLOAT, GL_FLOAT >> bit*1, GL_FLOAT >> bit*2, GL_FLOAT >> bit*3, vertices, verticesMode, 0.5f, 0.5f >> bit*1, 0.5f >> bit*2, 0.5f >> bit*3);

    return 0;
}
