#include "extFiles.h"

using namespace file;

extFiles::extFiles(std::string filename)
{
    fName = filename;
    file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
}

void extFiles::writeFile(void* data, uint8_t dataType, uint8_t bytes)
{
    uint8_t* formattedData = static_cast<uint8_t*>(data);

    for(uint16_t i = 0; i<bytes/dataType; i++)
    {
        //std::cout << static_cast<int>(dataType) << std::endl;
        file.write(reinterpret_cast<char*>(&dataType), 1);
        for(uint8_t j = 0; j<dataType; j++)
        {
            file.write(reinterpret_cast<char*>(&formattedData[j]), 1);
            //std::cout << static_cast<int>(formattedData[j]) << std::endl;
        }
    }
}

void extFiles::readFile(uint64_t* data, uint16_t dataPos)
{
    uint8_t dataSize = 0;
    uint64_t longData = 0;

    file.seekg(0);

    if(!file.std::ios::good())
            return;

    for(uint16_t i=1; i<dataPos; i++)
    {
        file >> dataSize;
        file.ignore(dataSize);
        if(!file.std::ios::good())
            return;
    }

    file.read(reinterpret_cast<char*>(&dataSize), 1);
    if(!file.std::ios::good())
        return;

    file.read(reinterpret_cast<char*>(data), dataSize);
    if(!file.std::ios::good())
        return;

    std::cout << static_cast<int>(dataSize) << std::endl;
    std::cout << static_cast<long>(*data) << std::endl;

    /*if(dataSize == FILE_BYTE)
        *data = static_cast<uint8_t>(longData);
    else if(dataSize == FILE_2BYTE)
        *data = static_cast<uint16_t>(longData);
    else if(dataSize == FILE_4BYTE)
        *data = static_cast<uint32_t>(longData);
    else if(dataSize == FILE_8BYTE)
        *data = static_cast<uint64_t>(longData);*/
}

void extFiles::refreshFile()
{
    file.close();

    remove(fName.c_str());
    file.open(fName.c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
}

extFiles::~extFiles()
{
    file.close();
}
