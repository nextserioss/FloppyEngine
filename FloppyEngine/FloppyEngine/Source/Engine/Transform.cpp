#include "../../Header/Engine/Transform.h"
#include "../../Header/Engine/Renderer.h"
#include "../../Header/Engine/GameEngineMainLoop.h"
#include "../../Header/Engine/Composite.h"
#include "../../Header/Engine/GameState.h"
#include "../../Header/Engine/LogManager.h"

Transform::Transform()
{
	DLOG("Transform");
	m_pfnTween[TweenArg::TYPE_MOVE] = &Transform::MoveTween;
	m_pfnTween[TweenArg::TYPE_MOVE_X] = &Transform::MoveXTween;
	m_pfnTween[TweenArg::TYPE_MOVE_Y] = &Transform::MoveYTween;
	m_pfnTween[TweenArg::TYPE_SCALE] = &Transform::ScaleTween;
	m_pfnTween[TweenArg::TYPE_FRAME] = &Transform::FrameTween;
	m_pfnTween[TweenArg::TYPE_DELAY] = &Transform::DelayTween;
	m_pfnTween[TweenArg::TYPE_END_CALLBK] = &Transform::EndCallTween;
	m_pfnTween[TweenArg::TYPE_FRAME_END_CALLBK] = &Transform::EndCallFrame;

	m_pfnEase[TweenArg::TYPE_LINEAR] = &Transform::LinearTween;
	m_pfnEase[TweenArg::TYPE_EASE_IN_QUAD] = &Transform::EaseInQuad;
	m_pfnEase[TweenArg::TYPE_EASE_OUT_QUAD] = &Transform::EaseOutQuad;
	m_pfnEase[TweenArg::TYPE_EASE_INOUT_QUAD] = &Transform::EaseInOutQuad;
	m_pfnEase[TweenArg::TYPE_EASE_IN_CUBIC] = &Transform::EaseInCubic;
	m_pfnEase[TweenArg::TYPE_EASE_OUT_CUBIC] = &Transform::EaseOutCubic;
	m_pfnEase[TweenArg::TYPE_EASE_INOUT_CUBIC] = &Transform::EaseInOutCubic;
	m_pfnEase[TweenArg::TYPE_EASE_IN_QUART] = &Transform::EaseInQuart;
	m_pfnEase[TweenArg::TYPE_EASE_OUT_QUART] = &Transform::EaseOutQuart;
	m_pfnEase[TweenArg::TYPE_EASE_INOUT_QUART] = &Transform::EaseInOutQuart;
	m_pfnEase[TweenArg::TYPE_EASE_IN_SINE] = &Transform::EaseInSine;
	m_pfnEase[TweenArg::TYPE_EASE_OUT_SINE] = &Transform::EaseOutSine;
	m_pfnEase[TweenArg::TYPE_EASE_INOUT_SINE] = &Transform::EaseInOutSine;
	m_pfnEase[TweenArg::TYPE_EASE_IN_EXPO] = &Transform::EaseInExpo;
	m_pfnEase[TweenArg::TYPE_EASE_OUT_EXPO] = &Transform::EaseOutExpo;
	m_pfnEase[TweenArg::TYPE_EASE_INOUT_EXPO] = &Transform::EaseInOutExpo;
	m_pfnEase[TweenArg::TYPE_EASE_IN_CIRC] = &Transform::EaseInCirc;
	m_pfnEase[TweenArg::TYPE_EASE_OUT_CIRC] = &Transform::EaseOutCirc;
	m_pfnEase[TweenArg::TYPE_EASE_INOUT_CIRC] = &Transform::EaseInOutCirc;
}

void Transform::Add(GameEngine* gameEngine, Composite* composite)
{
	m_pComposite = composite;
}

void Transform::Update(GameEngine* gameEngine)
{
	if (m_bTweenPlay == true)
	{
		TweenArg arg = m_TweenSequnce.front();
		(this->*m_pfnTween[arg.m_nType])(arg);
	}
	if (m_bFrameTweenPlay == true)
	{
		TweenArg arg = m_FrameSequnce.front();
		(this->*m_pfnTween[arg.m_nType])(arg);
	}
}

