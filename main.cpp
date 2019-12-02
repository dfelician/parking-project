#include <windows.h>
#include <stdlib.h>
#include <string>
#include "ParkingLot.h"

ParkingLot farmingdale;

/******************** header ***************************/
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg) {								//checks message from screen
		case WM_COMMAND:
			switch (wp) {							//when a button is pressed
			case HOME:
				DestroyWindow(hWnd);
				hWnd = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
					0, 0, farmingdale.getWindowWidth(), farmingdale.getWindowHeight(), NULL, NULL, NULL, NULL);
				farmingdale.loadImages(hWnd);			//opens new window
				break;
			case LOG_IN:
				hWnd = farmingdale.logIn(hWnd);							//opens log in window
				break;
			case SIGN_IN:	
				farmingdale.signIn(hWnd);						//after user presses enter in log in window
				break;
			case LOT18:
				hWnd = farmingdale.lot18(hWnd);				//displays lot 18 spots
				break;
			case LOT15:
				hWnd = farmingdale.lot15(hWnd);					//displays lot 18 spots
				break;
			case LOT12:
				hWnd = farmingdale.lot12(hWnd);				//staff lot
				break;
			case STDLOTMENU:
				hWnd = farmingdale.studentParkingLotMenu(hWnd);				//student parking lot menu
				break;
			case STFLOTMENU:
				hWnd = farmingdale.staffParkingLotMenu(hWnd);
				break;
			case PROFILE:
				hWnd = farmingdale.viewProfile(hWnd);				//to edit car info and classes
				break;
			case RESERVE:
				farmingdale.reserve(hWnd);							//reserve spot in lot
				break;
			case RELEASE:
				farmingdale.release(hWnd);							//release spot from lot
				break;
			case REGISTER:
				farmingdale.setUserGroup("U");								//students
				hWnd = farmingdale.registerAccount(hWnd);					//opens registration window
				break;
			case STAFF:
				farmingdale.setUserGroup("F");								//staff
				hWnd = farmingdale.registerAccount(hWnd);					//opens registration window
				break;
			case TAKEN:
				hWnd = farmingdale.spotTaken(hWnd);
				break;
			case SIGN_UP:
				farmingdale.signUp(hWnd);						//after enter is pressed in register window
				break;
			case STDINFO:
				farmingdale.addInfo(hWnd);
				break;
			case SEND:
				farmingdale.sendReport(hWnd);
				break;
			case ADMIN:
				hWnd = farmingdale.administrator(hWnd);
				break;
			case ADMINMENU:
				hWnd = farmingdale.adminMenu(hWnd);
				break;
			case ADMINOPTS:
				farmingdale.adminOptions(hWnd);
				break;
			case MORE:
				farmingdale.sendRequest(hWnd);
				break;
			case MULTIPLE:
				hWnd = farmingdale.requestMoreSpots(hWnd);
				break;
			case VIEW:
				hWnd = farmingdale.displayReservations(hWnd);
				break;
			case REPORT:
				hWnd = farmingdale.displayReports(hWnd);
				break;
			case REQUEST:
				hWnd = farmingdale.displayRequests(hWnd);
				break;
			case MAP:
				system("start https://www.google.com/maps/d/u/0/viewer?mid=1j1rve4UJGKS_ZtWJmjdVU99C9bia0bBl&ll=40.75218208268823%2C-73.42791349999999&z=16");
				break;
			case EXIT:
				DestroyWindow(hWnd);							//close program
				break;
			default:
				break;
			}
		case WM_CREATE:
			farmingdale.addMenus(hWnd);										//displays menu items
			break;
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
		return 0;
}
int main(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	WNDCLASSW wc = { 0 };

	wc.hbrBackground = CreateSolidBrush(RGB(0, 175, 0));				//background color
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//for mouse
	wc.hInstance = hInstance;
	wc.lpszClassName = L"myWindowClass";					//window
	wc.lpfnWndProc = WindowProcedure;						//displays window

	if (!RegisterClassW(&wc))
		return -1;

	HWND hStart = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, farmingdale.getWindowWidth(), farmingdale.getWindowHeight(), NULL, NULL, NULL, NULL);			//open new window
	farmingdale.loadImages(hStart);				//display image

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
