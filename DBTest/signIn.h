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


void showSQLError(unsigned int handleType, const SQLHANDLE& handle);
void signIn();

#endif
