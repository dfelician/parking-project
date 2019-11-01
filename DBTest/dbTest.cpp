
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>

#include "signIn.h"
#include "register.h"
 
using namespace std;

int main()
{
	int choice;
	cout << "Enter 1 to Login or 2 to Register" << endl;
	cin >> choice;

	if (choice == 1) {
		signIn();
	}
	else if (choice == 2) {
		registerUser();
	}
	else {
		cout << "Invalid input" << endl;
	}
}