void Transform::Draw(GameEngine* gameEngine)
{

}

void Transform::CleanUp(GameEngine* gameEngine)
{

}

void Transform::MoveTween(TweenArg arg)
{
	m_fTimeElapsed += ENGINE_MAIN->m_fTimeDelta;

	//EndX가 StartX보다 더 클 경우 arg.m_fStartX - fValueX이고 EndX가 StartX보다 더 작을경우 arg.m_fStartX + fValueX이다.
	int nValueReverseX = 1;
	if (arg.m_fStartX - arg.m_fEndX > 0)
	{
		nValueReverseX = -1;
	}
	float fValueX = (this->*m_pfnEase[arg.m_nEaseType])(m_fTimeElapsed, 0.0f, abs(arg.m_fStartX - arg.m_fEndX), arg.m_fDurationSecond);
	m_fX = arg.m_fStartX + (fValueX * nValueReverseX);
	
	int nValueReverseY = 1;
	if (arg.m_fStartY - arg.m_fEndY > 0)
	{
		nValueReverseY = -1;
	}
	float fValueY = (this->*m_pfnEase[arg.m_nEaseType])(m_fTimeElapsed, 0.0f, abs(arg.m_fStartY - arg.m_fEndY), arg.m_fDurationSecond);
	m_fY = arg.m_fStartY + (fValueY * nValueReverseY);
	

	if (m_fTimeElapsed >= arg.m_fDurationSecond)
	{
		m_fX = arg.m_fEndX;
		m_fY = arg.m_fEndY;
		AnimationNext();
	}
}

void Transform::MoveXTween(TweenArg arg)
{
	m_fTimeElapsed += ENGINE_MAIN->m_fTimeDelta;

	//EndX가 StartX보다 더 클 경우 arg.m_fStartX - fValueX이고 EndX가 StartX보다 더 작을경우 arg.m_fStartX + fValueX이다.
	int nValueReverseX = 1;
	if (arg.m_fStartX - arg.m_fEndX > 0)
	{
		nValueReverseX = -1;
	}
	float fValueX = (this->*m_pfnEase[arg.m_nEaseType])(m_fTimeElapsed, 0.0f, abs(arg.m_fStartX - arg.m_fEndX), arg.m_fDurationSecond);
	m_fX = arg.m_fStartX + (fValueX * nValueReverseX);

	if (m_fTimeElapsed >= arg.m_fDurationSecond)
	{
		m_fX = arg.m_fEndX;
		AnimationNext();
	}
}

void Transform::MoveYTween(TweenArg arg)
{
	m_fTimeElapsed += ENGINE_MAIN->m_fTimeDelta;

	int nValueReverseY = 1;
	if (arg.m_fStartY - arg.m_fEndY > 0)
	{
		nValueReverseY = -1;
	}
	float fValueY = (this->*m_pfnEase[arg.m_nEaseType])(m_fTimeElapsed, 0.0f, abs(arg.m_fStartY - arg.m_fEndY), arg.m_fDurationSecond);
	m_fY = arg.m_fStartY + (fValueY * nValueReverseY);

	if (m_fTimeElapsed >= arg.m_fDurationSecond)
	{
		m_fY = arg.m_fEndY;
		AnimationNext();
	}
}

