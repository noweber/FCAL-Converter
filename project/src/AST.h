#ifndef Node_H
#define Node_H

/*! \mainpage FCAL Translator
\section intro_sec Introduction
FCAL to C++ translator for CSci 3081W Fall 2014
@file mainpage.h
@author Nicholas Weber
@author Amy Le
Student IDs: 3408684 4520434
Class: 3081W Fall 2014
Section: 04
TA: Kevin Thomsen
@brief Header file for Writing Assignment 3 main page
*/

/*!	
	@file AST.h
	@author Nicholas Weber
	@author Amy Le
	Student IDs: 3408684 4520434
	Class:		3081W Fall 2014
	Section:	04
	TA:		Kevin Thomsen
	@brief Header file for Abstract Syntax Tree classes
*/



#include <string>
//#include <iostream>
#include "scanner.h"

class Node;
class Root;
class Stmts;
class EmptyStmts;
class MultipleStmts;
class Decl;
class StdDecl;
class MatrixDecl;
class Stmt;
class DeclStmt;
class StmtStmts;
class IfStmt;
class IfElseStmt;
class PrintStmt;
class ForLoopStmt;
class WhileStmt;
class SemiColonStmt;
class VarStmt;
class Expr;
class VarNameExpr;
class BoolExpr;
class NormExpr;
class CallExpr;
class MatrixREFExpr;
class BinaryExpr;
class ConstExpr;
class LetExpr;
class IfExpr;
class NotExpr;
/*! This is the base class for the Abstract Syntax Tree (AST).  All AST nodes are derived from this class.
*/
class Node {
    public:
	/*! This is a virtual function which is defined in all inheriting classes. It returns a string of the unparsed FCAL language. */
	virtual std::string unparse () = 0; //{ return " This should be pure virtual ";} ;
	/*! This is a virtual function which is defined in all inheriting classes.  It returns a string of the translated FCAL to C++ source code. */
        virtual std::string cppCode () = 0; //{ return " This should be pure virtual" ; } ;
	/*! This is the class destructor. */
        virtual ~Node();
};

/*! Root is the root of the AST.  It represents the definition of the main function within the
FCAL program and the C++ program.  It is a concrete class derived from Node.

Program ::= varName '(' ')' '{' Stmts '}'  //root
*/
class Root : public Node {
    public:
	/*! This is a constructor for Root.
		\param v A VarNameExpr object for the name of the program's main function.  Typically main.
		\param ss A Stmts object which forms the root of all subsequent statements within the program.
	*/
	Root(VarNameExpr *v, Stmts* ss);
	/*! This is the class destructor. */
	~Root();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	VarNameExpr *programName;
	Stmts *programStmts;
};

/*! Stmts is an abstract class derived from Node.  It is the base class for all of the Stmts productions in the FCAL grammar.
*/
class Stmts : public Node {
    public:
	/*! This is the class destructor. */
	~Stmts();
};

/*! EmptyStmts is a concrete class derived from Stmts.  It represents the empty statement production from the FCAL grammar.

Stmts ::= <<empty>>
*/
class EmptyStmts : public Stmts {
    public:
	/*! This is a constructor for EmptyStmts.   No parameters.
	*/
	EmptyStmts();
	/*! This is the class destructor. */
	~EmptyStmts();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
};

/*! MultipleStmts is a concrete class derived from Stmts.  It represents the single stmt and stmts production rule from the FCAL grammar.

Stmts ::= Stmt Stmts
*/
class MultipleStmts : public Stmts {
    public:
	/*! This is the constructor for MultipleStmts.
		\param s A Stmt object for MultipleStmts.
		\param ss A Stmts object for holding multiple statements.
	*/
	MultipleStmts(Stmt *s, Stmts *ss);
	/*! This is the class destructor. */
	~MultipleStmts();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Stmt *statement;
	Stmts *statements;
};

/*! Decl is an abstract class derived from Node.  It is the base class for all of the Decl productions in the FCAL grammar.
*/
class Decl : public Node {
    public:
	/*! This is the class destructor. */
	~Decl();

};

/*! StdDecl is a concrete class derived from Decl.  It represents a declaration of a variable of an integer, float, or string type from the FCAL grammar productions.

Decl ::= integerKwd varName ';'
Decl ::= floatKwd varName ';'
Decl ::= stringKwd varName ';'
*/
class StdDecl : public Decl {
    public:
	/*! This is the constructor for StdDecl.
		\param tt A tokenType that will be the data type of the variable.
		\param name A VarNameExpr for the name of the variable.
	*/
	StdDecl(tokenEnumType tt, VarNameExpr *name);
	/*! This is the class destructor. */
	~StdDecl();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST 	*/
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	tokenEnumType tType;
	VarNameExpr *varName;


};

