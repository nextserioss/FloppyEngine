#include "../../Header/Engine/SpriteManager.h"
#include "../../Header/Engine/LogManager.h"

SpriteManager* SpriteManager::instance = NULL;

SpriteManager::SpriteManager()
{

}

void SpriteManager::ModifyDIBPaletteEntry(HBITMAP hbmColour, COLORREF crTarget, COLORREF crModify)
{
	if (hbmColour == NULL)
	{
		return;
	}
	BITMAP bm;
	GetObject(hbmColour, sizeof(BITMAP), &bm);
	// 8비트 이하만 지원
	if (bm.bmBitsPixel > 8)
	{
		return;
	}
	int nColorTableEntries = (1 << bm.bmBitsPixel);
	// BITMAPINFO 구조체 할당
	DWORD dwBiSize = sizeof(BITMAPINFOHEADER) + nColorTableEntries * sizeof(RGBQUAD);
	BITMAPINFO* pbi = (BITMAPINFO*)new BYTE[dwBiSize];
	if (!pbi)
	{
		return;
	}
	ZeroMemory(pbi, dwBiSize);
	pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbi->bmiHeader.biWidth = bm.bmWidth;
	pbi->bmiHeader.biHeight = bm.bmHeight;
	pbi->bmiHeader.biPlanes = 1;
	pbi->bmiHeader.biBitCount = bm.bmBitsPixel;
	pbi->bmiHeader.biCompression = BI_RGB;
	pbi->bmiHeader.biClrUsed = nColorTableEntries;
	// 픽셀 데이터 크기 계산
	DWORD dwBytesPerLine = ((bm.bmWidth * bm.bmBitsPixel + 31) / 32) * 4;
	DWORD dwTotalSize = dwBytesPerLine * bm.bmHeight;
	BYTE* pPixels = new BYTE[dwTotalSize];
	if (!pPixels)
	{
		delete[](BYTE*)pbi;
		return;
	}
	// 픽셀 데이터 및 팔레트 읽기
	HDC hdc = GetDC(NULL);
	int nScanlines = GetDIBits(hdc, hbmColour, 0, bm.bmHeight, pPixels, pbi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);
	if (nScanlines == 0)
	{
		delete[] pPixels;
		delete[](BYTE*)pbi;
		return;
	}
	// 팔레트에서 타겟 색상을 변경
	RGBQUAD* pPalette = pbi->bmiColors;
	for (int i = 0; i < nColorTableEntries; ++i)
	{
		if (pPalette[i].rgbRed == GetRValue(crTarget) &&
			pPalette[i].rgbGreen == GetGValue(crTarget) &&
			pPalette[i].rgbBlue == GetBValue(crTarget))
		{
			/*
			DLOG("Modify Palette Entry: Index %d from (%d,%d,%d) to (%d,%d,%d)", i,
				pPalette[i].rgbRed, pPalette[i].rgbGreen, pPalette[i].rgbBlue,
				GetRValue(crModify), GetGValue(crModify), GetBValue(crModify));
			*/
			pPalette[i].rgbRed = GetRValue(crModify);
			pPalette[i].rgbGreen = GetGValue(crModify);
			pPalette[i].rgbBlue = GetBValue(crModify);
		}
	}

	// 올바른 LOGPALETTE 생성
	DWORD dwLogPalSize = sizeof(LOGPALETTE) + (nColorTableEntries - 1) * sizeof(PALETTEENTRY);
	LOGPALETTE* pLogPal = (LOGPALETTE*)new BYTE[dwLogPalSize];
	if (!pLogPal)
	{
		delete[] pPixels;
		delete[](BYTE*)pbi;
		return;
	}

	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = (WORD)nColorTableEntries;
	for (int i = 0; i < nColorTableEntries; ++i)
	{
		pLogPal->palPalEntry[i].peRed = pPalette[i].rgbRed;
		pLogPal->palPalEntry[i].peGreen = pPalette[i].rgbGreen;
		pLogPal->palPalEntry[i].peBlue = pPalette[i].rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
	}

	// 비트맵 업데이트
	HDC memDC = CreateCompatibleDC(NULL);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hbmColour);

	// 논리 팔레트 생성 및 선택
	HPALETTE hPal = CreatePalette(pLogPal);
	HPALETTE hOldPal = NULL;
	if (hPal)
	{
		hOldPal = SelectPalette(memDC, hPal, FALSE);
		RealizePalette(memDC);

		// SetDIBColorTable 호출 (팔레트 직접 수정)
		SetDIBColorTable(memDC, 0, nColorTableEntries, pbi->bmiColors);
	}

	// SetDIBits 호출
	SetDIBits(memDC, hbmColour, 0, bm.bmHeight, pPixels, pbi, DIB_RGB_COLORS);

	// 정리
	SelectObject(memDC, hOldBitmap);
	if (hPal)
	{
		if (hOldPal) SelectPalette(memDC, hOldPal, FALSE);
		DeleteObject(hPal);
	}
	DeleteDC(memDC);

	delete[] pLogPal;
	delete[] pPixels;
	delete[](BYTE*)pbi;
}

