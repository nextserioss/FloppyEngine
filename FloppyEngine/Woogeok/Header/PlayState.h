#pragma once

#include "Header/FloppyEngine.h"

//0 백그라운드 1 오브젝트 2 케릭터 및 이펙트
#define LAYER_NUM 3
#define MAX_CLOUD_NUM  4

//케릭터
#define INIT_X 280
#define INIT_Y 388
#define PLAYER_BOXCOLLISION_OFFSET 5
#define FRAME_ANIMATION_INTERVAL 0.12f
#define DEFAULT_SPEED 200
#define SPEEDUP_SPEED 340
#define MAX_LEFT_MOVE 0 
#define MAX_RIGHT_MOVE 560
#define JUMP_TIME 0.5f
#define JUMP_Y 270
#define INVICIVLE_TIME 2.0f
#define CHARACTER_SPEED_UP_TIME 3.0f

//피격 이펙트
#define DAMAGE_RED_DELAY_TIME 0.1f

//자동차
#define MAX_CAR 6
#define CAR_BOXCOLLISION_OFFSET 5
#define CAR_INIT_X -100.0f
#define CAR_INIT_Y 440.0f
#define CAR_LEFT_X_MAX -100.0f
#define CAR_RIGHT_X_MAX 640.0f
#define CAR_ARROW_LEFT_MOVE_INIT_X 590.0f
#define CAR_ARROW_RIGHT_MOVE_INIT_X -0.0f
#define MIN_CAR_INTERVAL 3.0f
#define MINUS_CAR_INTERVAL 0.5f

//과일
#define MAX_FRUIT 30
#define LOW_FRUIT 0
#define HIGH_FRUIT 1
#define BAD_FRUIT 2
#define SPECTIAL_FRUIT 3
#define DOWN_Y 500.0f
#define MAX_DOWN_INTERVAL 0.1f
#define MAX_DOWN_SPEED 1.7f
#define MIN_LOW_FRUIT_RATE 10
#define MIN_HIGH_FRUIT_RATE 30
#define MINUS_FRUIT_DOWN_INTERVAL 0.1f
#define MINUS_FRUIT_DOWN_SPEED 0.2f
#define MINUS_FRUIT_RATE 5

//과일 이펙트
#define MAX_FRUIT_EFFECT 10
#define FRUIT_EFFECT_TIME 0.05f

//일정 시간 마다 난이도가 올라간다.
#define DIFICULTY_TIME 7.0f

//하트
#define MAX_HEART 3

class PlayState : public GameState
{
	//구름 
	Transform* m_CloudTrans[MAX_CLOUD_NUM];

	//플레이어
	Composite* m_Layer[LAYER_NUM];
	Composite* m_PlayerComp;
	Transform* m_PlayerTrans;
	BoxCollider* m_PlayerBoxCollider;
	vector<string> m_PlayerFrameImage;
	vector<string> m_RedPlayerFrameImage;
	int m_nPlayerSpeed = DEFAULT_SPEED;
	bool m_bSpeedUp = false;
	float m_fPlayerSpeedUpTimeElapsed = 0.0f;
	//피격당하면 몃초 동안 무적 상태이다.
	bool m_bInvincible = false;
	float m_fPlayerInvincibleTimeElapsed = 0.0f;

	//피격 이펙트
	Composite* m_DamageRedComp;
	Transform* m_DamageRedTrans;

	//과일
	Composite* m_FruitComp[MAX_FRUIT];
	Transform* m_FruitTrans[MAX_FRUIT];
	Renderer* m_FruitRenderer[MAX_FRUIT];
	BoxCollider* m_FruitBoxCollider[MAX_FRUIT];
	int m_nFruitIndex = 0;
	float m_fFruitTimeElapsed = 0.0f;
	int m_nFruitType[MAX_FRUIT];
	float m_nFruitDownSpeed = 3.5f;
	float m_nFruitDownInverval = 1.0f;
	//일정 시간이 지나면 난이도를 올려준다.
	float m_fFruitDifficultyTimeElapsed = 0.0f;
	int m_nLowFruitRate = 60;
	int m_nHighFruitRate = 80;
	int m_nBadFruitRate = 90;
	int m_nSpecialFruitRate = 99;

	//과일 이펙트
	Composite* m_FruitEffectComp[MAX_FRUIT_EFFECT];
	Transform* m_FruitEffectTrans[MAX_FRUIT_EFFECT];
	vector<string> m_FruitEffectImage;
	int m_nFruitEffectIndex = 0;

	//차
	Composite* m_CarComp[MAX_CAR];
	Transform* m_CarTrans[MAX_CAR];
	BoxCollider* m_CarBoxCollider[MAX_CAR];
	int m_nCarIndex = 0;
	float m_fCarTimeElapsed = 0.0f;
	float m_fCarTimeInterval = 7.0f;
	float m_fCarMoveTime = 2.5f;
	float m_fCarDifficultyTimeElapsed = 0.0f;

	//차 왼쪽 오른쪽 나타내주는 화살표
	Composite* m_CarArrowComp[MAX_CAR];
	Renderer* m_CarArrowRenderer[MAX_CAR];
	Transform* m_CarArrowTrans[MAX_CAR];

	//텍스트 UI
	Composite* m_ScoreComp;
	Text* m_ScoreText;
	char m_Score[128];
	int m_nScore = 0;

	//하트 UI
	Composite* m_HeartComp[MAX_HEART];
	Transform* m_HeartTrans[MAX_HEART];
	int m_nHeartIndex = 0;

	//게임 오버 UI
	Composite* m_GameOverComp;
	Transform* m_GameOverTrans;
	bool m_bGameOver = false;

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
