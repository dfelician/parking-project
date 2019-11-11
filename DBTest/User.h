#include <string>
using namespace std;

class User {
private:
	string userName;
	string userGroup;
public:
	string getUserName();
	void setUserName(string inUserName);

	string getUserGroup();
	void setUserGroup(string uG);
};
