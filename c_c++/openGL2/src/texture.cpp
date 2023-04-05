#include "texture.h"

#include<stdio.h>

#define PIXEL_FORMAT GL_RGB
#define PIXEL_DATA_FORMAT GL_BGR_EXT
#define PIXEL_COMPONENTS 3
#define PIXEL_COMPONENT_SIZE sizeof(BYTE)*8
#define PIXEL_ELEMENT_ENUM GL_UNSIGNED_BYTE
#define UNPACK_ALIGN 4

int getBitmapFromBMP(void** bitmap, const char* filename, RECT* dimensions)
{
	HDC hScreenDC;
	HBITMAP hBmp;
	int error = 0;
	{
		hBmp = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE | LR_DEFAULTSIZE);
		if(hBmp == NULL)
		{
			printf("%x", GetLastError());
			error = 1;
			goto GBFBD_ERR_LOAD_BITMAP_A;
		}


		hScreenDC = GetDC(NULL);
		if(hScreenDC == NULL)
		{
			error = 2;
			goto GBFBD_ERR_GET_DC;
		}

		BITMAPINFO bmpInfo = {0};
		bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
		if(GetDIBits(hScreenDC, hBmp, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS) == 0)
		{
			error = 3;
			goto GBFBD_ERR_GET_DI_BITS_INFO;
		}

		*bitmap = malloc(bmpInfo.bmiHeader.biSizeImage);
		if(*bitmap == NULL)
		{
			error = 4;
			goto GBFBD_ERR_MALLOC;
		}

		bmpInfo.bmiHeader.biCompression = BI_RGB;
		if(GetDIBits(hScreenDC, hBmp, 0, bmpInfo.bmiHeader.biHeight, *bitmap, &bmpInfo, DIB_RGB_COLORS) == 0)
		{
			error = 5;
			goto GBFBD_ERR_GET_DI_BITS;
		}

		/*printf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", bmpInfo.bmiHeader.biSize, bmpInfo.bmiHeader.biWidth, bmpInfo.bmiHeader.biHeight, bmpInfo.bmiHeader.biPlanes,
				bmpInfo.bmiHeader.biBitCount, bmpInfo.bmiHeader.biCompression, bmpInfo.bmiHeader.biSizeImage, bmpInfo.bmiHeader.biXPelsPerMeter, bmpInfo.bmiHeader.biYPelsPerMeter,
				bmpInfo.bmiHeader.biClrUsed, bmpInfo.bmiHeader.biClrImportant);*/

		dimensions->top=0;
		dimensions->left=0;
		dimensions->bottom=bmpInfo.bmiHeader.biHeight;
		dimensions->right=bmpInfo.bmiHeader.biWidth;

		/*for(size_t i = 0; i < bmpInfo.bmiHeader.biSizeImage; ++i)
		{
			printf("%x ", ((BYTE*)*bitmap)[i]);
			if(i % 16 == 15)
				printf("\n");
		}*/
	}

	GBFBD_ERR_GET_DI_BITS:
	GBFBD_ERR_MALLOC:
	GBFBD_ERR_GET_DI_BITS_INFO:
		DeleteObject(hBmp);
	GBFBD_ERR_GET_DC:
	GBFBD_ERR_LOAD_BITMAP_A:
		ReleaseDC(NULL, hScreenDC);
		return error;
}

