#include <iostream>
using namespace std;

class parkingSpace{
    bool reserved;
    string student;
    parkingSpace *nextSpot;
    int spotNumber;
    
  public:
    parkingSpace(){
        reserved = false;
        nextSpot = NULL;
#include <iostream>
using namespace std;

class parkingSpace{
    bool reserved;
    string student;
    parkingSpace *nextSpot;
    int spotNumber;
    
  public:
    parkingSpace(){
        reserved = false;
        nextSpot = NULL;
        student = "";
    }
    void setSpaceNum(int spotNumber){
      this->spotNumber = spotNumber;
    }
    int getSpaceNum(){
      return this->spotNumber;
    }
    void reserveSpot(string student, int spotNumber){
        
    }
};
int main() {
  parkingSpace *parkingLot[10];

  for(int i = 0; i < 10; i++){
    parkingLot[i] = new parkingSpace;
    parkingLot[i]->setSpaceNum(i);
  }
  for(int i = 0; i < 10; i++){
    cout << parkingLot[i]->getSpaceNum() << endl;
  }
}        student = "";
    }
    void reserveSpot(string student, int spotNumber){
        
    }
};
int main() {
  parkingSpace *row[10];

  for(int i = 0; i < 10; i++){
    row[i] = &i;
    cout << *(row[i]) << endl; 
  }
}
