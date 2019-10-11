#include <iostream>
#include <string>
using namespace std;

/***
 ***    parkingSpace    ***
 **************************/
class parkingSpace{
    bool reserved;
    string student;
    parkingSpace *nextSpot;
    
    parkingSpace(){
        reserved = false;
        nextSpot = NULL;
        student = "";
    }
    void reserveSpot(string student, int spotNumber){
        
    }
};
/**************************
 ***    parkingLotMenu  ***
 **************************/
char parkingLotMenu(){
    char slct;
    
    cout << "(A) Student Lot 3\n";
    cout << "(B) Student Lot 5\n";
    cout << "(C) Student Lot 18\n";
    cout << "\nSelect a lot.\n";
    cin >> slct;
    
    return slct;
}

int main(){
    char select;
    
    do{
        select = parkingLotMenu();
    } while (select != 'A' && select != 'B' && select != 'C');
    
    if(select == 'A'){
        
    }else if(select == 'B'){
        
    }else{
        
    }
    
    return 0;
}