HBITMAP SpriteManager::CreateTintedBitmap(HBITMAP hbmColour, COLORREF crTint, float fBrightness)
{
	if (hbmColour == NULL)
	{
		return NULL;
	}

	BITMAP bm;
	GetObject(hbmColour, sizeof(BITMAP), &bm);

	// 8비트 이하만 지원
	if (bm.bmBitsPixel > 8)
	{
		return NULL;
	}

	int nColorTableEntries = (1 << bm.bmBitsPixel);

	// BITMAPINFO 구조체 할당
	DWORD dwBiSize = sizeof(BITMAPINFOHEADER) + nColorTableEntries * sizeof(RGBQUAD);
	BITMAPINFO* pbi = (BITMAPINFO*)new BYTE[dwBiSize];
	if (!pbi)
	{
		return NULL;
	}

	ZeroMemory(pbi, dwBiSize);
	pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbi->bmiHeader.biWidth = bm.bmWidth;
	pbi->bmiHeader.biHeight = bm.bmHeight;
	pbi->bmiHeader.biPlanes = 1;
	pbi->bmiHeader.biBitCount = bm.bmBitsPixel;
	pbi->bmiHeader.biCompression = BI_RGB;
	pbi->bmiHeader.biClrUsed = nColorTableEntries;

	// 픽셀 데이터 크기 계산
	DWORD dwBytesPerLine = ((bm.bmWidth * bm.bmBitsPixel + 31) / 32) * 4;
	DWORD dwTotalSize = dwBytesPerLine * bm.bmHeight;
	BYTE* pPixels = new BYTE[dwTotalSize];
	if (!pPixels)
	{
		delete[](BYTE*)pbi;
		return NULL;
	}

	// 픽셀 데이터 및 팔레트 읽기
	HDC hdc = GetDC(NULL);
	int nScanlines = GetDIBits(hdc, hbmColour, 0, bm.bmHeight, pPixels, pbi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);

	if (nScanlines == 0)
	{
		delete[] pPixels;
		delete[](BYTE*)pbi;
		return NULL;
	}

	// 팔레트의 각 색상을 회색으로 변환 후 밝기 조정 및 Multiply 블렌딩
	RGBQUAD* pPalette = pbi->bmiColors;
	BYTE tintR = GetRValue(crTint);
	BYTE tintG = GetGValue(crTint);
	BYTE tintB = GetBValue(crTint);

	// 밝기 값 클램핑 (일반적으로 0.0 ~ 2.0 범위)
	if (fBrightness < 0.0f) fBrightness = 0.0f;

	for (int i = 0; i < nColorTableEntries; ++i)
	{
		BYTE r = pPalette[i].rgbRed;
		BYTE g = pPalette[i].rgbGreen;
		BYTE b = pPalette[i].rgbBlue;

		// 완전 파란색(0, 0, 255)은 완전 검은색으로 변경
		if (r == 0 && g == 0 && b == 255)
		{
			pPalette[i].rgbRed = 0;
			pPalette[i].rgbGreen = 0;
			pPalette[i].rgbBlue = 0;
			continue;
		}

		// 그레이스케일 변환 (표준 luminance 공식 사용)
		float gray = 0.299f * r + 0.587f * g + 0.114f * b;

		// 밝기 조정
		gray *= fBrightness;

		// 255 초과 방지
		if (gray > 255.0f) gray = 255.0f;

		BYTE grayByte = (BYTE)gray;

		// Multiply 블렌딩: (gray / 255) * (tint / 255) * 255
		int newR = (grayByte * tintR) / 255;
		int newG = (grayByte * tintG) / 255;
		int newB = (grayByte * tintB) / 255;

		// 255 초과 방지 (안전장치)
		pPalette[i].rgbRed = (BYTE)(newR > 255 ? 255 : newR);
		pPalette[i].rgbGreen = (BYTE)(newG > 255 ? 255 : newG);
		pPalette[i].rgbBlue = (BYTE)(newB > 255 ? 255 : newB);
	}

	// 새 비트맵 생성
	HDC memDC = CreateCompatibleDC(NULL);
	HBITMAP hNewBitmap = CreateDIBitmap(hdc = GetDC(NULL), &pbi->bmiHeader, CBM_INIT,
		pPixels, pbi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);

	if (hNewBitmap)
	{
		// 논리 팔레트 생성 및 적용
		DWORD dwLogPalSize = sizeof(LOGPALETTE) + (nColorTableEntries - 1) * sizeof(PALETTEENTRY);
		LOGPALETTE* pLogPal = (LOGPALETTE*)new BYTE[dwLogPalSize];

		if (pLogPal)
		{
			pLogPal->palVersion = 0x300;
			pLogPal->palNumEntries = (WORD)nColorTableEntries;

			for (int i = 0; i < nColorTableEntries; ++i)
			{
				pLogPal->palPalEntry[i].peRed = pPalette[i].rgbRed;
				pLogPal->palPalEntry[i].peGreen = pPalette[i].rgbGreen;
				pLogPal->palPalEntry[i].peBlue = pPalette[i].rgbBlue;
				pLogPal->palPalEntry[i].peFlags = 0;
			}

			HPALETTE hPal = CreatePalette(pLogPal);
			if (hPal)
			{
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hNewBitmap);
				HPALETTE hOldPal = SelectPalette(memDC, hPal, FALSE);
				RealizePalette(memDC);
				SetDIBColorTable(memDC, 0, nColorTableEntries, pbi->bmiColors);

				SelectPalette(memDC, hOldPal, FALSE);
				SelectObject(memDC, hOldBitmap);
				DeleteObject(hPal);
			}

			delete[] pLogPal;
		}
	}

	DeleteDC(memDC);
	delete[] pPixels;
	delete[](BYTE*)pbi;

	return hNewBitmap;
}

