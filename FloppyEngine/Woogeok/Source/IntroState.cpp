#include "../resource.h"
#include "Header/FloppyEngine.h"
#include "../Header/IntroState.h"
#include "../Header/PlayState.h"
#include "../Header/TwoPlayState.h"

enum { START_TYPE, DUAL_TYPE, SOUND_TYPE, QUIT_TYPE };

const bool CLOUD_FLIP[] = { true, false, true, false };
const float CLOUD_X[] = { 640.0f, 940.0f, 1240.0f, 1480.0f };
const float CLOUD_Y[] = { 30.0f, 85.0f, 55.0f, 105.0f };
const float CLOUD_TIME[] = { 45.0f, 38.0f, 55.0f, 70.0f };
const float CLOUD_DIST_Y = -100.0f;

#define TITLE_X 145.0f
#define TITLE_Y 78.0f

#define ONEP_X 10.0f
#define ONEP_Y 380.0f

#define TWOP_X 535.0f
#define TWOP_Y 380.0f

const float MENU_X[] = { 236.0f, 236.0f, 236.0f, 236.0f };
const float MENU_Y[] = { 249.0f, 303.0f, 357.0f, 411.0f };
const float MENU_TEXT_X[] = { 301.0f, 301.0f, 291.0f, 301.0f };
const float MENU_TEXT_Y[] = { 257.0f, 312.0f, 367.0f, 422.0f };
const string MENU_SOUND_ON_TEXT = "소리 켬";
const string MENU_SOUND_OFF_TEXT = "소리 끔";
const string MENU_TEXT[] = { "시작", "대전", MENU_SOUND_ON_TEXT, "종료" };

const float MENU_SLECT_X[] = { 232.0f, 232.0f, 232.0f, 232.0f };
const float MENU_SLECT_Y[] = { 244.0f, 298.0f, 352.0f, 406.0f };

void IntroState::Init(GameEngine* game)
{
	SpriteManager::GetInstance()->CreateSprite("Menu.bmp", IDC_MENU, false);
	SpriteManager::GetInstance()->CreateSprite("Select.bmp", IDC_SELECT, true);
	SpriteManager::GetInstance()->CreateSprite("ScoreTextBack.bmp", IDC_SCORETEXTBACK, false);
	SpriteManager::GetInstance()->CreateSprite("Background.bmp", IDC_BACKGROUND, false);
	SpriteManager::GetInstance()->CreateSprite("Title.bmp", IDC_TITLE, true);
	SpriteManager::GetInstance()->CreateSprite("Cloud_0.bmp", IDC_CLOUD_0, true);
	SpriteManager::GetInstance()->CreateSprite("Cloud_1.bmp", IDC_CLOUD_1, true);
	SpriteManager::GetInstance()->CreateSprite("Cloud_2.bmp", IDC_CLOUD_2, true);
	SpriteManager::GetInstance()->CreateSprite("1P.bmp", IDC_1P, true);
	SpriteManager::GetInstance()->CreateSprite("2P.bmp", IDC_2P, true);


	for (int i = 0; i < INTROSTATE_MAX_LAYER; i++)
	{
		char layerName[128];
		sprintf_s(layerName, "layer_%d", i);
		m_Layer[i] = Composite::Create(game, layerName);
		m_MainComposite.Add(m_Layer[i]);
	}
	Composite* background = Composite::CreateSprite(game, "Background", "Background.bmp");
	m_Layer[0]->Add(background);
	
	UIInit(game);
	CloudInit(game);
	SoundInit(game);
}

