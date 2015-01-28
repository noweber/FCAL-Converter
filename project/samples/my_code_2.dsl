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
  print(sum);
  print("\n");
}
