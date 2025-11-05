#include "../resource.h"
#include "Header/FloppyEngine.h"
#include "../Header/IntroState.h"
#include "../Header/PlayState.h"

void PlayState::Init(GameEngine* game)
{
	SpriteManager::GetInstance()->CreateSprite("Background.bmp", IDC_BACKGROUND, false);
	SpriteManager::GetInstance()->CreateSprite("ScoreTextBack.bmp", IDC_SCORETEXTBACK, false);
	SpriteManager::GetInstance()->CreateSprite("Player_0.bmp", IDC_PLAYER_0, RGB(239, 148, 99), true);
	SpriteManager::GetInstance()->CreateSprite("Player_1.bmp", IDC_PLAYER_1, RGB(239, 148, 99), true);
	SpriteManager::GetInstance()->CreateSprite("Player_0.bmp", IDC_PLAYER_0, RGB(230, 96, 96), true);
	SpriteManager::GetInstance()->CreateSprite("Player_1.bmp", IDC_PLAYER_1, RGB(230, 96, 96), true);
	SpriteManager::GetInstance()->CreateSprite("Heart.bmp", IDC_HEART, RGB(255, 0, 74), true);
	SpriteManager::GetInstance()->CreateSprite("Fruit.bmp", IDC_FRUIT, RGB(189, 230, 74), true);
	SpriteManager::GetInstance()->CreateSprite("Fruit.bmp", IDC_FRUIT, RGB(230, 33, 99), true);
	SpriteManager::GetInstance()->CreateSprite("BadFruit.bmp", IDC_BADFRUIT, true);
	SpriteManager::GetInstance()->CreateSprite("SpecialFruit.bmp", IDC_SPECIALFRUIT, true);
	SpriteManager::GetInstance()->CreateSprite("FruitEffect_0.bmp", IDC_FRUITEFFECT_0, true);
	SpriteManager::GetInstance()->CreateSprite("FruitEffect_1.bmp", IDC_FRUITEFFECT_1, true);
	SpriteManager::GetInstance()->CreateSprite("FruitEffect_2.bmp", IDC_FRUITEFFECT_2, true);
	SpriteManager::GetInstance()->CreateSprite("FruitEffect_3.bmp", IDC_FRUITEFFECT_3, true);
	SpriteManager::GetInstance()->CreateSprite("FruitEffect_4.bmp", IDC_FRUITEFFECT_4, true);
	SpriteManager::GetInstance()->CreateSprite("Car.bmp", IDC_CAR, true);
	SpriteManager::GetInstance()->CreateSprite("RightArrow.bmp", IDC_RIGHTARROW, true);
	SpriteManager::GetInstance()->CreateSprite("GameOver.bmp", IDC_GAMEOVER, true);
	SpriteManager::GetInstance()->CreateSprite("Cloud_0.bmp", IDC_CLOUD_0, true);
	SpriteManager::GetInstance()->CreateSprite("Cloud_1.bmp", IDC_CLOUD_1, true);
	SpriteManager::GetInstance()->CreateSprite("Cloud_2.bmp", IDC_CLOUD_2, true);
	SpriteManager::GetInstance()->CreateSprite("Red.bmp", IDC_RED, true);

	for (int i = 0; i < LAYER_NUM; i++)
	{
		char layerName[128];
		sprintf_s(layerName, "layer_%d", i);
		m_Layer[i] = Composite::Create(game, layerName);
		m_MainComposite.Add(m_Layer[i]);
	}

	Composite* background = Composite::CreateSprite(game, "Background", "Background.bmp");
	m_Layer[0]->Add(background);

	Composite* scoreBack = Composite::CreateSprite(game, "ScoreBack", "ScoreTextBack.bmp");
	m_Layer[2]->Add(scoreBack);

	CloudInit(game);
	PlayerInit(game);
	FruitInit(game);
	CarInit(game);
	UIInit(game);
	DamageInit(game);

	//사운드
	Composite* soundBGMComp = Composite::CreateBGMSound(game, "BGMSound", "WoogeokBGM.mp3");
	m_BGMSound = (Sound*)soundBGMComp->FindComponent(COMPONENT_NAME[SOUND]);
	m_Layer[2]->Add(soundBGMComp);
	m_BGMSound->Play();

	Composite* soundEatComp = Composite::CreateSound(game, "EatSound", "Eat_0.wav");
	m_EatSound = (Sound*)soundEatComp->FindComponent(COMPONENT_NAME[SOUND]);
	m_Layer[2]->Add(soundEatComp);

	Composite* soundSpecialEatComp = Composite::CreateSound(game, "SpecialSound", "Eat_1.wav");
	m_SpecialEatSound = (Sound*)soundSpecialEatComp->FindComponent(COMPONENT_NAME[SOUND]);
	m_Layer[2]->Add(soundSpecialEatComp);

	Composite* soundDamageComp = Composite::CreateSound(game, "DamageSound", "Damage.wav");
	m_DamageSound = (Sound*)soundDamageComp->FindComponent(COMPONENT_NAME[SOUND]);
	m_Layer[2]->Add(soundDamageComp);

	if (RegDataManager::GetInstance()->ReadDWORDValue("SoundOff") == 0)
	{
		m_BGMSound->VolumeOnOff(true);
		m_EatSound->VolumeOnOff(true);
		m_SpecialEatSound->VolumeOnOff(true);
		m_DamageSound->VolumeOnOff(true);
	}
	else
	{
		m_BGMSound->VolumeOnOff(false);
		m_EatSound->VolumeOnOff(false);
		m_SpecialEatSound->VolumeOnOff(false);
		m_DamageSound->VolumeOnOff(false);
	}
}