void IntroState::UIInit(GameEngine* game)
{
	for (int i = 0; i < INTROSTATE_MAX_MENU; i++)
	{
		char menuName[128];
		sprintf_s(menuName, "menu_%d", i);
		Composite* menu = Composite::CreateSprite(game, menuName, "Menu.bmp");
		m_MenuTransform[i] = (Transform*)menu->FindComponent(TRANSFORM);
		m_MenuTransform[i]->m_fX = MENU_X[i];
		m_MenuTransform[i]->m_fY = MENU_Y[i];
		m_Layer[2]->Add(menu);

		char menuNameText[128];
		sprintf_s(menuNameText, "menuText_%d", i);
		Composite* menuText = Composite::CreateText(game, menuName, MENU_TEXT[i]);
		m_MenuTransformText[i] = (Transform*)menuText->FindComponent(TRANSFORM);
		m_MenuTransformText[i]->m_fX = MENU_TEXT_X[i];
		m_MenuTransformText[i]->m_fY = MENU_TEXT_Y[i];
		m_Layer[2]->Add(menuText);
		if (i == SOUND_TYPE)
		{
			m_MenuSoundText = (Text*)menuText->FindComponent(TEXTCOMP);
			if (RegDataManager::GetInstance()->ReadDWORDValue("SoundOff") == 0)
			{
				m_MenuSoundText->ChangeText(MENU_SOUND_ON_TEXT);
			}
			else
			{
				m_MenuSoundText->ChangeText(MENU_SOUND_OFF_TEXT);
			}
		}
	}

	Composite* selectBar = Composite::CreateSprite(game, "Select", "Select.bmp");
	m_SelectBarTramsform = (Transform*)selectBar->FindComponent(TRANSFORM);
	m_SelectBarTramsform->m_fX = MENU_SLECT_X[0];
	m_SelectBarTramsform->m_fY = MENU_SLECT_Y[0];
	m_SelectBarTramsform->m_bXFlip = true;
	m_Layer[2]->Add(selectBar);

	Composite* title = Composite::CreateSprite(game, "Title", "Title.bmp");
	Transform* titleTrans = (Transform*)title->FindComponent(TRANSFORM);
	titleTrans->m_fX = TITLE_X;
	titleTrans->m_fY = TITLE_Y;
	m_Layer[2]->Add(title);

	Composite* scoreBack = Composite::CreateSprite(game, "ScoreBack", "ScoreTextBack.bmp");
	m_Layer[2]->Add(scoreBack);


	char strScore[128];
	int nScore = RegDataManager::GetInstance()->ReadDWORDValue("Score");
	sprintf_s(strScore, "High Score : %d", nScore);
	Composite* highScoreComp = Composite::CreateTextColorFontSize(game, "score", strScore, RGB(255, 255, 255), 13);
	Transform* highScoreTrans = (Transform*)highScoreComp->FindComponent(TRANSFORM);
	highScoreTrans->m_fX = 5;
	highScoreTrans->m_fY = 6;
	m_Layer[2]->Add(highScoreComp);


	Composite* onePlayer = Composite::CreateSprite(game, "1P", "1P.bmp");
	Transform* onePlayerTrans = (Transform*)onePlayer->FindComponent(TRANSFORM);
	onePlayerTrans->m_fX = ONEP_X;
	onePlayerTrans->m_fY = ONEP_Y;
	m_Layer[2]->Add(onePlayer);

	Composite* twoPlayer = Composite::CreateSprite(game, "2P", "2P.bmp");
	Transform* twoPlayerTrans = (Transform*)twoPlayer->FindComponent(TRANSFORM);
	twoPlayerTrans->m_fX = TWOP_X;
	twoPlayerTrans->m_fY = TWOP_Y;
	m_Layer[2]->Add(twoPlayer);
}

void IntroState::CloudInit(GameEngine* game)
{
	for (int i = 0; i < INTROSTATE_MAX_CLOUD_NUM; i++)
	{
		char strCloud[128];
		sprintf_s(strCloud, "Cloud_%d", i);
		char strCloudImage[128];
		sprintf_s(strCloudImage, "Cloud_%d.bmp", i % 3);
		Composite* cloud = Composite::CreateSprite(game, strCloud, strCloudImage);
		m_CloudTrans[i] = (Transform*)cloud->FindComponent(TRANSFORM);
		m_CloudTrans[i]->m_fX = CLOUD_X[i];
		m_CloudTrans[i]->m_fY = CLOUD_Y[i];
		m_Layer[2]->Add(cloud);

		m_CloudTrans[i]->TweenMoveX(TweenArg::TYPE_LINEAR, CLOUD_X[i], CLOUD_DIST_Y, CLOUD_TIME[i]);
		char strCloudCallback[64];
		sprintf_s(strCloudCallback, "Cloud_%d", i);
		m_CloudTrans[i]->TweenEndCallback(this, strCloudCallback);
		m_CloudTrans[i]->TweenPlay();
	}

}

