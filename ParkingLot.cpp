#pragma once
#include "ParkingLot.h"

/*
 ****	loadImages function		****
 ***********************************/
void ParkingLot::loadImages(HWND wnd) {
	screen.GetDesktopResolution(windowWidth, windowHeight);
	hBackgroundPic = (HBITMAP)LoadImageW(NULL, L"Farmingdale_State_College_logo_1.bmp", IMAGE_BITMAP, 
		windowWidth - 90, windowHeight - 180, LR_LOADFROMFILE);
	hBackPic = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 40,
		50, 100, 100, wnd, NULL, NULL, NULL);
	SendMessageW(hBackPic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBackgroundPic);
	EnableScrollBar(wnd, SB_BOTH, ESB_ENABLE_BOTH);
}
/*
 ****	addMenus	****
 ***********************/
void ParkingLot::addMenus(HWND hWnd) {
	screen.GetDesktopResolution(windowWidth, windowHeight);

	buttonWidth = windowWidth / 10, buttonHeight = windowHeight / 27, bottomButton = windowHeight - 200,
		centerX = (windowWidth / 2 - buttonWidth / 2), centerY = (windowHeight / 2 - buttonHeight / 2) - 100,
		headingWidth = buttonWidth * 2, headingHeight = buttonHeight * 2, headingCenterX = (windowWidth / 2 - headingWidth / 2);
	
	hMenu = CreateMenu();

	AppendMenu(hMenu, MF_STRING, HOME, "Home");							//menu items on top of the screen
	AppendMenu(hMenu, MF_STRING, LOG_IN, "Log In");
	AppendMenu(hMenu, MF_STRING, STAFF, "Register Faculty");
	AppendMenu(hMenu, MF_STRING, REGISTER, "Register Student");
	AppendMenu(hMenu, MF_STRING, ADMIN, "Adminstrator");
	AppendMenu(hMenu, MF_STRING, MAP, "Farmingdale Campus Map");
	AppendMenu(hMenu, MF_STRING, EXIT, "Exit");

	SetMenu(hWnd, hMenu);									//add menu to window
}
	/***************				Registration Menu		**************					**********/
/*
 ****	addInfo function	****
 *******************************/
void ParkingLot::addInfo(HWND wndw) {
		char make[256], model[256], plate[256];

	if (userGroup == "U") {
		char class1[256], class2[256], class3[256], class4[256], class5[256], class6[256];

		GetWindowText(hClass1, class1, 128);
		GetWindowText(hClass2, class2, 128);
		GetWindowText(hClass3, class3, 128);
		GetWindowText(hClass4, class4, 128);
		GetWindowText(hClass5, class5, 128);
		GetWindowText(hClass6, class6, 128);

		std::string classes[6]{ (std::string) class1, (std::string)class2, (std::string)class3,
			(std::string)class4, (std::string)class5, (std::string)class6 };

		for (int i = 0; i < 6; i++) {		//check each class
			if (i == 0)
				if (classes[i] == "") {					//class one empty
					MessageBoxW(wndw, L"You must have at least one class in your schedule (1)Class 1", L"Class Schedule", MB_OK);
					return;
				}

			if (classes[i] == "" || classes[i] == "NONE") {					//no class entered
				classes[i] = "NULL";				//make it null
				continue;
			}
			if (classes[i].length() != 5) {					//is not 5 characters
				MessageBoxW(wndw, L"CRN is a 5 digit identifier", L"Class Schedule", MB_OK);
				return;
			}
			for (unsigned int k = 0; k < classes[i].length(); k++) {
				if ((classes[i])[k] < '0' || (classes[i])[k] > '9') {				//is not all numbers
					MessageBoxW(wndw, L"CRN is a 5 digit identifie", L"Class Schedule", MB_OK);
					return;
				}
			}
			bool courseExist = databaseConnect.findCourse("Select CRN from course where CRN = '" +
				(std::string)classes[i] + "'", classes[i]);			//does course exist?

			if (courseExist == false) {				//course does not exist
				MessageBoxW(wndw, L"You entered an invalid course", L"Class Schedule", MB_OK);
				return;
			}
			if (classes[i] != "") {						//valid CRN was entered
				classes[i] = "'" + classes[i] + "'";		//put quotes for inserting
			}
		}										///		end class check

		for (unsigned int i = 0; i < classes->size(); i++)						//get rid of duplicates
			for (unsigned int j = i; j < classes->size(); j++)
				if (classes[j] == classes[i])
					classes[j] = "NULL";

		//insert to student's schedule
		databaseConnect.insertStatement("Insert into schedule_test values((select ramid from userlogin where username like '"
			+ (std::string) user + "')," + classes[0] + "," + classes[1] + "," + classes[2]
			+ "," + classes[3] + "," + classes[4] + "," + classes[5] + ")");

		//update student schedule
		std::string updateSch = "Update schedule_test set class1 = " + classes[0] + ", class2 = " + classes[1]
			+ ", class3 = " + classes[2] + ", class4 = " + classes[3] + ", class5 = " + classes[4] + ", class6 = "
			+ classes[5] + " where ramID = (select ramid from userlogin where username like '"
			+ (std::string) user + "')";

		databaseConnect.insertStatement(updateSch);
	}

	GetWindowText(hMake, make, 128);
	GetWindowText(hModel, model, 128);
	GetWindowText(hPlate, plate, 128);

	if (((std::string) plate) == "" || ((std::string) model) == "" || ((std::string) plate) == "") {
		MessageBoxW(wndw, L"All Fields Required\n(1)Make\n(2)Model\n(3)License Plate", L"Vehicle Info", MB_OK);
		return;
	}

	if (((std::string) plate).length() > 8) {		//license plate has more characters than max (8)
		MessageBoxW(wndw, L"License plate number cannot be greater than 8 characters", L"Vehicle Info", MB_OK);
		return;
	}

	//insert data to permit table
	databaseConnect.insertStatement("insert into permit values( (select parkingpermit from userlogin where username = '" + (std::string) user+ "'),'" + (std::string)make + "','"
		+ (std::string) model + "','" + (std::string) plate + "')");
	//update permit table
	std::string updatePermit = "update permit set carmake = '" + (std::string) make + "', carmodel = '" +
		(std::string) model + "', licenseplatenum = '" + (std::string) plate +
		"'where permitnumber = (select parkingpermit from userlogin where username = '" + (std::string) user + "')";
	
	databaseConnect.insertStatement(updatePermit);

	if (userGroup == "U")
		wndw = studentParkingLotMenu(wndw);
	else
		wndw = staffParkingLotMenu(wndw);
}

