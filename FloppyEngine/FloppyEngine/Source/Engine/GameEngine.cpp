#include "../../Header/Engine/GameState.h"
#include "../../Header/Engine/GameEngine.h"

int GameEngine::ms_nScreenWidth = 0;
int GameEngine::ms_nScreenHeight = 0;

void GameEngine::Init(HWND hWnd, int nScreenWidth, int nScreenHeight)
{
	ms_nScreenWidth = nScreenWidth;
	ms_nScreenHeight = nScreenHeight;

	m_bRunning = true;
	m_hWnd = hWnd;
	m_hDc = GetDC(m_hWnd);
	m_hBackMemDC = CreateCompatibleDC(m_hDc);
	m_BackGroundBrush = CreateSolidBrush(RGB(95, 95, 95));

	//백버퍼의 비트맵을 한번 생성하여 연결 해둔다.
	HBITMAP backBitmap = CreateCompatibleBitmap(m_hDc, ms_nScreenWidth, ms_nScreenHeight);
	SelectObject(m_hBackMemDC, backBitmap);
}

void GameEngine::Cleanup()
{
	// 상태에 있는 컴포지트 및 상태들을 정리 한다.
	while (!m_States.empty())
	{
		m_States.back()->m_MainComposite.CleanUp(this);
		m_States.back()->Cleanup();
		m_States.pop_back();
	}
	//DC 정리
	ReleaseDC(m_hWnd, m_hDc);
}

void GameEngine::ChangeState(GameState* state)
{
	//현재 상태를 정리 한다.
	if (!m_States.empty())
	{
		m_States.back()->m_MainComposite.CleanUp(this);
		m_States.back()->Cleanup();
		m_States.pop_back();
	}

	// 변경된 상태를 저장하고 초기화 한다.
	state->m_MainComposite.SetGameEngine(this);
	m_States.push_back(state);
	m_States.back()->Init(this);
}

void GameEngine::PushState(GameState* state, bool bPauseDraw)
{
	//먼저 현재 상태를 일시정지 한다.
	if (!m_States.empty())
	{
		m_States.back()->m_bPauseDraw = bPauseDraw;
		m_States.back()->Pause();
	}

	//상태를 넣어두고 초기화 한다.
	state->m_MainComposite.SetGameEngine(this);
	m_States.push_back(state);
	m_States.back()->Init(this);
}

GameState* GameEngine::getState(int index)
{
	if (index < 0 || index >= (int)m_States.size())
	{
		// 범위를 벗어났으므로 NULL을 반환합니다.
		return NULL;
	}

	// 범위 내에 있으므로, 해당 위치의 GameState* 포인터를 반환합니다.
	return m_States[index];
}

void GameEngine::PopState()
{
	//현재 상태를 정리 한다.
	if (!m_States.empty())
	{
		m_States.back()->m_MainComposite.CleanUp(this);
		m_States.back()->Cleanup();
		m_States.pop_back();
	}

	//이전 상태를 다시 시작 한다.
	if (!m_States.empty())
	{
		m_States.back()->Resume();
	}
}

void GameEngine::HandleEvents()
{
	//포커스가 있을 경우에만 이벤트를 처리한다.
	HWND currentFocus = GetFocus();
	if (currentFocus != m_hWnd)
	{
		return;
	}
	//핸들 이벤트를 상태에 전달 한다.
	m_States.back()->HandleEvents(this);
}

void GameEngine::Update()
{
	//현재 상태를 업데이트 한다.
	m_States.back()->m_MainComposite.Update(this);
	m_States.back()->Update(this);
}

void GameEngine::Draw()
{	
	SelectObject(m_hBackMemDC, m_BackGroundBrush);
	Rectangle(m_hBackMemDC, 0, 0, ms_nScreenWidth, ms_nScreenHeight);

	//컴포지트의 Draw를 호출 한다.
	for (size_t i = 0;i < m_States.size();i++)
	{
		if (m_States[i]->m_bPauseDraw == true)
		{
			m_States[i]->m_MainComposite.Draw(this);
		}
	}

	//m_hBackMemDC을 실제 화면 m_hDc에 복사 한다.
	BitBlt(m_hDc, 0, 0, ms_nScreenWidth, ms_nScreenHeight, m_hBackMemDC, 0, 0, SRCCOPY);

	//Draw가 끝났을때 처리 할 개념으로 남겨 두었다. 잘 쓰이지는 않는다.
	m_States.back()->AfterDraw(this);
}
