#include<windows.h>
#include<commdlg.h>

#include<exception>
#include<iostream>
#include<fstream>
#include<string>

#ifndef COMMONDLG
#define COMMONDLG

void initFileCommDlg(OPENFILENAME* commDlg, HWND hwnd)
{
	commDlg->Flags = 0;
	commDlg->FlagsEx = 0;
	commDlg->hInstance = nullptr;
	commDlg->hwndOwner = hwnd;
	commDlg->lCustData = 0;
	commDlg->lpfnHook = nullptr;
	commDlg->lpstrCustomFilter = nullptr;
	commDlg->lpstrDefExt = L"bmp";
	commDlg->lpstrFile = nullptr;
	commDlg->lpstrFileTitle = nullptr;
	commDlg->lpstrFilter = L"bmp files (*.BMP)\0*.bmp\0"
		"all files (*.*)\0*.*\0";
	commDlg->lpstrInitialDir = nullptr;
	commDlg->lpstrTitle = nullptr;
	commDlg->lpTemplateName = nullptr;
	commDlg->lStructSize = sizeof(OPENFILENAME);
	commDlg->nFileExtension = 0;
	commDlg->nFileOffset = 0;
	commDlg->nFilterIndex = 0;
	commDlg->nMaxCustFilter = 0;
	commDlg->nMaxFile = MAX_PATH;
	commDlg->nMaxFileTitle = MAX_PATH;
}

BOOL openFileCommDlg(HWND hwnd, OPENFILENAME* commDlg, std::wstring* filePath, std::wstring* fileName)
{
	filePath->resize(MAX_PATH);
	fileName->resize(MAX_PATH);

	commDlg->hwndOwner = hwnd;
	commDlg->lpstrFile = const_cast<wchar_t*>(filePath->c_str());
	commDlg->lpstrFileTitle = const_cast<wchar_t*>(fileName->c_str());
	commDlg->Flags = OFN_CREATEPROMPT | OFN_HIDEREADONLY;

	return GetOpenFileName(commDlg);
}

BOOL saveFileCommDlg(HWND hwnd, OPENFILENAME* commDlg, std::wstring* filePath, std::wstring* fileName)
{
	filePath->resize(MAX_PATH);
	fileName->resize(MAX_PATH);

	commDlg->hwndOwner = hwnd;
	commDlg->lpstrFile = const_cast<wchar_t*>(filePath->c_str());
	commDlg->lpstrFileTitle = const_cast<wchar_t*>(fileName->c_str());
	commDlg->Flags = OFN_OVERWRITEPROMPT;

	return GetSaveFileName(commDlg);
}

void setbmpInfo(HBITMAP hBmp, PBITMAPINFO *bpInfo)
{
	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	uint8_t bits = bmp.bmBitsPixel;

	if (bits <= 8)
	{
		*bpInfo = reinterpret_cast<PBITMAPINFO>(new char[sizeof(BITMAPINFOHEADER) + (1 << bits) * sizeof(RGBQUAD)]);
		(*bpInfo)->bmiHeader.biClrUsed = 1 << bits;
	}
	else
	{
		*bpInfo = reinterpret_cast<PBITMAPINFO>(new char[sizeof(BITMAPINFOHEADER)]);
		(*bpInfo)->bmiHeader.biClrUsed = 0;
	}

	(*bpInfo)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	(*bpInfo)->bmiHeader.biWidth = bmp.bmWidth;
	(*bpInfo)->bmiHeader.biHeight = bmp.bmHeight;
	(*bpInfo)->bmiHeader.biPlanes = 1;
	(*bpInfo)->bmiHeader.biBitCount = bits;
	(*bpInfo)->bmiHeader.biCompression = BI_RGB;
	(*bpInfo)->bmiHeader.biSizeImage = (((bmp.bmWidth * bits + 31) & ~31) >> 3) * bmp.bmHeight;

	HDC screenDC = GetDC(nullptr);
	(*bpInfo)->bmiHeader.biXPelsPerMeter = GetDeviceCaps(screenDC, HORZRES) / GetDeviceCaps(screenDC, HORZSIZE)/1000;
	(*bpInfo)->bmiHeader.biXPelsPerMeter = GetDeviceCaps(screenDC, VERTRES) / GetDeviceCaps(screenDC, VERTSIZE) / 1000;
	ReleaseDC(nullptr, screenDC);

	(*bpInfo)->bmiHeader.biClrImportant = 0;
}

BOOL saveFile(HDC hDC, std::wstring filePath, HBITMAP hBmp)
{
	try{
		PBITMAPINFO bpInfo = nullptr;
		setbmpInfo(hBmp, &bpInfo);

		BITMAPFILEHEADER bmpFileH;
		bmpFileH.bfType = 0x4D42;
		bmpFileH.bfSize = sizeof(BITMAPFILEHEADER) + bpInfo->bmiHeader.biSize + bpInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD) + bpInfo->bmiHeader.biSizeImage;
		bmpFileH.bfReserved1 = 0;
		bmpFileH.bfReserved2 = 0;
		bmpFileH.bfOffBits = sizeof(BITMAPFILEHEADER) + bpInfo->bmiHeader.biSize + bpInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);

		std::ofstream saveFile;
		saveFile.exceptions(std::ofstream::badbit | std::ofstream::failbit);
		saveFile.open(filePath.c_str(), std::ios::binary | std::ios::out);

		saveFile.write(reinterpret_cast<char*>(&bmpFileH), sizeof(BITMAPFILEHEADER));
		saveFile.write(reinterpret_cast<char*>(bpInfo), sizeof(BITMAPINFOHEADER) + bpInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD));

		PBYTE bits = new BYTE[bpInfo->bmiHeader.biSizeImage];
		if (GetDIBits(hDC, hBmp, 0, bpInfo->bmiHeader.biHeight, bits, bpInfo, DIB_RGB_COLORS) == 0)
			throw std::logic_error("GetDIBits() failed\n");
		saveFile.write(reinterpret_cast<char*>(bits), bpInfo->bmiHeader.biSizeImage);

		saveFile.close();
		delete[] bits;
		delete[] bpInfo;
	} catch (std::exception& e)
	{
		throw e;
		return TRUE;
	}
	return FALSE;
}

#endif