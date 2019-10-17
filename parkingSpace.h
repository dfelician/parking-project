#include <iostream>
using namespace std;

/***
 ***    parkingSpace    ***
 **************************/
struct parkingSpace{
  int spotNumber;
  bool reserved;
  string student;
  parkingSpace *nextSpot;

  parkingSpace(){
      this->reserved = false;
      this->student = "";
      this->nextSpot = NULL;
  }
};
/***
 ***    LotRow    ***
 ********************/
class LotRow{
  private:
    parkingSpace* firstSpot, *anotherSpot;
  public:
    LotRow(int numOfSpots){
      firstSpot = new parkingSpace;
      firstSpot->spotNumber = 1;
      anotherSpot = firstSpot;

      cout << "Spot " << anotherSpot->spotNumber << " has been added.\n";
      
      for(int i = 1; i < numOfSpots; i++){
        parkingSpace* temp = new parkingSpace;
        temp->spotNumber = i + 1;

        anotherSpot->nextSpot = temp;
        anotherSpot = temp;
        
        cout << "Spot " << anotherSpot->spotNumber << " has been added.\n";
      }
    }
    ~LotRow(){
      while (firstSpot != NULL){
        parkingSpace* temp;
        temp = firstSpot;

        cout << "Spot " << firstSpot->spotNumber << " has been removed.\n";
        firstSpot = firstSpot->nextSpot;
        delete temp;
      }
      delete firstSpot;
    }
    void reserveSpot(string student, int spotNumber){
        
    }
};
