#pragma once

#include<cinttypes>
#include<fstream>

#define MAP_HEADER 0x4350414D
#define MAP_STATIC 0x54415453
#define MAP_ENTITY 0x49544E45
#define MAP_INIT 0x54494E49
#define MAP_TRIGGER 0x47495254

#define TRIG_LOAD_MAP 0x50414D4C
#define TRIG_LOAD_ENTITY 0x544E454C
#define TRIG_LOAD_TRIGGER 0x4752544C
#define TRIG_CONFIG 0x464E4F43

namespace mapFile {
	typedef struct {
		uint32_t header;
		uint16_t width;
		uint16_t height;
		uint32_t size;
		uint8_t dataSize;
		uint8_t flags;
		uint16_t extra;
	}mapSectionHeaderFileBin;

	typedef struct {
		uint32_t header;
		uint32_t offset;
	}mapHeaderSectionData;

	typedef struct {
		uint32_t index;
		float rot;
		uint32_t offstX;
		uint32_t offstY;
		float scaleX;
		float scaleY;
		float speed;
		uint16_t maskIndex;
		uint16_t binFunc;
	} mapSectionData;

	typedef struct {
		mapSectionHeaderFileBin header;
		mapSectionData data[1];
	}mapSectionFileBin;

	typedef struct {
		mapSectionHeaderFileBin header;
		mapHeaderSectionData data[1];
	}mapHeaderSectionFileBin;

	mapHeaderSectionFileBin* getHeaderMapSection(std::string mapName, mapHeaderSectionFileBin** mapSection)
	{
		if (mapSection == nullptr)
			return nullptr;

		std::ifstream mapFile(mapName.c_str(), std::ios::binary);

		*mapSection = reinterpret_cast<mapHeaderSectionFileBin*>(malloc(sizeof(mapHeaderSectionFileBin)));
		if (*mapSection == nullptr)
			return nullptr;

		mapFile.read(reinterpret_cast<char*>(*mapSection), sizeof(mapSectionHeaderFileBin));

		mapHeaderSectionFileBin* temp = reinterpret_cast<mapHeaderSectionFileBin*>(realloc(*mapSection, sizeof(mapSectionHeaderFileBin)+((*mapSection)->header.extra)*sizeof(mapHeaderSectionData)));
		if (temp == nullptr)
		{
			free(*mapSection);
			return nullptr;
		}
		*mapSection = temp;

		mapFile.read(reinterpret_cast<char*>((*mapSection)->data), static_cast<uint64_t>((*mapSection)->header.extra) * sizeof(mapHeaderSectionData));

		return *mapSection;
	}

	mapSectionFileBin* getMapSection(std::string mapName, mapHeaderSectionFileBin* headerSectionMap, uint32_t section, uint16_t index)
	{
		std::ifstream mapFile(mapName.c_str(), std::ios::binary);

		mapSectionFileBin* mapSection = reinterpret_cast<mapSectionFileBin*>(malloc(sizeof(mapSectionFileBin)));
		if (mapSection == nullptr)
			return nullptr;

		size_t i = 0;
		do {
			for (; i < headerSectionMap->header.extra && headerSectionMap->data[i].header != section; ++i);
			if (i > headerSectionMap->header.extra)
				return nullptr;

			mapFile.seekg(headerSectionMap->data[i++].offset);

			mapFile.read(reinterpret_cast<char*>(mapSection), sizeof(mapSectionHeaderFileBin));
		} while (mapSection->header.extra != index);

		mapSectionFileBin* temp = reinterpret_cast<mapSectionFileBin*>(realloc(mapSection, sizeof(mapSectionHeaderFileBin) + static_cast<uint64_t>(mapSection->header.size) * sizeof(mapSectionData)));
		if (temp == nullptr)
		{
			free(mapSection);
			return nullptr;
		}
		mapSection = temp;

		mapFile.read(reinterpret_cast<char*>(mapSection->data), static_cast<uint64_t>(mapSection->header.size) * sizeof(mapSectionData));

		return mapSection;
	}

	mapSectionFileBin* getMapLoadSection(std::string mapName, mapHeaderSectionFileBin* headerSectionMap)
	{
		std::ifstream mapFile(mapName.c_str(), std::ios::binary);

		size_t i;
		for (i = 0; i <= headerSectionMap->header.extra && headerSectionMap->data[i].header != MAP_INIT; ++i);
		if (i > headerSectionMap->header.extra)
			return nullptr;

		mapFile.seekg(headerSectionMap->data[i].offset);

		mapSectionFileBin* mapLoadSection = reinterpret_cast<mapSectionFileBin*>(malloc(sizeof(mapSectionFileBin)));
		if (mapLoadSection == nullptr)
			return nullptr;

		mapFile.read(reinterpret_cast<char*>(mapLoadSection), sizeof(mapSectionHeaderFileBin));

		mapSectionFileBin* temp = reinterpret_cast<mapSectionFileBin*>(realloc(mapLoadSection, sizeof(mapSectionFileBin) + mapLoadSection->header.size * sizeof(mapSectionData)));
		if (temp == nullptr)
		{
			free(mapLoadSection);
			return nullptr;
		}
		mapLoadSection = temp;

		mapFile.read(reinterpret_cast<char*>(mapLoadSection->data), static_cast<uint64_t>(mapLoadSection->header.size) * sizeof(mapSectionData));

		return mapLoadSection;
	}
}