void PlayState::CloudInit(GameEngine* game)
{
	for (int i = 0; i < MAX_CLOUD_NUM; i++)
	{
		char strCloud[128];
		sprintf_s(strCloud, "Cloud_%d", i);
		char strCloudImage[128];
		sprintf_s(strCloudImage, "Cloud_%d.bmp", i % 3);
		Composite* cloud = Composite::CreateSprite(game, strCloud, strCloudImage);
		m_CloudTrans[i] = (Transform*)cloud->FindComponent(COMPONENT_NAME[TRANSFORM]);
		m_CloudTrans[i]->m_fX = CLOUD_X[i];
		m_CloudTrans[i]->m_fY = CLOUD_Y[i];
		m_Layer[0]->Add(cloud);

		m_CloudTrans[i]->TweenMoveX(TweenArg::TYPE_LINEAR, CLOUD_X[i], CLOUD_DIST_Y, CLOUD_TIME[i]);
		char strCloudCallback[64];
		sprintf_s(strCloudCallback, "Cloud_%d", i);
		m_CloudTrans[i]->TweenEndCallback(this, strCloudCallback);
		m_CloudTrans[i]->TweenPlay();
	}

}

void PlayState::PlayerInit(GameEngine* game)
{
	m_PlayerComp = Composite::CreateSpriteBox(game, "Player", "Player_0.bmpRGB239148099", PLAYER_BOXCOLLISION_OFFSET, PLAYER_BOXCOLLISION_OFFSET);
	m_PlayerBoxCollider = (BoxCollider*)m_PlayerComp->FindComponent(COMPONENT_NAME[BOXCOLLIDER]);
	m_PlayerTrans = (Transform*)m_PlayerComp->FindComponent(COMPONENT_NAME[TRANSFORM]);
	m_PlayerTrans->m_fX = INIT_X;
	m_PlayerTrans->m_fY = INIT_Y;
	m_Layer[2]->Add(m_PlayerComp);

	m_PlayerFrameImage.push_back("Player_0.bmpRGB239148099");
	m_PlayerFrameImage.push_back("Player_1.bmpRGB239148099");
	m_PlayerFrameImage.push_back("Player_0.bmpRGB239148099");
	m_PlayerTrans->TweenFrame(m_PlayerFrameImage, FRAME_ANIMATION_INTERVAL);
	m_PlayerTrans->FrameTweenPlayForever();

	m_RedPlayerFrameImage.push_back("Player_0.bmpRGB230096096");
	m_RedPlayerFrameImage.push_back("Player_1.bmpRGB230096096");
	m_RedPlayerFrameImage.push_back("Player_0.bmpRGB230096096");
}

