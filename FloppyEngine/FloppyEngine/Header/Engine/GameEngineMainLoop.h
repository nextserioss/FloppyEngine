#pragma once

#include <string>
#include <Windows.h>

using namespace std;

class GameState;

class GameEngineMainLoop
{
private:
	GameEngineMainLoop(){};
	~GameEngineMainLoop(){};
	static GameEngineMainLoop* instance;
public:
	static GameEngineMainLoop* GetInstance()
	{
		if (instance == NULL)
		{
			instance = new GameEngineMainLoop();
			atexit(Destroy);
		}
		return instance;
	}
	static void Destroy()
	{
		delete instance;
	}
	//Target FPS
	int m_nTargetFps = 60;
	//단위 : 세컨드
	float m_fTimeDelta = 0.0f;
	int EngineStart(string strGamename, GameState* state, int nScreenWidth, int nScreenHeight, HINSTANCE hInstance, int nCmdShow);
};

#define ENGINE_MAIN GameEngineMainLoop::GetInstance()