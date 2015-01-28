/* Some matrix computations */

main ( ) {

Matrix m [ 10, 10 ] i, j = i * j ;

Int i ;
Int j ;

for (i = 0 : 9 ) {
  for ( j = 0 : 9 ) {
    print ( m[i,j] ) ;
    print ("  ");
  }
  print ( "\n" ) ;
}

for (i = 0 : 9 ) {
  for ( j = 0 : 9 ) {
    m[i,j] = m[i,j] + 100 ;
  }
}

for (i = 0 : 9 ) {
  for ( j = 0 : 9 ) {
    print ( m[i,j] ) ;
    print ("  ");
  }
  print ( "\n" ) ;
}

}