void PlayState::DamageInit(GameEngine* game)
{
	m_DamageRedComp = Composite::CreateSprite(game, "DamageRed", "Red.bmp");
	m_DamageRedTrans = (Transform*)m_DamageRedComp->FindComponent(COMPONENT_NAME[TRANSFORM]);
	m_DamageRedTrans->m_fScaleX = 640;
	m_DamageRedTrans->m_fScaleY = 480;
	m_Layer[2]->Add(m_DamageRedComp);
	m_DamageRedComp->SetEnable(false);
}

void PlayState::FruitInit(GameEngine* game)
{
	for (int i = 0; i < MAX_FRUIT; i++)
	{
		char compName[128];
		sprintf_s(compName, "fruit_%d", i);
		m_FruitComp[i] = Composite::CreateSpriteBox(game, compName, "Fruit.bmpRGB189230074", 0, 0);
		m_FruitRenderer[i] = (Renderer*)m_FruitComp[i]->FindComponent(COMPONENT_NAME[RENDERER]);
		m_FruitBoxCollider[i] = (BoxCollider*)m_FruitComp[i]->FindComponent(COMPONENT_NAME[BOXCOLLIDER]);
		m_FruitTrans[i] = (Transform*)m_FruitComp[i]->FindComponent(COMPONENT_NAME[TRANSFORM]);
		m_FruitTrans[i]->m_fY = -m_FruitRenderer[i]->GetHeightF();
		m_Layer[1]->Add(m_FruitComp[i]);
	}

	for (int i = 0; i < MAX_FRUIT_EFFECT; i++)
	{
		char compName[128];
		sprintf_s(compName, "fruitEffect_%d", i);
		m_FruitEffectComp[i] = Composite::CreateSpriteBox(game, compName, "FruitEffect_0.bmp", 0, 0);
		m_FruitEffectTrans[i] = (Transform*)m_FruitEffectComp[i]->FindComponent(COMPONENT_NAME[TRANSFORM]);
		m_Layer[2]->Add(m_FruitEffectComp[i]);
		m_FruitEffectComp[i]->SetEnable(false);
	}

	m_FruitEffectImage.push_back("FruitEffect_0.bmp");
	m_FruitEffectImage.push_back("FruitEffect_1.bmp");
	m_FruitEffectImage.push_back("FruitEffect_2.bmp");
	m_FruitEffectImage.push_back("FruitEffect_3.bmp");
	m_FruitEffectImage.push_back("FruitEffect_4.bmp");
}
void PlayState::CarInit(GameEngine* game)
{
	for (int i = 0; i < MAX_CAR; i++)
	{
		char compName[128];
		sprintf_s(compName, "Car_%d", i);
		m_CarComp[i] = Composite::CreateSpriteBox(game, compName, "Car.bmp", CAR_BOXCOLLISION_OFFSET, CAR_BOXCOLLISION_OFFSET);
		m_CarBoxCollider[i] = (BoxCollider*)m_CarComp[i]->FindComponent(COMPONENT_NAME[BOXCOLLIDER]);
		m_CarTrans[i] = (Transform*)m_CarComp[i]->FindComponent(COMPONENT_NAME[TRANSFORM]);
		m_CarTrans[i]->m_fX = CAR_INIT_X;
		m_CarTrans[i]->m_fY = CAR_INIT_Y;
		m_Layer[1]->Add(m_CarComp[i]);
	}

	for (int i = 0; i < MAX_CAR; i++)
	{
		char compName[128];
		sprintf_s(compName, "CarArrow_%d", i);
		m_CarArrowComp[i] = Composite::CreateSprite(game, compName, "RightArrow.bmp");
		m_CarArrowRenderer[i] = (Renderer*)m_CarArrowComp[i]->FindComponent(COMPONENT_NAME[RENDERER]);
		m_CarArrowTrans[i] = (Transform*)m_CarArrowComp[i]->FindComponent(COMPONENT_NAME[TRANSFORM]);
		m_CarArrowTrans[i]->m_fX = -100.0f;
		m_CarArrowTrans[i]->m_fY = CAR_INIT_Y;
		m_Layer[1]->Add(m_CarArrowComp[i]);
		m_CarArrowComp[i]->SetEnable(false);
	}
}

