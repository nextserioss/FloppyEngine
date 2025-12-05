#pragma once

#include "Header/FloppyEngine.h"

//배열 최대 개수
#define PLAYSTATE_LAYER_NUM 3
#define PLAYSTATE_MAX_CLOUD_NUM  4
#define PLAYSTATE_MAX_CAR 6
#define PLAYSTATE_MAX_FRUIT 30
#define PLAYSTATE_MAX_FRUIT_EFFECT 10
#define PLAYSTATE_MAX_HEART 3

#define PLAYSTATE_DEFAULT_SPEED 200

class PlayState : public GameState
{
	//구름 
	Transform* m_CloudTrans[PLAYSTATE_MAX_CLOUD_NUM];

	//플레이어
	Composite* m_Layer[PLAYSTATE_LAYER_NUM];
	Composite* m_PlayerComp;
	Transform* m_PlayerTrans;
	BoxCollider* m_PlayerBoxCollider;
	vector<string> m_PlayerFrameImage;
	vector<string> m_RedPlayerFrameImage;
	int m_nPlayerSpeed = PLAYSTATE_DEFAULT_SPEED;
	bool m_bSpeedUp = false;
	float m_fPlayerSpeedUpTimeElapsed = 0.0f;
	//피격당하면 몃초 동안 무적 상태이다.
	bool m_bInvincible = false;
	float m_fPlayerInvincibleTimeElapsed = 0.0f;

	//피격 이펙트
	Composite* m_DamageRedComp;
	Transform* m_DamageRedTrans;

	//과일
	Composite* m_FruitComp[PLAYSTATE_MAX_FRUIT];
	Transform* m_FruitTrans[PLAYSTATE_MAX_FRUIT];
	Renderer* m_FruitRenderer[PLAYSTATE_MAX_FRUIT];
	BoxCollider* m_FruitBoxCollider[PLAYSTATE_MAX_FRUIT];
	int m_nFruitIndex = 0;
	float m_fFruitTimeElapsed = 0.0f;
	int m_nFruitType[PLAYSTATE_MAX_FRUIT];
	float m_nFruitDownSpeed = 3.5f;
	float m_nFruitDownInverval = 1.0f;
	//일정 시간이 지나면 난이도를 올려준다.
	float m_fFruitDifficultyTimeElapsed = 0.0f;
	int m_nLowFruitRate = 60;
	int m_nHighFruitRate = 80;
	int m_nBadFruitRate = 90;
	int m_nSpecialFruitRate = 99;

	//과일 이펙트
	Composite* m_FruitEffectComp[PLAYSTATE_MAX_FRUIT_EFFECT];
	Transform* m_FruitEffectTrans[PLAYSTATE_MAX_FRUIT_EFFECT];
	vector<string> m_FruitEffectImage;
	int m_nFruitEffectIndex = 0;

	//차
	Composite* m_CarComp[PLAYSTATE_MAX_CAR];
	Transform* m_CarTrans[PLAYSTATE_MAX_CAR];
	BoxCollider* m_CarBoxCollider[PLAYSTATE_MAX_CAR];
	int m_nCarIndex = 0;
	float m_fCarTimeElapsed = 0.0f;
	float m_fCarTimeInterval = 7.0f;
	float m_fCarMoveTime = 2.5f;
	float m_fCarDifficultyTimeElapsed = 0.0f;

	//차 왼쪽 오른쪽 나타내주는 화살표
	Composite* m_CarArrowComp[PLAYSTATE_MAX_CAR];
	Renderer* m_CarArrowRenderer[PLAYSTATE_MAX_CAR];
	Transform* m_CarArrowTrans[PLAYSTATE_MAX_CAR];

	//텍스트 UI
	Composite* m_ScoreComp;
	Text* m_ScoreText;
	char m_Score[128];
	int m_nScore = 0;

	//하트 UI
	Composite* m_HeartComp[PLAYSTATE_MAX_HEART];
	Transform* m_HeartTrans[PLAYSTATE_MAX_HEART];
	int m_nHeartIndex = 0;

	//게임 오버 UI
	Composite* m_GameOverComp;
	Transform* m_GameOverTrans;
	bool m_bGameOver = false;

	//사운드
	Sound* m_BGMSound;
	Sound* m_EatSound;
	Sound* m_SpecialEatSound;
	Sound* m_DamageSound;

public:
	PlayState() {}
	void Init(GameEngine* game);
	void CloudInit(GameEngine* game);
	void PlayerInit(GameEngine* game);
	void DamageInit(GameEngine* game);
	void FruitInit(GameEngine* game);
	void CarInit(GameEngine* game);
	void UIInit(GameEngine* game);
	void SoundInit(GameEngine* game);

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void FruitUpdate();
	void CarUpdate();
	void CollisionUpdate();
	void FruitEffectStart(int nFruitIndex);
	void PalyerSpeedUpUpdate();
	void PalyerInvincibleUpdate();
	void AfterDraw(GameEngine* game);
	void AnimationEnd(string strAniName);
	void Cleanup();
};
#pragma once