void Transform::ScaleTween(TweenArg arg)
{
	m_fTimeElapsed += ENGINE_MAIN->m_fTimeDelta;

	//EndX가 StartX보다 더 클 경우 arg.m_fStartX - fValueX이고 EndX가 StartX보다 더 작을경우 arg.m_fStartX + fValueX이다.
	int nValueReverseX = 1;
	if (arg.m_fStartX - arg.m_fEndX > 0)
	{
		nValueReverseX = -1;
	}
	float fValueX = (this->*m_pfnEase[arg.m_nEaseType])(m_fTimeElapsed, 0.0f, abs(arg.m_fStartX - arg.m_fEndX), arg.m_fDurationSecond);
	m_fScaleX = arg.m_fStartX + (fValueX * nValueReverseX);

	int nValueReverseY = 1;
	if (arg.m_fStartY - arg.m_fEndY > 0)
	{
		nValueReverseY = -1;
	}
	float fValueY = (this->*m_pfnEase[arg.m_nEaseType])(m_fTimeElapsed, 0.0f, abs(arg.m_fStartY - arg.m_fEndY), arg.m_fDurationSecond);
	m_fScaleY = arg.m_fStartY + (fValueY * nValueReverseY);

	if (m_fTimeElapsed >= arg.m_fDurationSecond)
	{
		m_fScaleX = arg.m_fEndX;
		m_fScaleY = arg.m_fEndY;
		AnimationNext();
	}
}

void Transform::FrameTween(TweenArg arg)
{
	m_fFrameTimeElapsed += ENGINE_MAIN->m_fTimeDelta;
	//최초 진입이면 최대값을 주어서 맨 처음 Sprite를 일단 보여준다. 
	if (m_nFrameIndex == 0)
	{
		m_fFrameTimeElapsed = MAX_TIME_ELAPSED;
	}
	if (m_fFrameTimeElapsed >= arg.m_fInterval)
	{
		if (m_nFrameIndex >= arg.m_frameImage.size())
		{
			m_nFrameIndex = 0;
			FrameAnimationNext();
			return;
		}
		Renderer* renderer = (Renderer*)m_pComposite->FindComponent(COMPONENT_NAME[RENDERER]);
		//DLOG("%s", arg.m_frameImage[m_nFrameIndex].c_str());
		renderer->ChangeBitmap(arg.m_frameImage[m_nFrameIndex]);
		m_fFrameTimeElapsed = 0.0f;
		m_nFrameIndex++;		
	}
}

void Transform::DelayTween(TweenArg arg)
{
	m_fTimeElapsed += ENGINE_MAIN->m_fTimeDelta;
	if (m_fTimeElapsed >= arg.m_fDurationSecond)
	{
		AnimationNext();
	}
}

void Transform::EndCallTween(TweenArg arg)
{
	m_pState->AnimationEnd(arg.m_strTweenName);
	AnimationNext();
}

void Transform::EndCallFrame(TweenArg arg)
{
	m_pState->AnimationEnd(arg.m_strTweenName);
	FrameAnimationNext();
}

void Transform::AnimationNext()
{
	m_fTimeElapsed = 0.0f;
	if (m_bForever == true)
	{
		TweenArg arg = m_TweenSequnce.front();
		m_TweenSequnce.push(arg);
	}	
	m_TweenSequnce.pop();
	
	if (m_TweenSequnce.empty() == true)
	{
		m_bTweenPlay = false;
	}
}

void Transform::FrameAnimationNext()
{
	m_fFrameTimeElapsed = 0.0f;
	if (m_bFrameForever == true)
	{
		TweenArg arg = m_FrameSequnce.front();
		m_FrameSequnce.push(arg);
	}

	m_FrameSequnce.pop();

	if (m_FrameSequnce.empty() == true)
	{
		m_bFrameTweenPlay = false;
	}
}

