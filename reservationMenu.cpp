#include <windows.h>
#include <stdlib.h>
#include <string>

#define LOG_IN 1
#define REGISTER 2
#define EXIT 3

HMENU hMenu;
HWND hRam_ID, hPassword, hPasswordCopy, hPermit_ID, hOut;

void AddControls(HWND hWnd) {
	CreateWindowW(L"Button", L"Log In", WS_VISIBLE | WS_CHILD,
		175, 100, 150, 25, hWnd, (HMENU) LOG_IN, NULL, NULL);
	CreateWindowW(L"Button", L"Register", WS_VISIBLE | WS_CHILD,
		175, 127, 150, 25, hWnd, (HMENU) REGISTER, NULL, NULL);
	CreateWindowW(L"Button", L"Exit", WS_VISIBLE | WS_CHILD, 400, 10, 75, 50, hWnd, (HMENU)EXIT, NULL, NULL);
}
/*
void AddMenus(HWND hWnd) {
	hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenu(hSubMenu, MF_STRING, NULL, L"Sub Menu");

	AppendMenu(hFileMenu, MF_STRING, REGISTER, L"Register");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR) hSubMenu, L"Open SubMenu");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, EXIT, L"Exit");

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, L"Home");
	AppendMenu(hMenu, MF_STRING, NULL, L"Help");

	SetMenu(hWnd, hMenu);
}*/
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	std::string conString = "DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:seniorproject4.database.windows.net, 1433; DATABASE=ParkingLot; UID=seniorproject; PWD=P@ssword;";

	switch (msg) {
	case WM_COMMAND:
		switch (wp) {
			char out[50];
		case LOG_IN:
			CreateWindowW(L"Static", L"Log in", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				175, 100, 150, 25, hWnd, NULL, NULL, NULL);
			hRam_ID = CreateWindowW(L"Edit", L"RAM ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				175, 127, 150, 25, hWnd, NULL, NULL, NULL);
			hPassword = CreateWindowW(L"Edit", L"Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL,
				175, 154, 150, 25, hWnd, NULL, NULL, NULL);

			char stdntID[7], pWord[20];


			GetWindowText(hRam_ID, (LPWSTR)stdntID, 3.5);
			GetWindowText(hPassword, (LPWSTR)pWord, 10);

			strcpy_s(out, stdntID);
			strcpy_s(out, pWord);


			break;
		case REGISTER:
			CreateWindowW(L"Static", L"Register", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				175, 100, 150, 25, hWnd, NULL, NULL, NULL);
			hRam_ID = CreateWindowW(L"Edit", L"RAM ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				175, 127, 150, 25, hWnd, NULL, NULL, NULL);
			hPermit_ID = CreateWindowW(L"Edit", L"Permit_ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				175, 154, 150, 25, hWnd, NULL, NULL, NULL);
			hPassword = CreateWindowW(L"Edit", L"Create Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL,
				175, 181, 150, 25, hWnd, NULL, NULL, NULL);
			hPasswordCopy = CreateWindowW(L"Edit", L"Re-Enter Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL,
				175, 208, 150, 25, hWnd, NULL, NULL, NULL);

			char ramID[7], permit[7], psswrd[20], psswrdCpy[20];

			GetWindowText(hRam_ID, (LPWSTR)ramID, 3.5);
			GetWindowText(hPermit_ID, (LPWSTR)permit, 3.5);
			GetWindowText(hPassword, (LPWSTR)psswrd, 10);
			GetWindowText(hPasswordCopy, (LPWSTR)psswrdCpy, 10);

			strcpy_s(out, ramID);
			strcpy_s(out, permit);
			strcpy_s(out, psswrd);
			strcpy_s(out, psswrdCpy);
			
			break;
		case EXIT:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_CREATE:
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow){
	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
