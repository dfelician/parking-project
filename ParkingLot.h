#pragma once
#include "SQLConnection.h"
#include "Screen Resolution.h"
#include <ctime>

class ParkingLot {
private:
	ConnectToDB databaseConnect;				//used to connect to database
	ScreenResolution screen;

	HBITMAP hBackgroundPic;						//used to load picture
	HMENU hMenu = NULL;										//top menu
	HWND hRam_ID = NULL, hPassword = NULL, hPermit_ID = NULL, hUserName = NULL, hFirstName = NULL,
		hLastName = NULL, hSpotNumber = NULL, hBackPic = NULL, hClass1 = NULL, hClass2 = NULL,			//used to store user inputs
		hClass3 = NULL, hClass4 = NULL, hClass5 = NULL, hClass6 = NULL, hMake = NULL, hModel = NULL, hPlate = NULL,
		hLot = NULL, hDate = NULL, hSpots = NULL, hEvent = NULL;

	HWND userScheduleAndCarInfo(HWND);
	std::wstring widen(const std::string&);

	std::string getCurrentTime();
	std::string getCurrentTimePlus(int);
	std::string getWeekDay();


	std::string userGroup, lotName;
	int  windowWidth = 1440, windowHeight = 860, buttonWidth = windowWidth / 10, buttonHeight = windowHeight / 27, bottomButton = windowHeight - 200,
		centerX = (windowWidth / 2 - buttonWidth / 2), centerY = (windowHeight / 2 - buttonHeight / 2) - 100,
		headingWidth = buttonWidth * 2, headingHeight = buttonHeight * 2, headingCenterX = (windowWidth / 2 - headingWidth / 2);
	char ramID[256] = "", permit[256] = "", psswrd[256] = "", user[256] = "", first[256] = "",
		last[256] = "";
public:
#define HOME 1						//options when buttons are pressed
#define LOG_IN 2
#define REGISTER 3
#define SIGN_IN 4
#define SIGN_UP 5
#define EXIT 6
#define LOT18 7
#define LOT15 8
#define VIEW 9
#define RELEASE 10
#define RESERVE 11
#define ADMIN 12
#define ADMINOPTS 13
#define MAP 14
#define STDLOTMENU 15
#define STDINFO 16
#define PROFILE 17
#define STAFF 18
#define STFLOTMENU 19
#define TAKEN 20
#define LOT12 21
#define REPORT 22
#define REQUEST 23
#define SEND 24
#define ADMINMENU 25
#define MULTIPLE 26
#define MORE 27

	HWND adminMenu(HWND);
	void sendReport(HWND);
	void sendRequest(HWND);
	HWND requestMoreSpots(HWND);
	void setUserGroup(std::string);
	void loadImages(HWND);
	HWND studentParkingLotMenu(HWND);
	HWND staffParkingLotMenu(HWND);
	HWND administrator(HWND);
	void adminOptions(HWND);
	HWND registerAccount(HWND);
	void signUp(HWND);
	void addInfo(HWND);
	HWND viewProfile(HWND);
	HWND logIn(HWND);
	void signIn(HWND);
	HWND lot18(HWND);
	HWND lot15(HWND);
	HWND lot12(HWND);
	void reserve(HWND);
	void release(HWND);
	int getWindowWidth();
	int getWindowHeight();
	int getButtonWidth();
	int getButtonHeight();
	void addMenus(HWND);
	HWND displayReservations(HWND);
	HWND displayReports(HWND);
	HWND displayRequests(HWND);
	HWND spotTaken(HWND);
};