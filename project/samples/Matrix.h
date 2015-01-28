#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <iostream>
#include <fstream>

class Matrix {
public:
    Matrix(int i, int j) ;
    Matrix (const Matrix& m) ;

    int numRows ( ) ;
    int numCols ( ) ;

    int getRows ( ) const;
    int getCols ( ) const;

    float *access(const int i, const int j) const ;
    friend std::ostream& operator<<(std::ostream &os, Matrix &m) ;

    static Matrix readMatrix ( std::string filename ) ;

private:
    Matrix() { }
    int rows ;
    int cols ;

    /* Your implementation of "data" may vary.  There are ways in
       which data can be an array of arrays and thus simplify the
       access method, at the cost of complicating the process of
       allocating space for data.  The choice is entirely up to
       you. */
    float *data ;
} ;

#endif // MATRIX_H