/*! MatrixDecl is a concrete class derived from Decl.  It represents the declaration of a matrix from the FCAL production rules.

Decl ::= 'Matrix' varName1 '[' Expr1 ',' Expr2 ']' varName2 ',' varName3 '=' Expr3 ';'
Decl ::= 'Matrix' varName1 '=' Expr3 ';'
*/
class MatrixDecl : public Decl {
    public:
	/*! This is a constructor for MatrixDecl.
		\param n A VarNameExpr object for the name of the matrix.
		\param e An Expr object for the matrix.
	*/
	MatrixDecl(VarNameExpr *n, Expr *e);
	/*! This is a constructor for MatrixDecl.
		\param n1 A VarNameExpr object for the name of the matrix.
		\param e1 An Expr object for the matrix.
		\param e2 An Expr object for the matrix.
		\param n2 A VarNameExpr object for the name of the matrix.
		\param n3 A VarNameExpr object for the name of the matrix.
		\param e3 An Expr object for the matrix.
	*/
	MatrixDecl(VarNameExpr *n1, Expr *e1, Expr *e2, VarNameExpr *n2,
		   VarNameExpr *n3, Expr *e3);
	/*! This is the class destructor. */
	~MatrixDecl();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	VarNameExpr *varName1;
	VarNameExpr *varName2;
	VarNameExpr *varName3;
	Expr *expr1;
	Expr *expr2;
	Expr *expr3;
};

/*! Stmt is an abstract class derived from Node.  It represents the Stmt production rules within the FCAL grammar.
*/
class Stmt : public Node {
    public:
	/*! This is the class destructor. */
	~Stmt();
};

/*! DeclStmt is a concrete class derived from Stmt.  It represents a statement which is only a declaration.

Stmt ::= Decl
*/
class DeclStmt : public Stmt {
    public:
	/*! This is the constructor for DeclStmt.
		\param d A Decl object for the statement.
	*/
	DeclStmt(Decl *d);
	/*! This is the class destructor. */
	~DeclStmt();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST.  */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Decl *decl;
};

/*! StmtStmts is a concrete class derived from Stmt.  It represents a statement which contains an Stmts.

Stmt ::= '{' Stmts '}'
*/
class StmtStmts : public Stmt {
    public:
	/*! This is the constructor for StmtStmts.
		\param ss A Stmts object that will be in between the curly braces.
	*/
	StmtStmts(Stmts *ss);
	/*! This is the class destructor. */
	~StmtStmts();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Stmts *statements;
};

/*! IfStmt is a concrete class derived from Stmt.  It represents a statement with an if conditional's predicate and consequent.
 
Stmt ::= 'if' '(' Expr ')' Stmt 
*/
class IfStmt : public Stmt {
    public:
	/*! The constructor for IfStmt.
		\param r An Expr object that is the conditional predicate.
		\param s A Stmt object that is the consequent of the predicate. 
	*/
	IfStmt(Expr *e, Stmt *s);
	/*! This is the class destructor. */
	~IfStmt();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Expr *expr;
	Stmt *statement;
};


/*! IfElseStmt is a concrete class derived from Stmt.  It represents a statement with an if conditional's predicate and consequent as well as an else portion.
 
Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
*/
class IfElseStmt : public Stmt {
    public:
	/*! The constructor for IfElseStmt.
		\param e An Expr object for the predicate. 
		\param s1 A Stmt object for the consequent of the predicate if true.
		\param s2 A Stmt object for the consequent of the predicate if false.
	*/
	IfElseStmt(Expr *e, Stmt *s1, Stmt *s2);
	/*! This is the class destructor. */
	~IfElseStmt();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
        Expr *expr;
        Stmt *statement1;
        Stmt *statement2;
};

/*! PrintStmt is a concrete class derived from Stmt.  It represents a statement that prints out a given expression.
 
Stmt ::= 'print' '(' Expr ')' ';'
*/
class PrintStmt : public Stmt {
    public:
	/*! This is the constructor for PrintStmt.
		\param e An Expr object that will be printed.
	*/
	PrintStmt(Expr *e);
	/*! This is the class destructor. */
	~PrintStmt();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Expr *expr;
};

