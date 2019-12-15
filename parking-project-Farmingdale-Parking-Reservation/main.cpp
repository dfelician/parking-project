#include <windows.h>
#include <stdlib.h>
#include <string>
#include "ParkingLot.h"

ParkingLot farmingdale;

/******************** header ***************************/
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	static int g_scrollY = 0;
	static int g_scrollX = 0;

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
			case APROVE:
				hWnd = farmingdale.displayApprovedRequests(hWnd);
				break;
			case ADMINOPTS:
				farmingdale.adminOptions(hWnd);
				break;
			case APPROVEREQUEST:
				farmingdale.approveRequest(hWnd);
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
		case WM_CREATE: {
			farmingdale.addMenus(hWnd);										//displays menu items
			EnableScrollBar(hWnd, SB_BOTH, ESB_ENABLE_BOTH);
		}
			case WM_LBUTTONDOWN: {
				SCROLLINFO si = { 0 };
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;
				si.nPos = 0;
				si.nTrackPos = 0;
				GetScrollInfo(hWnd, SB_VERT, &si);
				GetScrollInfo(hWnd, SB_HORZ, &si);
				break;
			}
			case WM_VSCROLL: {
				auto action = LOWORD(wp);
				HWND hScroll = (HWND)lp;
				int pos = -1;
				RECT r;

				if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) {
					pos = HIWORD(wp);
				}
				else if (action == SB_LINEDOWN) {
					pos = g_scrollY + 50;
				}
				else if (action == SB_LINEUP) {
					pos = g_scrollY - 50;
				}
				else if (action == SB_PAGEUP) {
					GetClientRect(hWnd, &r);
					pos = g_scrollY - r.top;
				}
				else if (action == SB_PAGEDOWN) {
					GetClientRect(hWnd, &r);
					pos = g_scrollY + r.bottom;
				}
				if (pos == -1)
					break;
				SCROLLINFO si = { 0 };
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;
				si.nPos = pos;
				si.nTrackPos = 0;
				SetScrollInfo(hWnd, SB_VERT, &si, true);

				GetScrollInfo(hWnd, SB_VERT, &si);
				pos = si.nPos;
				POINT pt;
				pt.x = 0;
				pt.y = pos - g_scrollY;
				auto hdc = GetDC(hWnd);
				LPtoDP(hdc, &pt, 1);
				ReleaseDC(hWnd, hdc);
				ScrollWindow(hWnd, 0, -pt.y, NULL, NULL);
				g_scrollY = pos;
				return 0;
			}
			case WM_HSCROLL: {
				auto action = LOWORD(wp);
				HWND hScroll = (HWND)lp;
				int pos = -1;
				RECT r;

				if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) {
					pos = HIWORD(wp);
				}
				else if (action == SB_LINELEFT) {
					pos = g_scrollX + 50;
				}
				else if (action == SB_LINERIGHT) {
					pos = g_scrollX - 50;
				}
				else if (action == SB_PAGELEFT) {
					GetClientRect(hWnd, &r);
					pos = g_scrollX - r.left;
				}
				else if (action == SB_PAGERIGHT) {
					GetClientRect(hWnd, &r);
					pos = g_scrollX + r.right;
				}
				if (pos == -1)
					break;
				SCROLLINFO si = { 0 };
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;
				si.nPos = pos;
				si.nTrackPos = 0;
				SetScrollInfo(hWnd, SB_HORZ, &si, true);

				GetScrollInfo(hWnd, SB_HORZ, &si);
				pos = si.nPos;
				POINT pt;
				pt.x = pos - g_scrollX;
				pt.y = 0;
				auto hdc = GetDC(hWnd);
				LPtoDP(hdc, &pt, 1);
				ReleaseDC(hWnd, hdc);
				ScrollWindow(hWnd, -pt.x, 0, NULL, NULL);
				g_scrollX = pos;
				return 0;
			}
			case WM_SIZE: {
				RECT rc = { 0 };
				GetClientRect(hWnd, &rc);
				SCROLLINFO si = { 0 };

				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_ALL;
				si.nMin = 0;
				si.nMax = farmingdale.getWindowHeight();
				if (si.nPos > 100)
					si.nMax = 5000;
				if (SB_THUMBPOSITION == 100) {
					si.nMax = farmingdale.getWindowHeight();
				}
				si.nPage = (rc.bottom - rc.top);
				si.nPos = 0;
				si.nTrackPos = 0;

				SetScrollInfo(hWnd, SB_VERT, &si, true);

				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_ALL;
				si.nMin = 0;
				si.nMax = farmingdale.getWindowWidth();
				if (si.nPos > 100)
					si.nMax = 5000;
				if (SB_THUMBPOSITION == 100) {
					si.nMax = farmingdale.getButtonWidth();
				}
				si.nPos = 0;
				si.nTrackPos = 0;

				SetScrollInfo(hWnd, SB_HORZ, &si, true);
				break;
			}
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
		}
		return 0;
}
int main(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	ConnectToDB morning;
	morning.lotProc();
	morning.morningProc();

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
