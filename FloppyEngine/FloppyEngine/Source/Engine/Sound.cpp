#include "../../Header/Engine/Sound.h"
#include "../../Header/Engine/GameEngine.h"
#include "../../Header/Engine/Composite.h"
#include "../../Header/Engine/LogManager.h"

Sound::Sound()
{

}
Sound::Sound(string soundFile, bool bEffect)
{
	char currentPath[MAX_PATH];
	GetModuleFileName(NULL, currentPath, MAX_PATH);

	string strCurrentPath(currentPath);
	size_t found = strCurrentPath.rfind("\\");
	if (found != std::string::npos)
	{
		strCurrentPath = strCurrentPath.substr(0, found);
	}
	char midiFilePath[MAX_PATH];
	sprintf_s(midiFilePath, "%s\\%s", strCurrentPath.c_str(), soundFile.c_str());
	m_strSoundFile.append(midiFilePath);

	m_bEffect = bEffect;
}

void Sound::Add(GameEngine* gameEngine, Composite* composite)
{
	m_Composite = composite;
	m_hWnd = gameEngine->m_hWnd;
		
	if (m_bEffect == true)
	{
		MCI_OPEN_PARMS mciOpenParms;
		//midi 파일은 sequencer wav는 waveaudio mp3는 mpegvideo
		mciOpenParms.lpstrDeviceType = "waveaudio";
		mciOpenParms.lpstrElementName = m_strSoundFile.c_str();

		DWORD dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms);

		if (dwReturn)
		{
			//에러가 나면 이곳으로 온다.
			DLOG("Sound Error %d", dwReturn);
		}

		// The device opened successfully; get the device ID.
		m_DeviceID = mciOpenParms.wDeviceID;
	}
	else
	{
		MCI_OPEN_PARMS mciOpenParms;
		//midi 파일은 sequencer wav는 waveaudio mp3는 mpegvideo
		mciOpenParms.lpstrDeviceType = "mpegvideo";
		mciOpenParms.lpstrElementName = m_strSoundFile.c_str();

		DWORD dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID)&mciOpenParms);

		if (dwReturn)
		{
			//에러가 나면 이곳으로 온다.
			DLOG("Sound Error %d", dwReturn);
		}

		// The device opened successfully; get the device ID.
		m_DeviceID = mciOpenParms.wDeviceID;
	}
	
}

void Sound::Update(GameEngine* gameEngine)
{

}

void Sound::Draw(GameEngine* gameEngine)
{

}

void Sound::CleanUp(GameEngine* gameEngine)
{
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, NULL);
}

//BGM은 m_bVolumeOn상관없이 Play된다.
void Sound::Play()
{	
	if (m_bEffect == true && m_bVolumeOn == true)
	{
		DWORD dwReturn;
		//처음 위치로 되돌린다.	
		dwReturn = mciSendCommand(m_DeviceID, MCI_SEEK, MCI_SEEK_TO_START, NULL);
		if (dwReturn)
		{
			mciSendCommand(m_DeviceID, MCI_CLOSE, 0, NULL);
			//에러가 나면 이곳으로 온다.
			DLOG("Sound Error %d", dwReturn);
		}
		else
		{
			dwReturn = mciSendCommand(m_DeviceID, MCI_PLAY, NULL, NULL);
			if (dwReturn)
			{
				mciSendCommand(m_DeviceID, MCI_CLOSE, 0, NULL);
				//에러가 나면 이곳으로 온다.
				DLOG("Sound Error %d", dwReturn);
			}
		}		
	}
	else if(m_bEffect == false)
	{
		MCI_PLAY_PARMS mciPlayParms;
		// 알림을 받을 윈도우 핸들을 설정합니다.
		mciPlayParms.dwCallback = (DWORD_PTR)m_hWnd;
		DWORD dwReturn = mciSendCommand(m_DeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&mciPlayParms);
		if (dwReturn)
		{
			mciSendCommand(m_DeviceID, MCI_CLOSE, 0, NULL);
			//에러가 나면 이곳으로 온다.
			DLOG("Sound Error %d", dwReturn);
		}
	}
	
}

void Sound::VolumeOnOff(bool bOn)
{
	if (m_bEffect == true)
	{
		m_bVolumeOn = bOn;
	}
	else
	{
		MCI_SET_PARMS setParms;
		ZeroMemory(&setParms, sizeof(setParms));

		MCIERROR err;
		if (bOn == true)
		{
			err = mciSendCommand(m_DeviceID, MCI_SET, MCI_SET_AUDIO | MCI_SET_ON, (DWORD_PTR)&setParms);
		}
		else
		{
			err = mciSendCommand(m_DeviceID, MCI_SET, MCI_SET_AUDIO | MCI_SET_OFF, (DWORD_PTR)&setParms);
		}

		if (err != 0)
		{
			char errMsg[256];
			mciGetErrorString(err, errMsg, sizeof(errMsg));
			printf("Audio Error: %s\n", errMsg);
		}
		m_bVolumeOn = bOn;
	}
}

void Sound::Stop()
{
	mciSendCommand(m_DeviceID, MCI_CLOSE, 0, NULL);
}
