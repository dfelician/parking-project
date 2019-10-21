#include <windows.h>
#include <stdlib.h>
#include <string>
#include "Reservation.h"

#define HOME 1
#define LOG_IN 2
#define REGISTER 3
#define SIGN_IN 4
#define SIGN_UP 5
#define EXIT 6

HMENU hMenu;
HWND hRam_ID, hPassword, hPasswordCopy, hPermit_ID;
void signIn(HWND hSignIN) {
	char stdntID[7], pWord[20];
	ConnectToDB connectToDB;

	GetWindowText(hRam_ID, (LPSTR)stdntID, 3.5);
	GetWindowText(hPassword, (LPSTR)pWord, 10);

	std::string query = "SELECT * FROM UserLogin WHERE RamID = '" + (std::string) stdntID + "'";
	bool stdFound = connectToDB.queryDB(query, (std::string) stdntID);

	if (stdFound == true) {
		std::string query = "SELECT * FROM UserLogin WHERE Password = '" + (std::string) pWord + "'";
		bool pssFound = connectToDB.queryDB(query, (std::string) stdntID);

		if (pssFound == true)
			MessageBoxW(hSignIN, L"Log In", L"Account Found", MB_OK);
		else
			MessageBoxW(hSignIN, L"Log In", L"Account Not Found", MB_OK);
	}

}
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
		175, 181, 150, 25, hLogIn, HMENU(SIGN_IN), NULL, NULL);
}
void registerAccount(HWND hWnd) {
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
		175, 235, 150, 25, hRegister, (HMENU)SIGN_UP, NULL, NULL);

	if (SIGN_UP) {
		char ramID[7], permit[7], psswrd[20], psswrdCpy[20];

		GetWindowText(hRam_ID, (LPSTR)ramID, 3.5);
		GetWindowText(hPermit_ID, (LPSTR)permit, 3.5);
		GetWindowText(hPassword, (LPSTR)psswrd, 10);
		GetWindowText(hPasswordCopy, (LPSTR)psswrdCpy, 10);
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
			case SIGN_IN:
				signIn(hWnd);
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