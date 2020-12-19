#include "pch.h"
#include "framework.h"
#include "Winter2020.h"
#include <stdlib.h>
#include <windows.h>
#include <CommDlg.h> //	OPENFILENAME 안될 때 사용
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4
#define OPEN_FILE_BUTTON 5
#define FIND_BUTTON 6

HMENU hMenu;
HWND hName, hAge, hOut;

class Person
{
public:
	TCHAR name[30];
	TCHAR age[10];
	Person(TCHAR s[], TCHAR a[]) {
		_tcscpy(name, s);
		_tcscpy(age, a);
	}

};

// 버튼 및 Edit 컨트롤
void addControls(HWND hWnd)
{
	CreateWindowW(L"static", L"Name :", WS_VISIBLE | WS_CHILD, 10, 60, 98, 22, hWnd, NULL, NULL, NULL);
	hName = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 60, 60, 98, 22, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Age :", WS_VISIBLE | WS_CHILD, 10, 87, 98, 22, hWnd, NULL, NULL, NULL);
	hAge = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 60, 87, 98, 22, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"Button", L"저장", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 120, 98, 38, hWnd, (HMENU)GENERATE_BUTTON, NULL, NULL);
	CreateWindowW(L"Button", L"조회", WS_VISIBLE | WS_CHILD | WS_BORDER, 113, 120, 98, 38, hWnd, (HMENU)FIND_BUTTON, NULL, NULL);
	CreateWindowW(L"Button", L"파일 찾기", WS_VISIBLE | WS_CHILD | WS_BORDER, 266, 120, 98, 38, hWnd, (HMENU)OPEN_FILE_BUTTON, NULL, NULL);

	hOut = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 100, 200, 300, 200, hWnd, NULL, NULL, NULL);


}

// 메뉴
void addMenus(HWND hWnd)
{

	hMenu = CreateMenu(); // 메뉴를 만듦
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hSubMenu, MF_STRING, NULL, L"SubMenu Item");

	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, L"Open");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");


	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	AppendMenu(hMenu, MF_STRING, NULL, L"Help");
	SetMenu(hWnd, hMenu);
}

// 창 꾸미기
void addPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC h_dc = BeginPaint(hWnd, &ps);

	HFONT h_font = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"굴림체");

	SetTextColor(h_dc, RGB(255, 255, 255)); // 폰트 색
	SetBkMode(h_dc, TRANSPARENT); // 배경색이랑 폰트 배경색 맞추기
	HGDIOBJ h_old_font = SelectObject(h_dc, h_font); // 폰트 결정
	RECT r = { 10, 10, 480, 50 }; // 사각형 위치, 크기
	FillRect(h_dc, &r, (HBRUSH)GetStockObject(DKGRAY_BRUSH)); // 사각형 채우기
	// 사각형 맞춰서 문자열 쓰기
	DrawText(h_dc, L"회원 관리 명단", 8, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	SelectObject(h_dc, h_old_font); // 폰트 설정 끝

	//SetTextColor(h_dc, RGB(0, 0, 0)); // 폰트 컬러
	//TextOut(h_dc, 10, 80, L"안녕안녕", 4);
	DeleteObject(h_font);
	EndPaint(hWnd, &ps);
	return;
}

// 파일 출력
void displayFile(TCHAR* path)
{
	FILE* file;
	file = _tfopen(path, L"rt");

	fseek(file, 0, SEEK_END);
	int _sz = ftell(file);
	rewind(file); // file 맨 앞 가리키게 함

	TCHAR* data = new TCHAR(_sz + 1);
	fgetws(data, _sz, file);
	SetWindowText(hOut, data);

	return;
}

// 파일 찾기 함수
void open_file(HWND hWnd)
{
	OPENFILENAME ofn;
	TCHAR file_name[100];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = L"모든 파일(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;

	if (GetOpenFileName(&ofn))
	{
		SetWindowText(hOut, ofn.lpstrFile); // 파일 경로 출력
		MessageBox(NULL, ofn.lpstrFile, L"Open File", MB_OK); // 파일 찾음 메시지
	}
	displayFile(ofn.lpstrFile);

	return;
}

// 파일 입력
void InsertMemo(HWND hWnd, TCHAR* out, vector<Person> myVector)
{
	TCHAR c = '\n';
	TCHAR d = ',';
	FILE* file;
	file = _tfopen(L"회원 리스트.txt", L"a");
	fseek(file, 0, SEEK_SET);
	fputws(myVector[0].name, file);
	fputwc(d, file);
	fputws(myVector[0].age, file);
	fputwc(c, file);
	fclose(file);

	return;
}

