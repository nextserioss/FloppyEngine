#pragma once

#include <Windows.h>
#include "Component.h"

class Composite;
class Transform;
class GameEngine;

class BoxCollider: public Component
{
private:
	Composite* m_Composite;
	Transform* m_Transform;
	HBRUSH m_Brush;
	HBRUSH m_OldBrush;
	int m_nOffsetX;
	int m_nOffsetY;
	int m_nWidth;
	int m_nHeight;
	RECT m_Rect;
public:
	BoxCollider(int nOffsetX, int nOffsetY, int nWidth, int nHeight);
	void Add(GameEngine* gameEngine, Composite* composite);
	void Update(GameEngine* gameEngine);
	void Draw(GameEngine* gameEngine);
	void CleanUp(GameEngine* gameEngine);
	RECT GetRect();
	bool intersectRect(RECT rect);
};