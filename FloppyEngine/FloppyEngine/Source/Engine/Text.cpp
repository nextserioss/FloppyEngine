#include "../../Header/Engine/Text.h"
#include "../../Header/Engine/GameEngine.h"
#include "../../Header/Engine/Composite.h"
#include "../../Header/Engine/Transform.h"

Text::Text(string strText)
{
	m_strText = strText;
}
void Text::Add(GameEngine* gameEngine, Composite* composite)
{
	m_Composite = composite;
	m_Transform = (Transform*)m_Composite->FindComponent(COMPONENT_NAME[TRANSFORM]);
	m_hTextMemDC = CreateCompatibleDC(gameEngine->m_hDc);

	SetBkMode(m_hTextMemDC, TRANSPARENT);
	m_hBackBitmap = CreateCompatibleBitmap(gameEngine->m_hDc, MAX_WIDTH, MAX_HEIGHT);
	SelectObject(m_hTextMemDC, m_hBackBitmap);	

	rect.left = 0;
	rect.top = 0;
	rect.right = MAX_WIDTH;
	rect.bottom = MAX_HEIGHT;
}

void Text::Update(GameEngine* gameEngine)
{

}

void Text::Draw(GameEngine* gameEngine)
{
	FillRect(m_hTextMemDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	SetTextColor(m_hTextMemDC, RGB(255, 255, 255));	
	TextOut(m_hTextMemDC, 0, 0, m_strText.c_str(), m_strText.length());
	BitBlt(gameEngine->m_hBackMemDC, m_Transform->m_fX, m_Transform->m_fY, MAX_WIDTH, MAX_HEIGHT, m_hTextMemDC, 0, 0, SRCPAINT);
	//BitBlt(gameEngine->m_hBackMemDC, m_Transform->m_fX, m_Transform->m_fY, MAX_WIDTH, MAX_HEIGHT, m_hTextMemDC, 0, 0, SRCCOPY);
}

void Text::CleanUp(GameEngine* gameEngine)
{
	DeleteObject(m_hBackBitmap);
	DeleteDC(m_hTextMemDC);
}

void Text::ChangeText(string strText)
{
	m_strText = strText;
}