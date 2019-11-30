#ifndef SIGNIN_H
#define SIGNIN_H

#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include "reserveSpot.h"
#include "User.h"
#include "releaseSpot.h"
#include <vector>
#include "lotReport.h"
#include "sendStudentReport.h"
#include "viewStudentReport.h"
#include "sendFacultyRequest.h"


void showSQLError(unsigned int handleType, const SQLHANDLE& handle);
void signIn();

#endif
