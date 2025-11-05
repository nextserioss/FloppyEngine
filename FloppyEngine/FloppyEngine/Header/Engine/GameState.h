#pragma once

#include "Composite.h"

class GameEngine;

class GameState
{
public:
	//메인 컴포지트는 각 상태마다 하나만 반드시 존재 한다.
	//메인 컴포지트에 많은 컴포지트 또는 컴포넌트들을 추가하여 상태를 구성 한다.
	Composite m_MainComposite;

	virtual void Init(GameEngine* game) = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(GameEngine* game) = 0;
	virtual void Update(GameEngine* game) = 0;
	virtual void AfterDraw(GameEngine* game) = 0;

	virtual void AnimationEnd(string strAniName) = 0;

protected:
	GameState() { }
};
