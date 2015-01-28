#include "Matrix.h"
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;
#include <stdio.h> // For malloc
#include <string>
#include <fstream>
using std::ifstream;
#include <cstring> // strtok
#include <sstream>
#include <boost/lexical_cast.hpp>

Matrix::Matrix(int i, int j) {
    this->rows = i;
    this->cols = j;
    this->data = (float *) malloc((i * j) * sizeof(float));
}

Matrix::Matrix(const Matrix& m) {
    this->rows = m.getRows();
	// or m->numRows(); ??
    this->cols = m.getCols();
}

int Matrix::numRows() {
    int ret = this->rows;
    return ret;
}

int Matrix::numCols() {
    int ret = this->cols;
    return ret;
}

int Matrix::getRows() const {
    int ret = rows;
    return ret;
}

int Matrix::getCols() const {
    int ret = cols;
    return ret;
}



float* Matrix::access(const int i, const int j) const {
    // return address or value?
    float *ret = NULL;
    // data is a single contiguous block of memory for the matrix
    // Matrix[row][col] == data[(3 * row) + col]
    ret = &data[(cols * i) + j];
    return ret;
}

std::ostream& operator<<(std::ostream &os, Matrix &m) {
    int i, j;
    os << m.numRows() << " " << m.numCols() << "\n";
    for(i = 0; i < m.numRows(); i++) {
        for(j = 0; j < m.numCols(); j++) {
            os << *(m.access(i, j));
            os << "  ";
        }
        os << "\n";
    }
}

Matrix Matrix::readMatrix ( std::string filename ) {
    int i, j, numRows, numCols, numLines;
    Matrix m;
    const int MAX_CHARS_PER_LINE = 1024;
    const int MAX_TOKENS_PER_LINE = 512;
    const char* const DELIMITER = " ";
    const char *cstr = filename.c_str();
    std::ifstream matrix1;
    matrix1.open(cstr);
    if(!matrix1.good()) {
        m = Matrix(0,0);
        return m;
    }

    int iterCt = -1;
    while(!matrix1.eof()) {
        iterCt++;
        char buf[MAX_CHARS_PER_LINE];
        matrix1.getline(buf, MAX_CHARS_PER_LINE);
        int n = 0;
        const char* token[MAX_TOKENS_PER_LINE] = {};
        token[0] = strtok(buf, DELIMITER);
        if (token[0]) {
            for (n = 1; n < MAX_TOKENS_PER_LINE; n++) {
                token[n] = strtok(0, DELIMITER);
                if (!token[n]) break; // no more tokens
            }

            if(iterCt == 0) {
                int rowCt = boost::lexical_cast<int>(token[0]);
                int colCt = boost::lexical_cast<int>(token[1]);
                m = Matrix(rowCt, colCt);
            }

            if(iterCt != 0) {
                int cRow = iterCt - 1;
                for(int cCol = 0; cCol < n; cCol++) {
                    int cValue = boost::lexical_cast<int>(token[cCol]);
                    m.data[(n*cRow) + cCol] = cValue;
                    //
                }
            }
        }
    }

    matrix1.close();
    return m;
}