HBITMAP SpriteManager::CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	HDC hdcSourceMem, hdcDestMem;
	HBITMAP hbmMask;
	BITMAP bm;
	HBITMAP hbmOldSource, hbmOldDest;

	// 1. 컬러 비트맵 정보 가져오기
	GetObject(hbmColour, sizeof(BITMAP), &bm);
	// 2. 흑백(1비트) 마스크 비트맵 생성 (크기는 원본과 동일)
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	// 3. 임시 메모리 DC 생성
	hdcSourceMem = CreateCompatibleDC(0);
	hdcDestMem = CreateCompatibleDC(0);

	// 4. DC에 각 비트맵 할당
	hbmOldSource = (HBITMAP)SelectObject(hdcSourceMem, hbmColour);
	hbmOldDest = (HBITMAP)SelectObject(hdcDestMem, hbmMask);

	// 5. 투명하게 만들 색상을 소스 DC의 배경색으로 설정
	SetBkColor(hdcSourceMem, crTransparent);

	// 6. 소스에서 데스트(마스크)로 BitBlt (SRCCOPY)
	BitBlt(hdcDestMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcSourceMem, 0, 0, SRCCOPY);

	// 7. 사용한 DC 선택 후 해제
	SelectObject(hdcSourceMem, hbmOldSource);
	SelectObject(hdcDestMem, hbmOldDest);
	
	DeleteDC(hdcSourceMem);
	DeleteDC(hdcDestMem);

	// 8.원본 이미지의 투명 영역을 검은색으로 변경
	//BitBlt(hdcSourceMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcDestMem, 0, 0, SRCINVERT);
	ModifyDIBPaletteEntry(hbmColour, crTransparent, RGB(0,0,0));

	return hbmMask;
}

