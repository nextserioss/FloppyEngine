#include "../Header/FloppyEngine.h"
#include "../Header/Content/IntroState.h"

// 프로그램 진입점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	return ENGINE_MAIN->EngineStart(new IntroState(), 640, 480, hInstance, nCmdShow);
}