/*! ForLoopStmt is a concrete class derived from Stmt.  It represents the statement of a for loop.
 
Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt
*/
class ForLoopStmt : public Stmt {
    public:
	/*! This is the constructor for ForLoopStmt.
		\param name A VarNameExpr object for the index variable name.
		\param e1 An Expr object for initializing the expression of the loop.
		\param e2 An Expr object for the conditional of the loop. 
		\param s A Stmt object for the body of the loop.
	*/
	ForLoopStmt(VarNameExpr *name, Expr *e1, Expr *e2, Stmt *s);
	/*! This is the class destructor. */
	~ForLoopStmt();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	VarNameExpr *varName;
	Expr *expr1;
	Expr *expr2;
	Stmt *statement;
};

/*! WhileStmt is a concrete class derived from Stmt.  It represents the statement of a while loop.
 
Stmt ::= 'while' '(' Expr ')' Stmt
*/
class WhileStmt : public Stmt {
    public:
	/*! This is the constructor for WhileStmt.
		\param e An Expr object for the conditional of the loop.
		\param s A Stmt object for body of the loop. 
	*/
	WhileStmt(Expr *e, Stmt *s);
	/*! This is the class destructor. */
	~WhileStmt();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Expr *expr;
	Stmt *statement;
};

/*! SemiColonStmt is a concrete class derived from Stmt.  It represents the statement which contains only a semi colon.
 
Stmt ::= ';'
*/
class SemiColonStmt : public Stmt {
    public:
	/*! This is the constructor of SemiColonStmt. No parameters. */
	SemiColonStmt();
	/*! This is the class destructor.
	*/
	~SemiColonStmt();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
};

/*! VarStmt is a concrete class derived from Stmt.  It represents the statement which defines a variable of the given name.
 
Stmt ::= varName '=' Expr3 ';'  | varName '[' Expr1 ',' Expr2 ']' '=' Expr3 ';'
*/
class VarStmt : public Stmt {
    public:
	/*! This is a constructor of VarStmt.
		\param name A VarNameExpr object for the variable's name.
		\param e An Expr object for defining the variable.
	*/
	VarStmt(VarNameExpr *name, Expr *e);
	/*! This is a constructor for VarStmt for a matrix.
		\param name A VarNameExpr object for the variable's name.
		\param e1 An Expr object for defining the row number of the matrix.
		\param e2 An Expr object for defining the column number of the matrix.
		\param e3 An Expr object for defining the entry corresponding to a row and column in the matrix. 
	*/
	VarStmt(VarNameExpr *name, Expr *e1, Expr *e2, Expr *e3);
	/*! This is the class destructor. */
	~VarStmt();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	VarNameExpr *varName;
	Expr *expr1;
	Expr *expr2;
	Expr *expr3;
};


/*! Expr is an abstract class derived from Node.  It represents the Expr production rules within the FCAL grammar.
*/
class Expr : public Node {
    public:
	/*! This is the class destructor. */
	~Expr();
};

/*! VarNameExpr is a concrete class derived from Expr.  It represents an expression which is a variable name.
 
Expr ::= varName
*/
class VarNameExpr : public Expr {
    public:
	/*! This is the constructor for VarNameExpr.
		\param c A string that contains the variable name.
	*/
	VarNameExpr(std::string c);
	/*! This is the class destructor. */
	~VarNameExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    //private:
	std::string name;
};

/*! NormExpr is a concrete class derived from Expr.  It represents an expression within a pair of parentheses.
 
Expr ::= '(' Expr ')'  
*/
class NormExpr : public Expr {
    public:
	/*! This is the constructor for NormExpr.
		\param e An Expr object that will be containined within the parentheses.
	*/
	NormExpr(Expr *e);
	/*! This is the class destructor. */
	~NormExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Expr *expr;
};

/*! CallExpr is a concrete class derived from Expr.  It represents an expression which is calling a function by the given name with the given expression as its parameter.
 
Expr ::= varName '(' Expr ')'        //NestedOrFunctionCall  
*/
class CallExpr : public Expr {
    public:
	/*! This is the constructor for CallExpr.
		\param name A VarNameExpr object that will be the name of the function being called.
		\param e An Expr object that will be passed as the parameter. 
	*/
	CallExpr(VarNameExpr *name, Expr *e);
	/*! This is the class destructor. */
	~CallExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	VarNameExpr *varName;
	Expr *expr;
};