//파일에서 읽어	올때 사용
void SpriteManager::CreateSprite(string strImagePath, bool bTransparent)
{
	//중복된 이미지 패스가 아니라면 추가해줌
	if (m_SpriteMap.find(strImagePath) == m_SpriteMap.end())
	{
		DLOG("%s", strImagePath.c_str());
		HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, TEXT(strImagePath.c_str()), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		m_SpriteMap.insert(pair<string, HBITMAP>(strImagePath, hBitmap));
		//투명이 되어야 한다면
		if (bTransparent == true)
		{
			//중복된 이미지 패스가 아니라면 추가해줌
			if (m_MaskSpriteMap.find(strImagePath) == m_MaskSpriteMap.end())
			{
				DLOG("Mask %s", strImagePath.c_str());
				HBITMAP hMaskBitmap = CreateBitmapMask(hBitmap, RGB(0, 0, 255));
				m_MaskSpriteMap.insert(pair<string, HBITMAP>(strImagePath, hMaskBitmap));
			}
		}
	}
}


//파일에서 읽어	올때 사용 (색상 변경 포함 8비트만 적용됨) strImagePath이름 뒤에 RGB000000000 형식으로 색상값이 붙는다.
void SpriteManager::CreateSprite(string strImagePath, COLORREF crColor, bool bTransparent)
{
	BYTE r = GetRValue(crColor);
	BYTE g = GetGValue(crColor);
	BYTE b = GetBValue(crColor);

	string colorString(20, '\0');
	int written_len = snprintf(&colorString[0], colorString.size(), "RGB%03d%03d%03d", (int)r, (int)g, (int)b);
	if (written_len > 0) {
		colorString.resize(written_len);
	}
	string spriteMapImagePath = strImagePath + colorString;

	//중복된 이미지 패스가 아니라면 추가해줌
	if (m_SpriteMap.find(spriteMapImagePath) == m_SpriteMap.end())
	{
		DLOG("%s", spriteMapImagePath.c_str());
		HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, TEXT(strImagePath.c_str()), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);		
		m_SpriteMap.insert(pair<string, HBITMAP>(spriteMapImagePath, hBitmap));
		//투명이 되어야 한다면
		if (bTransparent == true)
		{
			//중복된 이미지 패스가 아니라면 추가해줌
			if (m_MaskSpriteMap.find(spriteMapImagePath) == m_MaskSpriteMap.end())
			{
				DLOG("Mask %s", spriteMapImagePath.c_str());
				HBITMAP hMaskBitmap = CreateBitmapMask(hBitmap, RGB(0, 0, 255));
				m_MaskSpriteMap.insert(pair<string, HBITMAP>(spriteMapImagePath, hMaskBitmap));				
			}
		}

		ModifyDIBPaletteEntry(hBitmap, RGB(255, 255, 255), crColor);
	}
}

