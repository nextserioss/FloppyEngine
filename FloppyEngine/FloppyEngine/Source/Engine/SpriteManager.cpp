#include "../../Header/Engine/SpriteManager.h"
#include "../../Header/Engine/LogManager.h"

SpriteManager* SpriteManager::instance = NULL;

SpriteManager::SpriteManager()
{

}

HBITMAP SpriteManager::CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	HDC hdcSourceMem, hdcDestMem;
	HBITMAP hbmMask;
	BITMAP bm;

	// 1. 컬러 비트맵 정보 가져오기
	GetObject(hbmColour, sizeof(BITMAP), &bm);
	// 2. 흑백(1비트) 마스크 비트맵 생성 (크기는 원본과 동일)
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	// 3. 임시 메모리 DC 생성
	hdcSourceMem = CreateCompatibleDC(0);
	hdcDestMem = CreateCompatibleDC(0);

	// 4. DC에 각 비트맵 할당
	SelectObject(hdcSourceMem, hbmColour);
	SelectObject(hdcDestMem, hbmMask);

	// 5. 투명하게 만들 색상을 소스 DC의 배경색으로 설정
	SetBkColor(hdcSourceMem, crTransparent);

	// 6. 소스에서 데스트로 BitBlt (SRCCOPY)
	BitBlt(hdcDestMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcSourceMem, 0, 0, SRCCOPY);

	// 7. 마스크를 사용하여 원본 이미지의 투명 영역을 검은색으로 변경
	BitBlt(hdcSourceMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcDestMem, 0, 0, SRCINVERT);

	// 8. 사용한 DC 해제
	DeleteDC(hdcSourceMem);
	DeleteDC(hdcDestMem);

	return hbmMask;
}

//bm.bmBitsPixel 변수가 32비트만 동작하도록 되어있음
void SpriteManager::ChangeBitmapColorMultiply(HBITMAP hbmColour, COLORREF crMultiplyColor)
{
	if (hbmColour == NULL)
	{
		return;
	}

	// 1. 비트맵 정보 얻기
	BITMAP bm;
	GetObject(hbmColour, sizeof(BITMAP), &bm);

	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(bi));

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = bm.bmWidth;
	bi.bmiHeader.biHeight = bm.bmHeight; // DIB는 음수 높이로 저장될 수 있지만, 여기서는 양수로 처리
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = bm.bmBitsPixel; // 32
	bi.bmiHeader.biCompression = BI_RGB; // 압축 없음

	if (bm.bmBitsPixel != 32)
	{
		return;
	}

	// 2. 픽셀 데이터 크기 계산 및 메모리 할당
	// 32비트 정렬을 고려하여 실제 픽셀 데이터의 크기를 계산합니다.
	DWORD dwWidth = bm.bmWidth;
	DWORD dwHeight = bm.bmHeight;
	DWORD dwBPP = bm.bmBitsPixel; // 32

	// 비트맵 너비의 바이트 수 (4바이트 정렬 고려)
	DWORD dwBytesPerLine = ((dwWidth * dwBPP + 31) / 32) * 4;
	DWORD dwTotalSize = dwBytesPerLine * dwHeight;

	BYTE* pPixels = new BYTE[dwTotalSize];
	if (!pPixels) return;

	// 3. 픽셀 데이터 읽기 (DIB 형식으로)
	HDC hdc = GetDC(NULL);
	int nScanlines = GetDIBits(hdc, hbmColour, 0, dwHeight, pPixels, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);

	if (nScanlines == 0)
	{
		delete[] pPixels;
		return;
	}

	// 4. 픽셀 조작
	// 픽셀 포맷은 보통 BGR 순서입니다.
	for (DWORD y = 0; y < dwHeight; ++y)
	{
		// 현재 라인의 시작 포인터 (DIB는 보통 아래에서 위로 저장되므로, y를 dwHeight-1-y로 처리할 수도 있지만, 
		// GetDIBits를 사용하면 보통 y축 순서대로 읽습니다.)
		BYTE* pLine = pPixels + y * dwBytesPerLine;

		for (DWORD x = 0; x < dwWidth; ++x)
		{
			// 32비트는 4바이트
			BYTE* pPixel = pLine + x * 4;

			//(255,0,0)으로 multiply 합성
			int R_new = (int)(pPixel[2] * GetRValue(crMultiplyColor)) / 255;   //R
			int G_new = (int)(pPixel[1] * GetGValue(crMultiplyColor)) / 255;   //G
			int B_new = (int)(pPixel[0] * GetBValue(crMultiplyColor)) / 255;   //B

			// 클리핑
			pPixel[2] = (BYTE)R_new; // R
			pPixel[1] = (BYTE)G_new; // G
			pPixel[0] = (BYTE)B_new; // B
			// 알파 채널 (pPixel[3])은 그대로 유지하거나 255로 설정
		}
	}


	// 5. 비트맵 데이터 업데이트
	hdc = GetDC(NULL);
	SetDIBits(hdc, hbmColour, 0, dwHeight, pPixels, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	ReleaseDC(NULL, hdc);
	// 메모리 해제
	delete[] pPixels;
}