void PlayState::UIInit(GameEngine* game)
{
	sprintf_s(m_Score, "StageScore : %d", m_nScore);
	m_ScoreComp = Composite::CreateText(game, "score", m_Score);
	m_ScoreText = (Text*)m_ScoreComp->FindComponent(COMPONENT_NAME[TEXTCOMP]);
	Transform* scoreText = (Transform*)m_ScoreComp->FindComponent(COMPONENT_NAME[TRANSFORM]);
	scoreText->m_fX = 3;
	scoreText->m_fY = 4;
	m_Layer[2]->Add(m_ScoreComp);

	for (int i = 0; i < MAX_HEART; i++)
	{
		m_HeartComp[i] = Composite::CreateSprite(game, "Heart", "Heart.bmpRGB255000074");
		m_HeartTrans[i] = (Transform*)m_HeartComp[i]->FindComponent(COMPONENT_NAME[TRANSFORM]);
		m_HeartTrans[i]->m_fX = 530.0f + (i * 35.0f);
		m_HeartTrans[i]->m_fY = 0;
		m_Layer[2]->Add(m_HeartComp[i]);
	}

	m_GameOverComp = Composite::CreateSprite(game, "GameOver", "GameOver.bmp");
	m_GameOverTrans = (Transform*)m_GameOverComp->FindComponent(COMPONENT_NAME[TRANSFORM]);
	m_GameOverTrans->m_fX = 186;
	m_GameOverTrans->m_fY = 214;
	m_Layer[2]->Add(m_GameOverComp);
	m_GameOverComp->SetEnable(false);
}

void PlayState::Pause()
{
}

void PlayState::Resume()
{
}

void PlayState::HandleEvents(GameEngine* game)
{
	if (m_bGameOver == false)
	{
		if (KEY_DOWN(0x41) == true)
		{
			m_PlayerTrans->m_bXFlip = true;
			m_PlayerTrans->m_fX -= (m_nPlayerSpeed * ENGINE_MAIN->m_fTimeDelta);
			if (m_PlayerTrans->m_fX < MAX_LEFT_MOVE)
			{
				m_PlayerTrans->m_fX = MAX_LEFT_MOVE;
			}
		}

		if (KEY_DOWN(0x44) == true)
		{
			m_PlayerTrans->m_bXFlip = false;
			m_PlayerTrans->m_fX += (m_nPlayerSpeed * ENGINE_MAIN->m_fTimeDelta);
			if (m_PlayerTrans->m_fX > MAX_RIGHT_MOVE)
			{
				m_PlayerTrans->m_fX = MAX_RIGHT_MOVE;
			}
		}

		if (KEY_DOWN(0x57) == true)
		{
			if (m_PlayerTrans->IsTween() == false)
			{
				m_PlayerTrans->TweenMoveY(TweenArg::TYPE_EASE_OUT_CIRC, m_PlayerTrans->m_fY, JUMP_Y, JUMP_TIME);
				m_PlayerTrans->TweenMoveY(TweenArg::TYPE_EASE_IN_CIRC, JUMP_Y, INIT_Y, JUMP_TIME);
				m_PlayerTrans->TweenPlay();
			}
		}
	}

	if (m_bGameOver == true)
	{
		if (KEY_DOWN_KEYHIT(VK_RETURN) == true)
		{
			game->ChangeState(new IntroState());
		}
	}

	if (KEY_DOWN_KEYHIT(VK_ESCAPE) == true)
	{
		game->Quit();
	}
	/*
	if (KEY_DOWN_KEYHIT(VK_F1) == true)
	{
		m_FruitEffectComp[0]->SetEnable(true);
		m_FruitEffectTrans[0]->TweenFrame(m_FruitEffectImage, 0.05f);
		m_FruitEffectTrans[0]->FrameEndCallback(this, "FruitEffect_0");
		m_FruitEffectTrans[0]->FrameTweenPlay();
	}
	*/
}