//fCurrentTime(밀리세컨드) 애니메이션이 시작하고 지난 시간 fStart 시작 지점 항상 0이다. fChange 차이값 fDurarion(밀리세컨드) 애니메이션 지속(종료) 시간
//주의 사항 fChange가 항상 fStart보다 절대값으로 커야 한다.
float Transform::LinearTween(float fCurrentTime, float fStart, float fChange, float fDurarion) {	
	return fChange * fCurrentTime / fDurarion + fStart;
}
float Transform::EaseInQuad(float fCurrentTime, float fStart, float fChange, float fDurarion) {
	fCurrentTime /= fDurarion;
	return fChange*fCurrentTime*fCurrentTime + fStart;
}
float Transform::EaseOutQuad(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion;
	return -fChange * fCurrentTime*(fCurrentTime - 2) + fStart;
}
float Transform::EaseInOutQuad(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion / 2;
	if (fCurrentTime < 1) return fChange / 2 * fCurrentTime*fCurrentTime + fStart;
	fCurrentTime--;
	return -fChange / 2 * (fCurrentTime*(fCurrentTime - 2) - 1) + fStart;
}
float Transform::EaseInCubic(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion;
	return fChange*fCurrentTime*fCurrentTime*fCurrentTime + fStart;
}
float Transform::EaseOutCubic(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion;
	fCurrentTime--;
	return fChange*(fCurrentTime*fCurrentTime*fCurrentTime + 1) + fStart;
}
float Transform::EaseInOutCubic(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion / 2;
	if (fCurrentTime < 1) return fChange / 2 * fCurrentTime*fCurrentTime*fCurrentTime + fStart;
	fCurrentTime -= 2;
	return fChange / 2 * (fCurrentTime*fCurrentTime*fCurrentTime + 2) + fStart;
}
float Transform::EaseInQuart(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion;
	return fChange*fCurrentTime*fCurrentTime*fCurrentTime*fCurrentTime + fStart;
}
float Transform::EaseOutQuart(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion;
	fCurrentTime--;
	return -fChange * (fCurrentTime*fCurrentTime*fCurrentTime*fCurrentTime - 1) + fStart;
}
float Transform::EaseInOutQuart(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion / 2;
	if (fCurrentTime < 1) return fChange / 2 * fCurrentTime*fCurrentTime*fCurrentTime*fCurrentTime + fStart;
	fCurrentTime -= 2;
	return -fChange / 2 * (fCurrentTime*fCurrentTime*fCurrentTime*fCurrentTime - 2) + fStart;
}
float Transform::EaseInSine(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	return (float)(-fChange * cos(fCurrentTime / fDurarion * (M_PI / 2)) + fChange + fStart);
}
float Transform::EaseOutSine(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	return (float)(fChange * sin(fCurrentTime / fDurarion * (M_PI / 2)) + fStart);
}
float Transform::EaseInOutSine(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	return (float)(-fChange / 2 * (cos(M_PI*fCurrentTime / fDurarion) - 1) + fStart);
}
float Transform::EaseInExpo(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	return (float)(fChange * pow(2, 10 * (fCurrentTime / fDurarion - 1)) + fStart);
}
float Transform::EaseOutExpo(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	return (float)(fChange * (-pow(2, -10 * fCurrentTime / fDurarion) + 1) + fStart);
}
float Transform::EaseInOutExpo(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion / 2;
	if (fCurrentTime < 1) return (float)(fChange / 2 * pow(2, 10 * (fCurrentTime - 1)) + fStart);
	fCurrentTime--;
	return (float)(fChange / 2 * (-pow(2, -10 * fCurrentTime) + 2) + fStart);
}
float Transform::EaseInCirc(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion;
	return -fChange * (sqrt(1 - fCurrentTime*fCurrentTime) - 1) + fStart;
}
float Transform::EaseOutCirc(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion;
	fCurrentTime--;
	return fChange * sqrt(1 - fCurrentTime*fCurrentTime) + fStart;
}
float Transform::EaseInOutCirc(float fCurrentTime, float fStart, float fChange, float fDurarion)
{
	fCurrentTime /= fDurarion / 2;
	if (fCurrentTime < 1) return -fChange / 2 * (sqrt(1 - fCurrentTime*fCurrentTime) - 1) + fStart;
	fCurrentTime -= 2;
	return fChange / 2 * (sqrt(1 - fCurrentTime*fCurrentTime) + 1) + fStart;
}

void Transform::TweenMove(int nEaseType, float fStartX, float fStartY, float fEndX, float fEndY, float fDurationSecond)
{	
	TweenArg arg;
	arg.m_nType = TweenArg::TYPE_MOVE;
	arg.m_nEaseType = nEaseType;
	arg.m_fStartX = fStartX;
	arg.m_fStartY = fStartY;
	arg.m_fEndX = fEndX;
	arg.m_fEndY = fEndY;
	arg.m_fDurationSecond = fDurationSecond;

	m_TweenSequnce.push(arg);
}

