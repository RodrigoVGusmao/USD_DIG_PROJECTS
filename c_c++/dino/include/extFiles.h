#ifndef EXTFILES_H
#define EXTFILES_H

#include<fstream>
#include<iostream>
#include<variant>
#include<vector>

#define FILE_CHAR 0x01
#define FILE_UNSIGNED_CHAR 0x01
#define FILE_BYTE 0x01
#define FILE_BOOL 0x01
#define FILE_BIT 0x01
#define FILE_SHORT 0x02
#define FILE_UNSIGNED_SHORT 0x02
#define FILE_2BYTE 0x02
#define FILE_INT 0x04
#define FILE_UNSIGNED_INT 0x04
#define FILE_4BYTE 0x04
#define FILE_LONG 0x08
#define FILE_UNSIGNED_LONG 0x08
#define FILE_8BYTE 0x08

namespace file
{
class extFiles
{
    public:
        extFiles(std::string);
        void writeFile(void*, uint8_t, uint8_t);
        void readFile(uint64_t*, uint16_t);
        void refreshFile();
        ~extFiles();

    protected:
        std::fstream file;
        std::string fName;

    private:
};
}

#endif // EXTFILES_H
