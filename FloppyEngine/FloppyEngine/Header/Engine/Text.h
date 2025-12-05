#pragma once

#include <string>
#include <Windows.h>
#include "Component.h"

using namespace std;

class Composite;
class Transform;
class GameEngine;

#define MAX_WIDTH 400
#define MAX_HEIGHT 30

class Text : public Component
{
private:
	Composite* m_Composite;
	Transform* m_Transform;
	HDC m_hTextMemDC;
	HDC m_hMaskTextMemDC;
	HBITMAP m_hBackBitmap;
	HBITMAP m_hOldBackBitmap;
	HBITMAP m_hMaskBitmap;
	HBITMAP m_hOldMaskBitmap;
	string m_strText;
	COLORREF m_crColor = RGB(255, 255, 255);
	HFONT m_hFont = NULL;
	RECT rect;
public:
	Text(string strText);
	Text(string strText, COLORREF crColor);
	Text(string strText, COLORREF crColor, int height);
	void Add(GameEngine* gameEngine, Composite* composite);
	SIZE GetTextSize();
	void Update(GameEngine* gameEngine);
	void Draw(GameEngine* gameEngine);
	void CleanUp(GameEngine* gameEngine);
	void ChangeText(string strText);
};