//bmBitsPixel 8비트 대응 버전 (흰색 부분을 특정 색으로 변경)
void SpriteManager::ChangeBitmapColor(HBITMAP hbmColour, COLORREF crColor)
{
	if (hbmColour == NULL) return;

	// 1. 비트맵 정보 얻기
	BITMAP bm;
	GetObject(hbmColour, sizeof(BITMAP), &bm);

	// BITMAPINFO 구조체 준비 - 8비트 비트맵을 위해 팔레트 공간을 고려해야 합니다.
	// 8비트: BITMAPINFOHEADER + 256 * RGBQUAD (팔레트 있음)
	int nColorTableEntries = 0;
	if (bm.bmBitsPixel <= 8) 
	{
		nColorTableEntries = (1 << bm.bmBitsPixel); // 2^8 = 256
	}
	else
	{
		//8비트 이하가 아니면 이 함수는 지원하지 않음
		return;
	}

	// BITMAPINFO 구조체 크기 계산
	DWORD dwBiSize = sizeof(BITMAPINFOHEADER) + nColorTableEntries * sizeof(RGBQUAD);
	BITMAPINFO* pbi = (BITMAPINFO*)new BYTE[dwBiSize];
	if (!pbi) return;
	ZeroMemory(pbi, dwBiSize);

	pbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbi->bmiHeader.biWidth = bm.bmWidth;
	pbi->bmiHeader.biHeight = bm.bmHeight;
	pbi->bmiHeader.biPlanes = 1;
	pbi->bmiHeader.biBitCount = bm.bmBitsPixel;
	pbi->bmiHeader.biCompression = BI_RGB;
	pbi->bmiHeader.biClrUsed = nColorTableEntries;
	
	// 2. 픽셀 데이터 크기 계산 및 메모리 할당
	DWORD dwWidth = bm.bmWidth;
	DWORD dwHeight = bm.bmHeight;
	DWORD dwBPP = bm.bmBitsPixel;

	DWORD dwBytesPerLine = ((dwWidth * dwBPP + 31) / 32) * 4;
	DWORD dwTotalSize = dwBytesPerLine * dwHeight;

	BYTE* pPixels = new BYTE[dwTotalSize];
	if (!pPixels) { delete[](BYTE*)pbi; return; }
	

	// 3. 픽셀 데이터 및 팔레트 정보 읽기 (DIB 형식으로)
	HDC hdc = GetDC(NULL);
	// 8비트의 경우 pbi->bmiColors에 팔레트 정보가 읽혀집니다.
	int nScanlines = GetDIBits(hdc, hbmColour, 0, dwHeight, pPixels, pbi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);

	if (nScanlines == 0)
	{
		delete[] pPixels;
		delete[](BYTE*)pbi;
		return;
	}

	// 4. 픽셀 조작 (8비트)
	if (dwBPP <= 8) // 8비트 이하 처리 (팔레트 색상 변경)
	{
		// 8비트 비트맵은 픽셀 데이터가 아닌 팔레트(bmiColors)를 조작해야 합니다.
		RGBQUAD* pPalette = pbi->bmiColors;
		for (int i = 0; i < nColorTableEntries; ++i)
		{
			//bmiColors가 흰색이면
			if (pPalette[i].rgbRed == 255 && pPalette[i].rgbGreen == 255 && pPalette[i].rgbBlue == 255)
			{
				pPalette[i].rgbRed = GetRValue(crColor);
				pPalette[i].rgbGreen = GetGValue(crColor);
				pPalette[i].rgbBlue = GetBValue(crColor);
				// rgbReserved는 그대로 유지
			}									
		}		
	}
	else
	{
		delete[] pPixels;
		delete[](BYTE*)pbi;
		return;
	}

	// 5. 비트맵 데이터 업데이트
	HDC memDC = CreateCompatibleDC(NULL);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hbmColour);

	// (선택 사항이지만 권장) 논리 팔레트 생성 및 선택
	HPALETTE hPal = CreatePalette((LOGPALETTE*)pbi); // BITMAPINFO 구조체를 사용하여 LOGPALETTE로 변환 가능
	HPALETTE hOldPal = SelectPalette(memDC, hPal, FALSE);
	RealizePalette(memDC);

	// SetDIBColorTable 호출
	SetDIBColorTable(memDC, 0, nColorTableEntries, pbi->bmiColors);

	// SetDIBits 호출 (memDC 사용)
	SetDIBits(memDC, hbmColour, 0, dwHeight, pPixels, pbi, DIB_RGB_COLORS);

	// 뒷정리
	SelectObject(memDC, hOldBitmap);
	if (hPal)
	{
		SelectPalette(memDC, hOldPal, FALSE);
		DeleteObject(hPal);
	}
	DeleteDC(memDC);
		
	// 메모리 해제
	delete[] pPixels;
	delete[](BYTE*)pbi;
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
void SpriteManager::CreateSprite(string strImagePath, COLORREF crMultiplyColor, bool bTransparent)
{
	BYTE r = GetRValue(crMultiplyColor);
	BYTE g = GetGValue(crMultiplyColor);
	BYTE b = GetBValue(crMultiplyColor);

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
				ChangeBitmapColor(hBitmap, crMultiplyColor);
			}
		}
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
void SpriteManager::CreateSprite(string strImagePath, int nResourceID, COLORREF crMultiplyColor, bool bTransparent)
{
	BYTE r = GetRValue(crMultiplyColor);
	BYTE g = GetGValue(crMultiplyColor);
	BYTE b = GetBValue(crMultiplyColor);

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
				ChangeBitmapColor(hBitmap, crMultiplyColor);
			}
		}
	}
}

HBITMAP SpriteManager::GetSprite(string strImagePath)
{	
	return m_SpriteMap.find(strImagePath)->second;
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