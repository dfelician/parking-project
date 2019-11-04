#ifndef CHECKSPOTS_H
#define CHECKSPOTS_H

#include <string>
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include "signIn.h"
#include <vector>

using namespace std;

vector<string> checkSpots(string lotName);

#endif