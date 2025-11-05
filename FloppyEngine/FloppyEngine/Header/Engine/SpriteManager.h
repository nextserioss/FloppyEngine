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
	HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);
	void ChangeBitmapColorMultiply(HBITMAP hbmColour, COLORREF crMultiplyColor);
	//흰색 부분 특정 색으로 변경 한다. 8비트 전용
	void ChangeBitmapColor(HBITMAP hbmColour, COLORREF crColor);
	void CreateSprite(string strImagePath, bool bTransparent = false);
	void CreateSprite(string strImagePath, COLORREF crMultiplyColor, bool bTransparent = false);
	void CreateSprite(string strImagePath, int nResourceID, bool bTransparent = false);
	void CreateSprite(string strImagePath, int nResourceID, COLORREF crMultiplyColor, bool bTransparent = false);
	HBITMAP GetSprite(string strImagePath);
	HBITMAP GetMaskSprite(string strImagePath);
	bool isMaskImage(string strImagePath);
};