void PlayState::Update(GameEngine* game)
{
	if (m_bGameOver == false)
	{
		FruitUpdate();
		CarUpdate();
		CollisionUpdate();
		PalyerSpeedUpUpdate();
		PalyerInvincibleUpdate();
	}
}

void PlayState::FruitUpdate()
{
	m_fFruitTimeElapsed += ENGINE_MAIN->m_fTimeDelta;
	if (m_fFruitTimeElapsed > m_nFruitDownInverval)
	{
		//DLOG("%d", m_nFruitIndex);
		srand(GetTickCount() * GetTickCount());
		int nRandX = rand() % 600;
		int nFruitTypeRange = rand() % 100;
		if (nFruitTypeRange >= 0 && nFruitTypeRange <= m_nLowFruitRate)
		{
			m_FruitRenderer[m_nFruitIndex]->ChangeBitmap("Fruit.bmpRGB189230074");
			m_nFruitType[m_nFruitIndex] = LOW_FRUIT;
		}
		else if (nFruitTypeRange > m_nLowFruitRate && nFruitTypeRange <= m_nHighFruitRate)
		{
			m_FruitRenderer[m_nFruitIndex]->ChangeBitmap("Fruit.bmpRGB230033099");
			m_nFruitType[m_nFruitIndex] = HIGH_FRUIT;
		}
		else if (nFruitTypeRange > m_nHighFruitRate && nFruitTypeRange <= m_nBadFruitRate)
		{
			m_FruitRenderer[m_nFruitIndex]->ChangeBitmap("BadFruit.bmp");
			m_nFruitType[m_nFruitIndex] = BAD_FRUIT;
		}
		else if (nFruitTypeRange > m_nBadFruitRate && nFruitTypeRange <= m_nSpecialFruitRate)
		{
			m_FruitRenderer[m_nFruitIndex]->ChangeBitmap("SpecialFruit.bmp");
			m_nFruitType[m_nFruitIndex] = SPECTIAL_FRUIT;
		}
		m_FruitTrans[m_nFruitIndex]->m_fX = (float)nRandX;
		m_FruitComp[m_nFruitIndex]->SetEnable(true);
		m_FruitTrans[m_nFruitIndex]->TweenMoveY(TweenArg::TYPE_EASE_IN_CIRC, 0.0f, DOWN_Y, m_nFruitDownSpeed);
		m_FruitTrans[m_nFruitIndex]->TweenPlay();
		m_nFruitIndex++;
		if (m_nFruitIndex == MAX_FRUIT)
		{
			m_nFruitIndex = 0;
		}

		m_fFruitTimeElapsed = 0.0f;
	}

	m_fFruitDifficultyTimeElapsed += ENGINE_MAIN->m_fTimeDelta;
	if (m_fFruitDifficultyTimeElapsed > DIFICULTY_TIME)
	{
		m_nFruitDownInverval -= MINUS_FRUIT_DOWN_INTERVAL;
		if (m_nFruitDownInverval < MAX_DOWN_INTERVAL)
		{
			m_nFruitDownInverval = MAX_DOWN_INTERVAL;
		}

		m_nFruitDownSpeed -= MINUS_FRUIT_DOWN_SPEED;
		if (m_nFruitDownSpeed < MAX_DOWN_SPEED)
		{
			m_nFruitDownSpeed = MAX_DOWN_SPEED;
			//만약 최대 속도가 되면 나쁜 과일이 나올 확률을 높여준다.
			m_nLowFruitRate -= MINUS_FRUIT_RATE;
			if (m_nLowFruitRate < MIN_LOW_FRUIT_RATE)
			{
				m_nLowFruitRate = MIN_LOW_FRUIT_RATE;
			}
			m_nHighFruitRate -= MINUS_FRUIT_RATE;
			if (m_nHighFruitRate < MIN_HIGH_FRUIT_RATE)
			{
				m_nHighFruitRate = MIN_HIGH_FRUIT_RATE;
			}
		}

		//DLOG("DifficultyTimeElapsed %f %f", m_nFruitDownInverval, m_nFruitDownSpeed);
		m_fFruitDifficultyTimeElapsed = 0.0f;
	}
}

