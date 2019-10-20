#include <windows.h>
#include <stdlib.h>
#include <string>
#include "Reservation.h"

#define LOG_IN 1
#define REGISTER 2
#define ENTER 3
#define EXIT 4
#define HOME 5

HMENU hMenu;
HWND hRam_ID, hPassword, hPasswordCopy, hPermit_ID, hOut;
ConnectToDB connectToDB;

void logIn() {
	HWND hLogIn = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		100, 100, 500, 500, NULL, NULL, NULL, NULL);

	CreateWindowW(L"Static", L"Log in", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		175, 100, 150, 25, hLogIn, NULL, NULL, NULL);
	hRam_ID = CreateWindowW(L"Edit", L"RAM ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		175, 127, 150, 25, hLogIn, NULL, NULL, NULL);
	hPassword = CreateWindowW(L"Edit", L"Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL,
		175, 154, 150, 25, hLogIn, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD,
		175, 181, 150, 25, hLogIn, (HMENU)ENTER, NULL, NULL);

	if (ENTER) {
		char stdntID[7], pWord[20];

		GetWindowText(hRam_ID, (LPSTR)stdntID, 3.5);
		GetWindowText(hPassword, (LPSTR)pWord, 10);

		if (connectToDB.searchID(stdntID) == false) {
			std::cout << "Ram ID not found.\n";
		}
		else {
			std::cout << "Ram ID found.\n";
		}
	}
}
void registerAccount(HWND hWnd) {
	char out[50];
	HWND hRegister = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 500, 500, NULL, NULL, NULL, NULL);

	CreateWindowW(L"Static", L"Register", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		175, 100, 150, 25, hRegister, NULL, NULL, NULL);
	hRam_ID = CreateWindowW(L"Edit", L"RAM ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		175, 127, 150, 25, hRegister, NULL, NULL, NULL);
	hPermit_ID = CreateWindowW(L"Edit", L"Permit_ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		175, 154, 150, 25, hRegister, NULL, NULL, NULL);
	hPassword = CreateWindowW(L"Edit", L"Create Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL,
		175, 181, 150, 25, hRegister, NULL, NULL, NULL);
	hPasswordCopy = CreateWindowW(L"Edit", L"Re-Enter Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL,
		175, 208, 150, 25, hRegister, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD,
		175, 235, 150, 25, hRegister, (HMENU)ENTER, NULL, NULL);

	if (ENTER) {
		char ramID[7], permit[7], psswrd[20], psswrdCpy[20];

		GetWindowText(hRam_ID, (LPSTR)ramID, 3.5);
		GetWindowText(hPermit_ID, (LPSTR)permit, 3.5);
		GetWindowText(hPassword, (LPSTR)psswrd, 10);
		GetWindowText(hPasswordCopy, (LPSTR)psswrdCpy, 10);

		strcpy_s(out, ramID);
		strcpy_s(out, permit);
		strcpy_s(out, psswrd);
		strcpy_s(out, psswrdCpy);
	}

}
void AddMenus(HWND hWnd) {
	hMenu = CreateMenu();

	AppendMenu(hMenu, MF_POPUP, HOME, "Home");
	AppendMenu(hMenu, MF_STRING, LOG_IN, "Log In");
	AppendMenu(hMenu, MF_STRING, REGISTER, "Register");
	AppendMenu(hMenu, MF_STRING, NULL, "Adminstrator");
	AppendMenu(hMenu, MF_STRING, EXIT, "Exit");

	SetMenu(hWnd, hMenu);
}
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg) {
		case WM_COMMAND:
			switch (wp) {
			case LOG_IN:
				logIn();
				break;
			case REGISTER:
				registerAccount(hWnd);
				break;
			case HOME:
	CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 500, 500, NULL, NULL, NULL, NULL);
				break;
			case EXIT:
				DestroyWindow(hWnd);
				break;
			}
			break;
		case WM_CREATE:
			AddMenus(hWnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
}
int main(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

	if (connectToDB.getConnectionStatus() == true)
		std::cout << "Connected.\n";
	else
		std::cout << "Not Connected.\n";

	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 500, 500, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
