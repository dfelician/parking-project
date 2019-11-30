#include "signIn.h"
using namespace std;

void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
		// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
		cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << endl;
}

void signIn() {
	User user;
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

	string inputUserName;
	string inputPassword;

	bool success = false;
	while (success == false) {
		cout << "\n---------------------------Login Page-------------------------------\n";
		//cout<<"---If you are a guest enter Guest as the username and 0 as the password---\n";

		cout << "Enter username: \n";
		cin >> inputUserName;
		cout << "Enter password: \n";
		cin >> inputPassword;

		if (inputUserName == "Guest") {
			success = true;
			user.setUserName("Guest");
			string reserveStatus = reserveSpot(user.getUserName());
			cout << reserveStatus << endl;
		}
		else {
			string SQLQuery = "SELECT * FROM UserLogin WHERE UserName = '" + inputUserName + "'";
			do {
				if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
					// Allocates the environment
					break;

				if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
					// Sets attributes that govern aspects of environments
					break;

				if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
					// Allocates the connection
					break;

				if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
					// Sets attributes that govern aspects of connections
					break;

				SQLCHAR retConString[1024]; // Conection string
				switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
					// Establishes connections to a driver and a data source
				case SQL_SUCCESS:
					break;
				case SQL_SUCCESS_WITH_INFO:
					break;
				case SQL_NO_DATA_FOUND:
					showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
					retCode = -1;
					break;
				case SQL_INVALID_HANDLE:
					showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
					retCode = -1;
					break;
				case SQL_ERROR:
					showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
					retCode = -1;
					break;
				default:
					break;
				}

				if (retCode == -1)
					break;

				if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
					// Allocates the statement
					break;

				if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery.c_str(), SQL_NTS)) {
					// Executes a preparable statement
					showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
					break;
				}
				else {
					char userName[256];
					char password[256];
					char ramID[256];
					char userGroup[256];
					int parkingPermit;
					//int enrolled[256];
					//int activePermit[256];
					while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
						// Fetches the next rowset of data from the result
						SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &userName, sizeof(userName), NULL);
						SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &password, sizeof(password), NULL);
						SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &ramID, sizeof(ramID), NULL);
						SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &userGroup, sizeof(userGroup), NULL);
						SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &parkingPermit, sizeof(parkingPermit), NULL);
						if (inputUserName == userName && inputPassword == password) {
							user.setUserName(userName);
							user.setUserGroup(userGroup);
							cout << "Signed On " << user.getUserName() << " Permit Number: " << parkingPermit<< endl;
							success = true;
						}
					}
				}
			} while (FALSE);



			SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
			SQLDisconnect(SQLConnectionHandle);
			SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
			SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
		}
		// Frees the resources and disconnects

		if (success == false) {
			cout << "Incorrect username or password" << endl;
		}
	}


	if (user.getUserGroup() == "A") {
		int s;
		cout << "----------Administrator Menu----------" << endl;
		cout << "What would you like to do?" << endl;
		cout << "1. Print Lot Report" << endl;
		cout << "2.View Student Reports" << endl;
		cout << "3. View Faculty Requests" << endl;
		cin >> s;
		if (s == 1) {
			lotReport();
		}
		else if (s == 2 || s == 3) {
			viewStudentReport(s);
		}
		
	}
	else if (user.getUserGroup() == "F") {
		int s;
		cout << "---------Faculty Menu---------" << endl;
		cout << "What would you like to do?" << endl;
		cout << "1. Send Multiple Reserve Requests\n";
		cin >> s;
		if (s == 1) {
			sendFacultyRequest(user.getUserName());
		}
	}
	else if(user.getUserName() != "Guest") {
		int select;
		cout << "---------Student Menu---------" << endl;
		cout << "What would you like to do?" << endl;
		cout << "1.Reserve a spot\n2.Release your spot\n3.Report your spot taken\n";
		cin >> select;
		if (select == 1) {
			string reserveStatus = reserveSpot(user.getUserName());
			cout << reserveStatus << endl;
		}
		else if (select == 2) {
			releaseSpot(user.getUserName());
		}
		else if (select == 3) {
			sendStudentReport(user.getUserName());
		}
	}
}

