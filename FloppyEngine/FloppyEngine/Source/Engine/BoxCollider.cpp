#include "../../Header/Engine/BoxCollider.h"
#include "../../Header/Engine/GameEngine.h"
#include "../../Header/Engine/Transform.h"
#include "../../Header/Engine/Composite.h"
#include "../../Header/Engine/Renderer.h"

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
	m_Transform = (Transform*)m_Composite->FindComponent(TRANSFORM);
	m_Brush = (HBRUSH)GetStockObject(NULL_BRUSH);

	m_Renderer = (Renderer*)m_Composite->FindComponent(RENDERER);
	m_fCenterX = m_Transform->m_fX + m_Renderer->GetWidthF() / 2.0f;
	m_fCenterY = m_Transform->m_fY + m_Renderer->GetHeightF() / 2.0f;
	m_fRadius = m_Renderer->GetWidthF() / 2.0f;
}

Rect BoxCollider::GetRect()
{
	return m_Rect;
}

Point BoxCollider::GetCenter()
{
	return { m_fCenterX, m_fCenterY };
}

float BoxCollider::GetRadius()
{
	return m_fRadius;
}

void BoxCollider::SettingRect()
{
	m_Rect.left = m_Transform->m_fX + m_nOffsetX * m_Transform->m_fScaleX;
	m_Rect.top = m_Transform->m_fY + m_nOffsetY * m_Transform->m_fScaleY;
	m_Rect.right = m_Transform->m_fX + m_nWidth * m_Transform->m_fScaleX;
	m_Rect.bottom = m_Transform->m_fY + m_nHeight * m_Transform->m_fScaleY;

	m_fCenterX = (m_Transform->m_fX + (m_Renderer->GetWidthF() * m_Transform->m_fScaleX) / 2.0f);
	m_fCenterY = (m_Transform->m_fY + (m_Renderer->GetHeightF() * m_Transform->m_fScaleY) / 2.0f);
	m_fRadius = (m_Renderer->GetWidthF() * m_Transform->m_fScaleX) / 2.0f;
}

void BoxCollider::Update(GameEngine* gameEngine)
{
	SettingRect();
}

void BoxCollider::Draw(GameEngine* gameEngine)
{
	if (m_bDebugDraw == true)
	{		
		m_OldBrush = (HBRUSH)SelectObject(gameEngine->m_hBackMemDC, m_Brush);
		Rectangle(gameEngine->m_hBackMemDC, (int)m_Rect.left, (int)m_Rect.top, (int)m_Rect.right, (int)m_Rect.bottom);
		SelectObject(gameEngine->m_hBackMemDC, m_OldBrush);	

		float left = m_fCenterX - m_fRadius;
		float top = m_fCenterY - m_fRadius;
		float right = m_fCenterX + m_fRadius;
		float bottom = m_fCenterY + m_fRadius;

		m_OldBrush = (HBRUSH)SelectObject(gameEngine->m_hBackMemDC, m_Brush);
		Ellipse(gameEngine->m_hBackMemDC, (int)left, (int)top, (int)right, (int)bottom);

		SelectObject(gameEngine->m_hBackMemDC, m_OldBrush);
	}
}

void BoxCollider::CleanUp(GameEngine* gameEngine)
{
	DeleteObject(m_Brush);
}

bool BoxCollider::intersectRect(Rect rect)
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

//아무것도 충돌 안되면 { -1, -1 }로 리턴된다.
Point BoxCollider::IntersectionRectCenterPoint(Rect rect)
{
	if (m_Rect.left < rect.right &&
		m_Rect.top < rect.bottom &&
		m_Rect.right > rect.left &&
		m_Rect.bottom > rect.top)
	{
		// 겹치는 영역 (I) 계산
		float i_left = max(m_Rect.left, rect.left);
		float i_top = max(m_Rect.top, rect.top);
		float i_right = min(m_Rect.right, rect.right);
		float i_bottom = min(m_Rect.bottom, rect.bottom);

		// 겹치는 영역의 중점 (C) 계산
		Point center;
		// X 중심점: (left + right) / 2
		center.x = (i_left + i_right) / 2;

		// Y 중심점: (top + bottom) / 2
		center.y = (i_top + i_bottom) / 2;
		return center;
	}
	
	return  { -1, -1 };
}

//아무것도 충돌 안되면 { -1, -1 }로 리턴된다.
Point BoxCollider::IntersectionCircleCenterPoint(Point otherXY, float otherRadius)
{
	// 1. 중심 간 거리의 제곱 계산
	float dx = otherXY.x - m_fCenterX; // 방향 벡터
	float dy = otherXY.y - m_fCenterY;
	float distanceSquared = (dx * dx) + (dy * dy);

	// 2. 두 반지름의 합의 제곱 계산
	float sumOfRadii = m_fRadius + otherRadius;
	float sumOfRadiiSquared = sumOfRadii * sumOfRadii;

	// 3. 충돌 체크
	if (distanceSquared <= sumOfRadiiSquared)
	{
		// === 충돌 발생: 접촉점 계산 시작 ===

		// 실제 중심 간 거리 (Magnitude) 계산
		float distance = sqrtf(distanceSquared); // <cmath> 필요

		// 예외 처리: 두 원의 중심이 거의 같아 거리가 0에 가까울 경우,
		// 법선 벡터를 정의할 수 없으므로 { -1, -1 }을 반환하거나 특별 처리 필요
		if (distance < FLT_EPSILON) // 아주 작은 값(EPSILON)보다 작으면
		{
			return { -1, -1 };
		}

		// 단위 벡터 (법선 벡터) 계산
		float nx = dx / distance;
		float ny = dy / distance;

		// 접촉점 (Contact Point) 계산 자신의 원줄레 위치에서 다른 원이 충돌된 위치를 알려준다.
		float contactX = m_fCenterX + m_fRadius * nx;
		float contactY = m_fCenterY + m_fRadius * ny;

		// 접촉점을 POINT 구조체로 변환하여 반환
		return { contactX, contactY };
	}
	else
	{
		// 충돌하지 않음
		return { -1, -1 };
	}
}