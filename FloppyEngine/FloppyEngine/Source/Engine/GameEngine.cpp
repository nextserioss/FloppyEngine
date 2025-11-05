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
	// cleanup the all states
	while (!m_States.empty())
	{
		m_States.back()->m_MainComposite.CleanUp(this);
		m_States.back()->Cleanup();
		m_States.pop_back();
	}
	//CleanUp DC
	ReleaseDC(m_hWnd, m_hDc);
}

void GameEngine::ChangeState(GameState* state)
{
	// cleanup the current state
	if (!m_States.empty())
	{		
		m_States.back()->m_MainComposite.CleanUp(this);
		m_States.back()->Cleanup();
		m_States.pop_back();
	}

	// store and init the new state
	state->m_MainComposite.SetGameEngine(this);
	m_States.push_back(state);
	m_States.back()->Init(this);
}

void GameEngine::PushState(GameState* state)
{
	// pause current state
	if (!m_States.empty())
	{
		m_States.back()->Pause();
	}

	// store and init the new state
	state->m_MainComposite.SetGameEngine(this);
	m_States.push_back(state);
	m_States.back()->Init(this);
}

void GameEngine::PopState()
{
	// cleanup the current state
	if (!m_States.empty())
	{
		m_States.back()->m_MainComposite.CleanUp(this);
		m_States.back()->Cleanup();
		m_States.pop_back();
	}

	// resume previous state
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
	// let the state handle events
	m_States.back()->HandleEvents(this);
}

void GameEngine::Update()
{
	// let the state update the game
	m_States.back()->m_MainComposite.Update(this);
	m_States.back()->Update(this);
}

void GameEngine::Draw()
{	
	SelectObject(m_hBackMemDC, m_BackGroundBrush);
	Rectangle(m_hBackMemDC, 0, 0, ms_nScreenWidth, ms_nScreenHeight);

	// let the state draw the screen
	m_States.back()->m_MainComposite.Draw(this);

	BitBlt(m_hDc, 0, 0, ms_nScreenWidth, ms_nScreenHeight, m_hBackMemDC, 0, 0, SRCCOPY);

	//Draw가 끝났을때 처리 할 개념으로 남겨 두었다. 잘 쓰이지는 않는다.
	m_States.back()->AfterDraw(this);
}
