#include "../../Header/Engine/Renderer.h"
#include "../../Header/Engine/GameEngine.h"
#include "../../Header/Engine/Composite.h"
#include "../../Header/Engine/SpriteManager.h"
#include "../../Header/Engine/Transform.h"

Renderer::Renderer(string strImagePath)
{
	m_strImagePath = strImagePath;	
}

void Renderer::Add(GameEngine* gameEngine, Composite* composite)
{
	m_Composite = composite;
	m_Transform = (Transform*)m_Composite->FindComponent(COMPONENT_NAME[TRANSFORM]);
	m_hRendererMemDC = CreateCompatibleDC(gameEngine->m_hDc);
	m_hBitmap = SpriteManager::GetInstance()->GetSprite(m_strImagePath);
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	//마스크 이미지가 있다면 투명 이미지 렌더러를 사용한다.
	if (SpriteManager::GetInstance()->isMaskImage(m_strImagePath) == true)
	{
		m_nImageType = Renderer::TRANSPARET_IMAGE;
		m_hMaskBitmap = SpriteManager::GetInstance()->GetMaskSprite(m_strImagePath);
	}
}
void Renderer::ChangeBitmap(string strImagePath)
{
	m_strImagePath = strImagePath;

	m_hBitmap = SpriteManager::GetInstance()->GetSprite(m_strImagePath);
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitmapInfo);
	//투명색이 지정된 이미지의 경우 마스크 이미지를 만든다.
	if (m_nImageType == Renderer::TRANSPARET_IMAGE)
	{
		m_hMaskBitmap = SpriteManager::GetInstance()->GetMaskSprite(m_strImagePath);
	}
}

void Renderer::Update(GameEngine* gameEngine)
{
	
}

void Renderer::Draw(GameEngine* gameEngine)
{
	if (m_nImageType == Renderer::TRANSPARET_IMAGE)
	{
		m_hOldBitmap = (HBITMAP)SelectObject(m_hRendererMemDC, m_hMaskBitmap);
		if (m_Transform->m_bXFlip == true)
		{
			StretchBlt(gameEngine->m_hBackMemDC, (int)m_Transform->m_fX + (int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)m_Transform->m_fY,
				-(int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)(m_BitmapInfo.bmHeight * m_Transform->m_fScaleY), m_hRendererMemDC, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight, SRCAND);
		}
		else
		{
			StretchBlt(gameEngine->m_hBackMemDC, (int)m_Transform->m_fX, (int)m_Transform->m_fY,
				(int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)(m_BitmapInfo.bmHeight * m_Transform->m_fScaleY), m_hRendererMemDC, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight, SRCAND);
		}
		
		SelectObject(m_hRendererMemDC, m_hOldBitmap);
		
		m_hOldBitmap = (HBITMAP)SelectObject(m_hRendererMemDC, m_hBitmap);
		if (m_Transform->m_bXFlip == true)
		{
			StretchBlt(gameEngine->m_hBackMemDC, (int)m_Transform->m_fX + (int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)m_Transform->m_fY,
				-(int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)(m_BitmapInfo.bmHeight * m_Transform->m_fScaleY), m_hRendererMemDC, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight, SRCPAINT);
		}
		else
		{
			StretchBlt(gameEngine->m_hBackMemDC, (int)m_Transform->m_fX, (int)m_Transform->m_fY,
				(int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)(m_BitmapInfo.bmHeight * m_Transform->m_fScaleY), m_hRendererMemDC, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight, SRCPAINT);
		}
		
		SelectObject(m_hRendererMemDC, m_hOldBitmap);
		
	}
	else if (m_nImageType == Renderer::IMAGE)
	{	
		m_hOldBitmap = (HBITMAP)SelectObject(m_hRendererMemDC, m_hBitmap);
		if (m_Transform->m_bXFlip == true)
		{
			StretchBlt(gameEngine->m_hBackMemDC, (int)m_Transform->m_fX + (int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)m_Transform->m_fY,
				-(int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)(m_BitmapInfo.bmHeight * m_Transform->m_fScaleY), m_hRendererMemDC, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight, SRCCOPY);
		}
		else
		{
			StretchBlt(gameEngine->m_hBackMemDC, (int)m_Transform->m_fX, (int)m_Transform->m_fY,
				(int)(m_BitmapInfo.bmWidth * m_Transform->m_fScaleX), (int)(m_BitmapInfo.bmHeight * m_Transform->m_fScaleY), m_hRendererMemDC, 0, 0, m_BitmapInfo.bmWidth, m_BitmapInfo.bmHeight, SRCCOPY);
		}
		
		SelectObject(m_hRendererMemDC, m_hOldBitmap);
	}
}

void Renderer::CleanUp(GameEngine* gameEngine)
{
	DeleteObject(m_hOldBitmap);
	DeleteDC(m_hRendererMemDC);
}

int Renderer::GetWidth()
{
	return m_BitmapInfo.bmWidth;
}

int Renderer::GetHeight()
{
	return m_BitmapInfo.bmHeight;
}

float Renderer::GetWidthF()
{
	return (float)m_BitmapInfo.bmWidth;
}

float Renderer::GetHeightF()
{
	return (float)m_BitmapInfo.bmHeight;
}