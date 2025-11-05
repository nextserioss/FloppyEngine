#include "../../Header/Engine/BoxCollider.h"
#include "../../Header/Engine/Transform.h"
#include "../../Header/Engine/Composite.h"
//#include "LogManager.h"

BoxCollider::BoxCollider(int nOffsetX, int nOffsetY, int nWidth, int nHeight)
{
	m_nOffsetX = nOffsetX;
	m_nOffsetY = nOffsetY;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

void BoxCollider::Add(GameEngine* gameEngine, Composite* composite)
{
	m_Composite = composite;
	m_Transform = (Transform*)m_Composite->FindComponent(COMPONENT_NAME[TRANSFORM]);
	m_Brush = (HBRUSH)GetStockObject(NULL_BRUSH);
}

void BoxCollider::Update(GameEngine* gameEngine)
{
	m_Rect.left = (long)(m_Transform->m_fX + m_nOffsetX);
	m_Rect.top = (long)(m_Transform->m_fY + m_nOffsetY);
	m_Rect.right = (long)(m_Transform->m_fX + m_nWidth);
	m_Rect.bottom = (long)(m_Transform->m_fY + m_nHeight);
}

void BoxCollider::Draw(GameEngine* gameEngine)
{
	/*
	m_OldBrush = (HBRUSH)SelectObject(gameEngine->m_hBackMemDC, m_Brush);
	Rectangle(gameEngine->m_hBackMemDC, m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);
	SelectObject(gameEngine->m_hBackMemDC, m_OldBrush);
	*/
	//Rectangle(gameEngine->m_hBackMemDC, m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);
}

void BoxCollider::CleanUp(GameEngine* gameEngine)
{
	DeleteObject(m_Brush);
}

RECT BoxCollider::GetRect()
{
	return m_Rect;
}

bool BoxCollider::intersectRect(RECT rect)
{
	if (m_Rect.left < rect.right &&
		m_Rect.top < rect.bottom &&
		m_Rect.right > rect.left &&
		m_Rect.bottom > rect.top)
	{
		return true;
	}
	return false;
}