/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk
   Modified: Robert Edge        

   This algorithm is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work is based on an
   algorithm described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: Oct 13, 2013.

*/

/*!
	@file parser.cpp
	@author Nicholas Weber
	@author Amy Le
	Student IDs: 3408684 4520434
	Class:		3081W Fall 2014
	Section:	04
	TA:		Kevin Thomsen
	@brief CPP file for Parser methods
*/

#include "parser.h"
#include "scanner.h"
#include "extToken.h"
#include "AST.h"
#include <stdio.h>
#include <assert.h>
#include <string>
#include <cstring>
#include <stdlib.h> // atoi()

using namespace std ;

Parser::~Parser() {
    if (s) delete s ;

    ExtToken *extTokenToDelete ;
    currToken = tokens ;
    while (currToken) {
        extTokenToDelete = currToken ;
        currToken = currToken->next ;
        delete extTokenToDelete ;
    }

    Token *tokenToDelete ;
    Token *currScannerToken = stokens ;
    while (currScannerToken) {
        tokenToDelete = currScannerToken ;
        currScannerToken = currScannerToken->next ;
        delete tokenToDelete ;
    }

}



Parser::Parser ( ) { 
    currToken = NULL; prevToken = NULL ; tokens = NULL; 
    s = NULL; stokens = NULL; 
}

ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;

    ParseResult pr ;
    try {
        s = new Scanner() ;
        stokens = s->scan (text) ;        
        tokens = extendTokenList ( this, stokens ) ;

        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */


// Program
/*! Creates objects for the AST node for the concrete class Root.
    Covers the following FCAL grammar rules:
	Program ::= varName '(' ')' '{' Stmts '}'  //root
    Returns a ParseResult with the AST field set to the node.
*/
ParseResult Parser::parseProgram () {
    //printf("Creating: Root.\n");
    ParseResult pr ;
    // root
    // Program ::= varName '(' ')' '{' Stmts '}' 
    match(variableName) ;

    // Create VarNameExpr
    string name( prevToken->lexeme ) ;
    VarNameExpr *varName = NULL;
    varName = new VarNameExpr(name);

    match(leftParen) ;
    match(rightParen) ;
    match(leftCurly);

    // Create Stmts
    ParseResult prStmts = parseStmts() ;
    Stmts *stmts = NULL;
    if(prStmts.ast) {
		stmts = dynamic_cast<Stmts *>(prStmts.ast);
    }

	// Create the appropriate Node
    // Create Root object
    Root *programRoot = NULL;
    if(varName != NULL && stmts != NULL) {
        programRoot = new Root(varName, stmts);
        // Set the AST node
        pr.ast = programRoot;

    }

    match(rightCurly);
    match(endOfFile) ;
    pr.ok = true;
    //printf("Finished: Root.\n");
    return pr ;
}


// MatrixDecl
// identical purpose of parseDecl, handles special matrix syntax.
/*! Creates objects for the AST node for the concrete class MatrixDecl.
    Covers the following FCAL grammar rules:
        Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
	Decl ::= 'Matrix' varName '=' Expr ';'
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseMatrixDecl () {
    ParseResult pr ;
    match(matrixKwd);
    match(variableName) ;

    // Create VarNameExpr
    string name( prevToken->lexeme ) ;
    VarNameExpr *v1 = NULL;
    v1 = new VarNameExpr(name);

    // Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
    if(attemptMatch(leftSquare)){
	//printf("Creating: MatrixDecl type 1\n");
        ///parseExpr(0);
        // Create the Expr object
        ParseResult prExpr1 = parseExpr(0);
	    Expr *e1 = NULL;
        if(prExpr1.ast) {
	            e1 = dynamic_cast<Expr *>(prExpr1.ast);
	}
        match(comma);

        ///parseExpr(0);
        // Create the Expr object
        ParseResult prExpr2 = parseExpr(0);
	    Expr *e2 = NULL;
        if(prExpr2.ast) {
	            e2 = dynamic_cast<Expr *>(prExpr2.ast);
	    }
	    
        match(rightSquare);
        
        // Create VarNameExpr
        ParseResult prVN2 = parseVariableName();
        VarNameExpr *v2 = NULL;
        if(prVN2.ast) {
            v2 = dynamic_cast<VarNameExpr *>(prVN2.ast);
	    }
	    
        match(comma);
        
        // Create VarNameExpr
        ParseResult prVN3 = parseVariableName();
        VarNameExpr *v3 = NULL;
        if(prVN3.ast) {
            v3 = dynamic_cast<VarNameExpr *>(prVN3.ast);
	    }
        
        match(assign);
        
        // Create the Expr object
        ParseResult prExpr3 = parseExpr(0);
	    Expr *e3 = NULL;
        if(prExpr3.ast) {
	            e3 = dynamic_cast<Expr *>(prExpr3.ast);
	    }
	    
	    // Create the appropriate Node object
	    //MatrixDecl::MatrixDecl(VarNameExpr *n1, Expr *e1, Expr *e2, VarNameExpr *n2,
			//VarNameExpr *n3, Expr *e3)
		MatrixDecl *md = NULL;
		md = new MatrixDecl(v1, e1, e2, v2, v3, e3);
		
	    // Set the AST node
        pr.ast = md;
	//printf("Finished: MatrixDecl type 1.\n");
    }
    
    
    // Decl ::= 'Matrix' varName '=' Expr ';'
    else if(attemptMatch(assign)){
	//printf("Creating: MatrixDecl type 2\n");
        ///parseExpr(0);
        // Create the Expr object
        ParseResult prExpr1 = parseExpr(0);
	    Expr *e1 = NULL;
        if(prExpr1.ast) {
	            e1 = dynamic_cast<Expr *>(prExpr1.ast);
	    }
	    
	    // Create the appropriate Node object
	    MatrixDecl *md = NULL;
		md = new MatrixDecl(v1, e1);
	    // Set the AST node
        pr.ast = md;
        //printf("Finished: MatrixDecl type 2.\n");
    }
    
    
    else{
        throw ( (string) "Bad Syntax of Matrix Decl in in parseMatrixDecl" ) ;
    }

    match(semiColon) ;
	pr.ok = true;
    return pr ;
}
//standardDecl 
//Decl ::= integerKwd varName | floatKwd varName | stringKwd varName
/*! Creates objects for the AST node for the concrete class StdDecl.
    Covers the following FCAL grammar rules:
	Decl ::= intKwd varName ';'
	Decl ::= floatKwd varName ';'
	Decl ::= stringKwd varName ';'
	Decl ::= boolKwd varName ';'
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseStandardDecl(){
    ParseResult pr ;
    bool Int, Float, String, Bool;
    Int = Float = String = Bool = false;

    if ( attemptMatch(intKwd) ) {
        // Type ::= intKwd
	Int = true;
    }
    else if ( attemptMatch(floatKwd) ) {
        // Type ::= floatKwd
	Float = true;
    }
    else if ( attemptMatch(stringKwd) ) {
        // Type ::= stringKwd
	String = true;
    }
    else if ( attemptMatch(boolKwd) ) {
        // Type ::= boolKwd
	Bool = true;
    }
    // Create VarNameExpr
    match(variableName) ;
    string name( prevToken->lexeme ) ;
    VarNameExpr *v = NULL;
    v = new VarNameExpr(name);

    // Create the appropriate Node
	// Set the AST node
    if(Int) {
        pr.ast = new StdDecl(intKwd, v);
    }
    else if(Float) {
	pr.ast = new StdDecl(floatKwd, v);
    }
    else if(String) {
	pr.ast = new StdDecl(stringKwd, v);
    }
    else if(Bool) {
	pr.ast = new StdDecl(boolKwd, v);
    }

    match(semiColon) ;
    pr.ok = true;
    return pr ;
}

// Decl
/*! The abstract class Decl that will be inherited by concrete
    classes MatrixDecl and StdDecl. 
    MatrixDecl covers FCAL grammar rules:
    	Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';'
	Decl ::= 'Matrix' varName '=' Expr ';'
    StdDecl covers FCAL grammar rules:
    	Decl ::= integerKwd varName ';'
	Decl ::= floatKwd varName ';'
	Decl ::= stringKwd varName ';'
	Decl ::= boolKwd varName ';'
    Returns ParseResult that has the set AST node. 
*/
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    // Decl :: Matrix variableName ....
    if(nextIs(matrixKwd)){
        pr =parseMatrixDecl();
    }
    // Decl ::= Type variableName semiColon
    else{
        pr=parseStandardDecl();
    }
    pr.ok = true;
    return pr ;
}



