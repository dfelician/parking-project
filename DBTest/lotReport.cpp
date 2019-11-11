#include "lotReport.h"
using namespace std;


void lotReport(){
	int selection;
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	string SQLQuery = "";
	string lotName = "";

	cout << "Which lot would you like a report on?\n1.Lot01\n2.Lot02\n3.Lot03\n";
	cin >> selection;

	if (selection == 1) {
		SQLQuery = "SELECT UserName, RAMID FROM Lot01";
		lotName = "Lot01";
	}
	else if (selection == 2) {
		SQLQuery = "SELECT UserName, RAMID FROM Lot02";
		lotName = "Lot02";
	}
	else if (selection == 3) {
		SQLQuery = "SELECT UserName, RAMID FROM Lot03";
		lotName = "Lot03";
	}
	else {
		cout << "Invalid input" << endl;
	}


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
		char studentID[256];
		cout << "Report for " << lotName << ":" << endl;
		cout << "UserName\t\t\t\tStudent ID" << endl;
		while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
			// Fetches the next rowset of data from the result
			SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &userName, sizeof(userName), NULL);
			SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &studentID, sizeof(studentID), NULL);
			cout << userName << "\t\t\t\t" << studentID << endl;
		}
	}
} while (FALSE);

SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
SQLDisconnect(SQLConnectionHandle);
SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
// Frees the resources and disconnects

}