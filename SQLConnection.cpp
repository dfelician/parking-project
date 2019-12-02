#include "SQLConnection.h"

void ConnectToDB::showSQLError(unsigned int handleType, const SQLHANDLE& handle) {

	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
		// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
		std::cout << "SQL driver message: " << message << "\nSQL state: " << SQLState << "." << std::endl;
}
bool ConnectToDB::findUser(std::string SQLQuery, std::string nameInput, std::string passInput, std::string &usrGrp) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	bool success = false;

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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			char username[256], password[256], grp[256];

			username[0] = '\0';
			password[0] = '\0';
			grp[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &username, sizeof(username), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &password, sizeof(password), NULL);
				SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &grp, sizeof(grp), NULL);

				if ((std::string)username == nameInput && (std::string)password == passInput) {
					success = true;
					usrGrp = grp;
				}
			}
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Frees the resources and disconnects

	return success;
}
bool ConnectToDB::findCourse(std::string SQLQuery, std::string courseInput) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			char course[256];

			course[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &course, sizeof(course), NULL);

				if ((std::string)course == courseInput) {
					SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
					SQLDisconnect(SQLConnectionHandle);
					SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
					SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

					return true;
				}
			}
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Frees the resources and disconnects

	return false;
}
int ConnectToDB::getSpotNumber(std::string SQLQuery) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	int permit;

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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &permit, sizeof(permit), NULL);

				SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
				SQLDisconnect(SQLConnectionHandle);
				SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
				SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
				// Frees the resources and disconnects

				return permit;
			}
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Frees the resources and disconnects

	return -1;
}
bool ConnectToDB::getStudent(std::string SQLQuery, std::string newStudent, std::string newRam) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			char std[256];
			char rmD[256];

			std[0] = '\0';
			rmD[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &std, sizeof(std), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &rmD, sizeof(rmD), NULL);

				if (std != NULL || newRam == rmD) {
					SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
					SQLDisconnect(SQLConnectionHandle);
					SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
					SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

					return true;
				}
			}
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Frees the resources and disconnects

	return false;
}
void ConnectToDB::insertStatement(std::string SQLQry) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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

		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQry.c_str(), SQL_NTS)) {
			// Executes a preparable statement
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
			break;
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Frees the resources and disconnects

}
void ConnectToDB::checkSpots(std::string lotName, std::vector<std::string> &spotCheck) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

	std::string SQLQuery = "SELECT SpotNumber, Available FROM " + lotName + " order by spotnumber";


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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			int spotNumber;
			char available[256];

			available[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				// Fetches the next rowset of data from the result
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &spotNumber, sizeof(spotNumber), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &available, sizeof(available), NULL);

				spotCheck.push_back((std::string)available);
			}
		}
	} while (FALSE);



	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
void ConnectToDB::reserveSpot(std::string updateQry) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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

		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)updateQry.c_str(), SQL_NTS)) {
			// Executes a preparable statement
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
			break;
		}
	} while (FALSE);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
	// Frees the resources and disconnects

}
void ConnectToDB::getReservations(std::vector<std::string> &nameVec, std::vector<int> &spotID, 
	std::vector<std::string>& plateVec, std::string currentLot) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

	std::string SQLQuery = "select userlogin.username, SpotNumber, LicensePlateNum from "
		+ currentLot + " join UserLogin on UserLogin.RamID = "
		+ currentLot + ".RamID left outer join Permit on permit.PermitNumber = UserLogin.ParkingPermit where licenseplatenum is not null and "
		+ currentLot + ".username is not null order by SpotNumber asc";


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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			int spot;
			char name[256];
			char plate[256];

			name[0] = '\0';
			plate[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				// Fetches the next rowset of data from the result
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, sizeof(name), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &spot, sizeof(spot), NULL);
				SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &plate, sizeof(plate), NULL);;

				nameVec.push_back((std::string)name);
				
				if((std::string)plate == "")
					plateVec.push_back("No License Plate");
				else
					plateVec.push_back((std::string)plate);
				spotID.push_back(spot);
			}
		}
	} while (FALSE);



	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