// Stmts
/*! The abstract class Stmts that will be inherited by concrete
    classes EmptyStmts and MultipleStmts. 
    EmptyStmts covers FCAL grammar rule:
	Stmts ::= <<empty>>
    MultipleStmts covers FCAL grammar rule:
	Stmts ::= Stmt Stmts
    Returns ParseResult that has the set AST node. 
*/
ParseResult Parser::parseStmts () {
    /*ParseResult pr ;
    if ( ! nextIs(rightCurly) && !nextIs(inKwd)  ) {
        // Stmts ::= Stmt Stmts
        ParseResult prStmt = parseStmt() ;
        ParseResult prStmts = parseStmts() ;
    }
    else {
        // Stmts ::= 
        // nothing to match.
    }
    return pr ;*/

    ParseResult pr ;
    if ( ! nextIs(rightCurly) && !nextIs(inKwd)  ) {
		// Stmts ::= Stmt Stmts
			// Create Stmt
		ParseResult prStmt = parseStmt() ;
		Stmt *s1 = NULL;
		if(prStmt.ast) {
			s1 = dynamic_cast<Stmt *>(prStmt.ast);
		}
		// Create Stmts
		ParseResult prStmts = parseStmts() ;
		Stmts *s2 = NULL;
		if(prStmts.ast) {
			s2 = dynamic_cast<Stmts *>(prStmts.ast);
		}
		// Create MultipleStmts and set as the node
		MultipleStmts *stmts = NULL;
		stmts = new MultipleStmts(s1, s2);
		pr.ast = stmts;
		//printf("MultipleStmts created\n");
    } else {
		EmptyStmts *eStmts = NULL;
		eStmts = new EmptyStmts();
		pr.ast = eStmts;
		//printf("EmptyStmts created\n");
			// Stmts ::= 
			// nothing to match.
    }
	pr.ok = true;
    return pr ;
}


