#pragma once

#include <Windows.h>
#include "Component.h"
#include "Types.h"

class Composite;
class Transform;
class Renderer;
class GameEngine;

class BoxCollider: public Component
{
private:
	Composite* m_Composite;
	Transform* m_Transform;
	Renderer* m_Renderer;
	HBRUSH m_Brush;
	HBRUSH m_OldBrush;
	int m_nOffsetX;
	int m_nOffsetY;
	int m_nWidth;
	int m_nHeight;
	float m_fCenterX;
	float m_fCenterY;
	float m_fRadius;
	Rect m_Rect;
public:
	bool m_bDebugDraw = false;

	BoxCollider(int nOffsetX, int nOffsetY, int nWidth, int nHeight);
	void Add(GameEngine* gameEngine, Composite* composite);
	Rect GetRect();
	Point GetCenter();
	float GetRadius();
	void SettingRect();
	void Update(GameEngine* gameEngine);
	void Draw(GameEngine* gameEngine);
	void CleanUp(GameEngine* gameEngine);	
	bool intersectRect(Rect rect);
	Point IntersectionRectCenterPoint(Rect rect);
	Point IntersectionCircleCenterPoint(Point otherXY, float otherRadius);
};