#include "../../Header/Engine/GameEngine.h"
#include "../../Header/Engine/Composite.h"
#include "../../Header/Engine/Transform.h"
#include "../../Header/Engine/Renderer.h"
#include "../../Header/Engine/Text.h"
#include "../../Header/Engine/BoxCollider.h"
#include "../../Header/Engine/LogManager.h"
#include "../../Header/Engine/Sound.h"
#include <typeinfo>

string Composite::m_sComponentName[] = { string(typeid(Composite).name()),
										string(typeid(Transform).name()),
										string(typeid(Renderer).name()),
										string(typeid(Text).name()),
										string(typeid(BoxCollider).name()),
										string(typeid(Sound).name()) };

Composite* Composite::Create()
{
	return new Composite();
}

Composite* Composite::Create(GameEngine* gameEngine, string id)
{
	return new Composite(gameEngine, id);
}

Composite* Composite::CreateSprite(GameEngine* gameEngine, string id, string strImagePath)
{
	Composite* comp = new Composite(gameEngine, id);
	Renderer* renderer = new Renderer(strImagePath);
	comp->Add(renderer);
	return comp;
}

Composite* Composite::CreateSpriteBox(GameEngine* gameEngine, string id, string strImagePath, int nOffsetXWidth, int nOffsetYHeight)
{
	Composite* comp = new Composite(gameEngine, id);
	Renderer* renderer = new Renderer(strImagePath);
	comp->Add(renderer);
	BoxCollider* boxCollider = new BoxCollider(nOffsetXWidth, nOffsetYHeight, renderer->GetWidth() - nOffsetXWidth, renderer->GetHeight() - nOffsetYHeight);
	comp->Add(boxCollider);
	return comp;
}

Composite* Composite::CreateSpriteBox(GameEngine* gameEngine, string id, string strImagePath, int nOffsetX, int nOffsetY, int nWidth, int nHeight)
{
	Composite* comp = new Composite(gameEngine, id);
	Renderer* renderer = new Renderer(strImagePath);
	comp->Add(renderer);
	BoxCollider* boxCollider = new BoxCollider(nOffsetX, nOffsetY, nWidth, nHeight);
	comp->Add(boxCollider);
	return comp;
}

Composite* Composite::CreateText(GameEngine* gameEngine, string id, string strText)
{
	Composite* comp = new Composite(gameEngine, id);
	Text* text = new Text(strText);
	comp->Add(text);
	return comp;
}

Composite * Composite::CreateBGMSound(GameEngine * gameEngine, string id, string strSoundFile)
{
	Composite* comp = new Composite(gameEngine, id);
	Sound* sound = new Sound(strSoundFile, false);
	comp->Add(sound);
	return comp;
}

Composite* Composite::CreateSound(GameEngine* gameEngine, string id, string strSoundFile)
{
	Composite* comp = new Composite(gameEngine, id);
	Sound* sound = new Sound(strSoundFile, true);
	comp->Add(sound);
	return comp;
}

//컴포지트는 기본적으로 트랜스폼을 하나 가지고 있다.
Composite::Composite()
{
	m_strId = "Default";
	Transform* defaultTransform = new Transform();
	Add(defaultTransform);
}
//컴포지트는 기본적으로 트랜스폼을 하나 가지고 있다.
Composite::Composite(GameEngine* gameEngine, string id)
{
	m_GameEngine = gameEngine;
	m_strId = id;
	Transform* defaultTransform = new Transform();
	Add(defaultTransform);
}

void Composite::SetGameEngine(GameEngine* gameEngine)
{
	m_GameEngine = gameEngine;
}

void Composite::Add(Component* obj)
{
	//Composite는 중복이라도 들어간다.
	if (typeid(*obj) == typeid(Composite))
	{
		m_Components.push_back(obj);
		obj->Add(m_GameEngine, this);
	}
	//Composite를 제외 하고 Component가 중복되지 않으면 들어 갈수 있다.
	else if (FindComponent(string(typeid(*obj).name())) == NULL)
	{
		m_Components.push_back(obj);
		obj->Add(m_GameEngine, this);
	}
	else
	{
		DLOG("Error Duple Comp %s", typeid(*obj).name());
	}
}

void Composite::Add(GameEngine * gameEngine, Composite * composite)
{

}

void Composite::Update(GameEngine* gameEngine)
{
	if (m_bEnable == true)
	{
		for (unsigned int i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->Update(m_GameEngine);
		}
	}	
}

void Composite::Draw(GameEngine* gameEngine)
{
	if (m_bEnable == true)
	{
		for (unsigned int i = 0; i < m_Components.size(); i++)
		{
			m_Components[i]->Draw(m_GameEngine);
		}
	}	
}

void Composite::CleanUp(GameEngine* gameEngine)
{
	for (unsigned int i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->CleanUp(m_GameEngine);
	}	

	RemoveAllComponent();
}

void Composite::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

//가장 먼저 찾은 컴포넌트 하나를 리턴해준다.
Component* Composite::FindComponent(string strComponent)
{	
	for (unsigned int i = 0; i < m_Components.size(); i++)
	{
		//컴포넌트 이름이 같은지 확인
		if (strComponent.compare(typeid(*m_Components[i]).name()) == 0)
		{
			return m_Components[i];
		}
	}	
	return NULL;
}

void Composite::RemoveAllComponent()
{
	for (unsigned int i = 0; i < m_Components.size(); i++)
	{
		delete m_Components[i];
	}
	m_Components.clear();
}