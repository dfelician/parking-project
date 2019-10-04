
#include <iostream>
#include <fstream>
#include <sstream>


void createAccount();
bool signIn();

int main()
{
	std::string login = "L";
	std::string newAccount = "C";
	std::string input;

	std::cout << "Enter L to Login or C to create a new account" << std::endl;
	std::cin >> input;

	if (input == login) {
		signIn();
	}
	else if (input == newAccount) {
		createAccount();
		signIn();
	}
	else {
		std::cout << "Invalid input\n";
	}
}

void createAccount() {
	std::string userName;
	std::string password;

	std::cout << "New username: ";
	std::cin >> userName;

	std::cout << "New password: ";
	std::cin >> password;


	std::ofstream userFile;
	userFile.open("userData\\" + userName + ".txt");
	userFile << userName << std::endl << password;
	userFile.close();
}

bool signIn() {
	std::string userName, password, readUserName, readPassword;

	std::cout << "Enter username: \n"; 
	std::cin >> userName;
	std::cout << "Enter password: \n"; 
	std::cin >> password;

	std::ifstream userData("userData\\" + userName + ".txt");
	std::getline(userData, readUserName);
	std::getline(userData, readPassword);

	bool success = false;

	while (success == false) {
		if (readUserName == userName && readPassword == password) {
			std::cout << "Success";
			return true;
		}
		else {
			std::cout << "Incorrect Username/Password\n";
			std::cout << "Enter username: \n";
			std::cin >> userName;
			std::cout << "Enter password: \n";
			std::cin >> password;
			std::ifstream userData("userData\\" + userName + ".txt");
			std::getline(userData, readUserName);
			std::getline(userData, readPassword);
		}
	}
}

