#include <iostream>
#include "Matrix.h"
#include <math.h>
using namespace std;
int main ( ) {
    int x ;
    x = 0 ;

    int y ;
    for (y = 1; y <= 10; y ++  ) {
        cout << y ;
        cout << "\n" ;
        x = (x + y) ;
    }

    cout << x ;
    cout << "\n" ;
}