// Stmt
/*! The abstract class Stmt that will be inherited by concrete
    classes DeclStmt, StmtStmts, IfStmt, IfElseStmt, PrintStmt, ForLoopStmt, WhileStmt,
    VarStmt, and SemiColonStmt
    DeclStmt covers FCAL grammar rule:
	Stmt ::= Decl
    StmtStmts covers FCAL grammar rule:
	Stmt ::= '{' Stmts '}'
    IfStmt covers FCAL grammar rule:
	Stmt ::= 'if' '(' Expr ')' Stmt 
    IfElseStmt covers FCAL grammar rule:
	Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    PrintStmt covers FCAL grammar rule:
	Stmt ::= 'print' '(' Expr ')' ';'
    ForLoopStmt covers FCAL grammar rule:
	Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
    WhileStmt covers FCAL grammar rule:
	Stmt ::= 'while' '(' Expr ')' Stmt
    VarStmt covers FCAL grammar rule:
	Stmt ::= varName '=' Expr ';'  | varName '[' Expr ',' Expr ']' '=' Expr ';'
    SemiColonStmt covers FCAL grammar rule:
	Stmt ::= ';'
    Returns ParseResult that has the set AST node. 
*/
ParseResult Parser::parseStmt () {
    ParseResult pr ;

    //Stmt ::= Decl
    if(nextIs(intKwd)||nextIs(floatKwd)||nextIs(matrixKwd)||nextIs(stringKwd)||nextIs(boolKwd)){
        ParseResult prStdDecl = parseDecl();
        Decl *d = NULL;
        if(prStdDecl.ast) {
            d = dynamic_cast<Decl *>(prStdDecl.ast);
        }
        DeclStmt *ds = NULL;
        ds = new DeclStmt(d);
        pr.ast = ds;
    }
    
    
    //Stmt ::= '{' Stmts '}'
    else if (attemptMatch(leftCurly)){
		
		// Create Stmts
        ParseResult prStmts = parseStmts() ; 
		Stmts *ss = NULL;
		if(prStmts.ast) {
			ss = dynamic_cast<Stmts *>(prStmts.ast);
		}
		
        match(rightCurly);
        
        // Create the appropriate Node
		StmtStmts *stst = NULL;
		stst = new StmtStmts(ss);
		
		// Set the AST Node
		pr.ast = stst;
    }
    
    
    //Stmt ::= 'if' '(' Expr ')' Stmt
    //Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    else if (attemptMatch(ifKwd)){
        match(leftParen);

		// Create Expr Object
		ParseResult ifExpr = parseExpr(0);
		Expr *iExpr = NULL;
		if(ifExpr.ast) {
			iExpr = dynamic_cast<Expr *>(ifExpr.ast);
		}

		match(rightParen);

		// Create Stmt Objects
		ParseResult prStmt = parseStmt();
		Stmt *stmt = NULL;
		if(prStmt.ast) {
			stmt = dynamic_cast<Stmt *>(prStmt.ast);
		}
		Stmt *elseStmt = NULL;
		if(attemptMatch(elseKwd)) {	// Then... Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
			ParseResult prElseStmt = parseStmt();
			if(prElseStmt.ast) {
				elseStmt = dynamic_cast<Stmt *>(prElseStmt.ast);
			}
		}

		// Create the correct type of Stmt
		if(elseStmt == NULL) {
			IfStmt *ifStmt = NULL;
			ifStmt = new IfStmt(iExpr, stmt);
			pr.ast = ifStmt;
		} else {
			IfElseStmt *ifElseStmt = NULL;
			ifElseStmt = new IfElseStmt(iExpr, stmt, elseStmt);
			pr.ast = ifElseStmt;
		}

    }
    
    
    //Stmt ::= varName '=' Expr3 ';'  | varName '[' Expr1 ',' Expr2 ']' '=' Expr3 ';'
    else if  ( attemptMatch (variableName) ) {
		//printf("Creating: VarStmt\n");
		// Create VarNameExpr
		string name( prevToken->lexeme ) ;
		VarNameExpr *v = NULL;
		v = new VarNameExpr(name);

		Expr *e1 = NULL;
		Expr *e2 = NULL;
		if (attemptMatch ( leftSquare ) ) {
		    //printf("Matched: leftSquare");
    		    // Expr1 and Expr2 here
	            ParseResult prExpr1 = parseExpr(0);
		    if(prExpr1.ast) {
			e1 = dynamic_cast<Expr *>(prExpr1.ast);
		    }
		    match ( comma ) ;
		    ParseResult prExpr2 = parseExpr (0) ;
		    if(prExpr2.ast) {
			e2 = dynamic_cast<Expr *>(prExpr2.ast);
		    }
		    match  ( rightSquare ) ;
		}
		// Match '='
		match(assign);

		// Right hand side Expr3
		ParseResult prExpr = parseExpr(0);
		Expr *eRight = NULL;
		if(prExpr.ast) {
		    eRight = dynamic_cast<Expr *>(prExpr.ast);
		}
		// Create the corresponding Stmt type
		VarStmt *stmt = NULL;
		if(e1 == NULL && e2 == NULL) { // Then... Stmt ::= varName '=' Expr ';'
			stmt = new VarStmt(v, eRight);
			pr.ast = stmt;
		}
		else if (e1 != NULL && e2 != NULL) { // Then... varName '[' Expr ',' Expr ']' '=' Expr ';'
			stmt = new VarStmt(v, e1, e2, eRight);
			pr.ast = stmt;
		}
		match(semiColon);
		//printf("Finished: VarStmt\n");
    }
    
    
    //Stmt ::= 'print' '(' Expr ')' ';'
    else if ( attemptMatch (printKwd) ) {
        match (leftParen) ;

        ParseResult prExpr = parseExpr(0);
		Expr *pExpr = NULL;
		if(prExpr.ast) {
			pExpr = dynamic_cast<Expr *>(prExpr.ast);
		}
		// Create the appropriate Node
		PrintStmt *pStmt = NULL;
		pStmt = new PrintStmt(pExpr);
		
		// Set the AST Node
		pr.ast = pStmt;
		match (rightParen) ;
		match (semiColon) ;
    }
    
    
    //Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
    else if ( attemptMatch (forKwd) ) {
        match (leftParen) ;
        
        // Create VarNameExpr
        match (variableName) ;
		string name( prevToken->lexeme ) ;
		VarNameExpr *v = NULL;
			v = new VarNameExpr(name);

			match (assign) ;

		// Create Expr
		ParseResult prExpr = parseExpr (0) ;
		Expr *pExpr = NULL;
		if(prExpr.ast) {
			pExpr = dynamic_cast<Expr *>(prExpr.ast);
		}

		match (colon) ;

		// Create Expr2
			ParseResult prExpr2 = parseExpr (0) ;
		Expr *pExpr2 = NULL;
		if(prExpr2.ast) {
			pExpr2 = dynamic_cast<Expr *>(prExpr2.ast);
		}

			match (rightParen) ;

		// Create Stmt
		ParseResult prStmt = parseStmt() ;
		Stmt *s1 = NULL;
		if(prStmt.ast) {
			s1 = dynamic_cast<Stmt *>(prStmt.ast);
		}

		// Create the appropriate Node
		ForLoopStmt *flStmt = NULL;
		flStmt = new ForLoopStmt(v, pExpr, pExpr2, s1);

		// Set the AST node
		pr.ast = flStmt;
    }
    
    
    //Stmt ::= 'while' '(' Expr ')' Stmt
    else if (attemptMatch(whileKwd)) {
        match(leftParen);
        
        //parseExpr(0);
        // Create Expr
		ParseResult prExpr = parseExpr (0) ;
		Expr *e = NULL;
		if(prExpr.ast) {
			e = dynamic_cast<Expr *>(prExpr.ast);
		}
        match(rightParen);
        
        // Create Stmt
        ParseResult prStmt = parseStmt() ; 
		Stmt *s = NULL;
		if(prStmt.ast) {
			s = dynamic_cast<Stmt *>(prStmt.ast);
		}
        
        // Create the appropriate Node
		WhileStmt *wStmt = NULL;
		wStmt = new WhileStmt(e, s);
		
		// Set the AST Node
		pr.ast = wStmt;
    }
    
    
    //Stmt ::= ';'
    else if ( attemptMatch (semiColon) ) {
        // parsed a skip
        
        // Create the appropriate Node
		SemiColonStmt *scs = NULL;
		scs = new SemiColonStmt();
		
		// Set the AST Node
		pr.ast = scs;
		pr.ok = true;
        
    }
    else{
        throw ( makeErrorMsg ( currToken->terminal ) + " while parsing a statement" ) ;
    }
    // Stmt ::= variableName assign Expr semiColon

    pr.ok = true;
    return pr ;
}



