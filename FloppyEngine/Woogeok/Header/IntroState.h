#pragma once

#include "Header/FloppyEngine.h"

#define MAX_LAYER   3

#define MAX_MENU	4
#define START_TYPE	0
#define DUAL_TYPE	1
#define SOUND_TYPE	2
#define QUIT_TYPE	3

#define MAX_CLOUD_NUM  4

const bool CLOUD_FLIP[] = { true, false, true, false };
const float CLOUD_X[] = { 640.0f, 940.0f, 1240.0f, 1480.0f };
const float CLOUD_Y[] = { 30.0f, 85.0f, 55.0f, 105.0f };
const float CLOUD_TIME[] = { 45.0f, 38.0f, 55.0f, 70.0f };
const float CLOUD_DIST_Y = -100.0f;

#define TITLE_X 145
#define TITLE_Y 78

#define ONEP_X 10
#define ONEP_Y 380

#define TWOP_X 535
#define TWOP_Y 380

const float MENU_X[] = { 236.0f, 236.0f, 236.0f, 236.0f };
const float MENU_Y[] = { 249.0f, 303.0f, 357.0f, 411.0f };
const float MENU_TEXT_X[] = { 301.0f, 301.0f, 291.0f, 301.0f };
const float MENU_TEXT_Y[] = { 257.0f, 312.0f, 367.0f, 422.0f };
const string MENU_SOUND_ON_TEXT = "소리 켬";
const string MENU_SOUND_OFF_TEXT = "소리 끔";
const string MENU_TEXT[] = { "시작", "대전", MENU_SOUND_ON_TEXT, "종료"};

const float MENU_SLECT_X[] = { 232.0f, 232.0f, 232.0f, 232.0f };
const float MENU_SLECT_Y[] = { 244.0f, 298.0f, 352.0f, 406.0f };

class IntroState : public GameState
{
public:
	IntroState() {}
	Composite* m_Layer[MAX_LAYER];
	Transform* m_SelectBarTramsform;
	Transform* m_MenuTransform[MAX_MENU];
	Transform* m_MenuTransformText[MAX_MENU];
	Text* m_MenuSoundText;	
	Transform* m_CloudTrans[MAX_CLOUD_NUM];

	Sound* m_BGMSound;
	Sound* m_ClickSound;

	int m_nMenuIndex;

	void Init(GameEngine* game);
	void CloudInit(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void AfterDraw(GameEngine* game);
	void AnimationEnd(string strAniName);
};
#pragma once
