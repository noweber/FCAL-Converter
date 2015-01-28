/* A sample program in the Forest Cover Analysis language.

   The program will declare a bool variable and use it in a while
   loop to sum the numbers 1 through 10.
*/
main () {
 Int sum;
  sum = 0;
  Int num;
  num = 1;

  Bool flag;
  flag = True;

  while (flag) {
    sum = sum + num;
    num = sum + 1;
    if (num > 10) {
      flag = False;
    }
  } 
}
