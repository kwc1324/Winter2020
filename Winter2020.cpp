#include "pch.h"
#include "framework.h"
#include "Winter2020.h"

// 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 응용프로그램에 VM_QUIT메시지를 전달해서 프로그램 종료
	if (uMsg == WM_DESTROY) PostQuitMessage(0);
	else if (uMsg == WM_CLOSE) // x누르면 발생하는 메시지
	{
		// 메시지박스 - return은 OK or CANCEL
		int check = MessageBox(hWnd, L"프로그램을 종료하시겠습니까?"
			, L"Panorama", MB_ICONQUESTION | MB_OKCANCEL);
		if (IDCANCEL == check) return 0;
	}

	if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(hWnd, &ps);
		//-------------------------------------------
		
		HFONT h_font = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"굴림체");
	
		SetTextColor(h_dc, RGB(255, 255, 255)); // 폰트 색
		SetBkMode(h_dc, TRANSPARENT); // 배경색이랑 폰트 배경색 맞추기
		HGDIOBJ h_old_font = SelectObject(h_dc, h_font); // 폰트 결정
		RECT r = { 10, 10, 370, 50 }; // 사각형 위치, 크기
		FillRect(h_dc, &r, (HBRUSH)GetStockObject(DKGRAY_BRUSH)); // 사각형 채우기
		// 사각형 맞춰서 문자열 쓰기
		DrawText(h_dc, L"노인복지관 명단", 8, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER );
		SelectObject(h_dc, h_old_font); // 폰트 설정 끝
		
		SetTextColor(h_dc, RGB(0, 0, 0)); // 폰트 컬러
		TextOut(h_dc, 10, 80, L"안녕안녕", 4);

		//-------------------------------------------
	
		DeleteObject(h_font);
		EndPaint(hWnd, &ps);
		return 0;
	}
	
	// 내가 처리하지 않은 메시지들의 기본 작업 대신 처리
	return DefWindowProc(hWnd, uMsg, wParam, lParam);

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmpLine, int nCmdShow)
{
	// 윈도우 클래스 등록
	WNDCLASS wc;

	wchar_t my_class_name[] = L"Panorama";
	wc.cbClsExtra = NULL; // window class를 위한 추가메모리 사용x
	wc.cbWndExtra = NULL; // window 개별메모리 사용x
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // 배경색
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 마우스 커서
	wc.hIcon = LoadIcon(NULL, IDI_INFORMATION); // 제목 표시줄 로고
	wc.hInstance = hInstance; // 첫 번째 매개변수 값 대입
	wc.lpfnWndProc = WndProc; // 기본 메시지 처리 함수 지정
	wc.lpszClassName = my_class_name; // 'Window class' 이름 지정
	wc.lpszMenuName = L"NULL"; // 메뉴바 사용x
	wc.style = CS_HREDRAW | CS_VREDRAW; // 윈도우 그리기 항목 설정

	RegisterClass(&wc); // wc 정보 기준으로 'Window class' 등록

	// 윈도우 생성
	HWND hWnd = CreateWindow(my_class_name, L"Panorama",
		WS_OVERLAPPEDWINDOW, 550, 250, 400, 350, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 프로그램에 전달된 메시지를 번역/실행
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// 추가 메시지 발생
		TranslateMessage(&msg);
		// WndProc함수 호출
		DispatchMessage(&msg);
	}

	return msg.wParam;
}