void ConnectToDB::getReports(std::vector<std::string>& nameVec, std::vector<std::string> &currentLot, std::vector<int>& spotID,
	std::vector<std::string>& plateVec) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

	std::string SQLQuery = "select username, lotname, spotnumber, licenseplate from pendingreports";


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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			int spot;
			char name[256];
			char plate[256];
			char lot[256];

			name[0] = '\0';
			plate[0] = '\0';
			lot[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				// Fetches the next rowset of data from the result
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, sizeof(name), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &lot, sizeof(lot), NULL);
				SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &spot, sizeof(spot), NULL);
				SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &plate, sizeof(plate), NULL);;

				nameVec.push_back((std::string)name);

				if ((std::string)plate == "")
					plateVec.push_back("No License Plate");
				else
					plateVec.push_back((std::string)plate);
				spotID.push_back(spot);
				currentLot.push_back((std::string)lot);
			}
		}
	} while (FALSE);



	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
void ConnectToDB::getRequests(std::vector<std::string>& nameVec, std::vector<std::string>& currentLot,
	std::vector<std::string>& events, std::vector<std::string>& date, std::vector<int>& allSpots) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;

	std::string SQLQuery = "select username, lotname, convert(varchar, dateofevent,7) numspots, eventname from pendingrequests";


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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			int spots;
			char name[256];
			char evnt[256];
			char lot[256];
			char dates[256];


			name[0] = '\0';
			dates[0] = '\0';
			lot[0] = '\0';
			evnt[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				// Fetches the next rowset of data from the result
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, sizeof(name), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &lot, sizeof(lot), NULL);
				SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &dates, sizeof(dates), NULL);
				SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &spots, sizeof(spots), NULL);
				SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &evnt, sizeof(evnt), NULL);

				nameVec.push_back((std::string)name);
				events.push_back((std::string)evnt);
				date.push_back((std::string)dates);
				allSpots.push_back(spots);
				currentLot.push_back((std::string)lot);
			}
		}
	} while (FALSE);



	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
