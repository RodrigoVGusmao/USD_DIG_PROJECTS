#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdarg.h>

#define bool char
#define false 0
#define true 1
#define charToNum(n) 'n'-'0'

struct member{
    char *text;
    int charNum;
    unsigned int wordsNum;
};

struct file{
    FILE *ptr;
    char *name;
    char *text;
    int charNum;
    unsigned int wordsNum;
    unsigned int membersNum;
    struct member *memberPtr;
};

struct clause{
    char *clause;
    unsigned short conjugation;
    bool regular;
    unsigned short person;
};

int readFile(FILE *ptr, struct file *file)
{
    unsigned int pos = 0;

    file->wordsNum = 0;
    file->membersNum = 0;

    fseek(ptr, 0, SEEK_END);
    if(ferror(ptr) != 0)
    {
        printf("can't get file %s's end", file->name);
        return 1;
    }
    file->charNum = ftell(ptr);

    fseek(ptr, 0, SEEK_SET);
    if(ferror(ptr) != 0)
    {
        printf("can't get file %s's start", file->name);
        return 2;
    }

    file->text = (char*)calloc(file->charNum, sizeof(char));

    while((file->text[pos] = getc(ptr)) != EOF)
    {

        if(file->text[pos] == ' ')
        {
            file->wordsNum++;
        }
        if(file->text[pos] == '\n')
        {
            file->charNum--;
            file->wordsNum++;
            file->membersNum++;
        }
        pos++;
    }

    file->text = (char*)realloc(file->text, sizeof(char)*file->charNum);

    file->wordsNum++;
    file->membersNum++;

    file->memberPtr = (struct member*)calloc(file->membersNum, sizeof(struct member));

    return 0;
}

void setMembers(struct file *file)
{
    unsigned short charPos, oldCharPos = 0, memberPos;

    for(memberPos = 0; memberPos < file->membersNum; memberPos++)
    {
        file->memberPtr[memberPos].text = (char*)calloc(file->charNum, sizeof(char));
        file->memberPtr[memberPos].wordsNum = 1;
    }

    memberPos = 0;

    for(charPos = 0;charPos < file->charNum; charPos++)
    {
        if(file->text[charPos] == ' ')
        {
            file->memberPtr[memberPos].wordsNum++;
        }
        if(file->text[charPos] == '\n')
        {
            file->memberPtr[memberPos].charNum = charPos - oldCharPos;
            memberPos++;
            oldCharPos = charPos;
        }
        file->memberPtr[memberPos].text[charPos - oldCharPos] = file->text[charPos];
    }
    file->memberPtr[0].charNum++;
    file->memberPtr[memberPos].charNum = charPos - oldCharPos - 1;

}

int stringSize(const char* string, ...)
{
    char *strArg, chArg;
    unsigned short pos, uIntArg, posArg, size = 0, subtract = 0;
    va_list arg;

    va_start(arg, string);

    for(pos = 0; string[pos] != '\0'; pos++)
    {
        if(string[pos] == '%')
        {
            switch(string[pos + 1])
            {
                case 's':
                    strArg = va_arg(arg, char*);
                    printf(strArg);
                    for(posArg = 0; strArg[posArg] != '\0'; posArg++)
                        size++;
                    subtract += 2;
                    free(strArg);
                    break;
                case 'c':
                    chArg = va_arg(arg, unsigned int);
                    subtract += 1;
                    break;
            }
        }
        size++;
    }
    size -= subtract;

    va_end(arg);
    return size;
}

char* conInfo(const char* format, ...)
{
    char *outputString, *strArg, chArg;
    unsigned short pos, posArg, truePos = 0;
    va_list arg;

    va_start(arg, format);

    outputString = (char*)malloc(sizeof(char));

    for(pos = 0; format[pos] != '\0'; pos++)
    {
        if(format[pos] == '%')
        {
            pos++;
            switch(format[pos])
            {
                case 's':
                    strArg = va_arg(arg, char*);
                    for(posArg = 0; strArg[posArg] != '\0';posArg++)
                    {
                        outputString = (char*)realloc(outputString, sizeof(char)*(truePos + posArg + 2));
                        outputString[truePos + posArg] = strArg[posArg];
                    }
                    truePos += posArg;
                    break;
                case 'c':
                    chArg = va_arg(arg, unsigned int);
                    outputString = (char*)realloc(outputString, sizeof(char)*(truePos + 2));
                    outputString[truePos] = chArg;
                    truePos++;
                    break;
                default:
                    outputString[truePos] = '%';
                    outputString[truePos+1] = format[pos];
                    truePos++;
            }
        }
        else
        {
            outputString = (char*)realloc(outputString, sizeof(char)*(truePos + 2));
            outputString[truePos] = format[pos];
            truePos++;
        }
    }

    outputString[truePos] = '\0';

    va_end(arg);
    return outputString;
}

