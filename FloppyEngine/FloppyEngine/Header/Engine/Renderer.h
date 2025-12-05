#pragma once

#include <string>
#include <Windows.h>
#include "Component.h"

class Composite;
class Transform;

using namespace std;

class Renderer : public Component
{
public:
	const static int IMAGE = 0;
	const static int TRANSPARET_IMAGE = 1;
private:	
	Composite* m_Composite;
	Transform* m_Transform;
	int m_nImageType;
	bool m_bModifyImage;
	HDC m_hRendererMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hModifyBitmap;
	HBITMAP m_hMaskBitmap;
	HBITMAP m_hOldBitmap;
	BITMAP  m_BitmapInfo;
	string m_strImagePath;

public:
	Renderer(string strImagePath);
	void Add(GameEngine* gameEngine, Composite* composite);
	void ChangeBitmap(string strImagePath);
	void Update(GameEngine* gameEngine);
	void Draw(GameEngine* gameEngine);
	void CleanUp(GameEngine* gameEngine);
	int GetWidth();
	int GetHeight();
	float GetWidthF();
	float GetHeightF();
	void SetModifyImage(bool bModify);
};