void ConnectToDB::getProfile(std::string usr, std::vector<std::string> &prflVctr, std::string GroupOfUser) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	std::string SQLQuery;

	if (GroupOfUser == "U")
		SQLQuery = "select CarMake, CarModel, LicensePlateNum, class1, (select class_name from course where crn = class1), convert(varchar, (select start_time from course where crn = class1), 8),convert(varchar,(select end_time from course where crn = class1), 8),class2, (select class_name from course where crn = class2), convert(varchar, (select start_time from course where crn = class2), 8),convert(varchar,(select end_time from course where crn = class2),8),class3, (select class_name from course where crn = class3), convert(varchar,(select start_time from course where crn = class3),8), convert(varchar,(select end_time from course where crn = class3),8),class4, (select class_name from course where crn = class4), convert(varchar, (select start_time from course where crn = class4), 8), convert(varchar,(select end_time from course where crn = class4),8), class5, (select class_name from course where crn = class5), convert(varchar, (select start_time from course where crn = class5), 8), convert(varchar, (select end_time from course where crn = class5), 8), class6, (select class_name from course where crn = class6), convert(varchar, (select start_time from course where crn = class6), 8),convert(varchar,(select end_time from course where crn = class6),8) from Permit join UserLogin on ParkingPermit = PermitNumber join schedule_test on schedule_test.ramID = UserLogin.RamID where username = '" + usr + "'";
	else
		SQLQuery = "select CarMake,CarModel,LicensePlateNum from Permit join UserLogin on ParkingPermit = PermitNumber where UserLogin.UserName = '" + usr + "'";

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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			char carMk[256], crMdel[256], pltNmbr[256], clss1[256], clss2[256], 
				clss3[256], clss4[256], clss5[256], clss6[256], class1Name[256], class2Name[256],
				class3Name[256], class4Name[256], class5Name[256], class6Name[256], class1Start[256], class2Start[256],
				class3Start[256], class4Start[256], class5Start[256], class6Start[256], class1End[256], class2End[256],
				class3End[256], class4End[256], class5End[256], class6End[256];

			carMk[0] = '\0';
			crMdel[0] = '\0';
			pltNmbr[0] = '\0';
			clss1[0] = '\0';
			clss2[0] = '\0';
			clss3[0] = '\0';
			clss4[0] = '\0';
			clss5[0] = '\0';
			clss6[0] = '\0';
			class1Name[0] = '\0';
			class2Name[0] = '\0';
			class3Name[0] = '\0';
			class4Name[0] = '\0';
			class5Name[0] = '\0';
			class6Name[0] = '\0';
			class1Start[0] = '\0';
			class2Start[0] = '\0';
			class3Start[0] = '\0';
			class4Start[0] = '\0';
			class5Start[0] = '\0';
			class6Start[0] = '\0';
			class1End[0] = '\0';
			class2End[0] = '\0';
			class3End[0] = '\0';
			class4End[0] = '\0';
			class5End[0] = '\0';
			class6End[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				// Fetches the next rowset of data from the result
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &carMk, sizeof(carMk), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &crMdel, sizeof(crMdel), NULL);
				SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &pltNmbr, sizeof(pltNmbr), NULL);
				SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &clss1, sizeof(clss1), NULL);
				SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &clss2, sizeof(clss2), NULL);
				SQLGetData(SQLStatementHandle, 6, SQL_C_DEFAULT, &clss3, sizeof(clss3), NULL);
				SQLGetData(SQLStatementHandle, 7, SQL_C_DEFAULT, &clss4, sizeof(clss4), NULL);
				SQLGetData(SQLStatementHandle, 8, SQL_C_DEFAULT, &clss5, sizeof(clss5), NULL);
				SQLGetData(SQLStatementHandle, 9, SQL_C_DEFAULT, &clss6, sizeof(clss6), NULL);
				SQLGetData(SQLStatementHandle, 10, SQL_C_DEFAULT, &class1Name, sizeof(class1Name), NULL);
				SQLGetData(SQLStatementHandle, 11, SQL_C_DEFAULT, &class2Name, sizeof(class2Name), NULL);
				SQLGetData(SQLStatementHandle, 12, SQL_C_DEFAULT, &class3Name, sizeof(class3Name), NULL);
				SQLGetData(SQLStatementHandle, 13, SQL_C_DEFAULT, &class4Name, sizeof(class4Name), NULL);
				SQLGetData(SQLStatementHandle, 14, SQL_C_DEFAULT, &class5Name, sizeof(class5Name), NULL);
				SQLGetData(SQLStatementHandle, 15, SQL_C_DEFAULT, &class6Name, sizeof(class6Name), NULL);
				SQLGetData(SQLStatementHandle, 16, SQL_C_DEFAULT, &class1Start, sizeof(class1Start), NULL);
				SQLGetData(SQLStatementHandle, 17, SQL_C_DEFAULT, &class2Start, sizeof(class2Start), NULL);
				SQLGetData(SQLStatementHandle, 18, SQL_C_DEFAULT, &class3Start, sizeof(class3Start), NULL);
				SQLGetData(SQLStatementHandle, 19, SQL_C_DEFAULT, &class4Start, sizeof(class4Start), NULL);
				SQLGetData(SQLStatementHandle, 20, SQL_C_DEFAULT, &class5Start, sizeof(class5Start), NULL);
				SQLGetData(SQLStatementHandle, 21, SQL_C_DEFAULT, &class6Start, sizeof(class6Start), NULL);
				SQLGetData(SQLStatementHandle, 22, SQL_C_DEFAULT, &class1End, sizeof(class1End), NULL);
				SQLGetData(SQLStatementHandle, 23, SQL_C_DEFAULT, &class2End, sizeof(class2End), NULL);
				SQLGetData(SQLStatementHandle, 24, SQL_C_DEFAULT, &class3End, sizeof(class3End), NULL);
				SQLGetData(SQLStatementHandle, 25, SQL_C_DEFAULT, &class4End, sizeof(class4End), NULL);
				SQLGetData(SQLStatementHandle, 26, SQL_C_DEFAULT, &class5End, sizeof(class5End), NULL);
				SQLGetData(SQLStatementHandle, 27, SQL_C_DEFAULT, &class6End, sizeof(class6End), NULL);

				prflVctr.push_back((std::string)carMk);
				prflVctr.push_back((std::string)crMdel);
				prflVctr.push_back((std::string)pltNmbr);
				prflVctr.push_back((std::string)clss1);
				prflVctr.push_back((std::string)clss2);
				prflVctr.push_back((std::string)clss3);
				prflVctr.push_back((std::string)clss4);
				prflVctr.push_back((std::string)clss5);
				prflVctr.push_back((std::string)clss6);
				prflVctr.push_back((std::string)class1Name);
				prflVctr.push_back((std::string)class2Name);
				prflVctr.push_back((std::string)class3Name);
				prflVctr.push_back((std::string)class4Name);
				prflVctr.push_back((std::string)class5Name);
				prflVctr.push_back((std::string)class6Name);
				prflVctr.push_back((std::string)class1Start);
				prflVctr.push_back((std::string)class2Start);
				prflVctr.push_back((std::string)class3Start);
				prflVctr.push_back((std::string)class4Start);
				prflVctr.push_back((std::string)class5Start);
				prflVctr.push_back((std::string)class6Start);
				prflVctr.push_back((std::string)class1End);
				prflVctr.push_back((std::string)class2End);
				prflVctr.push_back((std::string)class3End);
				prflVctr.push_back((std::string)class4End);
				prflVctr.push_back((std::string)class5End);
				prflVctr.push_back((std::string)class6End);
			}
		}
	} while (FALSE);



	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
