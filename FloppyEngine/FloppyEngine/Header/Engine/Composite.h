#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include "Component.h"
#include <typeinfo>

using namespace std;

class GameEngine;

#define COMPOSITE string(typeid(Composite).name())
#define TRANSFORM string(typeid(Transform).name())
#define RENDERER string(typeid(Renderer).name())
#define TEXTCOMP string(typeid(Text).name())
#define BOXCOLLIDER string(typeid(BoxCollider).name())
#define SOUND string(typeid(Sound).name())


class Composite : public Component
{
public:
	static Composite* Create();
	static Composite* Create(GameEngine* gameEngine, string id);
	static Composite* CreateSprite(GameEngine* gameEngine, string id, string strImagePath);
	//int nOffsetXWidth, int nOffsetYHeight 만큼 콜리전을 줄여준다.
	static Composite* CreateSpriteBox(GameEngine* gameEngine, string id, string strImagePath, int nOffsetXWidth, int nOffsetYHeight);
	static Composite* CreateSpriteBox(GameEngine* gameEngine, string id, string strImagePath, int nOffsetX, int nOffsetY, int nWidth, int nHeight);
	static Composite* CreateText(GameEngine* gameEngine, string id, string strText);
	static Composite* CreateTextColor(GameEngine* gameEngine, string id, string strText, COLORREF color);
	static Composite* CreateTextColorFontSize(GameEngine* gameEngine, string id, string strText, COLORREF color, int size);
	static Composite* CreateBGMSound(GameEngine* gameEngine, string id, string strSoundFile);
	static Composite* CreateSound(GameEngine* gameEngine, string id, string strSoundFile);
private:
	GameEngine* m_GameEngine;
	//컴포넌트는 테이블에 컴포지트는 제외하고 중복 없이 들어간다.
	vector <Component*> m_Components;
	string m_strId;
	bool m_bEnable = true;
public:	
	//주의 디폴트 생성자는 GameState에서 m_MainComposite를 생성할때 만 쓸수 있다.
	Composite();
	Composite(GameEngine* gameEngine, string id);

	void SetGameEngine(GameEngine* gameEngine);
	void Add(Component* obj);
	void Add(GameEngine* gameEngine, Composite* composite);
	void Update(GameEngine* gameEngine);
	void Draw(GameEngine* gameEngine);
	void CleanUp(GameEngine* gameEngine);

	bool IsEnable();
	void SetEnable(bool bEnable);
	//가장 먼저 찾은 컴포넌트 하나를 리턴해준다.
	Component* FindComponent(string strComponent);
	void RemoveAllComponent();
};