void IntroState::SoundInit(GameEngine* game)
{
	//사운드
	Composite* soundBGMComp = Composite::CreateBGMSound(game, "BGMSound", "WoogeokBGM.mp3");
	m_BGMSound = (Sound*)soundBGMComp->FindComponent(SOUND);
	m_Layer[2]->Add(soundBGMComp);
	m_BGMSound->Play();

	Composite* soundComp = Composite::CreateSound(game, "ClickSound", "Click.wav");
	m_ClickSound = (Sound*)soundComp->FindComponent(SOUND);
	m_Layer[2]->Add(soundComp);

	if (RegDataManager::GetInstance()->ReadDWORDValue("SoundOff") == 0)
	{
		m_BGMSound->VolumeOnOff(true);
		m_ClickSound->VolumeOnOff(true);
	}
	else
	{
		m_BGMSound->VolumeOnOff(false);
		m_ClickSound->VolumeOnOff(false);
	}
}

void IntroState::Pause()
{
}

void IntroState::Resume()
{
}

void IntroState::HandleEvents(GameEngine* game)
{
	if (KEY_DOWN_KEYHIT(VK_UP) == true || KEY_DOWN_KEYHIT(0x57) == true)
	{
		m_nMenuIndex--;
		if (m_nMenuIndex < 0)
		{
			m_nMenuIndex = 0;
		}
		m_SelectBarTramsform->m_fX = MENU_SLECT_X[m_nMenuIndex];
		m_SelectBarTramsform->m_fY = MENU_SLECT_Y[m_nMenuIndex];
		m_ClickSound->Play();
	}
	else if (KEY_DOWN_KEYHIT(VK_DOWN) == true || KEY_DOWN_KEYHIT(0x53) == true)
	{
		m_nMenuIndex++;
		if (m_nMenuIndex >= INTROSTATE_MAX_MENU)
		{
			m_nMenuIndex = INTROSTATE_MAX_MENU - 1;
		}
		m_SelectBarTramsform->m_fX = MENU_SLECT_X[m_nMenuIndex];
		m_SelectBarTramsform->m_fY = MENU_SLECT_Y[m_nMenuIndex];
		m_ClickSound->Play();
	}

	if (KEY_DOWN_KEYHIT(VK_RETURN) == true || KEY_DOWN_KEYHIT(VK_SPACE) == true)
	{
		m_ClickSound->Play();
		if (m_nMenuIndex == START_TYPE)
		{
			game->ChangeState(new PlayState());
		}
		else if (m_nMenuIndex == DUAL_TYPE)
		{
			game->ChangeState(new TwoPlayState());
		}
		else if (m_nMenuIndex == SOUND_TYPE)
		{
			if (RegDataManager::GetInstance()->ReadDWORDValue("SoundOff") == 0)
			{
				RegDataManager::GetInstance()->WriteDWORDValue("SoundOff", 1);
				m_MenuSoundText->ChangeText(MENU_SOUND_OFF_TEXT);
				m_BGMSound->VolumeOnOff(false);
				m_ClickSound->VolumeOnOff(false);
			}
			else
			{
				RegDataManager::GetInstance()->WriteDWORDValue("SoundOff", 0);
				m_MenuSoundText->ChangeText(MENU_SOUND_ON_TEXT);
				m_BGMSound->VolumeOnOff(true);
				m_ClickSound->VolumeOnOff(true);
			}
		}
		else if (m_nMenuIndex == QUIT_TYPE)
		{
			game->Quit();
		}
	}

	if (KEY_DOWN_KEYHIT(VK_ESCAPE) == true)
	{
		game->Quit();
	}
}

void IntroState::Update(GameEngine* game)
{

}

void IntroState::AfterDraw(GameEngine* game)
{

}

void IntroState::AnimationEnd(string strAniName)
{
	if (strAniName.find("Cloud") != string::npos)
	{
		size_t foundPos = strAniName.find("_");
		if (foundPos != string::npos)
		{
			string strNum = strAniName.substr(foundPos + 1);
			int nCloudIndex = atoi(strNum.c_str());

			m_CloudTrans[nCloudIndex]->TweenMoveX(TweenArg::TYPE_LINEAR, CLOUD_X[nCloudIndex], CLOUD_DIST_Y, CLOUD_TIME[nCloudIndex]);
			char strCloud[64];
			sprintf_s(strCloud, "Cloud_%d", nCloudIndex);
			m_CloudTrans[nCloudIndex]->TweenEndCallback(this, strCloud);
			m_CloudTrans[nCloudIndex]->TweenPlay();
		}
	}
	//DLOG("Ani End %s", strAniName.c_str());
}

void IntroState::Cleanup()
{
	delete this;
}