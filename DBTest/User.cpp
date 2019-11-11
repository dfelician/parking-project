#include "User.h"

string User::getUserName() {
	return userName;
}

void User::setUserName(string inUserName) {
	userName = inUserName;
}

string User::getUserGroup() {
	return userGroup;
}

void User::setUserGroup(string uG) {
	userGroup = uG;
}
