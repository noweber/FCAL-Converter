#include <iostream>
#include "Matrix.h"
#include <math.h>
using namespace std; 
int main ( ) { 
Matrix data ( Matrix::readMatrix ("../samples/myData.data") ) ; 

int rows ;

rows = data.numRows() ; 

int cols ;

cols = data.numCols() ; 

int season_length ;

season_length = 7 ; 

int years ;

years = ceil(((cols * 1.0) / season_length)) ; 

Matrix avgScore( rows,1) ;
 for (int row = 0; row < rows; row ++ ) { 
for (int irrelevant = 0; irrelevant < 1; irrelevant ++ ) { 
*(avgScore.access(row, irrelevant)) = ({ Matrix pt( years,season_length) ;
 for (int i = 0; i < years; i ++ ) { 
for (int j = 0; j < season_length; j ++ ) { 
*(pt.access(i, j)) = ({ int k ;

k = ((i * season_length) + j) ; 

 ( (k >= cols) ? (0.0 - 25) : *(data.access(row, k)) ) ; }) ;
} } 

Matrix comparisonMatrix( years,years) ;
 for (int i = 0; i < years; i ++ ) { 
for (int j = 0; j < years; j ++ ) { 
*(comparisonMatrix.access(i, j)) = ( (j <= i) ? 0.0 : ({ float diff ;

diff = 0 ; 

int k ;

for (k = 0; k <= (season_length - 1); k ++  ) 
{ 
diff = ((diff + *(pt.access(i, k))) - *(pt.access(j, k))) ; 

} 


 (diff / season_length) ; })  );
} } 

Matrix modelAvgScore( years,1) ;
 for (int yr = 0; yr < years; yr ++ ) { 
for (int dontcare = 0; dontcare < 1; dontcare ++ ) { 
*(modelAvgScore.access(yr, dontcare)) = ({ int x ;

int y ;

float score1 ;

score1 = 0.0 ; 

for (x = 0; x <= yr; x ++  ) 
{ 
for (y = (yr + 1); y <= (years - 1); y ++  ) 
{ 
score1 = (score1 + *(comparisonMatrix.access(x, y))) ; 

} 


} 


score1 = ((score1 * 2) / (yr * (years - yr))) ; 

float score2 ;

score2 = 0.0 ; 

for (x = 0; x <= yr; x ++  ) 
{ 
for (y = 0; y <= yr; y ++  ) 
{ 
score2 = (score2 + *(comparisonMatrix.access(x, y))) ; 

} 


} 


score2 = (score2 / (((yr - 1) * yr) / 2)) ; 

float score3 ;

score3 = 0.0 ; 

for (x = (yr + 1); x <= (years - 1); x ++  ) 
{ 
for (y = (yr + 1); y <= (years - 1); y ++  ) 
{ 
score3 = (score3 + *(comparisonMatrix.access(x, y))) ; 

} 


} 


score3 = (score3 / (((years - yr) * ((years - yr) - 1)) / 2)) ; 

 ((score1 - score2) - score3) ; }) ;
} } 

float maximum ;

maximum = (0.0 - 25) ; 

int k ;

k = 0 ; 

for (k = 0; k <= (years - 1); k ++  ) 
{ 
if ( (*(modelAvgScore.access(k, 0)) > maximum) ) 
{ 
maximum = *(modelAvgScore.access(k, 0)) ; 

} 


} 


 maximum ; }) ;
} } 

int j ;

for (j = 0; j <= (rows - 1); j ++  ) 
{ 
cout << *(avgScore.access(j, 0)) ;

cout << "\n" ;

} 



}