int LoadTexture(GLuint* textures, const void* bitmap, RECT imageDimensions, RECT clipDimension, size_t count, size_t offsetX, size_t offsetY, size_t cPerLine)
{
	int error = 0;
	HDC hDC, src, dst;
	HBITMAP hSrc, hClip;
	void* data;
	{
		glGenTextures(count, textures);

		hDC = GetDC(NULL);
		if(hDC == NULL)
		{
			error = 1;
			goto LTD_ERR_GET_DC;
		}

		src = CreateCompatibleDC(hDC);
		if(src == NULL)
		{
			error = 2;
			goto LTD_ERR_CREATE_COMPATIBLE_DC_SRC;
		}
		dst = CreateCompatibleDC(hDC);
		if(dst == NULL)
		{
			error = 3;
			goto LTD_ERR_CREATE_COMPATIBLE_DC_DST;
		}

		hSrc = CreateCompatibleBitmap(hDC, imageDimensions.right-imageDimensions.left, imageDimensions.bottom-imageDimensions.top);
		if(hSrc == NULL)
		{
			error = 4;
			goto LTD_ERR_CREATE_COMPATIBLE_BITMAP_SRC;
		}

		hClip = CreateCompatibleBitmap(hDC, clipDimension.right-clipDimension.left, clipDimension.bottom-clipDimension.top);
		if(hClip == NULL)
		{
			error = 5;
			goto LTD_ERR_CREATE_COMPATIBLE_BITMAP_CLIP;
		}

		BITMAPINFO bmpInfo;
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = imageDimensions.right;
		bmpInfo.bmiHeader.biHeight = imageDimensions.bottom;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 24;
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		bmpInfo.bmiHeader.biSizeImage = 0;
		bmpInfo.bmiHeader.biXPelsPerMeter = 0;
		bmpInfo.bmiHeader.biYPelsPerMeter = 0;
		bmpInfo.bmiHeader.biClrUsed = 0;
		bmpInfo.bmiHeader.biClrImportant = 0;

		if(SetDIBits(hDC, hSrc, 0, imageDimensions.bottom, bitmap, &bmpInfo, DIB_RGB_COLORS) == 0)
		{
			error = 6;
			goto LTD_ERR_SET_DI_BITS;
		}

		hSrc = (HBITMAP)SelectObject(src, hSrc);


		for(size_t i = 0; i < count; ++i)
		{
			//printf("debug\n");
			hClip = (HBITMAP)SelectObject(dst, hClip);
			BitBlt(dst, 0, 0, clipDimension.right + offsetX*(i%cPerLine), clipDimension.bottom + offsetY*(i/cPerLine),
					src, clipDimension.left + offsetX*(i%cPerLine), clipDimension.top + offsetY*(i/cPerLine), SRCCOPY);

			hClip = (HBITMAP)SelectObject(dst, hClip);

			bmpInfo.bmiHeader.biWidth = clipDimension.right-clipDimension.left;
			bmpInfo.bmiHeader.biHeight = clipDimension.bottom-clipDimension.top;

			if(GetDIBits(hDC, hClip, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS) == 0)
			{
				error = 7;
				goto LTD_ERR_GET_DI_BITS_INFO;
			}

			data = malloc(bmpInfo.bmiHeader.biSizeImage);
			if(data == NULL)
			{
				error = 8;
				goto LTD_ERR_MALLOC;
			}

			bmpInfo.bmiHeader.biCompression = BI_RGB;
			if(GetDIBits(hDC, hClip, 0, bmpInfo.bmiHeader.biHeight, data, &bmpInfo, DIB_RGB_COLORS) == 0)
			{
				error = 9;
				goto LTD_ERR_GET_DI_BITS;
			}

			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//GL_TEXTURE_WRAP?

			glPixelStorei(GL_UNPACK_ALIGNMENT, UNPACK_ALIGN);

			//glTexImage2D(GL_TEXTURE_2D, 0, PIXEL_FORMAT, clipDimension.right-clipDimension.left, clipDimension.bottom-clipDimension.top, 0, PIXEL_DATA_FORMAT, PIXEL_ELEMENT_ENUM, data);
			gluBuild2DMipmaps(GL_TEXTURE_2D, PIXEL_COMPONENTS, clipDimension.right-clipDimension.left, clipDimension.bottom-clipDimension.top, PIXEL_DATA_FORMAT, PIXEL_ELEMENT_ENUM, data);

			free(data);
			data = NULL;
		}
	}

	LTD_ERR_GET_DI_BITS:
	free(data);
	LTD_ERR_MALLOC:
	LTD_ERR_GET_DI_BITS_INFO:
	hSrc = (HBITMAP)SelectObject(src, hSrc);
	LTD_ERR_SET_DI_BITS:
	DeleteObject(hClip);
	LTD_ERR_CREATE_COMPATIBLE_BITMAP_CLIP:
	DeleteObject(hSrc);
	LTD_ERR_CREATE_COMPATIBLE_BITMAP_SRC:
	DeleteDC(dst);
	LTD_ERR_CREATE_COMPATIBLE_DC_DST:
	DeleteDC(src);
	LTD_ERR_CREATE_COMPATIBLE_DC_SRC:
	ReleaseDC(NULL, hDC);
    LTD_ERR_GET_DC:
    return error;
}
