#include "sendFacultyRequest.h"

vector<string> sendFacultyRequest(string userName, int i) {
	int selection;
	string lotName = "";
	int spotSelection;
	vector<string> checkSpot;
	vector<string> selectedSpots;
	//SQLQuery = "UPDATE PriorityParkingLot SET Number_Of_Spots_Reserved = Number_Of_Spots_Reserved + 1, Number_of_Spots_Available = Number_of_Spots_Available - 1 WHERE LotID = 'P01'";

	cout << "\n---------Multiple Spot Request---------\n";

	cout << "Which lot do you need?:\n1.Lot01\n2.Lot02\n3.Lot03\n";
	cin >> selection;
	if (selection == 1) {
		lotName = "Lot01";
		checkSpot = checkSpots(lotName);

		cout << "Which spots would you like and enter 0 when finished: " << endl;
		cin >> spotSelection;
		while(spotSelection != 0){

			if (checkSpot[spotSelection - 1] == "N") {
				cout << "Spot already taken." << endl;
			}
			else if (checkSpot[spotSelection - 1] == "Y") {
				cout << "Spot " << spotSelection << "added to request" << endl;
				selectedSpots.push_back(to_string(spotSelection));
			}
			cin >> spotSelection;
		}
	}
	return selectedSpots;
}