// Expr
/*! The abstract class Expr that will be inherited by concrete classes VarNameExpr,
    BoolExpr, NormExpr, CallExpr, MatrixREFExpr, BinaryExpr, ConstExpr, LetExpr,
    IfExpr, and NotExpr.
    VarNameExpr covers FCAL grammar rule:
	Expr ::= varName
    BoolExpr covers FCAL grammar rule:
	Expr ::= trueKwd | falseKwd
    NormExpr covers FCAL grammar rule:
        Expr ::= '(' Expr ')'
    CallExpr covers FCAL grammar rule:
    	Expr ::= varName '(' Expr ')'
    MatrixREFExpr covers FCAL grammar rule:
    	Expr ::= varName '[' Expr ',' Expr ']'
    BinaryExpr covers FCAL grammar rule:
    	Expr ::= Expr '*' Expr 
	Expr ::= Expr '/' Expr 
	Expr ::= Expr '+' Expr
	Expr ::= Expr '-' Expr
	Expr ::= Expr '>' Expr
	Expr ::= Expr '>=' Expr 
	Expr ::= Expr '<' Expr
	Expr ::= Expr '<=' Expr 
	Expr ::= Expr '==' Expr 
	Expr ::= Expr '!=' Expr
    ConstExpr covers FCAL grammar rule:
    	Expr ::= integerConst | floatConst |  stringConst
    LetExpr covers FCAL grammar rule:
    	Expr ::= 'let' Stmts 'in' Expr 'end'
    IfExpr covers FCAL grammar rule:
    	Expr ::= 'if' Expr 'then' Expr 'else' Expr
    NotExpr covers FCAL grammar rule:
    	Expr ::= '!' Expr
    Returns ParseResult that has the set AST node. 
*/
ParseResult Parser::parseExpr (int rbp) {
    /* Examine current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;

    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }
    
    return left ;
}


/*
 * parse methods for Expr productions
 * ----------------------------------
 */

 // Expr ::= trueKwd
/*! Creates objects for the AST node for the concrete class BoolExpr.
    Covers the following FCAL grammar rule:
	Expr ::= trueKwd
    Returns a ParseResult with the AST field set to the node. 
*/
 ParseResult Parser::parseTrueKwd ( ) {
     ParseResult pr ;
     match ( trueKwd ) ;
     
     // Create the appropriate Node
	 ConstExpr *val = NULL;
	 bool bb = true;
	 val = new ConstExpr(bb);
	 // Set the AST Node
	 pr.ast = val;
	 pr.ok = true;
	 return pr;
 }

 // Expr ::= falseKwd
/*! Creates objects for the AST node for the concrete class BoolExpr.
    Covers the following FCAL grammar rule:
	Expr ::= falseKwd
    Returns a ParseResult with the AST field set to the node. 
*/
 ParseResult Parser::parseFalseKwd ( ) {
     ParseResult pr ;
     match ( falseKwd ) ;
     // Create the appropriate Node
	 ConstExpr *val = NULL;
	 bool bb = false;
	 val = new ConstExpr(bb);
	
	 // Set the AST Node
	 pr.ast = val;
	 pr.ok = true;
	 return pr;
 }

// Expr ::= intConst
/*! Creates objects for the AST node for the concrete class ConstExpr.
    Covers the following FCAL grammar rule:
	Expr ::= intConst
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    
    match ( intConst ) ;
    
    // Create the appropriate Node
    string strValue( prevToken->lexeme ) ;
    int cValue = atoi(strValue.c_str());
	ConstExpr *val = NULL;
	val = new ConstExpr(cValue);
	
	// Set the AST Node
	pr.ast = val;
	pr.ok = true;
	
    return pr ;
}

// Expr ::= floatConst
/*! Creates objects for the AST node for the concrete class ConstExpr.
    Covers the following FCAL grammar rule:
	Expr ::= floatConst
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
    
    // Create the appropriate Node
    string strValue( prevToken->lexeme ) ;
    float cValue = atof(strValue.c_str());
	ConstExpr *val = NULL;
	val = new ConstExpr(cValue);
	
	// Set the AST Node
	pr.ast = val;
	pr.ok = true;
    
    return pr ;
}

// Expr ::= stringConst
/*! Creates objects for the AST node for the concrete class ConstExpr.
    Covers the following FCAL grammar rule:
	Expr ::= stringConst
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
    
    // Create the appropriate Node
    string strValue( prevToken->lexeme ) ;
	ConstExpr *val = NULL;
	val = new ConstExpr(strValue);
	
	// Set the AST Node
	pr.ast = val;
	pr.ok = true;
    
    return pr ;
}

// Expr ::= variableName .....
/*! Creates objects for the AST node for the concrete class VarNameExpr and CallExpr and MatrixREFExpr.
    VarNameExpr covers the following FCAL grammar rule:
	Expr ::= varName
    CallExpr covers the following FCAL grammar rule:
    	Expr ::= varName '(' Expr ')'
    MatrixREFExpr covers the following FCAL grammar rule:
    	Expr ::= varName '[' Expr ',' Expr ']'
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    
    // Create VarNameExpr
    match ( variableName ) ;
    string name1( prevToken->lexeme ) ;
    VarNameExpr *vn1 = NULL;
    vn1 = new VarNameExpr(name1);
    
    //Expr ::= varName '[' Expr ',' Expr ']'   //MatrixREf
    if(attemptMatch(leftSquare)){
	//printf("Creating: MatrixREFExpr\n");
        //parseExpr(0);
        // Create Expr
		ParseResult prExpr1 = parseExpr (0) ;
		Expr *e1 = NULL;
		if(prExpr1.ast) {
			e1 = dynamic_cast<Expr *>(prExpr1.ast);
		}
        
        match(comma);
        
        //parseExpr(0);
        // Create Expr
		ParseResult prExpr2 = parseExpr (0) ;
		Expr *e2 = NULL;
		if(prExpr2.ast) {
			e2 = dynamic_cast<Expr *>(prExpr2.ast);
		}
        
        match(rightSquare);
        
        // Create the appropriate Node
        // MatrixREFExpr::MatrixREFExpr(VarNameExpr *name, Expr *e1, Expr *e2)
		MatrixREFExpr *mREF = NULL;
		mREF = new MatrixREFExpr(vn1, e1, e2);
		
		// Set the AST Node
		pr.ast = mREF;
		pr.ok = true;
	//printf("Finished: MatrixREFExpr.\n");
    }
    //Expr ::= varableName '(' Expr ')'        //NestedOrFunctionCall
    else if(attemptMatch(leftParen)){
        //parseExpr(0);
        // Create Expr
		ParseResult prExpr1 = parseExpr (0) ;
		Expr *e1 = NULL;
		if(prExpr1.ast) {
			e1 = dynamic_cast<Expr *>(prExpr1.ast);
		}
        
        match(rightParen);
        
        // Create the appropriate Node
        // CallExpr::CallExpr(VarNameExpr *name, Expr *e)
		CallExpr *cExpr = NULL;
		cExpr = new CallExpr(vn1, e1);
		
		// Set the AST Node
		pr.ast = cExpr;
		pr.ok = true;
        
    }
    //Expr := variableName
    else{
        // variable
        
		// Set the AST Node
		pr.ast = vn1;
		pr.ok = true;
        
    }
    return pr ;
}


// Expr ::= leftParen Expr rightParen
/*! Creates objects for the AST node for the concrete class NormExpr
    NormExpr covers the following FCAL grammar rule:
	Expr ::= '(' Expr ')'
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseNestedExpr ( ) {
    ParseResult pr ;
    match ( leftParen ) ;
    
    //parseExpr(0) ; 
    // Create Expr
	ParseResult prExpr1 = parseExpr (0) ;
	Expr *e1 = NULL;
	if(prExpr1.ast) {
		e1 = dynamic_cast<Expr *>(prExpr1.ast);
	}
    
    match(rightParen) ;
    
    // Create the appropriate Node
	NormExpr *nExpr = NULL;
	nExpr = new NormExpr(e1);
	
	// Set the AST Node
	pr.ast = nExpr;
	pr.ok = true;
    
    return pr ;
}

//Expr ::= 'if' Expr 'then' Expr 'else' Expr
/*! Creates objects for the AST node for the concrete class IfElseExpr
    IfElseExpr covers the following FCAL grammar rule:
	Expr ::= 'if' Expr 'then' Expr 'else' Expr
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseIfExpr(){  
    ParseResult pr ; 
    
    match(ifKwd);
    
    //parseExpr(0);
	// Create Expr
	ParseResult prExpr1 = parseExpr (0) ;
	Expr *e1 = NULL;
	if(prExpr1.ast) {
		e1 = dynamic_cast<Expr *>(prExpr1.ast);
	}
    
    match(thenKwd);
    
    //parseExpr(0);
	// Create Expr
	ParseResult prExpr2 = parseExpr (0) ;
	Expr *e2 = NULL;
	if(prExpr2.ast) {
		e2 = dynamic_cast<Expr *>(prExpr2.ast);
	}
    
    match(elseKwd);
    
    //parseExpr(0);
	// Create Expr
	ParseResult prExpr3 = parseExpr (0) ;
	Expr *e3 = NULL;
	if(prExpr3.ast) {
		e3 = dynamic_cast<Expr *>(prExpr3.ast);
	}
    
    // Create the appropriate Node
    // IfExpr::IfExpr(Expr *e1, Expr *e2, Expr *e3)
	IfExpr *ifExpr = NULL;
	ifExpr = new IfExpr(e1, e2, e3);
	
	// Set the AST Node
	pr.ast = ifExpr;
	pr.ok = true;

    return pr;
}


// Expr ::= 'let' Stmts 'in' Expr 'end'
/*! Creates objects for the AST node for the concrete class LetExpr
    LetExpr covers the following FCAL grammar rule:
	Expr ::= 'let' Stmts 'in' Expr 'end'
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseLetExpr(){
    ParseResult pr ;
    match(letKwd);
    //printf("Creating: LetExpr\n");
    // Create Stmts
    ParseResult prStmts = parseStmts() ;
    Stmts *ss = NULL;
    if(prStmts.ast) {
        ss = dynamic_cast<Stmts *>(prStmts.ast);
    }

    match(inKwd);

    // Create Expr
    ParseResult prExpr = parseExpr (0) ;
    Expr *expr = NULL;
    if(prExpr.ast) {
        expr = dynamic_cast<Expr *>(prExpr.ast);
    }

    match(endKwd);

    // Create the appropriate Node
    LetExpr *lExpr = NULL;
    lExpr = new LetExpr(ss, expr);
    //printf("Finished: LetExpr.\n");

    // Set the AST node
    pr.ast = lExpr;
    pr.ok = true;
    return pr;
}

// Expr ::= '!' Expr 
/*! Creates objects for the AST node for the concrete class NotExpr
    NotExpr covers the following FCAL grammar rule:
	Expr ::= '!' Expr
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseNotExpr () {
    ParseResult pr ;
    match ( notOp ) ;
    
    //parseExpr( 0 ); 
    // Create Expr
    ParseResult prExpr = parseExpr (0) ;
    Expr *e = NULL;
    if(prExpr.ast) {
        e = dynamic_cast<Expr *>(prExpr.ast);
    }
    
    // Create the appropriate Node
    NotExpr *notE = NULL;
    notE = new NotExpr(e);

    // Set the AST node
    pr.ast = notE;
    pr.ok = true;
    return pr;
}

/*! Creates objects for the AST node for the concrete class BinaryExpr
    BinaryExpr covers the following FCAL grammar rule:
	Expr ::= Expr '+' Expr
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseAddition ( ParseResult prLeft ) {
// Expr ::= Expr plusSign Expr
    // parser has already matched left expression 
    ParseResult pr ;

    // Left Expr
    Expr *left = NULL;
    //prLeft = parseExpr( prevToken->lbp() );  --- Parser all ready matched this
    if(prLeft.ast) {
        left = dynamic_cast<Expr *> (prLeft.ast);
    }
    // Operator
    match ( plusSign ) ;
    string op = "+";//new string("+");

    // Right Expr
    ParseResult prRight = parseExpr( prevToken->lbp() ); 
    Expr *right = NULL;
    if(prRight.ast) {
        right = dynamic_cast<Expr *> (prRight.ast);
    }
    BinaryExpr *be = NULL;
    be = new BinaryExpr(left, op, right);
    pr.ast = be;
    pr.ok = true;
    return pr ;

}

// Expr ::= Expr star Expr
/*! Creates objects for the AST node for the concrete class BinaryExpr
    BinaryExpr covers the following FCAL grammar rule:
	Expr ::= Expr '*' Expr
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseMultiplication ( ParseResult prLeft ) {
    // parser has already matched left expression 
    /*ParseResult pr ;
    match ( star ) ;
    parseExpr( prevToken->lbp() ); 
    return pr ;*/
    //printf("Creating: BinaryExpr for multiplication.\n");
    // parser has already matched left expression 
    ParseResult pr ;

    // Left Expr
    Expr *left = NULL;
    //prLeft = parseExpr( prevToken->lbp() );  --- Parser all ready matched this
    if(prLeft.ast) {
        left = dynamic_cast<Expr *> (prLeft.ast);
    }
    // Operator
    match ( star ) ;
    string op = "*";//string* op = new string("*");

    // Right Expr
    ParseResult prRight = parseExpr( prevToken->lbp() ); 
    Expr *right = NULL;
    if(prRight.ast) {
        right = dynamic_cast<Expr *> (prRight.ast);
    }
    BinaryExpr *be = NULL;
    be = new BinaryExpr(left, op, right);
    pr.ast = be;
    pr.ok = true;
    //printf("Finished: BinaryExpr for multiplication.\n");
    return pr ;
	
}