// 정보 조회
bool findInformation(HWND hWnd, TCHAR* find)
{

	FILE* file;
	file = _tfopen(L"회원 리스트.txt", L"rt");

	fseek(file, 0, SEEK_END);
	int _sz = ftell(file);
	rewind(file); // file 맨 앞 가리키게 함

	return true;
}

// txt파일 class 구성 함수
void setClass(vector<Person> myVector)
{
	FILE* file;
	file = _tfopen(L"회원 리스트.txt", L"rt");
	
	while (1)
	{
		TCHAR* tempName = new TCHAR(100);
		TCHAR* tempAge = new TCHAR(10);
		bool pass = false;
		
	}
}

// 메시지 처리 함수
LRESULT CALLBACK WndProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	vector<Person> myVector;
	// txt파일 내용으로 class 구성
	setClass(myVector);
	int val;
	switch (msg)
	{
	case  WM_PAINT:
		addPaint(hWnd);
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case OPEN_FILE_BUTTON:
			open_file(hWnd);
			break;
		case FILE_MENU_EXIT:
			val = MessageBoxW(hWnd, L"정말 종료하시겠습니까?", L"Wait!", MB_ICONQUESTION | MB_OKCANCEL);
			if (val == IDOK) DestroyWindow(hWnd);
			else if (val == IDCANCEL) break;
		case FILE_MENU_NEW:
			MessageBeep(MB_ICONINFORMATION); // 소리나게
			break;
		case GENERATE_BUTTON:

			TCHAR name[30], age[10], out[100];
			GetWindowText(hName, name, 30);
			GetWindowText(hAge, age, 10);
			myVector.push_back(Person(name, age));
			if (_tcscmp(name, L"") == 0) // 이름 입력 안했을 때
			{
				MessageBoxW(hWnd, L"이름을 입력하시오.", L"정보", MB_OK | MB_ICONSTOP);
				return 0;	// break는 입력하지만 return은 함수 전체를 수행하지 않아서
							// 출력 안되게 할 수 있음
			}
			else if (_tcscmp(age, L"") == 0) // 나이 입력 안했을 때
			{
				MessageBoxW(hWnd, L"나이를 입력하시오.", L"정보", MB_OK | MB_ICONSTOP);
				return 0;
			}

			// 유니코드라서 TCHAR
			_tcscpy_s(out, 30, name);
			_tcscat_s(out, L", ");
			_tcscat_s(out, age);
			SetWindowText(hOut, out);
			InsertMemo(hWnd, out, myVector);
			break;
		case FIND_BUTTON:
			TCHAR find[1000];
			findInformation(hWnd, find);
			SetWindowText(hOut, find);
			break;
		}
		break;
	case WM_CREATE:
		addMenus(hWnd);
		addControls(hWnd);
		break;
	case WM_DESTROY: // 닫기
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}

}




int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{

	// 윈도우 클래스 등록
	WNDCLASS wc;

	wchar_t my_class_name[] = L"Panorama";
	wc.cbClsExtra = NULL; // window class를 위한 추가메모리 사용x
	wc.cbWndExtra = NULL; // window 개별메모리 사용x
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // 배경색
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // 마우스 커서
	wc.hIcon = LoadIcon(NULL, IDI_INFORMATION); // 제목 표시줄 로고
	wc.hInstance = hInst; // 첫 번째 매개변수 값 대입
	wc.lpfnWndProc = WndProcedure; // 기본 메시지 처리 함수 지정
	wc.lpszClassName = my_class_name; // 'Window class' 이름 지정
	wc.lpszMenuName = L"NULL"; // 메뉴바 사용x
	wc.style = CS_HREDRAW | CS_VREDRAW; // 윈도우 그리기 항목 설정

	RegisterClass(&wc); // wc 정보 기준으로 'Window class' 등록

	// 윈도우 생성
	HWND hWnd = CreateWindow(my_class_name, L"회원 관리",
		WS_OVERLAPPEDWINDOW, 550, 250, 500, 500, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// 프로그램에 전달된 메시지를 번역/실행
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		// 추가 메시지 발생
		TranslateMessage(&msg);
		// WndProc함수 호출
		DispatchMessage(&msg);
	}
	return 0;
}