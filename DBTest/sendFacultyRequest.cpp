#include "sendFacultyRequest.h"

void sendFacultyRequest(string userName) {
	int selection;
	string lotName = "";
	int spotSelection;
	vector<string> checkSpot;
	vector<string> selectedSpots;
	string nameOfEvent;
	string dateOfEvent;
	int numSpots;
	//SQLQuery = "UPDATE PriorityParkingLot SET Number_Of_Spots_Reserved = Number_Of_Spots_Reserved + 1, Number_of_Spots_Available = Number_of_Spots_Available - 1 WHERE LotID = 'P01'";

	cout << "\n---------Multiple Spot Request---------\n";

	cout << "Which lot do you need?:" <<endl << "1.Lot01" <<endl << "2.Lot02" <<endl << "3.Lot03"<<endl;
	cin >> selection;
	if (selection == 1) {
		lotName = "Lot01";
	}
	
	cout << "What is the name of your event?" << endl;
	cin >> nameOfEvent;
	cout << "What is the date of your event? MM-DD-YY" << endl;
	cin >> dateOfEvent;
	cout << "How many spots do you need?" << endl;
	cin >> numSpots;
	
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	string SQLQuery = "INSERT INTO PendingRequests(userName,lotName,dateOfEvent,numSpots,eventName,Approved) VALUES ('" + userName + "','" + lotName + "','" + dateOfEvent + "','" + to_string(numSpots) + "','" + nameOfEvent + "','N')";
	
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
			cout << "Request sent" << endl;
		}
	} while (FALSE);


	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}