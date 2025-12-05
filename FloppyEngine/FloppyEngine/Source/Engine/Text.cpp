#include "../../Header/Engine/Text.h"
#include "../../Header/Engine/GameEngine.h"
#include "../../Header/Engine/Composite.h"
#include "../../Header/Engine/Transform.h"

Text::Text(string strText)
{
	m_strText = strText;
	// cHeight (디폴트 -16)
	m_hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, NULL);
}
Text::Text(string strText, COLORREF crColor)
{
	m_strText = strText;
	m_crColor = crColor;
	// cHeight (디폴트 -16)
	m_hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, NULL);
}

Text::Text(string strText, COLORREF crColor, int height)
{
	m_strText = strText;
	m_crColor = crColor;
	m_hFont = CreateFontW(height * -1, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, NULL);
}
void Text::Add(GameEngine* gameEngine, Composite* composite)
{
	m_Composite = composite;
	m_Transform = (Transform*)m_Composite->FindComponent(TRANSFORM);

	m_hTextMemDC = CreateCompatibleDC(gameEngine->m_hDc);
	SetBkMode(m_hTextMemDC, TRANSPARENT);
	m_hBackBitmap = CreateCompatibleBitmap(gameEngine->m_hDc, MAX_WIDTH, MAX_HEIGHT);
	m_hOldBackBitmap = (HBITMAP)SelectObject(m_hTextMemDC, m_hBackBitmap);

	m_hMaskTextMemDC = CreateCompatibleDC(gameEngine->m_hDc);
	SetBkMode(m_hMaskTextMemDC, TRANSPARENT);	
	m_hMaskBitmap = CreateCompatibleBitmap(gameEngine->m_hDc, MAX_WIDTH, MAX_HEIGHT);
	m_hOldMaskBitmap = (HBITMAP)SelectObject(m_hMaskTextMemDC, m_hMaskBitmap);

	rect.left = 0;
	rect.top = 0;
	rect.right = MAX_WIDTH;
	rect.bottom = MAX_HEIGHT;
}

SIZE Text::GetTextSize()
{
	SIZE size;
	HFONT hOldFont = (HFONT)SelectObject(m_hTextMemDC, m_hFont);
	GetTextExtentPoint32(m_hTextMemDC, m_strText.c_str(), m_strText.length(), &size);
	SelectObject(m_hTextMemDC, hOldFont);
	return size;
}

void Text::Update(GameEngine* gameEngine)
{

}

void Text::Draw(GameEngine* gameEngine)
{
	//폰트 변경
	HFONT hOldMaskFont = (HFONT)SelectObject(m_hMaskTextMemDC, m_hFont);
	HFONT hOldFont = (HFONT)SelectObject(m_hTextMemDC, m_hFont);

	FillRect(m_hMaskTextMemDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));		
	SetTextColor(m_hMaskTextMemDC, RGB(0, 0, 0));
	TextOut(m_hMaskTextMemDC, 0, 0, m_strText.c_str(), m_strText.length());
	
	// 텍스트 DC 그리기 (배경: 검은색, 텍스트: 원하는 색상)
	FillRect(m_hTextMemDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	SetTextColor(m_hTextMemDC, m_crColor); // 원하는 색상 텍스트
	TextOut(m_hTextMemDC, 0, 0, m_strText.c_str(), m_strText.length());

	// **Pass 1: 마스크 적용 (SRCAND)**
	StretchBlt(gameEngine->m_hBackMemDC, (int)m_Transform->m_fX, (int)m_Transform->m_fY,
		(int)(MAX_WIDTH * m_Transform->m_fScaleX), (int)(MAX_HEIGHT * m_Transform->m_fScaleY), m_hMaskTextMemDC, 0, 0, MAX_WIDTH, MAX_HEIGHT, SRCAND);

	// **Pass 2: 텍스트 합성 (SRCPAINT)**
	StretchBlt(gameEngine->m_hBackMemDC, (int)m_Transform->m_fX, (int)m_Transform->m_fY,
		(int)(MAX_WIDTH * m_Transform->m_fScaleX), (int)(MAX_HEIGHT * m_Transform->m_fScaleY), m_hTextMemDC, 0, 0, MAX_WIDTH, MAX_HEIGHT, SRCPAINT);

	// 폰트 복원
	SelectObject(m_hMaskTextMemDC, hOldMaskFont);
	SelectObject(m_hMaskTextMemDC, hOldFont);
}

void Text::CleanUp(GameEngine* gameEngine)
{
	SelectObject(m_hMaskTextMemDC, m_hOldMaskBitmap);
	DeleteObject(m_hMaskBitmap);
	DeleteDC(m_hMaskTextMemDC);

	SelectObject(m_hTextMemDC, m_hOldBackBitmap);
	DeleteObject(m_hBackBitmap);
	DeleteDC(m_hTextMemDC);

	//폰트 객체 해제
	DeleteObject(m_hFont);
}

void Text::ChangeText(string strText)
{
	m_strText = strText;
}