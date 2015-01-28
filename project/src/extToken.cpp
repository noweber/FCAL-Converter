/* ExtToken: an extension of the Token class with new methods for parsing 
   (led, nud, lbp) and describing the token.

   Author: Eric Van Wyk

   Last modified: March 4, 2013.
*/

#include "extToken.h"
#include "parser.h"

#include <stdio.h>
#include <string>

using namespace std ;

ExtToken *extendToken (Parser *p, Token *tokens) {
    switch ( tokens->terminal ) {
    case letKwd: return new LetToken(p,tokens) ;
    case inKwd: return new ExtToken(p,tokens,"'in'") ;
    case endKwd: return new ExtToken(p,tokens,"'end'") ;

    case ifKwd: return new IfToken(p,tokens) ;
    case elseKwd: return new ExtToken(p,tokens,"'else'") ;
    case printKwd: return new ExtToken(p,tokens,"'print'") ;
    case forKwd: return new ExtToken(p,tokens,"'for'") ;
    case thenKwd: return new ExtToken(p,tokens,"'then'") ;
    case whileKwd: return new ExtToken(p,tokens,"'while'") ;
    // Keywords


    case intKwd: return new ExtToken(p,tokens,"'Int'") ;
    case floatKwd: return new ExtToken(p,tokens,"'Float'") ;
    case stringKwd: return new ExtToken(p,tokens,"'Str'") ;
    case boolKwd: return new ExtToken(p,tokens,"'Bool'") ;
    case trueKwd: return new TrueKwdToken(p,tokens) ;
    case falseKwd: return new FalseKwdToken(p,tokens) ;
    case matrixKwd: return new ExtToken(p,tokens,"'Matrix'") ;
    //case charKwd: return new ExtToken(p,tokens,"'char'") ;
    //case booleanKwd: return new ExtToken(p,tokens,"'boolean'") ;

    // Constants
    case intConst: return new IntConstToken(p,tokens) ;
    case floatConst: return new FloatConstToken(p,tokens) ;
    case stringConst: return new StringConstToken(p,tokens) ;

    // Names
    case variableName: return new VariableNameToken(p,tokens) ;

    // Punctuation
    case leftParen: return new LeftParenToken(p,tokens) ;
    case rightParen: return new ExtToken(p,tokens,")") ;
    case leftCurly: return new ExtToken(p,tokens,"{") ;
    case rightCurly: return new ExtToken(p,tokens,"}") ;
    case leftSquare: return new ExtToken(p,tokens,"[") ;
    case rightSquare: return new ExtToken(p,tokens,"]") ;

    //case colon: return new ExtToken(p,tokens,":") ;
    case comma: return new ExtToken(p,tokens,",") ;
    case semiColon: return new ExtToken(p,tokens,";") ;
    case colon: return new ExtToken(p,tokens,":") ;
    case assign: return new ExtToken(p,tokens,"=") ;

    case plusSign: return new PlusSignToken(p,tokens) ;
    case star: return new StarToken(p,tokens) ;
    case dash: return new DashToken(p,tokens) ;
    case forwardSlash: return new ForwardSlashToken(p,tokens) ;

    case equalsEquals: 
    case lessThan: 
    case greaterThan: 
    case lessThanEqual: 
    case greaterThanEqual: 
    case notEquals: 
        return new RelationalOpToken(p, tokens, tokens->lexeme) ;
    
    case notOp:
        return new NotOpToken(p,tokens);


    case lexicalError: return new ExtToken(p,tokens,"lexical error") ;
    case endOfFile: return new EndOfFileToken(p,tokens) ;


    default: 
        printf("%i not implemented extend",tokens->terminal);
        fflush(stdout);
        string msg = (string) "Unspecified terminal in extend." ;
        throw ( p->makeErrorMsg ( msg.c_str() ) ) ;
    }
}

ExtToken *extendTokenList (Parser *p, Token *tokens) {
    ExtToken *extTokens = NULL ;
    ExtToken *prevExtToken = NULL ;

    while (tokens != NULL) {
        ExtToken *currExtToken = extendToken(p, tokens) ;
 
        // Add new ExtToken et to the list
        if (extTokens != NULL) {
            prevExtToken->next = currExtToken ;
        }
        else {
            extTokens = currExtToken ;
        }
        prevExtToken = currExtToken ;

        tokens = tokens->next ;
    }

    return extTokens ;
}
