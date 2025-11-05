#include "Header/FloppyEngine.h"
#include "../Header/IntroState.h"

// 프로그램 진입점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return ENGINE_MAIN->EngineStart("Woogeok", new IntroState(), 640, 480, hInstance, nCmdShow);
}