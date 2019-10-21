#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

class ConnectToDB {
	SQLHANDLE SQLEnvHandle;
	SQLHANDLE SQLConnectionHandle;
	SQLHANDLE SQLStatementHandle;
	SQLRETURN retCode;
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	bool success = false;

	void showSQLError(unsigned int handleType, const SQLHANDLE& handle) {
		if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
			// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
			std::cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << std::endl;
	}
public:
	ConnectToDB() {
		SQLHANDLE SQLEnvHandle = NULL;
		SQLHANDLE SQLConnectionHandle = NULL;
		SQLHANDLE SQLStatementHandle = NULL;
		SQLRETURN retCode = 0;
	}
	bool queryDB(std::string SQLQuery, std::string userInput){
		const std::string userVar = userInput;

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
			switch (SQLDriverConnect(SQLConnectionHandle, NULL,
				(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:seniorproject4.database.windows.net, 1433; DATABASE=ParkingLot; UID=seniorproject; PWD=P@ssword;",
				SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT)) {
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
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &userInput, sizeof(userInput), NULL);
				if(userInput == userVar)
					success = true;
			}
		} while (false);

		SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
		SQLDisconnect(SQLConnectionHandle);
		SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
		SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
		// Frees the resources and disconnects

		return success;
	}
};
