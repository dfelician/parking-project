#include <iostream>
#include <string>
#include <vector>
#include "parkingSpace.h"
using namespace std;

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
    /*char select;
    
    do{
        select = parkingLotMenu();
    } while (select != 'A' && select != 'B' && select != 'C');
    
    if(select == 'A'){

    }else if(select == 'B'){
        
    }else if(select == 'C'){
        
    }else{
        
    }*/

    LotRow studentLot3[] = {2};
    LotRow studentLot5[] = {3, 2};
    return 0;
}
