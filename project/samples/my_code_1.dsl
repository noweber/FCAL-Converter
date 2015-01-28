/* This program is just a copy of sample_4.dsl. You should write your
   solution for my_code_1 in this file. */

main () {
  Matrix m = readMatrix ( "../samples/my_matrix_1.data" ) ;
  Matrix n = readMatrix ( "../samples/my_matrix_2.data" ) ;
  Int i;
  Int j;
  Int k;
  /* We're going to multiple m x n, so the resulting matrix is m.numCols() * n.numRows() */
  Matrix o [ 3, 3 ] i, j = i * j ;

  Int temp;
  for (i = 0 : 2 ) {
    for ( j = 0 : 2 ) {
      o[i,j] = 0 ;
      for( k = 0 : 2) {
        temp = o[i,j];
        o[i,j] = m[i,k] * n[k,j] + temp ;
      }
    }
  }

  print(o);
}




