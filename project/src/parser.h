/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk

   This algorithms is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work comes from that
   of Based on algorithms described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: March 5, 2013.
*/

/*!	
	@file parser.h
	@author Nicholas Weber
	@author Amy Le
	Student IDs: 3408684 4520434
	Class:		3081W Fall 2014
	Section:	04
	TA:		Kevin Thomsen
	@brief Header file for Parser class
*/

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "parseResult.h"
#include "AST.h"	// added by NoW 11/18/2014 @ 12:27 PM

#include <string>

class ExtToken ;

class Parser {

public:
    Parser() ;
    ~Parser() ;

    ParseResult parse (const char *text) ;
    // Parser methods for the nonterminals:

    ParseResult parseProgram () ;
    ParseResult parseDecl () ;
    ParseResult parseStandardDecl () ;
    ParseResult parseMatrixDecl () ;
    ParseResult parseStmts () ;
    ParseResult parseStmt () ;
    ParseResult parseExpr (int rbp) ;
    // methods for parsing productions for Expr
    ParseResult parseTrueKwd () ;
    ParseResult parseFalseKwd () ;
    ParseResult parseIntConst () ;
    ParseResult parseFloatConst () ;
    ParseResult parseStringConst () ;
    ParseResult parseCharConst () ;
    ParseResult parseVariableName () ;
    ParseResult parseNestedExpr () ;
    ParseResult parseNotExpr();
    ParseResult parseLetExpr() ;
    ParseResult parseIfExpr() ;
    ParseResult parseAddition ( ParseResult left ) ;
    ParseResult parseMultiplication ( ParseResult left ) ;
    ParseResult parseSubtraction ( ParseResult left ) ;
    ParseResult parseDivision ( ParseResult left ) ;

    ParseResult parseRelationalExpr ( ParseResult left ) ;

    // Helper function used by the parser.
    void match (tokenType tt) ;
    bool attemptMatch (tokenType tt) ;
    bool nextIs (tokenType tt) ;
    void nextToken () ;

    std::string terminalDescription ( tokenType terminal ) ;
    std::string makeErrorMsg ( tokenType terminal ) ;
    std::string makeErrorMsgExpected ( tokenType terminal ) ;
    std::string makeErrorMsg ( const char *msg ) ;

    ExtToken *tokens ;
    ExtToken *currToken ;
    ExtToken *prevToken ;

    Token *stokens ;
    Scanner *s ;
} ;

#endif /* PARSER_H */