void Transform::TweenMoveX(int nEaseType, float fStartX, float fEndX, float fDurationSecond)
{
	TweenArg arg;
	arg.m_nType = TweenArg::TYPE_MOVE_X;
	arg.m_nEaseType = nEaseType;
	arg.m_fStartX = fStartX;
	arg.m_fEndX = fEndX;
	arg.m_fDurationSecond = fDurationSecond;

	m_TweenSequnce.push(arg);
}

void Transform::TweenMoveY(int nEaseType, float fStartY, float fEndY, float fDurationSecond)
{
	TweenArg arg;
	arg.m_nType = TweenArg::TYPE_MOVE_Y;
	arg.m_nEaseType = nEaseType;
	arg.m_fStartY = fStartY;
	arg.m_fEndY = fEndY;
	arg.m_fDurationSecond = fDurationSecond;

	m_TweenSequnce.push(arg);
}

void Transform::TweenScale(int nEaseType, float fStartX, float fStartY, float fEndX, float fEndY, float fDurationSecond)
{
	TweenArg arg;
	arg.m_nType = TweenArg::TYPE_SCALE;
	arg.m_nEaseType = nEaseType;
	arg.m_fStartX = fStartX;
	arg.m_fStartY = fStartY;
	arg.m_fEndX = fEndX;
	arg.m_fEndY = fEndY;
	arg.m_fDurationSecond = fDurationSecond;

	m_TweenSequnce.push(arg);
}

void Transform::TweenFrame(vector<string> frameImage, float fInterval)
{
	TweenArg arg;
	arg.m_nType = TweenArg::TYPE_FRAME;
	arg.m_fInterval = fInterval;
	arg.m_frameImage = frameImage;	
	m_FrameSequnce.push(arg);
}

void Transform::FrameEndCallback(GameState* pState, string strAniName)
{
	m_pState = pState;
	TweenArg arg;
	arg.m_nType = TweenArg::TYPE_FRAME_END_CALLBK;
	arg.m_strTweenName = strAniName;
	m_FrameSequnce.push(arg);
}

void Transform::TweenDelay(float fDurationSecond)
{
	TweenArg arg;
	arg.m_nType = TweenArg::TYPE_DELAY;
	arg.m_fDurationSecond = fDurationSecond;
	m_TweenSequnce.push(arg);
}

void Transform::TweenEndCallback(GameState* pState, string strAniName)
{
	m_pState = pState;
	TweenArg arg;
	arg.m_nType = TweenArg::TYPE_END_CALLBK;	
	arg.m_strTweenName = strAniName;
	m_TweenSequnce.push(arg);
}

void Transform::TweenPlay()
{
	m_fTimeElapsed = 0.0f;
	m_bTweenPlay = true;
	m_bForever = false;
}

void Transform::TweenPlayForever()
{
	m_fTimeElapsed = 0.0f;
	m_bTweenPlay = true;
	m_bForever = true;
}

void Transform::TweenStop()
{
	m_bTweenPlay = false;

	while (!m_TweenSequnce.empty())
	{
		m_TweenSequnce.pop();
	}
}

void Transform::FrameTweenPlay()
{
	m_nFrameIndex = 0;
	m_fFrameTimeElapsed = 0.0f;
	m_bFrameTweenPlay = true;
	m_bFrameForever = false;
}

void Transform::FrameTweenPlayForever()
{
	m_nFrameIndex = 0;
	m_fFrameTimeElapsed = 0.0f;
	m_bFrameTweenPlay = true;
	m_bFrameForever = true;
}

void Transform::FrameTweenStop()
{
	m_bFrameTweenPlay = false;

	while (!m_FrameSequnce.empty())
	{
		m_FrameSequnce.pop();
	}
}

bool Transform::IsTween()
{
	return m_bTweenPlay;
}

bool Transform::IsFrameTween()
{
	return m_bFrameTweenPlay;
}