//리소스에서 읽어 올때 사용
void SpriteManager::CreateSprite(string strImagePath, int nResourceID, bool bTransparent)
{
	//중복된 이미지 패스가 아니라면 추가해줌
	if (m_SpriteMap.find(strImagePath) == m_SpriteMap.end())
	{
		DLOG("%s", strImagePath.c_str());
		HBITMAP hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(nResourceID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		m_SpriteMap.insert(pair<string, HBITMAP>(strImagePath, hBitmap));
		//투명이 되어야 한다면
		if (bTransparent == true)
		{
			//중복된 이미지 패스가 아니라면 추가해줌
			if (m_MaskSpriteMap.find(strImagePath) == m_MaskSpriteMap.end())
			{
				DLOG("Mask %s", strImagePath.c_str());
				HBITMAP hMaskBitmap = CreateBitmapMask(hBitmap, RGB(0, 0, 255));
				m_MaskSpriteMap.insert(pair<string, HBITMAP>(strImagePath, hMaskBitmap));
			}
		}
	}
}

//리소스에서 읽어 올때 사용 (색상 변경 포함 8비트만 적용됨)
void SpriteManager::CreateSprite(string strImagePath, int nResourceID, COLORREF crColor, bool bTransparent)
{
	BYTE r = GetRValue(crColor);
	BYTE g = GetGValue(crColor);
	BYTE b = GetBValue(crColor);

	string colorString(20, '\0');
	int written_len = snprintf(&colorString[0], colorString.size(), "RGB%03d%03d%03d", (int)r, (int)g, (int)b);
	if (written_len > 0) {
		colorString.resize(written_len);
	}
	string spriteMapImagePath = strImagePath + colorString;

	//중복된 이미지 패스가 아니라면 추가해줌
	if (m_SpriteMap.find(spriteMapImagePath) == m_SpriteMap.end())
	{
		DLOG("%s", spriteMapImagePath.c_str());
		HBITMAP hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(nResourceID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION);
		m_SpriteMap.insert(pair<string, HBITMAP>(spriteMapImagePath, hBitmap));
		//투명이 되어야 한다면
		if (bTransparent == true)
		{
			//중복된 이미지 패스가 아니라면 추가해줌
			if (m_MaskSpriteMap.find(spriteMapImagePath) == m_MaskSpriteMap.end())
			{
				DLOG("Mask %s", spriteMapImagePath.c_str());
				HBITMAP hMaskBitmap = CreateBitmapMask(hBitmap, RGB(0, 0, 255));
				m_MaskSpriteMap.insert(pair<string, HBITMAP>(spriteMapImagePath, hMaskBitmap));				
			}
		}

		ModifyDIBPaletteEntry(hBitmap, RGB(255, 255, 255), crColor);
	}
}

void SpriteManager::CreateAddModifySprite(string strImagePath, int nResourceID, COLORREF crColor, float fBrightness, bool bTransparent)
{
	//중복된 이미지 패스가 아니라면 추가해줌
	if (m_SpriteMap.find(strImagePath) == m_SpriteMap.end())
	{
		DLOG("%s", strImagePath.c_str());
		HBITMAP hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(nResourceID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION);		
		m_SpriteMap.insert(pair<string, HBITMAP>(strImagePath, hBitmap));
		HBITMAP modifyHBitmap = CreateTintedBitmap(hBitmap, crColor, fBrightness);
		m_ModifySpriteMap.insert(pair<string, HBITMAP>(strImagePath, modifyHBitmap));
		//투명이 되어야 한다면
		if (bTransparent == true)
		{
			//중복된 이미지 패스가 아니라면 추가해줌
			if (m_MaskSpriteMap.find(strImagePath) == m_MaskSpriteMap.end())
			{
				DLOG("Mask %s", strImagePath.c_str());
				HBITMAP hMaskBitmap = CreateBitmapMask(hBitmap, RGB(0, 0, 255));
				m_MaskSpriteMap.insert(pair<string, HBITMAP>(strImagePath, hMaskBitmap));
			}
		}
	}
}

HBITMAP SpriteManager::GetSprite(string strImagePath)
{	
	return m_SpriteMap.find(strImagePath)->second;
}

HBITMAP SpriteManager::GetModifySprite(string strImagePath)
{
	return m_ModifySpriteMap.find(strImagePath)->second;
}

HBITMAP SpriteManager::GetMaskSprite(string strImagePath)
{
	return m_MaskSpriteMap.find(strImagePath)->second;
}

bool SpriteManager::isMaskImage(string strImagePath)
{
	//마스크 이미지가 있다면 true
	if (m_MaskSpriteMap.find(strImagePath) != m_MaskSpriteMap.end())
	{
		return true;
	}
	return false;
}

bool SpriteManager::isModifyImage(string strImagePath)
{
	//마스크 이미지가 있다면 true
	if (m_ModifySpriteMap.find(strImagePath) != m_ModifySpriteMap.end())
	{
		return true;
	}
	return false;
}