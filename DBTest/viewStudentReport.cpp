#include "viewStudentReport.h"

void viewStudentReport(int s){
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	string SQLQuery;
	if (s == 2) {
		SQLQuery = "SELECT reportID, userName, lotName, spotNumber, licensePlate FROM PendingReports";
	}
	else if (s == 3) {
		SQLQuery = "SELECT * FROM PendingRequests";
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
			if (s == 2) {
				int reportID;
				char userName[256];
				char lotName[256];
				int spotNumber;
				char licensePlate[256];
				cout << "---------Student Report List---------" << endl;
				cout << "ReportID|Username|LotName|SpotNumber|LicensePlate" << endl;
				while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
					// Fetches the next rowset of data from the result
					SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &reportID, sizeof(reportID), NULL);
					SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &userName, sizeof(userName), NULL);
					SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &lotName, sizeof(lotName), NULL);
					SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &spotNumber, sizeof(spotNumber), NULL);
					SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &licensePlate, sizeof(licensePlate), NULL);
					cout << reportID << "|" << userName << "|" << lotName << "|" << spotNumber << "|" << licensePlate << endl;
				}
			}
			else if (s == 3) {
				char userName[256];
				char lotName[256];
				TIMESTAMP_STRUCT dateOfEvent;
				int numSpots;
				char eventName[256];
				char approved[256];
				int requestNum;
				cout << "----------Faculty Requests List---------" << endl;
				cout << "RequestNum|UserName|LotName|Event Name|Date Of Event|# Spots Needed|Approved" << endl;
				while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
					// Fetches the next rowset of data from the result
					SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &userName, sizeof(userName), NULL);
					SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &lotName, sizeof(lotName), NULL);
					SQLGetData(SQLStatementHandle, 3, SQL_C_TYPE_DATE, &dateOfEvent, sizeof(dateOfEvent), NULL);
					SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &numSpots, sizeof(numSpots), NULL);
					SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &eventName, sizeof(eventName), NULL);
					SQLGetData(SQLStatementHandle, 6, SQL_C_DEFAULT, &approved, sizeof(approved), NULL);
					SQLGetData(SQLStatementHandle, 7, SQL_C_DEFAULT, &requestNum, sizeof(requestNum), NULL);
					cout << requestNum << "|" << userName << "|" << lotName << "|" << eventName << "|" << dateOfEvent.day<< "|" << numSpots << "|" << approved << endl;
				}
			}
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

	if (s == 3) {
		approveRequest();
	}
}