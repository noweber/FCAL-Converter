/* A small meaningless comment */
#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include <string>

class Token ;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
 */
enum tokenEnumType { 

    intKwd, floatKwd, boolKwd,
        trueKwd, falseKwd, stringKwd,  matrixKwd,
/*4*/   letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd,
/*10*/  forKwd, whileKwd, printKwd,

    // Constants
/*13*/  intConst, floatConst, stringConst, 

    // Names
/*16*/  variableName ,

    // Punctuation
/*17*/  leftParen, rightParen, 
    leftCurly, rightCurly, 
/*21*/  leftSquare, rightSquare,

    comma, semiColon, colon,

    //Operators
/*25*/  assign, 
    plusSign, star, dash, forwardSlash,
/*30*/  lessThan, lessThanEqual, greaterThan, greaterThanEqual,
/*34*/  equalsEquals, notEquals,
/*36*/  andOp, orOp, notOp,

    // Special terminal types
/*39*/  endOfFile ,
    lexicalError
} ;
typedef enum tokenEnumType tokenType ;

// Below you need to write your class definitions for Token and Scanner.


#endif /* SCANNER_H */
