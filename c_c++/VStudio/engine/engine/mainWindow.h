#pragma once

#include<windows.h>

#include<cinttypes>
#include<cmath>
#include<fstream>

#include"keyboard.h"
#include"object.h"
#include"mapFile.h"
#include"debug.h"

typedef struct {
	uint16_t flag;
	uint16_t fileSpriteWidth;
	uint16_t fileSpriteHeight;
	uint16_t screenSpriteWidth;
	uint16_t screenSpriteHeight;
	uint16_t spritesPerLine;
	uint16_t entityMaxSpriteWidth;
	uint16_t entityMaxSpriteHeight;
	uint32_t imageBufferWidth;
	uint32_t imageBufferHeight;
} configFileBin;

configFileBin loadConfigFile()
{
	configFileBin configStruct;
	try {
		std::ifstream configFile("modeSettings.bin", std::ios::binary);

		configFile.read(reinterpret_cast<char*>(&configStruct), sizeof(configFileBin));
	}
	catch (std::exception e)
	{
		MessageBox(nullptr, L"could not load 'modeSettings.bin'", L"error", MB_ICONERROR);
		return {0};
	}

	return configStruct;
}

//mapDC must have be loaded with LoadImageW or be Created with CreateDIBSection
void printStaticMap(HDC dst, HDC src, mapFile::mapSectionFileBin* staticMapData, configFileBin config, DWORD rop)
{
	for (size_t i = 0; i < staticMapData->header.size; ++i)
	{
		StretchBlt(dst, i % staticMapData->header.width * config.screenSpriteWidth, 
			i / staticMapData->header.height * config.screenSpriteHeight, config.screenSpriteWidth, config.screenSpriteHeight,
			src, staticMapData->data[i].index % config.spritesPerLine * config.fileSpriteWidth, 
			staticMapData->data[i].index / config.spritesPerLine * config.fileSpriteHeight, config.fileSpriteWidth, config.fileSpriteHeight, rop);
	}
}

LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static RECT clientPts;

	static configFileBin configInfo;

	static HDC bufferDC;
	static HBITMAP oldBufferBmp;

	static HDC mapDC;
	static HBITMAP oldMapBmp;

	static HDC mapMaskDC;
	static HBITMAP oldMapMaskBmp;

	static HDC spritesDC;
	static HBITMAP oldSpritesBmp;

	static HDC maskDC;
	static HBITMAP oldMaskBmp;

	static HDC collisionSpritesDC;
	static HBITMAP oldCollisionSpritesBmp;

	static HDC collisionDC;
	static HBITMAP oldCollisionBmp;

	static HDC entityMaskDC;
	static HBITMAP oldEntityMaskBmp;

	static HDC moveCollisionDC;
	static HBITMAP oldMoveCollisionBmp;
	static HBITMAP moveCollisionBmp;
	static void* moveCollisionBits;

	static mapFile::mapHeaderSectionFileBin* headerMapSection;

	static object* entity;
	static mapFile::mapSectionFileBin* entityMapSection;

	switch (msg)
	{
	case WM_CREATE:
	{
		configInfo = loadConfigFile();

		key::initializeKey();

		GetClientRect(hwnd, &clientPts);

		HDC DC = GetDC(hwnd);
		//memory DCs with DDB
		bufferDC = CreateCompatibleDC(DC);
		oldBufferBmp = reinterpret_cast<HBITMAP>(SelectObject(bufferDC, reinterpret_cast<HGDIOBJ>(CreateCompatibleBitmap(DC, clientPts.right, clientPts.bottom))));

		collisionDC = CreateCompatibleDC(DC);
		oldCollisionBmp = reinterpret_cast<HBITMAP>(SelectObject(collisionDC, reinterpret_cast<HGDIOBJ>(CreateCompatibleBitmap(collisionDC, configInfo.imageBufferWidth, configInfo.imageBufferHeight))));

		mapDC = CreateCompatibleDC(DC);
		oldMapBmp = reinterpret_cast<HBITMAP>(SelectObject(mapDC, reinterpret_cast<HGDIOBJ>(CreateCompatibleBitmap(DC, configInfo.imageBufferWidth, configInfo.imageBufferHeight))));

		mapMaskDC = CreateCompatibleDC(DC);
		oldMapBmp = reinterpret_cast<HBITMAP>(SelectObject(mapMaskDC, reinterpret_cast<HGDIOBJ>(CreateCompatibleBitmap(DC, configInfo.imageBufferWidth, configInfo.imageBufferHeight))));

		entityMaskDC = CreateCompatibleDC(DC);
		oldEntityMaskBmp = reinterpret_cast<HBITMAP>(SelectObject(entityMaskDC, reinterpret_cast<HGDIOBJ>(CreateCompatibleBitmap(DC, configInfo.imageBufferWidth, configInfo.imageBufferHeight))));

		//image loading DCs
		spritesDC = CreateCompatibleDC(DC);
		HBITMAP spritesBmp = reinterpret_cast<HBITMAP>(LoadImageW(NULL, L"sprites.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (spritesBmp == nullptr)
		{
			MessageBoxW(nullptr, L"could not load 'sprites.bmp'", L"error", MB_ICONERROR);
			PostQuitMessage(1);
			return 0;
		}
		oldSpritesBmp = reinterpret_cast<HBITMAP>(SelectObject(spritesDC, reinterpret_cast<HGDIOBJ>(spritesBmp)));

		collisionSpritesDC = CreateCompatibleDC(DC);
		HBITMAP collisionSpritesBmp = reinterpret_cast<HBITMAP>(LoadImageW(NULL, L"collision.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
		if (collisionSpritesBmp == nullptr)
		{
			MessageBoxW(nullptr, L"could not load 'collision.bmp'", L"error", MB_ICONERROR);
			PostQuitMessage(1);
			return 0;
		}
		oldCollisionSpritesBmp = reinterpret_cast<HBITMAP>(SelectObject(collisionSpritesDC, reinterpret_cast<HGDIOBJ>(collisionSpritesBmp)));

		//memory DCs with DIBs
		BITMAPINFO DIBInfo = { 0 };
		DIBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		DIBInfo.bmiHeader.biWidth = configInfo.entityMaxSpriteWidth;
		DIBInfo.bmiHeader.biHeight = configInfo.entityMaxSpriteHeight;
		DIBInfo.bmiHeader.biPlanes = 1;
		DIBInfo.bmiHeader.biBitCount = 1;
		DIBInfo.bmiHeader.biCompression = BI_RGB;
		DIBInfo.bmiHeader.biSizeImage = 0;
		DIBInfo.bmiHeader.biClrUsed = 0;
		DIBInfo.bmiHeader.biClrImportant = 0;

		moveCollisionDC = CreateCompatibleDC(DC);
		HBITMAP moveCollisionBmp = CreateDIBSection(moveCollisionDC, &DIBInfo, DIB_RGB_COLORS, &moveCollisionBits, nullptr, 0);
		if (moveCollisionBmp == nullptr)
		{
			MessageBoxW(nullptr, L"could not create 'collision' DIBitmap", L"error", MB_ICONERROR);
			PostQuitMessage(1);
			return 0;
		}
		oldMoveCollisionBmp = reinterpret_cast<HBITMAP>(SelectObject(moveCollisionDC, reinterpret_cast<HGDIOBJ>(moveCollisionBmp)));

		//create Mask Bitmap
		BITMAP bmpInfo;
		GetObject(spritesBmp, sizeof(BITMAP), &bmpInfo);

		DIBInfo.bmiHeader.biWidth = bmpInfo.bmWidth;
		DIBInfo.bmiHeader.biHeight = bmpInfo.bmHeight;
		DIBInfo.bmiHeader.biBitCount = 24;
		void* maskBits;

		maskDC = CreateCompatibleDC(DC);
		oldMaskBmp = reinterpret_cast<HBITMAP>(SelectObject(maskDC, reinterpret_cast<HGDIOBJ>(CreateCompatibleBitmap(maskDC, bmpInfo.bmWidth, bmpInfo.bmHeight))));
		HDC tempDC = CreateCompatibleDC(DC);
		HBITMAP maskBmp = CreateDIBSection(tempDC, &DIBInfo, DIB_RGB_COLORS, &maskBits, nullptr, 0);
		if (maskBmp == nullptr)
		{
			MessageBoxW(nullptr, L"could not create 'mask' DIBitmap", L"error", MB_ICONERROR);
			PostQuitMessage(1);
			return 0;
		}
		HBITMAP oldtempBmp = reinterpret_cast<HBITMAP>(SelectObject(tempDC, reinterpret_cast<HGDIOBJ>(maskBmp)));
		HBRUSH oldTempBrush = reinterpret_cast<HBRUSH>(SelectObject(tempDC, CreateSolidBrush(GetPixel(spritesDC, 0, 0))));
		BitBlt(tempDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, spritesDC, 0, 0, 0x00C3006A);

		BitBlt(maskDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, tempDC, 0, 0, NOTSRCCOPY);

		DeleteObject(SelectObject(tempDC, oldtempBmp));
		DeleteObject(SelectObject(tempDC, oldTempBrush));
		DeleteDC(tempDC);
		ReleaseDC(hwnd, DC);

		//map file handling
		headerMapSection = nullptr;
		mapFile::getHeaderMapSection("map.map", &headerMapSection);
		if (headerMapSection == nullptr)
		{
			MessageBoxW(nullptr, L"could not load 'map.map' file", L"error", MB_ICONERROR);
			PostQuitMessage(1);
			return 0;
		}

		mapFile::mapSectionFileBin* loadMapSection = mapFile::getMapLoadSection("map.map", headerMapSection);
		if (loadMapSection == nullptr)
		{
			MessageBoxW(nullptr, L"could not load LSTC from 'map.map' file", L"error", MB_ICONERROR);
			PostQuitMessage(1);
			return 0;
		}

		PatBlt(mapDC, 0, 0, configInfo.imageBufferWidth, configInfo.imageBufferHeight, WHITENESS);
		PatBlt(collisionDC, 0, 0, configInfo.imageBufferWidth, configInfo.imageBufferHeight, BLACKNESS);

		SendMessageW(hwnd, WM_USER, 0, reinterpret_cast<LPARAM>(loadMapSection));

		SetTimer(hwnd, 1, 17, nullptr);
		return 0;
	}

	case WM_USER:
	{
		for (size_t i = 0; i < reinterpret_cast<mapFile::mapSectionFileBin*>(lParam)->header.size; ++i)
		{
			switch (reinterpret_cast<mapFile::mapSectionFileBin*>(lParam)->data[i].index)
			{
			case TRIG_CONFIG:
			{
				break;
			}
			case TRIG_LOAD_MAP:
			{
				mapFile::mapSectionFileBin* objectMapSection;

				if ((objectMapSection = mapFile::getMapSection("map.map", headerMapSection, MAP_STATIC, reinterpret_cast<mapFile::mapSectionFileBin*>(lParam)->data[i].maskIndex)) != nullptr)
				{
					printStaticMap(mapMaskDC, maskDC, objectMapSection, configInfo, NOTSRCCOPY);
					printStaticMap(mapMaskDC, spritesDC, objectMapSection, configInfo, SRCPAINT);
					BitBlt(mapDC, 0, 0, configInfo.imageBufferWidth, configInfo.imageBufferHeight, mapMaskDC, 0, 0, SRCAND);
					printStaticMap(collisionDC, collisionSpritesDC, objectMapSection, configInfo, SRCPAINT);
					free(objectMapSection);
				}
				break;
			}

			case TRIG_LOAD_ENTITY:
			{
				if ((entityMapSection = mapFile::getMapSection("map.map", headerMapSection, MAP_ENTITY, reinterpret_cast<mapFile::mapSectionFileBin*>(lParam)->data[i].maskIndex)) != nullptr)
				{
					entity = reinterpret_cast<object*>(calloc(entityMapSection->header.size, sizeof(object)));
					if (entity == nullptr)
					{
						MessageBoxW(nullptr, L"could not allocate enough ram space for entities", L"error", MB_ICONERROR);
						PostQuitMessage(1);
						return 0;
					}
					for (uint16_t i = 0; i < entityMapSection->header.size; ++i)
					{
						entity[i].position = { static_cast<float>(entityMapSection->data[i].offstX), static_cast<float>(entityMapSection->data[i].offstY) };
						entity[i].speed = 2.f;
						entity[i].spriteSize = { configInfo.entityMaxSpriteWidth, configInfo.entityMaxSpriteHeight };
					}
				}
				break;
			}

			case TRIG_LOAD_TRIGGER:
			{
				mapFile::mapSectionFileBin* objectMapSection;
				if ((objectMapSection = mapFile::getMapSection("map.map", headerMapSection, MAP_TRIGGER, i)) != nullptr)
				{
					for (uint16_t i = 0; i < objectMapSection->header.size; ++i)
					{
						if (objectMapSection->data[i].index == TRIG_CONFIG)
						{

						}
					}
				}
				break;
			}
			}
		}

		return 0;
	}

	case WM_KEYDOWN:
	{
		if (wParam == key::upKey)
			key::movementState |= 1;
		else if (wParam == key::downKey)
			key::movementState |= 3;
		else if (wParam == key::rightKey)
			key::movementState |= 0xC;
		else if (wParam == key::leftKey)
			key::movementState |= 4;

		return 0;
	}

	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1:
		{
			if (entityMapSection != nullptr)
			{
				for (uint16_t j = 0; j < entityMapSection->header.size; ++j)
				{
					entity[j].position.y += (key::movementState & 1) * (entity[j].speed / sqrt(((key::movementState & 4) >> 1) + 1)) * ((key::movementState & 2) - 1);
					entity[j].position.x += ((key::movementState & 4) >> 2) * (entity[j].speed / sqrt((key::movementState & 1) + 1)) * (((key::movementState & 8) >> 2) - 1);

					PatBlt(moveCollisionDC, 0, 0, configInfo.entityMaxSpriteWidth, configInfo.entityMaxSpriteHeight, BLACKNESS);
					StretchBlt(moveCollisionDC, 0, 0, entity[j].spriteSize.x, entity[j].spriteSize.y, collisionSpritesDC,
						entityMapSection->data[j].index % configInfo.spritesPerLine * configInfo.fileSpriteWidth,
						entityMapSection->data[j].index / configInfo.spritesPerLine * configInfo.fileSpriteHeight, configInfo.fileSpriteWidth, configInfo.fileSpriteHeight, SRCCOPY);
					BitBlt(moveCollisionDC, 0, 0, entity[j].spriteSize.x, entity[j].spriteSize.y, collisionDC, static_cast<int>(entity[j].position.x), static_cast<int>(entity[j].position.y), SRCAND);

					for (size_t i = 0; i < ((configInfo.entityMaxSpriteWidth + 31) / 32) * configInfo.entityMaxSpriteHeight; ++i)
					{
						if (reinterpret_cast<DWORD*>(moveCollisionBits)[i] != 0)
						{
							entity[j].position = entity[j].lastSafePos;
							break;
						}
					}
					entity[j].lastSafePos = entity[j].position;
				}
			}
			break;
		}
		}

		InvalidateRect(hwnd, nullptr, FALSE);
		return 0;
	}

	case WM_KEYUP:
	{
		if (wParam == key::upKey)
			key::movementState &= ~1;
		else if (wParam == key::downKey)
			key::movementState &= ~3;
		else if (wParam == key::rightKey)
			key::movementState &= ~0xC;
		else if (wParam == key::leftKey)
			key::movementState &= ~4;

		return 0;
	}

	case WM_PAINT:
	{
		BitBlt(bufferDC, 0, 0, clientPts.right, clientPts.bottom, mapDC, 0, 0, SRCCOPY);
		BitBlt(bufferDC, 0x180, 0, clientPts.right, clientPts.bottom, moveCollisionDC, 0, 0, SRCCOPY);
		BitBlt(bufferDC, 0, 0x180, clientPts.right, clientPts.bottom, collisionDC, 0, 0, SRCCOPY);
		BitBlt(bufferDC, 0x180, 0x180, clientPts.right, clientPts.bottom, spritesDC, 0, 0, SRCCOPY);
		BitBlt(bufferDC, 0x1B0, 0x180, clientPts.right, clientPts.bottom, collisionSpritesDC, 0, 0, SRCCOPY);
		BitBlt(bufferDC, 0x1E0, 0x180, clientPts.right, clientPts.bottom, maskDC, 0, 0, SRCCOPY);

		if (entityMapSection != nullptr)
		{
			for (uint16_t i = 0; i < entityMapSection->header.size; ++i)
			{
				PatBlt(entityMaskDC, 0, 0, configInfo.imageBufferWidth, configInfo.imageBufferHeight, WHITENESS);

				StretchBlt(entityMaskDC, static_cast<int>(entity[i].position.x), static_cast<int>(entity[i].position.y), entity[i].spriteSize.x, entity[i].spriteSize.y,
					maskDC, entityMapSection->data[i].index % configInfo.spritesPerLine * configInfo.fileSpriteWidth,
					entityMapSection->data[i].index / configInfo.spritesPerLine * configInfo.fileSpriteHeight, configInfo.fileSpriteWidth, configInfo.fileSpriteHeight, NOTSRCCOPY);
				StretchBlt(entityMaskDC, static_cast<int>(entity[i].position.x), static_cast<int>(entity[i].position.y), entity[i].spriteSize.x, entity[i].spriteSize.y,
					spritesDC, entityMapSection->data[i].index % configInfo.spritesPerLine * configInfo.fileSpriteWidth,
					entityMapSection->data[i].index / configInfo.spritesPerLine * configInfo.fileSpriteHeight, configInfo.fileSpriteWidth, configInfo.fileSpriteHeight, SRCPAINT);

				StretchBlt(bufferDC, static_cast<int>(entity[i].position.x), static_cast<int>(entity[i].position.y), entity[i].spriteSize.x, entity[i].spriteSize.y,
					maskDC, entityMapSection->data[i].index % configInfo.spritesPerLine * configInfo.fileSpriteWidth,
					entityMapSection->data[i].index / configInfo.spritesPerLine * configInfo.fileSpriteHeight, configInfo.fileSpriteWidth, configInfo.fileSpriteHeight, SRCPAINT);
				BitBlt(bufferDC, 0, 0, configInfo.imageBufferWidth, configInfo.imageBufferHeight, entityMaskDC, 0, 0, SRCAND);
			}
		}

		PAINTSTRUCT ps;
		HDC DC = BeginPaint(hwnd, &ps);
		BitBlt(DC, 0, 0, clientPts.right, clientPts.bottom, bufferDC, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_DESTROY:
	{
		DeleteObject(SelectObject(bufferDC, oldBufferBmp));
		DeleteDC(bufferDC);

		DeleteObject(SelectObject(spritesDC, oldSpritesBmp));
		DeleteDC(spritesDC);

		DeleteObject(SelectObject(collisionSpritesDC, oldCollisionSpritesBmp));
		DeleteDC(collisionSpritesDC);

		DeleteObject(SelectObject(collisionDC, oldCollisionBmp));
		DeleteDC(collisionDC);

		DeleteObject(SelectObject(moveCollisionDC, oldMoveCollisionBmp));
		DeleteDC(moveCollisionDC);

		DeleteObject(SelectObject(mapDC, oldMapBmp));
		DeleteDC(mapDC);

		DeleteObject(SelectObject(mapMaskDC, oldMapMaskBmp));
		DeleteDC(mapMaskDC);

		DeleteObject(SelectObject(maskDC, oldMaskBmp));
		DeleteDC(maskDC);

		DeleteObject(SelectObject(entityMaskDC, oldEntityMaskBmp));
		DeleteDC(entityMaskDC);

		free(headerMapSection);
		free(entity);
		free(entityMapSection);

		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}
}