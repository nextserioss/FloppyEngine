#pragma once

#include <string>
#include <Windows.h>
#include "Component.h"

using namespace std;

class Composite;
class Transform;
class GameEngine;

#define MAX_WIDTH 320
#define MAX_HEIGHT 20

class Text : public Component
{
private:
	Composite* m_Composite;
	Transform* m_Transform;
	HDC m_hTextMemDC;
	HBITMAP m_hBackBitmap;
	string m_strText;

	RECT rect;
public:
	Text(string strText);
	void Add(GameEngine* gameEngine, Composite* composite);	
	void Update(GameEngine* gameEngine);
	void Draw(GameEngine* gameEngine);
	void CleanUp(GameEngine* gameEngine);
	void ChangeText(string strText);
};