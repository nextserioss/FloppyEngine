#include "../../Header/Engine/GameEngineMainLoop.h"
#include "../../Header/Engine/GameEngine.h"
#include "../../Header/Engine/LogManager.h"

GameEngineMainLoop* GameEngineMainLoop::instance = NULL;

// 창 프로시저 (Window Procedure) 함수 선언
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int GameEngineMainLoop::EngineStart(string strGamename, GameState* state, int nScreenWidth, int nScreenHeight, HINSTANCE hInstance, int nCmdShow)
{	
#ifdef _DEBUG
    // 새 콘솔 할당
    if (AllocConsole())
    {
        FILE* pFile;
        // stdout 스트림을 새 콘솔 창의 출력 버퍼에 연결
        if (freopen_s(&pFile, "CONOUT$", "wt", stdout) == 0)
        {
            DLOG("--- 디버그 콘솔 창이 성공적으로 할당되었습니다 ---");
            DLOG("GUI 창이 곧 표시됩니다.");
        }
        else
        {
            // 연결 실패 시 처리
            FreeConsole();
        }

        // 1. 콘솔 입력 핸들을 얻습니다.
        HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
        DWORD dwMode;
        // 2. 현재 콘솔 모드를 가져옵니다.
        if (GetConsoleMode(hConsoleIn, &dwMode))
        {
            // 3. QuickEdit 모드 플래그 (및 Insert 모드)를 제거합니다.
            // QuickEdit 모드를 비활성화하면 마우스 드래그로 인한 창 멈춤을 방지합니다.
            dwMode &= ~(ENABLE_QUICK_EDIT_MODE | ENABLE_INSERT_MODE);

            // 4. 수정된 모드를 설정합니다.
            if (SetConsoleMode(hConsoleIn, dwMode))
            {
                DLOG("QuickEdit 모드 비활성화 완료. 마우스 드래그로 인한 정지 현상을 방지합니다.");
            }
        }

        // 콘솔 창 제목 설정
        SetConsoleTitle(TEXT("Debug Log Console"));
    }    
#endif
    // 1. 윈도우 클래스 정의 및 등록
    const char* szClassName = strGamename.c_str();
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // 단계 2: 창 클래스 정의
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc; // 창 프로시저 지정
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 배경색
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // 단계 3: 창 클래스 등록
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    DLOG("Win32 창 클래스 등록 성공");

    RECT rc = { 0, 0, nScreenWidth, nScreenHeight };
    // 사용할 스타일 지정
    // 크기 조절(WS_THICKFRAME)과 최대화 버튼(WS_MAXIMIZEBOX) 제거
    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
    DWORD dwExStyle = WS_EX_CLIENTEDGE;
    BOOL hasMenu = FALSE;

    // 클라이언트 크기를 만족시키기 위한 창 전체 크기 계산
    AdjustWindowRectEx(&rc, dwStyle, hasMenu, dwExStyle);

    // 계산된 너비와 높이
    int ActualWindowWidth = rc.right - rc.left;
    int ActualWindowHeight = rc.bottom - rc.top;

    // 단계 4: 창 생성
    hwnd = CreateWindowEx(dwExStyle, szClassName, strGamename.c_str(), dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, ActualWindowWidth, ActualWindowHeight, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    // 단계 5: 창 표시
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //게임 엔진 초기화
    GameEngine game;
    game.Init(hwnd, nScreenWidth, nScreenHeight);
    game.ChangeState(state);

    //게임 루프 관련 변수
    float fFPS = 0.0f;

    unsigned long nFrameCnt = 0;
    float fTimeElapsed = 0;
    unsigned long nCurrTime;
    unsigned long nLastTime = GetTickCount();

	//타이머 정밀도 향상
    timeBeginPeriod(1);

    // 6. 메시지 루프
    // Msg 구조체는 이미 선언되어 있다고 가정합니다. (MSG Msg;)
    while (TRUE) // 프로그램이 종료될 때까지 무한 루프
    {
        // PeekMessage 호출: 메시지 큐를 확인합니다.
        // PM_REMOVE: 메시지가 있으면 큐에서 제거합니다.
        if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            // 종료 메시지(WM_QUIT)를 받았는지 확인
            if (Msg.message == WM_QUIT)
            {
                DLOG("WM_QUIT 메시지 수신: 메시지 루프를 종료합니다.");
                break; // 루프를 빠져나가 프로그램 종료
            }

            // 메시지 번역 및 창 프로시저로 전달
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        else
        {
            nCurrTime = GetTickCount();
            //* 0.001f 초단위로 만든다.
            m_fTimeDelta = (nCurrTime - nLastTime) * 0.001f;
            //정의된 FPS보다 많이 나오면 (속도가 빠르면) 휴식을 취한다.
            if (m_fTimeDelta < (1.0f / m_nTargetFps))
            {
                //* 1000으로 밀리세컨드로 만든다.
                float fSleepTime = ((1.0f / m_nTargetFps) - m_fTimeDelta) * 1000;
                Sleep((DWORD)fSleepTime);

				//쉬고 난뒤 다시 시간 측정
                nCurrTime = GetTickCount();
                m_fTimeDelta = (nCurrTime - nLastTime) * 0.001f;
            }
                
            //게임이 동작중이면 업데이트 로직을 동작시켜준다.
            if (game.Running())
            {
                game.HandleEvents();
                game.Update();
                game.Draw();
            }
            else
            {
                break;
            }
            
            nFrameCnt++;
            fTimeElapsed += m_fTimeDelta;

			//1초가 지났으면 FPS를 계산하여 출력
            if (fTimeElapsed >= 1.0f)
            {
                fFPS = (float)nFrameCnt / fTimeElapsed;
                //DLOG("FPS %f", fFPS);
                fTimeElapsed = 0.0f;
                nFrameCnt = 0;
            }

            nLastTime = nCurrTime;
        }
    }

	//타이머 정밀도 복원
    timeEndPeriod(1);

#ifdef _DEBUG
    // 프로그램 종료 직전에 콘솔 해제 (선택 사항이지만 권장)
    FreeConsole();
#endif

    return Msg.wParam;
}


// 창 프로시저 정의
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        // 창이 생성될 때 호출됩니다.
        DLOG("WM_CREATE 메시지 수신: 창이 만들어졌습니다.");
        break;
    case MM_MCINOTIFY:
        // wParam: 알림 코드 (MCI_NOTIFY_SUCCESS = 0x01, MCI_NOTIFY_FAILURE 등)
        // lParam: 메시지를 보낸 MCI 장치의 ID (wDeviceID)
        if (LOWORD(wParam) == 0x01)
        {
            DWORD dwLoopReturn;
            // MIDI 재생이 성공적으로 완료되었습니다. (파일 끝에 도달)
			//Seek to start로 되돌아가서 다시 재생합니다.
            dwLoopReturn = mciSendCommand((UINT)lParam, MCI_SEEK, MCI_SEEK_TO_START, NULL);
            if (dwLoopReturn)
            {
                mciSendCommand((UINT)lParam, MCI_CLOSE, 0, NULL);
                DLOG("Error MM_MCINOTIFY %d", dwLoopReturn);
            }
            else
            {
                // 다시 MCI_PLAY 명령을 보냅니다.
                MCI_PLAY_PARMS loopParms;
                loopParms.dwCallback = (DWORD_PTR)hwnd; // 다시 알림을 요청

                // MCI_SEEK 명령을 사용하여 시작점으로 되돌아갈 수도 있지만,
                // MCI_PLAY 명령만으로도 보통 자동으로 시작부터 재생됩니다.
                dwLoopReturn = mciSendCommand((UINT)lParam, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&loopParms);

                if (dwLoopReturn)
                {
                    mciSendCommand((UINT)lParam, MCI_CLOSE, 0, NULL);
                    DLOG("Error MM_MCINOTIFY %d", dwLoopReturn);
                }
            }
        }
        break;
    case WM_CLOSE:
        // 사용자가 창 닫기 버튼을 누르거나 ALT+F4를 누를 때 호출됩니다.
        DLOG("WM_CLOSE 메시지 수신: 창을 닫으려 합니다.\n");
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        // 창이 완전히 파괴될 때 호출됩니다.
        DLOG("WM_DESTROY 메시지 수신: 프로그램 종료 요청.\n");
        PostQuitMessage(0); // 메시지 루프를 종료하도록 요청
        break;
    default:
        // 처리하지 않는 메시지는 기본 윈도우 프로시저에 맡깁니다.
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}