// Expr ::= Expr dash Expr
/*! Creates objects for the AST node for the concrete class BinaryExpr
    BinaryExpr covers the following FCAL grammar rule:
	Expr ::= Expr '-' Expr
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseSubtraction ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    //match ( dash ) ;
    //parseExpr( prevToken->lbp() ); 
    //return pr ;
    
    // Left Expr
    Expr *left = NULL;
    //prLeft = parseExpr( prevToken->lbp() );  --- Parser all ready matched this
    if(prLeft.ast) {
        left = dynamic_cast<Expr *> (prLeft.ast);
    }
    // Operator
    match ( dash ) ;
    string op = "-";//string* op = new string("-");

    // Right Expr
    ParseResult prRight = parseExpr( prevToken->lbp() ); 
    Expr *right = NULL;
    if(prRight.ast) {
        right = dynamic_cast<Expr *> (prRight.ast);
    }
    BinaryExpr *be = NULL;
    be = new BinaryExpr(left, op, right);
    pr.ast = be;
    pr.ok = true;
    return pr ;
}

// Expr ::= Expr forwardSlash Expr
/*! Creates objects for the AST node for the concrete class BinaryExpr
    BinaryExpr covers the following FCAL grammar rule:
	Expr ::= Expr '/' Expr
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseDivision ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    //match ( forwardSlash ) ;
    //parseExpr( prevToken->lbp() ); 
    //return pr ;
    
    // Left Expr
    Expr *left = NULL;
    //prLeft = parseExpr( prevToken->lbp() );  --- Parser all ready matched this
    if(prLeft.ast) {
        left = dynamic_cast<Expr *> (prLeft.ast);
    }
    // Operator
    match ( forwardSlash ) ;
    string op = "/";//string* op = new string("/");

    // Right Expr
    ParseResult prRight = parseExpr( prevToken->lbp() ); 
    Expr *right = NULL;
    if(prRight.ast) {
        right = dynamic_cast<Expr *> (prRight.ast);
    }
    BinaryExpr *be = NULL;
    be = new BinaryExpr(left, op, right);
    pr.ast = be;
    pr.ok = true;
    return pr ;
}


// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr
/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */
/*! Creates objects for the AST node for the concrete class BinaryExpr
    BinaryExpr covers the following FCAL grammar rule:
	Expr ::= Expr '>' Expr
	Expr ::= Expr '>=' Expr 
	Expr ::= Expr '<' Expr
	Expr ::= Expr '<=' Expr 
	Expr ::= Expr '==' Expr 
	Expr ::= Expr '!=' Expr
    Returns a ParseResult with the AST field set to the node. 
*/
ParseResult Parser::parseRelationalExpr ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;

    //nextToken( ) ;
    // just advance token, since examining it in parseExpr caused
    // this method being called.
    //string op = prevToken->lexeme ;

    //parseExpr( prevToken->lbp() ); 
    //return pr ;
    
    // Left Expr
    Expr *left = NULL;
    //prLeft = parseExpr( prevToken->lbp() );  --- Parser all ready matched this
    if(prLeft.ast) {
        left = dynamic_cast<Expr *> (prLeft.ast);
    }
    // Operator
    nextToken( ) ;
    // just advance token, since examining it in parseExpr caused
    // this method being called.
    string op = prevToken->lexeme ;

    // Right Expr
    ParseResult prRight = parseExpr( prevToken->lbp() ); 
    Expr *right = NULL;
    if(prRight.ast) {
        right = dynamic_cast<Expr *> (prRight.ast);
    }
    BinaryExpr *be = NULL;
    be = new BinaryExpr(left, op, right);
    pr.ast = be;
    pr.ok = true;
    return pr ;
    
}


// Helper function used by the parser.

void Parser::match (tokenType tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

bool Parser::attemptMatch (tokenType tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

bool Parser::nextIs (tokenType tt) {
    return currToken->terminal == tt ;
}

void Parser::nextToken () {
    if ( currToken == NULL ) 
        throw ( string("Internal Error: should not call nextToken in unitialized state"));
    else 
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

string Parser::terminalDescription ( tokenType terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

string Parser::makeErrorMsgExpected ( tokenType terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

string Parser::makeErrorMsg ( tokenType terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}