char* deleteCharacters(const char* string, unsigned short numOfCharacters, ...)
{
    unsigned short pos, numOfSubtractedChars = 0, *ignoredChars;
    unsigned short argPos;
    char *outputString;
    va_list arg;

    va_start(arg, numOfCharacters);

    ignoredChars = (unsigned short*)calloc(numOfCharacters, sizeof(unsigned short));
    for(argPos = 0; argPos < numOfCharacters; argPos++)
    {
        ignoredChars[argPos] = va_arg(arg, int);
    }

    outputString = (char*)calloc(stringSize(string), sizeof(char));

    for(pos = 0; string[pos] != '\0'; pos++)
    {
        outputString[pos - numOfSubtractedChars] = string[pos];
        for(argPos = 0; argPos < numOfCharacters; argPos++)
        {
            if(string[pos] == ignoredChars[argPos])
            {
                outputString[pos - numOfSubtractedChars] = outputString[pos - numOfSubtractedChars + 1];
                //outputString[pos - numOfSubtractedChars + 1] = 0;
                numOfSubtractedChars++;
            }
        }
        argPos = 0;
    }

    outputString[pos - numOfSubtractedChars] = '\0';

    outputString = (char*)realloc(outputString, sizeof(char)*(stringSize(string) - numOfSubtractedChars + 1));

    va_end(arg);

    return outputString;
}