void PlayState::CarUpdate()
{
	m_fCarTimeElapsed += ENGINE_MAIN->m_fTimeDelta;
	if (m_fCarTimeElapsed > m_fCarTimeInterval)
	{
		srand(GetTickCount() * GetTickCount());
		int nRandDirection = rand() % 2;
		//차가 왼쪽으로 이동
		if (nRandDirection == 0)
		{
			m_CarArrowComp[m_nCarIndex]->SetEnable(true);
			m_CarArrowTrans[m_nCarIndex]->m_bXFlip = true;
			m_CarArrowTrans[m_nCarIndex]->m_fX = CAR_ARROW_LEFT_MOVE_INIT_X;
			m_CarTrans[m_nCarIndex]->m_bXFlip = true;
			char strCarArrow[64];
			sprintf_s(strCarArrow, "CarArrow_%d", m_nCarIndex);
			m_CarTrans[m_nCarIndex]->TweenDelay(1.0f);
			m_CarTrans[m_nCarIndex]->TweenEndCallback(this, strCarArrow);
			m_CarTrans[m_nCarIndex]->TweenMoveX(TweenArg::TYPE_LINEAR, CAR_RIGHT_X_MAX, CAR_LEFT_X_MAX, m_fCarMoveTime);
			m_CarTrans[m_nCarIndex]->TweenEndCallback(this, "Car");
			m_CarTrans[m_nCarIndex]->TweenPlay();
		}
		//차가 오른쪽으로 이동
		else if (nRandDirection == 1)
		{
			m_CarArrowComp[m_nCarIndex]->SetEnable(true);
			m_CarArrowTrans[m_nCarIndex]->m_bXFlip = false;
			m_CarArrowTrans[m_nCarIndex]->m_fX = CAR_ARROW_RIGHT_MOVE_INIT_X;
			m_CarTrans[m_nCarIndex]->m_bXFlip = false;
			char strCarArrow[64];
			sprintf_s(strCarArrow, "CarArrow_%d", m_nCarIndex);
			m_CarTrans[m_nCarIndex]->TweenDelay(1.0f);
			m_CarTrans[m_nCarIndex]->TweenEndCallback(this, strCarArrow);
			m_CarTrans[m_nCarIndex]->TweenMoveX(TweenArg::TYPE_LINEAR, CAR_LEFT_X_MAX, CAR_RIGHT_X_MAX, m_fCarMoveTime);
			m_CarTrans[m_nCarIndex]->TweenEndCallback(this, "Car");
			m_CarTrans[m_nCarIndex]->TweenPlay();
		}
		m_nCarIndex++;
		if (m_nCarIndex == MAX_CAR)
		{
			m_nCarIndex = 0;
		}

		m_fCarTimeElapsed = 0.0f;
	}

	m_fCarDifficultyTimeElapsed += ENGINE_MAIN->m_fTimeDelta;
	if (m_fCarDifficultyTimeElapsed > DIFICULTY_TIME)
	{
		m_fCarTimeInterval -= MINUS_CAR_INTERVAL;
		if (m_fCarTimeInterval < MIN_CAR_INTERVAL)
		{
			m_fCarTimeInterval = MIN_CAR_INTERVAL;
		}
		m_fCarDifficultyTimeElapsed = 0.0f;
	}
}

