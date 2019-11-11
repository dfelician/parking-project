#include "storedProc.h"

void storedProc() {
	User user;
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

	bool success = false;
	string SQLQuery = "exec dbo.ReservationProc";


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
			cout << "Success" << endl;
			int id;
			char ramID[256];
			//int enrolled[256];
			//int activePermit[256];
			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				// Fetches the next rowset of data from the result
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &id, sizeof(id), NULL);
				//SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &password, sizeof(password), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &ramID, sizeof(ramID), NULL);
				//	SQLGetData(SQLStatementHandle, 4, SQL_C_NUMERIC, &enrolled, sizeof(enrolled), NULL);
				//  SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &activePermit, sizeof(activePermit), NULL);
				//cout << "ID: " << id << " Name: " << fName << " " << lName << " Enrolled: " << enrolled << " Active Permit: " << activePermit << endl;
				//cout << "Username : " << userName << "Password: " << password << "RAM ID: " << ramID << endl;
				cout << "ID " << id << " RamID " << ramID << endl;
			}
		}
	} while (FALSE);



	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

	// Frees the resources and disconnects

}