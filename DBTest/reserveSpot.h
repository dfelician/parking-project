#ifndef RESERVESPOT_H
#define RESERVESPOT_H

#include <string>
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include "signIn.h"
#include "checkSpots.h"


using namespace std;

string reserveSpot(string userName);

#endif