/*
****	viewProfile function	***
************************************/
HWND ParkingLot::viewProfile(HWND hPrev) {
	DestroyWindow(hPrev);
	HWND hProfile = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);
	EnableScrollBar(hProfile, SB_BOTH, ESB_ENABLE_BOTH);
	CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		windowWidth - (windowWidth/6), 20, buttonWidth, buttonHeight, hProfile, (HMENU)LOG_IN, NULL, NULL);
	CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		centerX,bottomButton, buttonWidth, buttonHeight, hProfile, (HMENU)STDINFO, NULL, NULL);
	if (userGroup == "U") {
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, hProfile, (HMENU)STDLOTMENU, NULL, NULL);
	}
	else {
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, hProfile, (HMENU)STFLOTMENU, NULL, NULL);
	}

	int xPos = centerX/2, secondColumnMargin = buttonWidth + 10,thirdColumnMargin = secondColumnMargin * 2, 
		fourthColumnMargin = secondColumnMargin * 3, fifthColumnMargin = secondColumnMargin * 4;
	//HWND hProfile[] = {hMake, hModel, hPlate, hClass1, hClass2, hClass3, hClass4, hClass5, hClass6};

	std::vector<std::string> profile;
	std::vector<std::wstring> profileInfo;

	databaseConnect.getProfile((std::string)user, profile, userGroup);

	for (unsigned int i = 0; i < profile.size(); i++) {
		if (profile[i] == "" || profile[i] == " ")
			profile[i] = "NONE";
		profileInfo.push_back(widen(profile[i]));
	}

		/*if (i < 2) {
			if (i % 2 == 0) {

			}
		}
		else {
			hProfile[i] = CreateWindowW(L"Edit", profileInfo[0].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				centerX + (centerX / 2), centerY - 81, buttonWidth, buttonHeight, hProfile, NULL, NULL, NULL);
		}
	}*/

	CreateWindowW(L"Static", L"VEHICLE INFO", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,			//vehicle info
		headingCenterX, buttonHeight + 2, headingWidth, headingHeight, hProfile, NULL, NULL, NULL);
	hMake = CreateWindowW(L"Edit", profileInfo[0].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		centerX, centerY - (buttonHeight + 2) *5, buttonWidth, buttonHeight, hProfile, NULL, NULL, NULL);
	hModel = CreateWindowW(L"Edit", profileInfo[1].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		centerX, centerY - (buttonHeight + 2)*4, buttonWidth, buttonHeight, hProfile, NULL, NULL, NULL);
	hPlate = CreateWindowW(L"Edit", profileInfo[2].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		centerX, centerY - (buttonHeight + 2) * 3, buttonWidth, buttonHeight, hProfile, NULL, NULL, NULL);

	if (userGroup == "U") {
		CreateWindowW(L"Static", L"CLASS SCHEDULE", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,			//Schedule
			headingCenterX, bottomButton - (buttonHeight + 2)*12+10, headingWidth, headingHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Leave blank where no class\n(Enter the CRN)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, bottomButton - (buttonHeight + 2)*10+10, buttonWidth, buttonHeight + 35, hProfile, NULL, NULL, NULL);

		CreateWindowW(L"Static", L"CRN", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos, bottomButton - (buttonHeight + 2)*8, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Class Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + secondColumnMargin, bottomButton - (buttonHeight + 2) * 8, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Start Time", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + thirdColumnMargin, bottomButton - (buttonHeight + 2) * 8, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"End Time", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fourthColumnMargin, bottomButton - (buttonHeight + 2) * 8, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Day", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fifthColumnMargin, bottomButton - (buttonHeight + 2) * 8, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);

		hClass1 = CreateWindowW(L"Edit", profileInfo[3].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos, bottomButton - (buttonHeight + 2) * 7, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[4].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + secondColumnMargin, bottomButton - (buttonHeight + 2) * 7, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[5].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + thirdColumnMargin, bottomButton - (buttonHeight + 2) * 7, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[6].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fourthColumnMargin, bottomButton - (buttonHeight + 2) * 7, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[7].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fifthColumnMargin, bottomButton - (buttonHeight + 2) * 7, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);

		hClass2 = CreateWindowW(L"Edit", profileInfo[8].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos, bottomButton - (buttonHeight + 2) * 6, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[9].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + secondColumnMargin, bottomButton - (buttonHeight + 2) * 6, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[10].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + thirdColumnMargin, bottomButton - (buttonHeight + 2) * 6, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[11].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fourthColumnMargin, bottomButton - (buttonHeight + 2) * 6, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[12].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fifthColumnMargin, bottomButton - (buttonHeight + 2) * 6, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);

		hClass3 = CreateWindowW(L"Edit", profileInfo[13].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos, bottomButton - (buttonHeight + 2) * 5, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[14].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + secondColumnMargin, bottomButton - (buttonHeight + 2) * 5, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[15].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + thirdColumnMargin, bottomButton - (buttonHeight + 2) * 5, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[16].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fourthColumnMargin, bottomButton - (buttonHeight + 2) * 5, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[17].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fifthColumnMargin, bottomButton - (buttonHeight + 2) * 5, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		
		hClass4 = CreateWindowW(L"Edit", profileInfo[18].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos, bottomButton - (buttonHeight + 2) * 4, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[19].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + secondColumnMargin, bottomButton - (buttonHeight + 2) * 4, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[20].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + thirdColumnMargin, bottomButton - (buttonHeight + 2) * 4, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[21].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fourthColumnMargin, bottomButton - (buttonHeight + 2) * 4, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[22].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fifthColumnMargin, bottomButton - (buttonHeight + 2) * 4, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);

		hClass5 = CreateWindowW(L"Edit", profileInfo[23].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos, bottomButton - (buttonHeight + 2) * 3, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[24].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + secondColumnMargin, bottomButton - (buttonHeight + 2) * 3, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[25].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + thirdColumnMargin, bottomButton - (buttonHeight + 2) * 3, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[26].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fourthColumnMargin, bottomButton - (buttonHeight + 2) * 3, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[27].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fifthColumnMargin, bottomButton - (buttonHeight + 2) * 3, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);

		hClass6 = CreateWindowW(L"Edit", profileInfo[28].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos, bottomButton - (buttonHeight + 2) * 2, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[29].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + secondColumnMargin, bottomButton - (buttonHeight + 2) * 2, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[30].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + thirdColumnMargin, bottomButton - (buttonHeight + 2) * 2, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[31].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fourthColumnMargin, bottomButton - (buttonHeight + 2) * 2, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
		CreateWindowW(L"Static", profileInfo[32].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			xPos + fifthColumnMargin, bottomButton - (buttonHeight + 2) * 2, buttonWidth + 10, buttonHeight, hProfile, NULL, NULL, NULL);
	}

	return hProfile;
}

/*
 ****	userScheduleAndCarInfo function	****
 ***********************************************/
HWND ParkingLot::userScheduleAndCarInfo(HWND hPrev) {
	DestroyWindow(hPrev);
	HWND hSchedule = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);
	EnableScrollBar(hSchedule, SB_BOTH, ESB_ENABLE_BOTH);
	CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		windowHeight - (windowWidth/6), 20, buttonWidth, buttonHeight, hSchedule, (HMENU)LOG_IN, NULL, NULL);
	CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		centerX, bottomButton, buttonWidth, buttonHeight, hSchedule, (HMENU)STDINFO, NULL, NULL);

	CreateWindowW(L"Static", L"VEHICLE INFO", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,			//vehicle info
		headingCenterX, (buttonHeight + 2), headingWidth, headingHeight, hSchedule, NULL, NULL, NULL);
	hMake = CreateWindowW(L"Edit",L"Car Make", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		centerX, centerY - (buttonHeight + 2) * 5, buttonWidth, buttonHeight, hSchedule, NULL, NULL, NULL);
	hModel = CreateWindowW(L"Edit", L"Car Model", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		centerX, centerY - (buttonHeight + 2) * 4, buttonWidth, buttonHeight, hSchedule, NULL, NULL, NULL);
	hPlate = CreateWindowW(L"Edit", L"License Plate", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		centerX, centerY - (buttonHeight + 2) * 3, buttonWidth, buttonHeight, hSchedule, NULL, NULL, NULL);

	if (userGroup == "U") {
		CreateWindowW(L"Static", L"CLASS SCHEDULE", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,			//Schedule
			headingCenterX, bottomButton - (buttonHeight + 2) * 11 + 10, headingWidth, headingHeight, hSchedule, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Leave blank where no class\n(Enter the CRN)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, bottomButton - (buttonHeight + 2) * 10 + 10, buttonWidth, buttonHeight + 35, hSchedule, NULL, NULL, NULL);

		hClass1 = CreateWindowW(L"Edit", L"Class 1", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, bottomButton - (buttonHeight + 2) * 7, buttonWidth + 10, buttonHeight, hSchedule, NULL, NULL, NULL);

		hClass2 = CreateWindowW(L"Edit", L"Class 2", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, bottomButton - (buttonHeight + 2) * 6, buttonWidth + 10, buttonHeight, hSchedule, NULL, NULL, NULL);

		hClass3 = CreateWindowW(L"Edit", L"Class 3", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, bottomButton - (buttonHeight + 2) * 5, buttonWidth + 10, buttonHeight, hSchedule, NULL, NULL, NULL);

		hClass4 = CreateWindowW(L"Edit", L"Class 4", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, bottomButton - (buttonHeight + 2) * 4, buttonWidth + 10, buttonHeight, hSchedule, NULL, NULL, NULL);

		hClass5 = CreateWindowW(L"Edit", L"Class 5", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, bottomButton - (buttonHeight + 2) * 3, buttonWidth + 10, buttonHeight, hSchedule, NULL, NULL, NULL);

		hClass6 = CreateWindowW(L"Edit", L"Class 6", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, bottomButton - (buttonHeight + 2) * 2, buttonWidth + 10, buttonHeight, hSchedule, NULL, NULL, NULL);
	}

	return hSchedule;
}
/***************************
 ****	signUp function	****
 ***************************/
	void ParkingLot::signUp(HWND hSignUp) {
		GetWindowText(hRam_ID, (LPSTR)ramID, 128);							//get user input from screen
		GetWindowText(hPassword, (LPSTR)psswrd, 128);
		GetWindowText(hUserName, (LPSTR)user, 128);
		GetWindowText(hFirstName, (LPSTR)first, 128);
		GetWindowText(hLastName, (LPSTR)last, 128);


		if (((std::string) ramID) == "" || ((std::string) psswrd) == "" || ((std::string) user) == ""
			|| ((std::string) first) == "" || ((std::string) last) == "") {
			MessageBoxW(hSignUp, L"All Fields Required\n(1)First Name\n(2)Last Name\n(3)Ram ID\n(4)User Name\n(5)Password", L"Sign Up", MB_OK);
			return;
		}

		bool inSystem = databaseConnect.getStudent("select username, ramid from userlogin where username like '" + (std::string) user
			+ "' or ramid = '" + (std::string) ramID + "'", (std::string) user, (std::string) ramID);		//check if already in database

		if (inSystem == true) {					//ram id or user name already in system
			MessageBoxW(hSignUp, L"User Name or Ram ID already registered", L"Register", MB_OK);

			return;
		}
		if (ramID[0] != 'R' || ((std::string) ramID).length() != 9) {			//ram ID not 9 charachers
			MessageBoxW(hSignUp, L"Invalid Ram ID\n(RXXXXXXXXX)\n\'R\' then 8 digits", L"Register", MB_OK);

			return;
		}
		for (int j = 1; j < 9; j++) {
			if (((std::string) ramID)[j] < '0' || ((std::string) ramID)[j] > '9') {			//following 8 characters are not all numbers
				MessageBoxW(hSignUp, L"Invalid Ram ID\n(RXXXXXXXXX)\n\'R\' then 8 digits", L"Register", MB_OK);
				return;
			}
		}

		std::string usrStatement = "Insert INTO UserLogin Values('" + (std::string) user + "','"
			+ (std::string) psswrd + "','" + (std::string) ramID + "','" + userGroup + "')";

		databaseConnect.insertStatement(usrStatement);
		hSignUp = userScheduleAndCarInfo(hSignUp);
	}
	/*******************************************
	 ****	registerAccount function	****
	 *******************************************/