void PlayState::CollisionUpdate()
{
	for (int i = 0; i < MAX_FRUIT; i++)
	{
		if (m_PlayerBoxCollider->intersectRect(m_FruitBoxCollider[i]->GetRect()) == true)
		{
			//DLOG("Coll %d", i);
			if (m_nFruitType[i] == LOW_FRUIT)
			{
				FruitEffectStart(i);
				m_nScore++;
				sprintf_s(m_Score, "StageScore : %d", m_nScore);
				m_ScoreText->ChangeText(m_Score);
				m_FruitTrans[i]->TweenStop();
				m_FruitTrans[i]->m_fY = -m_FruitRenderer[i]->GetHeightF();
				m_EatSound->Play();
			}
			else if (m_nFruitType[i] == HIGH_FRUIT)
			{
				FruitEffectStart(i);
				m_nScore += 3;
				sprintf_s(m_Score, "StageScore : %d", m_nScore);
				m_ScoreText->ChangeText(m_Score);
				m_FruitTrans[i]->TweenStop();
				m_FruitTrans[i]->m_fY = -m_FruitRenderer[i]->GetHeightF();
				m_EatSound->Play();
			}
			else if (m_nFruitType[i] == BAD_FRUIT)
			{
				if (m_bInvincible == false)
				{
					m_bInvincible = true;
					m_PlayerTrans->FrameTweenStop();
					m_PlayerTrans->TweenFrame(m_RedPlayerFrameImage, FRAME_ANIMATION_INTERVAL);
					m_PlayerTrans->FrameTweenPlayForever();

					m_DamageRedComp->SetEnable(true);
					m_DamageRedTrans->TweenDelay(DAMAGE_RED_DELAY_TIME);
					m_DamageRedTrans->TweenEndCallback(this, "DamageRed");
					m_DamageRedTrans->TweenPlay();

					m_HeartComp[m_nHeartIndex]->SetEnable(false);
					m_nHeartIndex++;
					if (m_nHeartIndex == MAX_HEART)
					{
						DLOG("Game Over");
						m_bGameOver = true;
						m_GameOverComp->SetEnable(true);
						RegDataManager::GetInstance()->WriteDWORDValue("Score", m_nScore);
					}
					m_DamageSound->Play();
				}
				m_FruitTrans[i]->TweenStop();
				m_FruitTrans[i]->m_fY = -m_FruitRenderer[i]->GetHeightF();
			}
			else if (m_nFruitType[i] == SPECTIAL_FRUIT)
			{
				DLOG("Speed UP");
				FruitEffectStart(i);
				m_bSpeedUp = true;
				m_nPlayerSpeed = SPEEDUP_SPEED;
				m_fPlayerSpeedUpTimeElapsed = 0.0f;
				m_FruitTrans[i]->TweenStop();
				m_FruitTrans[i]->m_fY = -m_FruitRenderer[i]->GetHeightF();
				m_SpecialEatSound->Play();
			}
		}
	}

	for (int i = 0; i < MAX_CAR; i++)
	{
		if (m_bInvincible == false)
		{
			if (m_PlayerBoxCollider->intersectRect(m_CarBoxCollider[i]->GetRect()) == true)
			{
				m_bInvincible = true;
				m_PlayerTrans->FrameTweenStop();
				m_PlayerTrans->TweenFrame(m_RedPlayerFrameImage, FRAME_ANIMATION_INTERVAL);
				m_PlayerTrans->FrameTweenPlayForever();
				m_DamageRedComp->SetEnable(true);

				m_DamageRedTrans->TweenDelay(DAMAGE_RED_DELAY_TIME);
				m_DamageRedTrans->TweenEndCallback(this, "DamageRed");
				m_DamageRedTrans->TweenPlay();

				m_HeartComp[m_nHeartIndex]->SetEnable(false);
				m_nHeartIndex++;
				if (m_nHeartIndex == MAX_HEART)
				{
					DLOG("Game Over");
					m_bGameOver = true;
					m_GameOverComp->SetEnable(true);
					if (RegDataManager::GetInstance()->ReadDWORDValue("Score") < (DWORD)m_nScore)
					{
						RegDataManager::GetInstance()->WriteDWORDValue("Score", m_nScore);
					}					
				}
				m_DamageSound->Play();
			}
		}
	}
}