void ConnectToDB::getStartOfClasses(std::string usr, std::vector<std::string>& classVctr) {
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLRETURN retCode = 0;
	std::string SQLQuery = "select convert(varchar(50),(select start_time from course where crn = class1),8),convert(varchar(50), (select start_time from course where crn = class2), 8),convert(varchar(50), (select start_time from course where crn = class3), 8),		convert(varchar(50), (select start_time from course where crn = class4), 8),		convert(varchar(50), (select start_time from course where crn = class5), 8),		convert(varchar(50), (select start_time from course where crn = class6), 8)		from Permit		join UserLogin on ParkingPermit = PermitNumber		join schedule_test on schedule_test.ramID = UserLogin.RamID where username = '" + usr + "'";

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
			(SQLCHAR*)"DRIVER={ODBC Driver 17 for SQL Server}; SERVER=tcp:parkingsystem.database.windows.net, 1433; DATABASE=ParkingSystem; UID=seniorproject; PWD=P@ssword;",
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
			char class1Start[256], class2Start[256],
				class3Start[256], class4Start[256], class5Start[256], class6Start[256];

			class1Start[0] = '\0';
			class2Start[0] = '\0';
			class3Start[0] = '\0';
			class4Start[0] = '\0';
			class5Start[0] = '\0';
			class6Start[0] = '\0';

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS) {
				// Fetches the next rowset of data from the result
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &class1Start, sizeof(class1Start), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &class2Start, sizeof(class2Start), NULL);
				SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &class3Start, sizeof(class3Start), NULL);
				SQLGetData(SQLStatementHandle, 4, SQL_C_DEFAULT, &class4Start, sizeof(class4Start), NULL);
				SQLGetData(SQLStatementHandle, 5, SQL_C_DEFAULT, &class5Start, sizeof(class5Start), NULL);
				SQLGetData(SQLStatementHandle, 6, SQL_C_DEFAULT, &class6Start, sizeof(class6Start), NULL);

				classVctr.push_back((std::string)class1Start);
				classVctr.push_back((std::string)class2Start);
				classVctr.push_back((std::string)class3Start);
				classVctr.push_back((std::string)class4Start);
				classVctr.push_back((std::string)class5Start);
				classVctr.push_back((std::string)class6Start);
			}
		}
	} while (FALSE);



	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}