/*
 ****	setUserGroup function	****
 **********************************/
	void ParkingLot::setUserGroup(std::string group) {
		this->userGroup = group;
	}
	HWND ParkingLot::registerAccount(HWND hHome) {
		HWND hRegister = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);							//opens new window
		EnableScrollBar(hRegister, SB_BOTH, ESB_ENABLE_BOTH);
		DestroyWindow(hHome);

		CreateWindowW(L"Static", L"REGISTER", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,			//heading
			headingCenterX, (buttonHeight + 2) * 1, headingWidth, headingHeight, hRegister, NULL, NULL, NULL);
		hFirstName = CreateWindowW(L"Edit", L"First Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//enter first name from screen
			centerX, centerY - (buttonHeight + 2) * 3, buttonWidth, buttonHeight, hRegister, NULL, NULL, NULL);
		hLastName = CreateWindowW(L"Edit", L"Last Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//last name
			centerX, centerY - (buttonHeight + 2) * 2, buttonWidth, buttonHeight, hRegister, NULL, NULL, NULL);
		hRam_ID = CreateWindowW(L"Edit", L"RAM ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,			//ramID
			centerX, centerY - (buttonHeight + 2) * 1, buttonWidth, buttonHeight, hRegister, NULL, NULL, NULL);
		hUserName = CreateWindowW(L"Edit", L"User Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//username
			centerX, centerY, buttonWidth, buttonHeight, hRegister, NULL, NULL, NULL);
		hPassword = CreateWindowW(L"Edit", L"Create Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, centerY + (buttonHeight + 2) * 1, buttonWidth, buttonHeight, hRegister, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD,
			centerX, centerY + (buttonHeight + 2) * 2, buttonWidth, buttonHeight, hRegister, (HMENU)SIGN_UP, NULL, NULL);		//SIGN_UP case

		return hRegister;
	}
			/*********						LOG IN MENU			****************			*************/
	/***
	 ****	signIn function	****
	 ***************************/
	void ParkingLot::signIn(HWND hSignIN) {
		GetWindowText(hUserName, (LPSTR)user, 128);			//get user data and store it
		GetWindowText(hPassword, (LPSTR)psswrd, 128);

		std::string query = "SELECT username, password, usergroup FROM UserLogin WHERE Username = '" + (std::string) user + "'";					//create query
		
		bool stdFound = databaseConnect.findUser(query, (std::string) user, (std::string)psswrd, userGroup);		//run query
		
		if (stdFound == true && userGroup == "U")							//studet login successful
			hSignIN = studentParkingLotMenu(hSignIN);						//open student parking Lot menu
		else if (stdFound == true && userGroup == "F") {
			hSignIN = staffParkingLotMenu(hSignIN);
		}						
		else											//login not successful
			MessageBoxW(hSignIN, L"Information Incorrect or Failed Network Connection", L"Account Not Found", MB_OK);			//display error message
	}
	/*
	 ****	logIn function	****
	 ***************************/
	HWND ParkingLot::logIn(HWND hHome) {
		DestroyWindow(hHome);
		HWND hLogIn = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);				//open new window
		EnableScrollBar(hLogIn, SB_BOTH, ESB_ENABLE_BOTH);

		CreateWindowW(L"Static", L"LOG IN", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,				//heading
			headingCenterX, (buttonHeight + 2) * 1, headingWidth, headingHeight, hLogIn, NULL, NULL, NULL);
		hUserName = CreateWindowW(L"Edit", L"User Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, centerY, buttonWidth, buttonHeight, hLogIn, NULL, NULL, NULL);					//enter user name from screen
		hPassword = CreateWindowW(L"Edit", L"Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, centerY + (buttonHeight + 2) * 1, buttonWidth, buttonHeight, hLogIn, NULL, NULL, NULL);				//password
		CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD,
			centerX, centerY + (buttonHeight + 2) * 2, buttonWidth, buttonHeight, hLogIn, HMENU(SIGN_IN), NULL, NULL);		//SIGN_IN case
		return hLogIn;
	}
	/*
	 ****	getWindowWidth	****
	****************************/
	int ParkingLot::getWindowWidth() {
		return this->windowWidth;
	}
	/*
	 ****	getWindowHeight	****
	****************************/
	int ParkingLot::getWindowHeight() {
		return this->windowHeight;
	}
	/*
	 ****	getWindowWidth	****
	****************************/
	int ParkingLot::getButtonWidth() {
		return this->buttonWidth;
	}
	/*
	 ****	getWindowHeight	****
	****************************/
	int ParkingLot::getButtonHeight() {
		return this->buttonHeight;
	}
	/****************************
	 ****	studentParkingLotMenu	*****
	 ****************************/
	HWND ParkingLot::studentParkingLotMenu(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hLotMenu = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);			//opens new window
		EnableScrollBar(hLotMenu, SB_BOTH, ESB_ENABLE_BOTH);

		CreateWindowW(L"Static", L"STUDENT PARKING LOT MENU", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//heading
			headingCenterX, (buttonHeight + 2) * 1, headingWidth, headingHeight, hLotMenu, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth -(windowWidth/6), 20, buttonWidth, buttonHeight, hLotMenu, (HMENU)LOG_IN, NULL, NULL);
		CreateWindowW(L"Button", L"Lot 18", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,					//buttons for each lot 
			centerX, centerY - (buttonHeight + 2) * 1, buttonWidth, buttonHeight, hLotMenu, (HMENU)LOT18, NULL, NULL);
		CreateWindowW(L"Button", L"Lot 15", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, centerY, buttonWidth, buttonHeight, hLotMenu, (HMENU)LOT15, NULL, NULL);
		CreateWindowW(L"Button", L"View Profile", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth - (windowWidth/6), (buttonHeight + 22) , buttonWidth, buttonHeight, hLotMenu, (HMENU)PROFILE, NULL, NULL);

		return hLotMenu;
	}
	/****************************
	 ****	staffParkingLotMenu	*****
	 ****************************/
	HWND ParkingLot::staffParkingLotMenu(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hLotMenu = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);			//opens new window
		EnableScrollBar(hLotMenu, SB_BOTH, ESB_ENABLE_BOTH);

		CreateWindowW(L"Static", L"STAFF PARKING LOT MENU", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//heading
			headingCenterX, (buttonHeight +2), headingWidth, headingHeight, hLotMenu, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Lot 12", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,					//buttons for each lot 
			centerX, centerY - buttonHeight+ 2, buttonWidth*1.5, buttonHeight, hLotMenu, (HMENU)LOT12, NULL, NULL);
		CreateWindowW(L"Button", L"Multiple Spot Request", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, centerY, buttonWidth*1.5, buttonHeight, hLotMenu, HMENU(MULTIPLE), NULL, NULL);
		CreateWindowW(L"Button", L"View Profile", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth -(windowWidth/6), (buttonHeight + 22), buttonWidth, buttonHeight, hLotMenu, (HMENU)PROFILE, NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth - (windowWidth / 6), 20, buttonWidth, buttonHeight, hLotMenu, (HMENU)LOG_IN, NULL, NULL);

		return hLotMenu;
	}
	/*
	****	requestMoreSpots Function	****
	************************************/
	HWND ParkingLot::requestMoreSpots(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hMoreSpots = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);			//opens new window
		EnableScrollBar(hMoreSpots, SB_BOTH, ESB_ENABLE_BOTH);

		CreateWindowW(L"Static", L"Multiple Spot Request", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//heading
			headingCenterX, (buttonHeight + 2) * 1, headingWidth, headingHeight, hMoreSpots, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth - (windowWidth / 6), 20, buttonWidth, buttonHeight, hMoreSpots, (HMENU)LOG_IN, NULL, NULL);
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, hMoreSpots, (HMENU)STFLOTMENU, NULL, NULL);

		hLot = CreateWindowW(L"Edit", L"Lot", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//enter first name from screen
			centerX, centerY - ( 2) * 4, buttonWidth + buttonHeight, buttonHeight, hMoreSpots, NULL, NULL, NULL);
		hDate = CreateWindowW(L"Edit", L"Date of Event(YYYY-MM-DD)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//last name
			centerX, centerY - ( 2) * 3, buttonWidth + buttonHeight, buttonHeight * 2, hMoreSpots, NULL, NULL, NULL);
		hSpots = CreateWindowW(L"Edit", L"Number of Spots", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,			//ramID
			centerX, centerY - ( 2) * 1, buttonWidth + buttonHeight, buttonHeight, hMoreSpots, NULL, NULL, NULL);
		hEvent = CreateWindowW(L"Edit", L"Event", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//username
			centerX, centerY, buttonWidth + buttonHeight, buttonHeight, hMoreSpots, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD,
			centerX, centerY + ( 2) * 2, buttonWidth + buttonHeight, buttonHeight, hMoreSpots, (HMENU)MORE, NULL, NULL);

		return hMoreSpots;
	}
	void ParkingLot::sendRequest(HWND hRequesting) {
		char requestLot[256], requestDate[256], requestSpots[256], eventRequest[256];
		int spotsRequested;

		GetWindowText(hLot, requestLot, 128);
		GetWindowText(hDate, requestDate, 128);
		GetWindowText(hSpots, requestSpots, 128);
		GetWindowText(hEvent, eventRequest, 128);

		if (((std::string) requestLot) == "" || ((std::string) requestDate) == "" ||
			((std::string) requestSpots) == "" || ((std::string) eventRequest) == "") {
			MessageBoxW(hRequesting, L"All Fields Required\n(1)Lot\n(2)Date\n(3)Number of Spots\n(4)Event Title", L"Request Spots", MB_OK);
			return;
		}

		if (((std::string) requestSpots)[0] > 'A' && ((std::string) requestSpots)[0] < 'Z'
			|| ((std::string) requestSpots)[0] > 'a' && ((std::string) requestSpots)[0] < 'z') {
			MessageBoxW(hRequesting, L"Enter a number", L"Request Spots", MB_OK);

			return;
		}

		spotsRequested = std::stoi((std::string) requestSpots);
		wchar_t spot[256];								//generartes spot number
		wsprintfW(spot, L"%d", spotsRequested);

		if (((std::string) requestDate).length() != 10) {					//is not 5 character
				MessageBoxW(hRequesting, L"Invalid Date (YYYY-MM-DD)", L"Request Spots", MB_OK);
				return;
		}

		for (unsigned int k = 0; k < ((std::string) requestDate).length(); k++) {
			if (k == 4 || k == 7) {			//check for dashes
				if (((std::string) requestDate)[k] != '-') {		//no dashes
					MessageBoxW(hRequesting, L"Invalid Date (YYYY-MM-DD)", L"Request Spots", MB_OK);
					return;
				}
				continue;						//has dashes
			}
			else if (((std::string) requestDate)[k] < '0' || ((std::string) requestDate)[k] > '9') {	//is not all numbers
				MessageBoxW(hRequesting, L"Invalid Date (YYYY-MM-DD)", L"Request Spots", MB_OK);
				return;
			}
			else if (k == 5) {								//check month input
				if (((std::string) requestDate)[k] > '1') {				//first 'M' cannot be greater than 1	
					MessageBoxW(hRequesting, L"Invalid Date (YYYY-MM-DD)", L"Request Spots", MB_OK);
					return;
				}
				else if (((std::string) requestDate)[k] == '1')	{			//when first 'M' is 1
					if (((std::string) requestDate)[k + 1] > '2') {				//second 'M' cannot be greater than 2
						MessageBoxW(hRequesting, L"Invalid Date (YYYY-MM-DD)", L"Request Spots", MB_OK);
						return;
					}
				}
			}
			else if (k == 8) {				//check date input
				if (((std::string) requestDate)[k] > '3') {			//first 'D' cannot be greater than 3
					MessageBoxW(hRequesting, L"Invalid Date (YYYY-MM-DD)", L"Request Spots", MB_OK);
					return;
				}
				else if(((std::string) requestDate)[k] == '3') {	//when first 'D' is 3
					if (((std::string) requestDate)[k + 1] > '1') {		//second 'D' cannot be greater than 1
						MessageBoxW(hRequesting, L"Invalid Date (YYYY-MM-DD)", L"Request Spots", MB_OK);
						return;
					}
				}
			}
		}

			databaseConnect.insertStatement("insert into pendingrequests values('" + (std::string) user
				+ "','" + (std::string) requestLot + "','" + (std::string) requestDate + "'," + (std::string) requestSpots
				+ ",'" + (std::string) eventRequest + "','N')");

			hRequesting = staffParkingLotMenu(hRequesting);
	}
	/*
	****	lot12 function	****
	**************************/

	HWND ParkingLot::lot12(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hLot12 = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);			//opens new window
		EnableScrollBar(hLot12, SB_BOTH, ESB_ENABLE_BOTH);
		int numOfSpots = 20;
		lotName = "Lot03";

		std::vector<std::string> lot15Spots;
		databaseConnect.checkSpots(lotName, lot15Spots);
		int reservedSpot = databaseConnect.getSpotNumber("Select spotnumber from " + lotName + " where ramID = (select ramid from userlogin where username = '"
			+ (std::string)user + "')");


		CreateWindowW(L"Static", L"Staff Lot 12", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//heading
			20, 20, buttonWidth, buttonHeight * 2, hLot12, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, hLot12, (HMENU)STFLOTMENU, NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth - (windowWidth / 6), 20, buttonWidth, buttonHeight, hLot12, (HMENU)LOG_IN, NULL, NULL);
		CreateWindowW(L"Static", L"Enter desired spot below to reserve/release", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, buttonHeight*3, buttonWidth, buttonHeight*2, hLot12, NULL, NULL, NULL);
		hSpotNumber = CreateWindowW(L"Edit", L"Spot Number", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//enter spot number from screen
			20, bottomButton - (buttonHeight + 2) * 5, buttonWidth, buttonHeight, hLot12, NULL, NULL, NULL);

		if (reservedSpot > 0) {				//user already has a spot
			CreateWindowW(L"Button", L"Release Spot", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//can release spot
				20, bottomButton - (buttonHeight + 2) * 4, buttonWidth, buttonHeight, hLot12, (HMENU)RELEASE, NULL, NULL);
			CreateWindowW(L"Button", L"Spot Taken?", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, bottomButton - (buttonHeight + 2) * 2, buttonWidth, buttonHeight, hLot12, (HMENU)TAKEN, NULL, NULL);					//can report spot taken

			wchar_t isSpot[256];
			wsprintfW(isSpot, L"%d", reservedSpot);
			std::wstring spotIsReserved = widen("You have this spot number reserved: ") + isSpot;

			CreateWindowW(L"Static", spotIsReserved.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, (buttonHeight + 2) * 5, buttonWidth, buttonHeight * 2, hLot12, NULL, NULL, NULL);
		}
		else {								//user does not have a spot
			CreateWindowW(L"Button", L"Reserve Spot", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, bottomButton - (buttonHeight + 2) * 4, buttonWidth, buttonHeight, hLot12, (HMENU)RESERVE, NULL, NULL);			// can reservse spot
		}

		for (int k = 1; k <= 4; k++) {
			for (int i = 1; i <= numOfSpots; i++) {
				int num = (i)+(k - 1) * numOfSpots;
				wchar_t spot[256];
				wsprintfW(spot, L"%d", num);				//generates spot number

				//displays vertical rows
				if (lot15Spots[num - 1] != "N")								//spot availible show number
					CreateWindowW(L"Static", spot, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,//displays horizontal rows
					(i * 40) + 150, (100 * k) + 100, buttonHeight, buttonHeight, hLot12, NULL, NULL, NULL);
				else																		//spot not availible dont show number
					CreateWindowW(L"Static", L"", COLOR_WINDOW | WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,//displays horizontal rows
					(i * 40) + 150, (100 * k) + 100, buttonHeight, buttonHeight, hLot12, NULL, NULL, NULL);
			}
		}
		return hLot12;
	}
	/****************************
	 ****	lot18	*****
	 ****************************/
	HWND ParkingLot::lot18(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hLot18 = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);			//opens new window
		EnableScrollBar(hLot18, SB_BOTH, ESB_ENABLE_BOTH);
		int numOfSpots = 20;
		lotName = "Lot01";

		std::vector<std::string> lot18Spots;
		databaseConnect.checkSpots(lotName, lot18Spots);

		int reservedSpot = databaseConnect.getSpotNumber("Select spotnumber from " + lotName + " where ramID = (select ramid from userlogin where username = '"
			+ (std::string)user + "')");

		CreateWindowW(L"Static", L"Priority Lot 18", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//heading
			20, 20, buttonWidth, buttonHeight*2, hLot18, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, hLot18, (HMENU)STDLOTMENU, NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth - (windowWidth/6), 20, buttonWidth, buttonHeight, hLot18, (HMENU)LOG_IN, NULL, NULL);
		CreateWindowW(L"Static", L"Enter desired spot below to reserve/release", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		
			20, buttonHeight*3, buttonWidth, buttonHeight*2, hLot18, NULL, NULL, NULL);
		hSpotNumber = CreateWindowW(L"Edit", L"Spot Number", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//enter spot number from screen
			20, bottomButton - (buttonHeight + 2) * 5, buttonWidth, buttonHeight, hLot18, NULL, NULL, NULL);

		if (reservedSpot > 0) {				//user already has a spot
			CreateWindowW(L"Button", L"Release Spot", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//can release spot
				20, bottomButton - (buttonHeight + 2) * 4, buttonWidth, buttonHeight, hLot18, (HMENU)RELEASE, NULL, NULL);
			CreateWindowW(L"Button", L"Spot Taken?", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, bottomButton - (buttonHeight +2) *2, buttonWidth, buttonHeight, hLot18, (HMENU)TAKEN, NULL, NULL);					//can report spot taken

			wchar_t isSpot[256];
			wsprintfW(isSpot, L"%d", reservedSpot);
			std::wstring spotIsReserved = widen("You have this spot number reserved: ") + isSpot;

			CreateWindowW(L"Static", spotIsReserved.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, (buttonHeight + 2) * 5, buttonWidth, buttonHeight * 2, hLot18, NULL, NULL, NULL);
		}
		else {								//user does not have a spot
			CreateWindowW(L"Button", L"Reserve Spot", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, bottomButton - (buttonHeight + 2) * 4, buttonWidth, buttonHeight, hLot18, (HMENU)RESERVE, NULL, NULL);			// can reservse spot
		}

		for (int i = 1; i <= numOfSpots; i++) {
			wchar_t spot[256];								//generartes spot number
			wsprintfW(spot, L"%d", i);

							//displays first horizontal row
			if (lot18Spots[i - 1] != "N")						//spot availible show number
				CreateWindowW(L"Static", spot, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				(i * 40) + 260, buttonHeight*2 +5, buttonHeight, buttonHeight, hLot18, NULL, NULL, NULL);
			else																		//spot not availible dont show number
				CreateWindowW(L"Static", L"", COLOR_WINDOW | WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				(i * 40) + 260, buttonHeight*2 + 5, buttonHeight, buttonHeight, hLot18, NULL, NULL, NULL);
		}
		for (int i = 1; i <= 5; i++) {
			for (int j = 1; j <= 25; j++) {
				int num = (j + numOfSpots) + (i - 1) * 25;
				wchar_t spot[256];				
				wsprintfW(spot, L"%d", num);				//generates spot number

					//displays vertical rows
				if (lot18Spots[num - 1] != "N")								//spot availible show number
					CreateWindowW(L"Static", spot, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(i * 200) + 100, buttonHeight*3 + 5 + (j * 25), buttonHeight, buttonHeight, hLot18, NULL, NULL, NULL);
				else																		//spot not availible dont show number
					CreateWindowW(L"Static", L"", COLOR_WINDOW | WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(i * 200) + 100, buttonHeight*3 + 5 + (j * 25), buttonHeight, buttonHeight, hLot18, NULL, NULL, NULL);
			}
		}
		return hLot18;
	}
	/****************************
	 ****	lot15	*****
	 ****************************/
	HWND ParkingLot::lot15(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hLot15 = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);			//opens new window
		EnableScrollBar(hLot15, SB_BOTH, ESB_ENABLE_BOTH);
		int numOfSpots = 30;
		lotName = "Lot02";

		std::vector<std::string> lot15Spots;
		databaseConnect.checkSpots(lotName, lot15Spots);
		int reservedSpot = databaseConnect.getSpotNumber("Select spotnumber from " + lotName + " where ramID = (select ramid from userlogin where username = '"
			+ (std::string)user + "')");

		CreateWindowW(L"Static", L"Non Priority Lot 15", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//heading
			20, 20, buttonWidth, buttonHeight * 2, hLot15, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, hLot15, (HMENU)STDLOTMENU, NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth - (windowWidth / 6), 20, buttonWidth, buttonHeight, hLot15, (HMENU)LOG_IN, NULL, NULL);
		CreateWindowW(L"Static", L"Enter desired spot below to reserve/release", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, buttonHeight*3, buttonWidth, buttonHeight*2, hLot15, NULL, NULL, NULL);
		hSpotNumber = CreateWindowW(L"Edit", L"Spot Number", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//enter spot number from screen
			20, bottomButton - (buttonHeight + 2) * 5, buttonWidth, buttonHeight, hLot15, NULL, NULL, NULL);

		if (reservedSpot > 0) {				//user already has a spot
			CreateWindowW(L"Button", L"Release Spot", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//can release spot
				20, bottomButton - (buttonHeight + 2) * 4, buttonWidth, buttonHeight, hLot15, (HMENU)RELEASE, NULL, NULL);
			CreateWindowW(L"Button", L"Spot Taken?", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, bottomButton - (buttonHeight + 2) * 2, buttonWidth, buttonHeight, hLot15, (HMENU)TAKEN, NULL, NULL);					//can report spot taken

			wchar_t isSpot[256];
			wsprintfW(isSpot, L"%d", reservedSpot);
			std::wstring spotIsReserved = widen("You have this spot number reserved: ") + isSpot;

			CreateWindowW(L"Static", spotIsReserved.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, (buttonHeight + 2) * 5, buttonWidth, buttonHeight * 2, hLot15, NULL, NULL, NULL);
		}
		else {								//user does not have a spot
			CreateWindowW(L"Button", L"Reserve Spot", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				20, bottomButton - (buttonHeight + 2) * 4, buttonWidth, buttonHeight, hLot15, (HMENU)RESERVE, NULL, NULL);			// can reservse spot
		}

		for (int k = 1; k <= 2; k++) {
			for (int i = 1; i <= numOfSpots; i++) {
				int num = (i)+(k - 1) * numOfSpots;
				wchar_t spot[256];
				wsprintfW(spot, L"%d", num);				//generates spot number

				CreateWindowW(L"Static", spot, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//displays horizontal rows
					(i * 40) + 125, (100 * k) + 200, buttonHeight, buttonHeight, hLot15, NULL, NULL, NULL);

				//displays vertical rows
				if (lot15Spots[num - 1] != "N")								//spot availible show number
					CreateWindowW(L"Static", spot, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,//displays horizontal rows
					(i * 40) + 125, (100 * k) + 200, buttonHeight, buttonHeight, hLot15, NULL, NULL, NULL);
				else																		//spot not availible dont show number
					CreateWindowW(L"Static", L"", COLOR_WINDOW | WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,//displays horizontal rows
					(i * 40) + 125, (100 * k) + 200, buttonHeight, buttonHeight, hLot15, NULL, NULL, NULL);
			}
		}
		return hLot15;
	}
	/*
	****	reserve function	****
	********************************/
	void ParkingLot::reserve(HWND hReserve) {
		char spotNumber[256];
		unsigned int spotSelection;
		bool hasClass = false;
		bool classToday = false;
		std::vector<std::string> availSpots;

		GetWindowText(hSpotNumber, (LPSTR)spotNumber, 128);								//get spot number selected

		for(unsigned int j = 0; j < ((std::string) spotNumber).length(); j++)
			if (((std::string) spotNumber)[j] >= 'A' && ((std::string) spotNumber)[j] <= 'Z'					//check if has letters
				|| ((std::string) spotNumber)[j] >= 'a' && ((std::string) spotNumber)[j] <= 'z') {
				MessageBoxW(hReserve, L"Enter a spot number", L"Reservation", MB_OK);
				return;
			}

		if (((std::string) spotNumber) > "0" && ((std::string) spotNumber) < std::to_string(999999)) {			//make sure it's just numbers
			spotSelection = std::stoi((std::string) spotNumber);										//if so covert to int type
		}else {
			MessageBoxW(hReserve, L"Enter a spot number", L"Reservation", MB_OK);							
			return;
		}

		wchar_t spot[256];								//generartes spot number for display
		wsprintfW(spot, L"%d", spotSelection);

		databaseConnect.checkSpots(lotName, availSpots);									//gets availibility for all spots in lot

		if (spotSelection > availSpots.size() || spotSelection < 1) {						//spot entered that is negative of greater than number of spots in lot
			MessageBoxW(hReserve, L"Spot does not exist", L"Reservation", MB_OK);
			return;
		}

		if (availSpots[spotSelection - 1] == "N")										//if unavailable spot is selected
			MessageBoxW(hReserve, L"Spot Already Taken", L"Reservation", MB_OK);
		else {
			if (userGroup == "A")													//if admin reserves spot overide
				databaseConnect.insertStatement("exec AdminReserveUpdate");
			else if (userGroup == "U") {											//if student
				if (lotName == "Lot01") {												//and is priority lot
					std::vector<std::string> classTimes;									
					std::string timeRN = getCurrentTime();								//get current time in EST
					std::string plus = getCurrentTimePlus(15);							//and current time plus 15
					std::string weekday = getWeekDay();								//get today's date

					databaseConnect.getStartOfClasses((std::string)user, classTimes);		//find start time of each class of students schedule

					for (unsigned int k = 6; k < classTimes.size(); k++)					//check if has class today
						for (unsigned int g = 0; g < classTimes[k].size(); g += 4)
							if (classTimes[k][g] == weekday[0] && classTimes[k][g + 1] == weekday[1]
								&& classTimes[k][g + 2] == weekday[2])
								classToday = true;

					if (classToday != true) {													//if not deny them
						MessageBoxW(hReserve, L"You do not have a class today", L"Cannot make reservation", MB_OK);
						return;
					}
					for (unsigned int i = 0; i < classTimes.size()/2; i++)					//if any class start time is within 15 min interval
						if (classTimes[i] < plus && classTimes[i] > timeRN) 
							hasClass = true;												//they may reserve a spot

					if (hasClass != true) {													//if not deny them
						MessageBoxW(hReserve, L"You do not have a class with in the 15 min", L"Cannot make reservation", MB_OK);
						return;
					}
				}
			}
			//update lot to n available with username and ram id
			std::string update = "UPDATE " + lotName + " SET Available = 'N', UserName = '" + (std::string) user
				+ "', RamID = (Select ramID from userlogin where username = '"
				+ (std::string) user + "') WHERE SpotNumber = " + (std::string) spotNumber;

			databaseConnect.reserveSpot((std::string) update);
			MessageBoxW(hReserve, spot, L"You Reserved Spot Number", MB_OK);						//tell user spot is reserved

			if (userGroup == "U")											//return to parking lot menu for...
				hReserve = studentParkingLotMenu(hReserve);						//student
			else
				hReserve = staffParkingLotMenu(hReserve);						//staff
		}
	}
	/*
	****	release function	****
	********************************/
	void ParkingLot::release(HWND hRelease) {
		char spotNumber[256];
		int spotSelection;
		int theirSpot = databaseConnect.getSpotNumber("select spotnumber from " + lotName + " where username = '" + (std::string)user + "'");

		GetWindowText(hSpotNumber, (LPSTR)spotNumber, 128);

		for (unsigned int j = 0; j < ((std::string) spotNumber).length(); j++)
			if (((std::string) spotNumber)[j] >= 'A' && ((std::string) spotNumber)[j] <= 'Z'
				|| ((std::string) spotNumber)[j] >= 'a' && ((std::string) spotNumber)[j] <= 'z') {
				MessageBoxW(hRelease, L"Enter a spot number", L"Reservation", MB_OK);
				return;
			}

		if (((std::string) spotNumber) > "0" && ((std::string) spotNumber) < std::to_string(999999)) {
			spotSelection = std::stoi((std::string) spotNumber);
		}
		else {
			MessageBoxW(hRelease, L"Enter a spot number", L"Reservation", MB_OK);
			return;
		}

		if (spotSelection == theirSpot) {
			wchar_t spot[256];								//generartes spot number
			wsprintfW(spot, L"%d", spotSelection);

			std::string update = "UPDATE " + lotName + " SET Available = 'Y', UserName = NULL, RamID = NULL WHERE SpotNumber = " + (std::string) spotNumber;

			databaseConnect.insertStatement("exec AdminReserveUpdate");
			databaseConnect.reserveSpot((std::string) update);
			MessageBoxW(hRelease, spot, L"You Released Spot Number", MB_OK);

			if (userGroup == "U")
				hRelease = studentParkingLotMenu(hRelease);
			else
				hRelease = staffParkingLotMenu(hRelease);
		}
		else {
			MessageBoxW(hRelease, L"You do not have this spot reserved", L"Spot Release", MB_OK);
		}
	}
						/************			ADMIN FUNCTIONS					*******************/	
	/*
	****	administrator	****
	****************************/
	HWND ParkingLot::administrator(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hAdmin = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);				//open new window
		EnableScrollBar(hAdmin, SB_BOTH, ESB_ENABLE_BOTH);

		CreateWindowW(L"Static", L"ADMINISTRATOR", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,				//heading
			headingCenterX, (buttonHeight + 2) * 1, headingWidth, headingHeight, hAdmin, NULL, NULL, NULL);
		hUserName = CreateWindowW(L"Edit", L"User Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			centerX, centerY, buttonWidth, buttonHeight, hAdmin, NULL, NULL, NULL);					//enter user name from screen
		hPassword = CreateWindowW(L"Edit", L"Password", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL,
			centerX, centerY + (buttonHeight + 2) * 1, buttonWidth, buttonHeight, hAdmin, NULL, NULL, NULL);				//password
		CreateWindowW(L"Button", L"Enter", WS_VISIBLE | WS_CHILD,
			centerX, centerY + (buttonHeight + 2) * 2, buttonWidth, buttonHeight, hAdmin, HMENU(ADMINOPTS), NULL, NULL);		//SIGN_IN case
		
		return hAdmin;
	}
	/*
	****	adminOptions function	****
	*************************************/
	void ParkingLot::adminOptions(HWND hAdminOpt) {
		GetWindowText(hUserName, (LPSTR)user, 128);			//get user data and store it
		GetWindowText(hPassword, (LPSTR)psswrd, 128);

		std::string query = "SELECT username, password, usergroup FROM Userlogin WHERE Username = '" + (std::string) user + "'";					//create query
	
		bool found = databaseConnect.findUser(query, (std::string) user, (std::string)psswrd, userGroup);		//run query

		if (found == true && userGroup == "A")							//login successful
			hAdminOpt = adminMenu(hAdminOpt);						//open parking Lot menu
		else											//login not successful
			MessageBoxW(hAdminOpt, L"Information Incorrect or Failed Network Connection", L"Account Not Found", MB_OK);			//display error message

	}
	/*	
	****	adminMenu function */
	HWND ParkingLot::adminMenu(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hAdminMenu = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);
		EnableScrollBar(hAdminMenu, SB_BOTH, ESB_ENABLE_BOTH);

		CreateWindowW(L"Static", L"ADMIN MENU", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//heading
			headingCenterX, (buttonHeight + 2) * 1, headingWidth, headingHeight, hAdminMenu, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Pending Reports", WS_VISIBLE | WS_CHILD,
			centerX, centerY - (buttonHeight + 2) * 1, buttonWidth*1.5, buttonHeight, hAdminMenu, (HMENU)REPORT, NULL, NULL);
		CreateWindowW(L"Button", L"View Reservations", WS_VISIBLE | WS_CHILD,
			centerX, centerY, buttonWidth *1.5, buttonHeight, hAdminMenu, (HMENU)VIEW, NULL, NULL);
		CreateWindowW(L"Button", L"Pending Requests", WS_VISIBLE | WS_CHILD,
			centerX, centerY + (buttonHeight + 2) * 1, buttonWidth*1.5, buttonHeight, hAdminMenu, (HMENU)REQUEST, NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			1200, 20, buttonWidth, buttonHeight, hAdminMenu, (HMENU)ADMIN, NULL, NULL);

		return hAdminMenu;
	}
	/*
	 ****	displayReservations function	****
	 *******************************************/
	HWND ParkingLot::displayReservations(HWND hPrev){
		int verticalMargin = 5, horizonatalMargin = 10, pushOver = buttonWidth + buttonHeight;
		DestroyWindow(hPrev);
		HWND reserveView = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);
		EnableScrollBar(reserveView, SB_BOTH, ESB_ENABLE_BOTH);
		std::string allLots[] = { "Lot01", "Lot02", "Lot03" };

		CreateWindowW(L"Static", L"Reservations", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, 20, buttonWidth, buttonHeight, reserveView, HMENU(), NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth - (windowWidth/6), 20, buttonWidth, buttonHeight, reserveView, (HMENU)ADMIN, NULL, NULL);
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, reserveView, HMENU(ADMINMENU), NULL, NULL);

		CreateWindowW(L"Static", L"Reservation ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				pushOver, (buttonHeight + 2) * 1, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"User Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			buttonWidth + horizonatalMargin + pushOver, (buttonHeight + 2)  * 1, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Lot Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 2 + pushOver, (buttonHeight + 2)  * 1, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Spot Number", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 3 + pushOver, (buttonHeight + 2)  * 1, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"License Plate", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 4 + pushOver, (buttonHeight + 2) * 1, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);

		std::vector<std::wstring> thisLot;
		int reserveID = 1;

		for (int c = 0; c < 3; c++) {
			std::vector<int> spot;
			std::vector<std::string> name, plate;
			std::vector<std::wstring> thisName, thisPlate;

			databaseConnect.getReservations(name, spot, plate, allLots[c]);
			thisLot.push_back(widen(allLots[c]));

			for (unsigned int k = 0; k < name.size(); k++) {
				wchar_t isReserved[256], isSpot[256];

				thisName.push_back(widen(name[k]));
				thisPlate.push_back(widen(plate[k]));

				wsprintfW(isSpot, L"%d", spot[k]);
				wsprintfW(isReserved, L"%d", reserveID);

				CreateWindowW(L"Static", isReserved, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					pushOver, reserveID * (buttonHeight+2), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", thisName[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					buttonWidth + horizonatalMargin + pushOver, reserveID * (buttonHeight + 2), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", thisLot[c].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(buttonWidth + horizonatalMargin) * 2 + pushOver, reserveID * (buttonHeight + 2), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", isSpot, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(buttonWidth + horizonatalMargin) * 3 + pushOver, reserveID * (buttonHeight + 2), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", thisPlate[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(buttonWidth + horizonatalMargin) * 4 + pushOver, reserveID * (buttonHeight+2), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				reserveID++;
			}
		}
		return reserveView;
	}
	/*
	 ****	displayRequests function	****
	 *******************************************/
	HWND ParkingLot::displayRequests(HWND hPrev) {
		int verticalMargin = 5, horizonatalMargin = 10, pushOver = buttonWidth + buttonHeight;
		DestroyWindow(hPrev);
		HWND reserveView = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);
		EnableScrollBar(reserveView, SB_BOTH, ESB_ENABLE_BOTH);
		std::vector<int> spots;
		std::vector<std::string> name, lot, date, event;

		CreateWindowW(L"Static", L"Pending Requests", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, 20, buttonWidth, buttonHeight, reserveView, HMENU(), NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth -(windowWidth/6), 20, buttonWidth, buttonHeight, reserveView, (HMENU)ADMIN, NULL, NULL);
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, reserveView, (HMENU)ADMINMENU, NULL, NULL);

		CreateWindowW(L"Static", L"Request ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			pushOver, (buttonHeight +2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"User Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			buttonWidth + horizonatalMargin + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Lot Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 2 + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Number Spots", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 3 + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Event", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 4 + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Event Date", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 5 + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);

		databaseConnect.getRequests(name, lot, event, date, spots);
		std::vector<std::wstring> thisName, thisEvent, thisLot, thisDate;

			for (unsigned int k = 0; k < name.size(); k++) {
				wchar_t isReserved[256], aSpot[256];
				thisName.push_back(widen(name[k]));
				thisEvent.push_back(widen(event[k]));
				thisLot.push_back(widen(lot[k]));
				thisDate.push_back(widen(date[k]));

				wsprintfW(aSpot, L"%d", spots[k]);
				wsprintfW(isReserved, L"%d", k + 1);

				CreateWindowW(L"Static", isReserved, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", thisName[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					buttonWidth + horizonatalMargin + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", thisLot[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(buttonWidth + horizonatalMargin) * 2 + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", aSpot, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(buttonWidth + horizonatalMargin) * 3 + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", thisEvent[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(buttonWidth + horizonatalMargin) * 4 + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
				CreateWindowW(L"Static", thisDate[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
					(buttonWidth + horizonatalMargin) * 5 + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
			}
		return reserveView;
	}
	/*
	 ****	displayRequests function	****
	 *******************************************/
	HWND ParkingLot::displayReports(HWND hPrev){
		int verticalMargin = 5, horizonatalMargin = 10, pushOver = buttonWidth + buttonHeight;
		DestroyWindow(hPrev);
		HWND reserveView = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);
		EnableScrollBar(reserveView, SB_BOTH, ESB_ENABLE_BOTH);
		std::vector<int> spot;
		std::vector<std::string> name, plate, lot, lcnsPlt;

		CreateWindowW(L"Static", L"Pending Reports", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, 20, buttonWidth, buttonHeight, reserveView, HMENU(), NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth -(windowWidth/6), 20, buttonWidth, buttonHeight, reserveView, (HMENU)ADMIN, NULL, NULL);
		CreateWindowW(L"Button", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth, buttonHeight, reserveView, (HMENU)ADMINMENU, NULL, NULL);

		CreateWindowW(L"Static", L"Report ID", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"User Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			buttonWidth + horizonatalMargin + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Lot Name", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 2 + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"Spot Taken", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 3 + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		CreateWindowW(L"Static", L"License Plate", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			(buttonWidth + horizonatalMargin) * 4 + pushOver, (buttonHeight + 2) * verticalMargin, buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);

		databaseConnect.getReports(name, lot, spot, lcnsPlt);
		std::vector<std::wstring> thisName, thisPlate, thisLot;

		for (unsigned int k = 0; k < name.size(); k++) {
			wchar_t isReserved[256], isSpot[256];
			thisName.push_back(widen(name[k]));
			thisPlate.push_back(widen(lcnsPlt[k]));
			thisLot.push_back(widen(lot[k]));

			wsprintfW(isSpot, L"%d", spot[k]);
			wsprintfW(isReserved, L"%d", k + 1);

			CreateWindowW(L"Static", isReserved, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
			CreateWindowW(L"Static", thisName[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				buttonWidth + horizonatalMargin + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
			CreateWindowW(L"Static", thisLot[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				(buttonWidth + horizonatalMargin) * 2 + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
			CreateWindowW(L"Static", isSpot, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				(buttonWidth + horizonatalMargin) * 3 + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
			CreateWindowW(L"Static", thisPlate[k].c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
				(buttonWidth + horizonatalMargin) * 4 + pushOver, (buttonHeight + 2) * (k + 1 + verticalMargin), buttonWidth, buttonHeight, reserveView, NULL, NULL, NULL);
		}
		return reserveView;
	}
	/*
	****	spotTaken function	****
	*********************************/
	HWND ParkingLot::spotTaken(HWND hPrev) {
		DestroyWindow(hPrev);
		HWND hTaken = CreateWindowW(L"myWindowClass", L"Parking Registration", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			0, 0, windowWidth, windowHeight, NULL, NULL, NULL, NULL);			//opens new window
		EnableScrollBar(hTaken, SB_BOTH, ESB_ENABLE_BOTH);

		CreateWindowW(L"Static", L"Stolen Spot Report", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,		//heading
			headingCenterX, 20, headingWidth, headingHeight, hTaken, NULL, NULL, NULL);
		CreateWindowW(L"Button", L"Back to Parking Lot Menu", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			20, bottomButton, buttonWidth + (buttonHeight * 2), buttonHeight, hTaken, (HMENU)STDLOTMENU, NULL, NULL);
		CreateWindowW(L"Button", L"Log Out", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
			windowWidth - (windowWidth/6), 20, buttonWidth, buttonHeight, hTaken, (HMENU)LOG_IN, NULL, NULL);

		int stolenSpot = databaseConnect.getSpotNumber("select spotnumber from "
			+ lotName + " where username = '" + (std::string) user + "'");

		hPlate = CreateWindowW(L"Edit", L"Enter license plate number of the car that took you spot", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_MULTILINE | ES_AUTOVSCROLL,
			centerX, centerY, buttonWidth, buttonHeight * 3, hTaken, NULL, NULL, NULL);

		CreateWindowW(L"Button", L"Send Report and Get New Spot", WS_VISIBLE | WS_CHILD | SS_CENTER,
			centerX - buttonHeight * 2, centerY + (buttonHeight +2) * 3, buttonWidth + buttonHeight * 3, buttonHeight, hTaken, HMENU(SEND), NULL, NULL);
		return hTaken;
	}
	/*
		sendReport function	****
	*********************************/
	void ParkingLot::sendReport(HWND hStolen) {
		char licensePlate[256];

		GetWindowText(hPlate, (LPSTR)licensePlate, 128);

		if (((std::string) licensePlate).length() > 8) {		//license plate has more characters than max (8)
			MessageBoxW(hStolen, L"License plate number cannot be greater than 8 characters", L"Notice", MB_OK);
			return;
		}
		if (((std::string) licensePlate) == "") {		//license plate has more characters than max (8)
			MessageBoxW(hStolen, L"Enter the license plate number", L"Notice", MB_OK);
			return;
		}

		std::string getSpot = "select spotnumber from "
			+ lotName + " where username = '" + (std::string) user + "'";

		int stolenSpot = databaseConnect.getSpotNumber(getSpot);

		std::string insert = "insert into pendingReports (username, lotname, spotnumber, licenseplate) values('" 
			+ (std::string)user + "','" + lotName + "'," + std::to_string(stolenSpot) + ",'" + (std::string)licensePlate + "')";

		databaseConnect.insertStatement(insert);

		MessageBoxW(hStolen, L"Report Sent", L"Campus police will be notified", MB_OK);

		if (userGroup == "U")
			hStolen = studentParkingLotMenu(hStolen);
		else
			hStolen = staffParkingLotMenu(hStolen);
	}
	/*
	 ****	widen function	****
	***************************/
	std::wstring ParkingLot::widen(const std::string& s){
		std::vector<wchar_t> buf(MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, 0, 0));
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, &buf[0], buf.size());
		return std::wstring(&buf[0]);
	}
	/*
	 ****	getCurrentTime function	****
	************************************/
	std::string ParkingLot::getCurrentTime() {
		time_t curr_time;

		struct tm* timeinfo;
		timeinfo = new tm;
		char buffer[80];

		time(&curr_time);
		localtime_s(timeinfo, &curr_time);

		strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
		std::string str(buffer);

		return str;
	}
	std::string ParkingLot::getCurrentTimePlus(int addedTime) {
		time_t curr_time;
		struct tm* timeinfo;
		timeinfo = new tm;
		char buffer[80];
		int minuteLimit = 60 - addedTime;

		time(&curr_time);
		localtime_s(timeinfo, &curr_time);

		if (timeinfo->tm_min >= minuteLimit) {
			if (timeinfo->tm_hour >= 23) {
				timeinfo->tm_hour -= 23;
				timeinfo -= minuteLimit;
			}
			else {
				timeinfo->tm_hour++;
				timeinfo->tm_min -= minuteLimit;
			}
		}
		else
			timeinfo->tm_min += addedTime;

		strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
		std::string str(buffer);

		return str;
	}

	/*
	****	getWeekDay function	*****
	**********************************/
	std::string ParkingLot::getWeekDay() {
		time_t t;
		struct tm* timePtr;
		timePtr = new tm;
		char buffer[80];

		time(&t);
		localtime_s(timePtr, &t);
		strftime(buffer, sizeof(buffer), "%a", timePtr);

		std::string day(buffer);

		std::cout << "Day\t" + day + '\n';

		return day;
	}