/*! MatrixREFExpr is a concrete class derived from Expr.  It represents an expression defines a matrix by the given name and expression.
 
Expr ::= varName '[' Expr ',' Expr ']'   //MatrixREf
*/
class MatrixREFExpr : public Expr {
    public:
	/*! This is the constructor for MatrixREFExpr.
		\param name A VarNameExpr object for the name of the matrix being defined. 
		\param e1 An Expr object for the matrix row expression.
		\param e2 An Expr object for the matrix column expression.
	*/
	MatrixREFExpr(VarNameExpr *name, Expr *e1, Expr *e2);
	/*! This is the class destructor. */
	~MatrixREFExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	VarNameExpr *varName;
	Expr *expr1;
	Expr *expr2;
};

/*! BinaryExpr is a concrete class derived from Expr.  It represents a pair of expressions joined by an operator.
 
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
Expr ::= Expr '&&' Expr
Expr ::= Expr '||' Expr 
*/
class BinaryExpr : public Expr {
    public:
	/*! This is the constructor for BinaryExpr.
		\param l An Expr object for the left side of the operator. 
		\param op A string that represents the operator in the source files.
		\param r An Expr object for the right side of the operator. 
	*/
	BinaryExpr(Expr *l, std::string op, Expr *r);
	/*! This is the class destructor. */
	~BinaryExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Expr *left;
	Expr *right;
	std::string operation;
};

/*! ConstExpr is a concrete class derived from Expr.  It represents a constant value for an int, float, or string.

Expr ::= integerConst | floatConst |  stringConst
*/
class ConstExpr : public Expr {
    public:
	/*! This is a constructor for ConstExpr for an integer constant.
		\param ii A value of the integer - if one exists.
	*/
	ConstExpr(int ii);
	/*! This is a constructor for ConstExpr for an float constant.
		\param fi A value of the float - if one exists.
	*/
	ConstExpr(float fi);
	/*! This is a constructor for ConstExpr for an string constant.
		\param constString A value of the string - if one exists.
	*/
	ConstExpr(std::string s);
	/*! This is a constructor for ConstExpr for an boolean value.
		\param v A value of the bool - if one exists.
	*/
	ConstExpr(bool v);
	/*! This is the class destructor. */
	~ConstExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
    /*! constType store the type of constant expression this is.  0 = int, 1 = float, 2 = bool, 3 = string... */
    int constType;
	int i;
	float f;
	bool b;
	std::string constString;
};

/*! LetExpr is a concrete class derived from Expr.  It represents a Stmts and Expr coupling for let within the FCAL grammar.
 
Expr ::= 'let' Stmts 'in' Expr 'end'  //LetExpr
*/
class LetExpr : public Expr {
    public:
	/*! This is the constructor for LetExpr.
		\param ss A Stmts object that is the let statements.
		\param e An Expr object that is the let expression.
	*/
	LetExpr(Stmts *ss, Expr *e);
	/*! This is the class destructor. */
	~LetExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Stmts *statements;
	Expr *expr;
};

/*! IfExpr is a concrete class derived from Expr.  It represents an if then expression.
 
Expr ::= 'if' Expr 'then' Expr 'else' Expr    //IfExpr
*/
class IfExpr : public Expr {
    public:
	/*! This is the constructor for IfExpr.
		\param e1 An Expr object for the if expression.
		\param e2 An Expr object for the then expression.
		\param e3 An Expr object for the else expression. 
	*/
	IfExpr(Expr *e1, Expr *e2, Expr *e3);
	/*! This is the class destructor. */
	~IfExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Expr *expr1;
	Expr *expr2;
	Expr *expr3;
};

/*! NotExpr is a concrete class derived from Expr.  It represents the negation of the given expression.
 
Expr ::= '!' Expr                          //NotExpr
*/
class NotExpr : public Expr {
    public:
	/*! This is the constructor for NotExpr. 
		\param e An Expr object for the expression to be negated.
	*/
	NotExpr(Expr *e);
	/*! This is the class destructor. */
	~NotExpr();

	/*! This class defines the inherited virtual function for unparse, which returns a string of the FCAL code generated from the AST. */
	std::string unparse();
	/*! This class defines the inherited virtual function for toCPP, which returns a string of the C++ code generated from the AST. */
	std::string cppCode();
    private:
	Expr *expr;
};


#endif