void PlayState::FruitEffectStart(int nFruitIndex)
{
	m_FruitEffectComp[m_nFruitEffectIndex]->SetEnable(true);
	m_FruitEffectTrans[m_nFruitEffectIndex]->m_fX = m_FruitTrans[nFruitIndex]->m_fX - 30.0f;
	m_FruitEffectTrans[m_nFruitEffectIndex]->m_fY = m_FruitTrans[nFruitIndex]->m_fY - 30.0f;
	m_FruitEffectTrans[m_nFruitEffectIndex]->TweenFrame(m_FruitEffectImage, FRUIT_EFFECT_TIME);
	char fruitEffectCallbackName[128];
	sprintf_s(fruitEffectCallbackName, "FruitEffect_%d", m_nFruitEffectIndex);
	m_FruitEffectTrans[m_nFruitEffectIndex]->FrameEndCallback(this, fruitEffectCallbackName);
	m_FruitEffectTrans[m_nFruitEffectIndex]->FrameTweenPlay();
	m_nFruitEffectIndex++;
	if (m_nFruitEffectIndex >= MAX_FRUIT_EFFECT)
	{
		m_nFruitEffectIndex = 0;
	}

}

void PlayState::PalyerSpeedUpUpdate()
{
	if (m_bSpeedUp == true)
	{
		m_fPlayerSpeedUpTimeElapsed += ENGINE_MAIN->m_fTimeDelta;
		if (m_fPlayerSpeedUpTimeElapsed > CHARACTER_SPEED_UP_TIME)
		{
			//DLOG("Speed Default");
			m_nPlayerSpeed = DEFAULT_SPEED;
			m_bSpeedUp = false;
		}
	}
}

void PlayState::PalyerInvincibleUpdate()
{
	if (m_bInvincible == true)
	{
		m_fPlayerInvincibleTimeElapsed += ENGINE_MAIN->m_fTimeDelta;
		if (m_fPlayerInvincibleTimeElapsed > INVICIVLE_TIME)
		{
			m_bInvincible = false;
			m_PlayerTrans->FrameTweenStop();
			m_PlayerTrans->TweenFrame(m_PlayerFrameImage, FRAME_ANIMATION_INTERVAL);
			m_PlayerTrans->FrameTweenPlayForever();
			m_fPlayerInvincibleTimeElapsed = 0.0f;
		}
	}
}

void PlayState::AfterDraw(GameEngine* game)
{

}

void PlayState::AnimationEnd(string strAniName)
{
	if (strAniName.find("CarArrow") != string::npos)
	{
		size_t foundPos = strAniName.find("_");
		if (foundPos != string::npos)
		{
			string strNum = strAniName.substr(foundPos + 1);
			int nCarArrowIndex = atoi(strNum.c_str());
			m_CarArrowComp[nCarArrowIndex]->SetEnable(false);
			//DLOG("%s %d %s", strAniName.c_str(), foundPos, strNum.c_str());
		}
	}
	else if (strAniName.find("FruitEffect") != string::npos)
	{
		DLOG("FruitEffect");
		size_t foundPos = strAniName.find("_");
		if (foundPos != string::npos)
		{
			string strNum = strAniName.substr(foundPos + 1);
			int nEffectIndex = atoi(strNum.c_str());
			m_FruitEffectComp[nEffectIndex]->SetEnable(false);
		}
	}
	else if (strAniName.find("DamageRed") != string::npos)
	{
		m_DamageRedComp->SetEnable(false);
	}
}

void PlayState::Cleanup()
{
	delete this;
}

