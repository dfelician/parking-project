
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>

#include "signIn.h"
#include "register.h"
#include "storedProc.h"
 
using namespace std;

int main()
{
	cout << "-------------Parking Reservation System---------------\n";

	int choice;
	cout << "\tEnter 1 to Login or 2 to Register" << endl;
	cin >> choice;

	if (choice == 1) {
		signIn();
	}
	else if (choice == 2) {
		registerUser();
	}
	else if (choice == 3) {
		storedProc();
	}
	else {
		cout << "Invalid input" << endl;
	}

	system("pause");

	return 0;
}