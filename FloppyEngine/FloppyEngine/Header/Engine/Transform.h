#pragma once

#include <string>
#include <queue>
#include <math.h>
#include <Windows.h>
#include "Component.h"

using namespace std;

#define M_PI       3.14159265358979323846

class GameState;
class Composite;
class BoxCollider;

class TweenArg
{
public:
	static const int TYPE_LINEAR = 0;
	static const int TYPE_EASE_IN_QUAD = 1;
	static const int TYPE_EASE_OUT_QUAD = 2;
	static const int TYPE_EASE_INOUT_QUAD = 3;
	static const int TYPE_EASE_IN_CUBIC = 4;
	static const int TYPE_EASE_OUT_CUBIC = 5;
	static const int TYPE_EASE_INOUT_CUBIC = 6;
	static const int TYPE_EASE_IN_QUART = 7;
	static const int TYPE_EASE_OUT_QUART = 8;
	static const int TYPE_EASE_INOUT_QUART = 9;
	static const int TYPE_EASE_IN_SINE = 10;
	static const int TYPE_EASE_OUT_SINE = 11;
	static const int TYPE_EASE_INOUT_SINE = 12;
	static const int TYPE_EASE_IN_EXPO = 13;
	static const int TYPE_EASE_OUT_EXPO = 14;
	static const int TYPE_EASE_INOUT_EXPO = 15;
	static const int TYPE_EASE_IN_CIRC = 16;
	static const int TYPE_EASE_OUT_CIRC = 17;
	static const int TYPE_EASE_INOUT_CIRC = 18;

	static const int TYPE_MOVE = 0;
	static const int TYPE_MOVE_X = 1;
	static const int TYPE_MOVE_Y = 2;
	static const int TYPE_MOVE_SCALE = 3;
	static const int TYPE_SCALE = 4;
	static const int TYPE_FRAME = 5;
	static const int TYPE_DELAY = 6;
	static const int TYPE_END_CALLBK = 7;
	static const int TYPE_FRAME_END_CALLBK = 8;

	string m_strTweenName;
	int m_nType;
	int m_nEaseType;
	float m_fStartX;
	float m_fStartY;
	float m_fEndX;
	float m_fEndY;
	float m_fSecondStartX;
	float m_fSecondStartY;
	float m_fSecondEndX;
	float m_fSecondEndY;
	float m_fDurationSecond;
	float m_fInterval;
	vector<string> m_frameImage;
};

#define MAX_TIME_ELAPSED 9999.99f

class Transform : public Component
{
private:
	GameState* m_pState;
	Composite* m_pComposite;
	BoxCollider* m_pBoxCollider = NULL;
public:
	float m_fX = 0;
	float m_fY = 0;
	float m_fScaleX = 1.0f;
	float m_fScaleY = 1.0f;
	bool m_bXFlip = false;
public:
	Transform();
	void Add(GameEngine* gameEngine, Composite* composite);
	void Init(BoxCollider* boxCollider);
	void Update(GameEngine* gameEngine);	
	void Draw(GameEngine* gameEngine);
	void CleanUp(GameEngine* gameEngine);

private:
	bool m_bTweenPlay = false;
	bool m_bForever = false;
	float m_fTimeElapsed;	
	queue<TweenArg> m_TweenSequnce;

	bool m_bFrameTweenPlay = false;
	bool m_bFrameForever = false;
	float m_fFrameTimeElapsed;
	int m_nFrameIndex = 0;
	queue<TweenArg> m_FrameSequnce;

	typedef void (Transform::*TWEEN_FUNC)(TweenArg);
	TWEEN_FUNC m_pfnTween[9];
	void MoveTween(TweenArg arg);
	void MoveXTween(TweenArg arg);
	void MoveYTween(TweenArg arg);
	void MoveScaleTween(TweenArg arg);
	void ScaleTween(TweenArg arg);
	void FrameTween(TweenArg arg);
	void DelayTween(TweenArg arg);
	void EndCallTween(TweenArg arg);
	void EndCallFrame(TweenArg arg);

	void AnimationNext();
	void FrameAnimationNext();

	typedef float (Transform::*EASE_FUNC)(float, float, float, float);
	EASE_FUNC m_pfnEase[19];
	float LinearTween(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInQuad(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseOutQuad(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInOutQuad(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInCubic(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseOutCubic(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInOutCubic(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInQuart(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseOutQuart(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInOutQuart(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInSine(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseOutSine(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInOutSine(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInExpo(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseOutExpo(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInOutExpo(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInCirc(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseOutCirc(float fCurrentTime, float fStart, float fChange, float fDurarion);
	float EaseInOutCirc(float fCurrentTime, float fStart, float fChange, float fDurarion);

public:
	void TweenMove(int nEaseType, float fStartX, float fStartY, float fEndX, float fEndY, float fDurationSecond);
	void TweenMoveX(int nEaseType, float fStartX, float fEndX, float fDurationSecond);
	void TweenMoveY(int nEaseType, float fStartY, float fEndY, float fDurationSecond);
	void TweenMoveScale(int nEaseType, float fStartX, float fStartY, float fEndX, float fEndY, float fSecondStartX, float fSecondStartY, float fSecondEndX, float fSecondEndY, float fDurationSecond);
	void TweenScale(int nEaseType, float fStartX, float fStartY, float fEndX, float fEndY, float fDurationSecond);
	void TweenFrame(vector<string> frameImage, float fInterval);
	void FrameEndCallback(GameState* pState, string strAniName);
	void TweenDelay(float fDurationSecond);
	void TweenEndCallback(GameState* pState, string strAniName);
	void TweenPlay();
	void TweenPlayForever();
	void TweenStop();
	void FrameTweenPlay();
	void FrameTweenPlayForever();
	void FrameTweenStop();
	bool IsTween();
	bool IsFrameTween();
	void SetXY(float x, float y);
	void SetX(float x);
	void SetY(float y);
};