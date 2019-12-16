#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <vector>

class ConnectToDB {
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];

	void showSQLError(unsigned int, const SQLHANDLE&);
public:
	bool findUser(std::string, std::string, std::string, std::string&);
	void insertStatement(std::string);
	void reserveSpot(std::string);
	void checkSpots(std::string, std::vector<std::string>&);
	int getSpotNumber(std::string);
	bool findCourse(std::string, std::string);
	bool findID(std::string, int);
	bool getStudent(std::string, std::string, std::string);
	void getReservations(std::vector<std::string>&, std::vector<int>&,
		std::vector<std::string>&, std::string);
	void getReports(std::vector<std::string>&, std::vector<std::string>&, 
		std::vector<int>&, std::vector<std::string>&);
	void getRequests(std::vector<int>&, std::vector<std::string>&, std::vector<std::string>&,
		std::vector<std::string>&, std::vector<std::string>&, std::vector<int>&, std::string);
	void getProfile(std::string, std::vector<std::string>&, std::string);
	void getStartOfClasses(std::string, std::vector<std::string>&);
};