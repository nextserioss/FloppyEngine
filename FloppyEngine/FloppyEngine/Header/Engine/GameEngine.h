#pragma once

#include <vector>
#include <Windows.h>

using namespace std;

class GameState;

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
//키가 히트 될때 마다 들어온다. 메모장에서 문자를 계속 누르고 있을 경우와 동일한 타임에 온다.
#define KEY_DOWN_KEYHIT(vk_code) ((GetAsyncKeyState(vk_code) == 0xffff8001) ? 1 : 0)

//게임에 상태를 관리하는 클래스
//각 상태는 GameState를 상속 받아서 만든다.
class GameEngine
{
public:
	static int ms_nScreenWidth;
	static int ms_nScreenHeight;

	HWND m_hWnd;
	HDC m_hDc;
	HDC m_hBackMemDC;			//백버퍼 메모리 DC
	HBRUSH m_BackGroundBrush;	//기본 배경 브러쉬

	void Init(HWND hWnd, int nScreenWidth, int nScreenHeight);
	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_bRunning; }
	void Quit() { m_bRunning = false; }

private:
	// the stack of states
	vector<GameState*> m_States;

	bool m_bRunning;
};

