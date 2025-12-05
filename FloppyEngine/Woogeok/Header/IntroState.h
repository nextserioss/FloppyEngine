#pragma once

#include "Header/FloppyEngine.h"

//배열 최대 개수
#define INTROSTATE_MAX_LAYER   3
#define INTROSTATE_MAX_MENU	4
#define INTROSTATE_MAX_CLOUD_NUM  4

class IntroState : public GameState
{
public:
	IntroState() {}
	Composite* m_Layer[INTROSTATE_MAX_LAYER];
	Transform* m_SelectBarTramsform;
	Transform* m_MenuTransform[INTROSTATE_MAX_MENU];
	Transform* m_MenuTransformText[INTROSTATE_MAX_MENU];
	Text* m_MenuSoundText;	
	Transform* m_CloudTrans[INTROSTATE_MAX_CLOUD_NUM];

	Sound* m_BGMSound;
	Sound* m_ClickSound;

	int m_nMenuIndex;

	void Init(GameEngine* game);
	void UIInit(GameEngine* game);
	void CloudInit(GameEngine* game);
	void SoundInit(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void AfterDraw(GameEngine* game);
	void AnimationEnd(string strAniName);
};