unsigned short compareStrings(const char* string1, const char* string2)
{
    unsigned int pos;
    if(stringSize(string1) == stringSize(string2))
    {
        for(pos = 0; pos < stringSize(string1); pos++)
        {
            if(string1[pos] != string2[pos])
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

char* getWord(char* string, char separatorChar, unsigned short wordPos)
{
    char *outputString;
    unsigned short words = 0;
    unsigned int pos = 0, startPos, endPos;

    if(wordPos < 1)
    {
        printf("invalid Posistion\n");
        return NULL;
    }

    while(words < wordPos - 1)
    {
        if(string[pos] == separatorChar)
        {
            words++;
        }
        if(string[pos] == '\0')
        {
            return NULL;
        }
        pos++;
    }
    startPos = pos;

    while(words < wordPos)
    {
        if(string[pos] == separatorChar || string[pos] == '\0')
        {
            words++;
        }
        pos++;
    }
    endPos = pos;
    pos = startPos;

    /*printf("\nws:%d|wp:%d|ps:%d\n", words, wordPos, pos);
    printf("varSize=>%d - %d = %d\n", endPos, startPos, endPos - startPos);*/

    outputString = (char*)calloc(endPos - startPos + 1, sizeof(char));
    while(string[pos] != separatorChar && string[pos] != '\0')
    {
        outputString[pos - startPos] = string[pos];
        pos++;
    }

    outputString[endPos - startPos] = '\0';

    return outputString;
}

int oFile(struct file *file, char* filename)
{
    unsigned short filenameSize;

    filenameSize = stringSize(filename);

    file->name = (char*)calloc(filenameSize, sizeof(char));

    file->name = filename;

    file->ptr = fopen(filename, "r");
    if(file->ptr == NULL)
    {
        printf("couldn't open %s: Maybe it wasn't created yet\n", filename);
        return 1;
    }

    if(readFile(file->ptr, file) != 0)
    {
        return 2;
    }

    fclose(file->ptr);

    setMembers(file);

    return 0;
}

void fileDebug(struct file file)
{
    unsigned short member;

    printf("\n\n\n\n\n\nFile Debug Info\n\n\n     text:{\n%s\n};\n     charNum:{\n%d\n};\n     wordsNum:{\n%d\n};\n     membersNum:{\n%d\n};\n", file.text, file.charNum, file.wordsNum, file.membersNum);
    for(member = 0; member < file.membersNum; member++)
    {
        printf("\n\n\n\n\nFile Member[%d] Debug Info\n\n\n     text:{\n%s\n};\n     charNum:{\n%d\n};\n     wordsNum:{\n%d\n};\n", member, file.memberPtr[member].text, file.memberPtr[member].charNum, file.memberPtr[member].wordsNum);
    }
}

int main()
{
    char *memberTitle, *fileNeeded, *info;

    unsigned short member, member2, varMember, varMemberMember, subMember, randWord, wordPos = 1;

    struct file initFile;
    struct file *membersFiles;
    struct clause clauses;

    memberTitle = (char*)malloc(sizeof(char));
    fileNeeded = (char*)malloc(sizeof(char));

    srand(time(NULL));

    oFile(&initFile, "init.txt");
    fileDebug(initFile);

    membersFiles = (struct file*)calloc(initFile.membersNum, sizeof(struct file));
    for(member = 0; member < initFile.membersNum; member++)
    {
        memberTitle = (char*)realloc(memberTitle ,sizeof(char)*(stringSize(initFile.memberPtr[member].text)-1));
        memberTitle = deleteCharacters(initFile.memberPtr[member].text, 1, '\n');
        oFile(&membersFiles[member], memberTitle);
        fileDebug(membersFiles[member]);
        printf("%s\n", memberTitle);
    }

    for(member = 0; compareStrings(membersFiles[member].name, "sintaxe.txt") == false && member+1 < initFile.membersNum; member++)
    {
        printf("searching...         %d/%d\n", member+1, initFile.membersNum-1);
    }
    if(compareStrings(membersFiles[member].name, "sintaxe.txt") == false)
    {
        printf("couldn't find sintaxe.txt\n");
        return 1;
    }
    subMember = rand() % membersFiles[member].membersNum;

    system("cls");

    for(varMember = 0; compareStrings(membersFiles[varMember].name, "variaveis.txt") == false && varMember+1 < initFile.membersNum; varMember++)
    {
        printf("searching...         %d/%d\n", varMember+1, initFile.membersNum-1);
    }
    if(compareStrings(membersFiles[varMember].name, "variaveis.txt") == false)
    {
        printf("couldn't find variaveis.txt\n");
        return 1;
    }

    system("cls");

    while(getWord(membersFiles[member].memberPtr[subMember].text, '+', wordPos) != NULL)
    {
        fileNeeded = (char*)realloc(fileNeeded, sizeof(char) * (stringSize(getWord(membersFiles[member].memberPtr[subMember].text, '+', wordPos))+5));
        fileNeeded = conInfo("%s%s", getWord(membersFiles[member].memberPtr[subMember].text, '+', wordPos), ".txt");

        for(member2 = 0; compareStrings(membersFiles[member2].name, fileNeeded) == false && member2+1 < initFile.membersNum; member2++)
        {
            //printf("searching...         %d/%d\n", member2, initFile.membersNum);
        }
        if(compareStrings(membersFiles[member2].name, fileNeeded) == true)
        {
            randWord = rand() % membersFiles[member2].membersNum;

            //for variaveis.txt
            for(varMemberMember = 0; compareStrings(membersFiles[member2].name, membersFiles[varMember].memberPtr[varMemberMember].text) == false && varMemberMember+1 < membersFiles[varMember].membersNum; varMemberMember++)
            {
                printf("searching...         %d/%d\n", varMemberMember, membersFiles[varMember].membersNum);
            }
            if(compareStrings(membersFiles[member2].name, membersFiles[varMember].memberPtr[varMemberMember].text) == true)
            {
                info = (char*)calloc(stringSize(deleteCharacters(getWord(membersFiles[member2].memberPtr[randWord].text, ' ', 2), 1, '-')), sizeof(char));
                info = deleteCharacters(getWord(membersFiles[member2].memberPtr[randWord].text, ' ', 2), 1, '-');

                switch(deleteCharacters(info, 3, '1', '2', '3'))
                {
                    case 'r':
                        clauses.regular = true;
                        break;
                    case 'i':
                        clauses.regular = false;
                        break;
                    default:
                        printf("too many arguments or wrong argument on verbos.txt:%s\n", membersFiles[member2].memberPtr[randWord].text);
                        return 1;
                }
                clauses.conjugation = charToNum(deleteCharacters(info, 2, 'r', 'i');
                if(clauses.conjugation == 0 || clauses.conjugation > 3)
                {
                    printf("wrong argument or syntax on verbos.txt:%s\nformat: verb -rn (r = r or i and n = 1 or 2 or 3)", membersFiles[member2].memberPtr[randWord].text);
                    return 1;
                }
            }

            //for pronomes.txt
            if(compareStrings(fileNeeded, "pronomes.txt") == true)
            {
                info = (char*)calloc(stringSize(deleteCharacters(getWord(membersFiles[member2].memberPtr[randWord].text, ' ', 2), 1, '-')), sizeof(char))
                info = deleteCharacters(getWord(membersFiles[member2].memberPtr[randWord].text, ' ', 2), 1, '-');
                clauses.person = charToNum(info);
                if(clauses.person == 0 || clauses.person > 6)
                {
                    printf("wrong argument on pronomes.txt:%s\nformat: pron -n (n = 1 or 2 or 3 or 4 or 5 or 6)", membersFiles[member2].memberPtr[randWord].text);
                }
            }

            printf("%s", conInfo("%s%c", deleteCharacters(membersFiles[member2].memberPtr[randWord].text, 1, '\n'), ' '));
        }
        else
        {
            printf("couldn't find %s\n", fileNeeded);
            return 1;
        }
        wordPos++;
    }

    return 0;
}
