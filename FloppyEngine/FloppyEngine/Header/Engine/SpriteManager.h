#pragma once

#include <stdio.h>
#include <string>
#include <Windows.h>
#include <unordered_map>

using namespace std;

class SpriteManager
{
private:
	SpriteManager();
	~SpriteManager(){};
	static SpriteManager* instance;
	unordered_map<string, HBITMAP> m_SpriteMap;
	unordered_map<string, HBITMAP> m_ModifySpriteMap;
	unordered_map<string, HBITMAP> m_MaskSpriteMap;
public:
	static SpriteManager* GetInstance()
	{
		if (instance == NULL)
		{
			instance = new SpriteManager();
			atexit(Destroy);
		}
		return instance;
	}
	static void Destroy()
	{
		delete instance;
	}
	//특정 부분	색상을 다른 색상으로 변경 한다. 8비트 전용
	void ModifyDIBPaletteEntry(HBITMAP hbmColour, COLORREF crTarget, COLORREF crModify);
	HBITMAP CreateTintedBitmap(HBITMAP hbmColour, COLORREF crTint, float fBrightness);
	HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);
	void CreateSprite(string strImagePath, bool bTransparent = false);
	void CreateSprite(string strImagePath, COLORREF crColor, bool bTransparent = false);
	//void CreateAddModifySprite(string strImagePath, COLORREF crColor, bool bTransparent = false);
	void CreateSprite(string strImagePath, int nResourceID, bool bTransparent = false);
	void CreateSprite(string strImagePath, int nResourceID, COLORREF crColor, bool bTransparent = false);
	void CreateAddModifySprite(string strImagePath, int nResourceID, COLORREF crColor, float fBrightness, bool bTransparent = false);
	HBITMAP GetSprite(string strImagePath);
	HBITMAP GetModifySprite(string strImagePath);
	HBITMAP GetMaskSprite(string strImagePath);
	bool isMaskImage(string strImagePath);
	bool isModifyImage(string strImagePath);
};