/* The forest-loss detection algorithm specified in the proposed
   domain specific language.

   Elements of this language design may change slightly.
 */

main() {

  // Read in satellite data - To Design
  Matrix data = readMatrix ( "../samples/myData.dat" ) ;

 Int rows;
  rows = numRows(data);
  Int cols;
  cols = numCols(data);
  
  Int season_length;
  season_length = 7;
  
  Int years; 
  years = ceil(  cols * 1.0 / season_length ) ;

  // Begin real code
  
  Matrix avgScore[rows, 1] row, irrelevant = 
    let
      Matrix pt[years, season_length] i,j = 
        let
          Int k;
          k = i*season_length + j;
        in
          if k >= cols 
          then 0.0-25 
          else data[row, k]
        end;
  
      Matrix comparisonMatrix[years, years] i,j =
        if j <= i then 0.0
        else 
          let
          // diff these two years
            Float diff;
            diff = 0;
            Int k;
            for(k = 0 : season_length-1) {
              diff = diff + pt[i,k] - pt[j,k];
            }
          in 
            diff / season_length
          end;

  
      Matrix modelAvgScore[years, 1] yr, dontcare =
        let
          Int x;
          Int y;
         
          Float score1;
          score1 = 0.0;
          for(x = 0 : yr) {
            for(y = yr+1 : years-1) {
              score1 = score1 + comparisonMatrix[x,y];
            }
          }
          score1 = score1 * 2 / (yr * (years - yr));
         
          Float score2;
          score2 = 0.0;
          for(x = 0 : yr) {
            for(y = 0 : yr) {
              score2 = score2 + comparisonMatrix[x,y];
            }
          }
          score2 = score2 / ((yr-1) * yr / 2);
         
          Float score3;
          score3 = 0.0;
          for(x = yr+1 : years-1) {
            for(y = yr+1 : years-1) {
              score3 = score3 + comparisonMatrix[x,y];
            }
          }
          score3 = score3 / ((years-yr)*(years-yr-1)/2);
        in
          score1 - score2 - score3
        end;

       
      Float maximum;
      maximum = 0.0-25;
      Int k;
      k = 0;
      for(k = 0 : years-1) {
        if(modelAvgScore[k,0] > maximum) {
           maximum = modelAvgScore[k,0] ;
        }
      }

    in // back to defining avgScore
      maximum
    end;
    
  // Output results  
  Int j;
  for(j = 0 : rows-1) {
    //print(avgScore[j,0]);
    print("\n");
  }
  
}
