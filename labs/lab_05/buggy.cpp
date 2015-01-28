// Author: Dan DaCosta for csci3081, lab 5.
// Debugged by team: LeWeber
// Team Members: Nicholas Weber - 3408684, Amy Le - 4520434
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

//newMat: Allocate a new matrix with dimensions x by y filling cell values 
// randomly.
int** newMat(int x,int y) {
  int ** a = new int*[x];
  for(int i = 0; i < x; i++) {
    a[i] = new int[y];
    for(int j = 0; j < y; j++) {
      a[i][j] = rand() % 13;
    }
  }
  return a;
}

// mulMat: Multiplies A with dimensions x by y by B with dimensions y by z and
// returns a new matrix C with dimensions x by z.
// Pseudocode for matrix multiplication:
// for i from 1 to x
//   for j from 1 to z
//     C[i,j] = 0
//     for k from 1 to y
//       C[i,j] += A[i,k]B[k,j]
int** mulMat(int x, int y, int z, int ** A, int ** B) {
  int ** R = newMat(x,z);
  for(int i = 0; i < x; i++) {
    for(int j = 0; j < z; j++) {
      R[i][j] = 0;
      for(int k = 0; k < y; k++) {
        R[i][j] += A[i][k]*B[j][k];
      }
    }
  }
  return R;
}

// prnMat: Print the matrix A with dimensions x by y.
void prnMat(int x, int y, int ** A) {
  cout<<"Printing "<<x<<" by "<<y<<" matrix:"<<endl;
  for(int i = 0; i < x; i++) {
    for(int j = 0; j < y; j++) {
      cout<<A[i][j]<<" ";
    }
    cout<<endl;
  }
  cout<<endl;
}


void execute() {

  int ** A = newMat(7,11);
  int ** B = newMat(11,13);
  int ** C = mulMat(7,11,13,A,B);
  //prnMat(71,13,C);
  

}

int main(int argc, char ** argv) {
  execute();
  return 0;
}
