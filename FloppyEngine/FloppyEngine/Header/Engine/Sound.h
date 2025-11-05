#pragma once

#include <string>
#include <Windows.h>
#include "Component.h"

#pragma comment(lib, "winmm.lib")

using namespace std;

class GameEngine;
class Composite;

class Sound : public Component
{
private:
	Composite * m_Composite;
	HWND m_hWnd;
	string m_strSoundFile;
	int m_nResourceID;
	UINT m_DeviceID;
	bool m_bEffect;
	bool m_bVolumeOn;
public:
	Sound();
	Sound(string soundFile, bool bEffect);
	void Add(GameEngine* gameEngine, Composite* composite);
	void Update(GameEngine* gameEngine);
	void Draw(GameEngine* gameEngine);
	void CleanUp(GameEngine* gameEngine);
	void Play();
	void VolumeOnOff(bool bOn);
	void Stop();
};