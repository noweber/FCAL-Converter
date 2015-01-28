#include "AST.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // atoi()
#include <boost/lexical_cast.hpp>

/*!
	@file AST.cpp
	@author Nicholas Weber
	@author Amy Le
	Student IDs: 3408684 4520434
	Class:		3081W Fall 2014
	Section:	04
	TA:		Kevin Thomsen
	@brief CPP file for AST methods
*/

// Node
Node::~Node() {

}



// Root
Root::Root(VarNameExpr *v, Stmts *ss) {
    programName = v;
    programStmts = ss;
}

Root::~Root() {

}

std::string Root::unparse() {
// cout << "int " << programName->unparse()<< " ( ) {" << programStmts->unparse()<< " }" << endl;
    std::string ret;
    if(programName != NULL && programStmts != NULL) {
    //ret += "main ";
        ret += programName->unparse();
        ret += " ( ) { ";
        ret += programStmts->unparse();
        ret += " } ";
    } else {
	printf("Failed: Root::unparse().\n");
    }
    //printf("\n%s\n", ret.c_str());
    return ret;
}

std::string Root::cppCode() {
    std::string retPaladin;
    if(programName != NULL && programStmts != NULL) {
	retPaladin += "#include \"Matrix.h\"\n#include <math.h>\n#include <iostream>\nusing namespace std;\n";
	retPaladin += "int " + programName->cppCode() + "( ) { \n" + programStmts->cppCode() + "\n }\n";
    } else {
	printf("Failed: Root::cppCode().\n");
    }
    return retPaladin;
    /*if(programName != NULL && programStmts != NULL) {
        std::cout << "#include \"Matrix.h\"\n";
	std::cout << "#include <math.h>\n";
	std::cout << "#include <iostrean>\n";
	//cout << "\n" << "using namespace std; \n";
	std::cout << "int main ( ) {\n";
	programStmts->cppCode();
	std::cout  << "\n }\n";
    } else {
	printf("Failed: Root::cppCode().\n");
    }*/
}

// Stmts
Stmts::~Stmts() {

}

// EmptyStmts
EmptyStmts::EmptyStmts() {

}

EmptyStmts::~EmptyStmts() {

}

std::string EmptyStmts::unparse() {
    std::string ret = "";
    return ret;
}

std::string EmptyStmts::cppCode() {
    std::string ret = "";
    return ret;
}

// MultipleStmts
MultipleStmts::MultipleStmts(Stmt *s, Stmts *ss) {
    statement = s;
    statements = ss;
}

MultipleStmts::~MultipleStmts() {

}

std::string MultipleStmts::unparse() {
    std::string ret;
    if(statement != NULL && statements != NULL) {
        ret += statement->unparse();
        ret += statements->unparse();
    } else {
	printf("Failed: MultipleStmts::unparse().\n");
    }
    return ret;
}

std::string MultipleStmts::cppCode() {
    std::string ret;
    if(statement != NULL && statements != NULL) {
        ret += statement->cppCode();
        ret += statements->cppCode();
    } else {
	printf("Failed: MultipleStmts::cppCode().\n");
    }
    return ret;
    /*if(statement != NULL && statements != NULL) {
        statement->cppCode();
        statements->cppCode();
    } else {
        printf("Failed: MultipleStmts::cppCode().\n");
    }*/
}

//Decl
Decl::~Decl() {

}

// StdDecl
StdDecl::StdDecl(tokenEnumType tt, VarNameExpr *name) {
    varName = name;
    tType = tt;
}

StdDecl::~StdDecl() {

}

std::string StdDecl::unparse() {
    std::string ret;
        ret += " ";
        if(tType == intKwd) {
	    ret += "Int";
        }
        else if(tType == floatKwd) {
            ret += "Float";
        }
        else if(tType == stringKwd) {
            ret += "String";
        }
        else if(tType == boolKwd) {
            ret += "Bool";
        } else {
	    printf("Failed: Unknown token type for StdDecl::unparse().\n");
        }
        ret += " " + this->varName->unparse() + " ; ";
    return ret;
}

std::string StdDecl::cppCode() {
    std::string ret;
    if(tType == intKwd) {
        ret += "int";
    }
    else if(tType == floatKwd) {
        ret += "float";
    }
    else if(tType == stringKwd) {
        ret += "String";
    }
    else if(tType == boolKwd) {
        ret += "bool";
    } else {
	printf("Failed: Unknown token type for StdDecl::cppCode().\n");
    }
    ret += " " + this->varName->cppCode() + " ;\n ";
    return ret;
	/*if(tType == intKwd) {
	    td::cout << "int ";
        }
        else if(tType == floatKwd) {
            std::cout << "float ";
        }
        else if(tType == stringKwd) {
		//change
            std::cout << "String ";
        }
        else if(tType == boolKwd) {
            std::cout << "bool ";
        }
        this->varName->cppCode();
	std::cout << " ;\n";*/
}

// MatrixDecl
MatrixDecl::MatrixDecl(VarNameExpr *n, Expr *e) {
    varName1 = n;
    varName2 = NULL;
    varName3 = NULL;
    expr1 = NULL;
    expr2 = NULL;
    expr3 = e;
}

MatrixDecl::MatrixDecl(VarNameExpr *n1, Expr *e1, Expr *e2, VarNameExpr *n2,
			VarNameExpr *n3, Expr *e3) {
    varName1 = n1;
    varName2 = n2;
    varName3 = n3;
    expr1 = e1;
    expr2 = e2;
    expr3 = e3;
}

MatrixDecl::~MatrixDecl() {

}
//Decl ::= 'Matrix' varName1 '[' Expr1 ',' Expr2 ']' varName2 ',' varName3 '=' Expr3 ';'
//Decl ::= 'Matrix' varName1 '=' Expr3 ';'
std::string MatrixDecl::unparse() {
    std::string ret;
    if(varName1 != NULL) {
        ret += " Matrix " + varName1->unparse();
    }
    if(expr1 == NULL && expr2 == NULL && expr3 != NULL) {
    	ret += " = " + expr3->unparse() + " ; ";
    } else if (expr1 != NULL && expr2 != NULL && expr3 != NULL && varName2 != NULL && varName3 != NULL){
        ret += " [ " + expr1->unparse() + " , " + expr2->unparse() + " ] " + varName2->unparse() + " , " + varName3->unparse() +  " = " + expr3->unparse() + " ; ";
    } else {
	printf("Failed: MatrixDecl::unparse().\n");
    }
    return ret;
}
//check sample file 7 for cpp syntax of matrix.
std::string MatrixDecl::cppCode() {
        // TODO Decl ::= 'Matrix' varName1 '[' Expr1 ',' Expr2 ']' varName2 ',' varName3 '=' Expr3 ';'
    std::string ret;
    /*for the Decl ::= 'Matrix' varName1 '=' Expr3 ';'*/
    if(expr1 == NULL && expr2 == NULL && expr3 != NULL) {
		ret += "Matrix " + varName1->cppCode() + " ( Matrix::" + expr3->cppCode() + " ) ;\n";
    }
    else if(expr1 != NULL && expr2 != NULL && expr3 != NULL && varName2 != NULL && varName3 != NULL) {
	ret += "Matrix " + varName1->cppCode() + "(" + expr1->cppCode() + " , " + expr2->cppCode() + ");\n";
        ret += "for (int " + varName2->cppCode() + " = 0; " + varName2->cppCode() + " < " + expr1->cppCode() + "; " + varName2->cppCode() + " ++) {\n"; 
        ret += "    for (int " + varName3->cppCode() + " = 0; " + varName3->cppCode() + " < " + expr2->cppCode() + "; " + varName3->cppCode() +" ++) {\n";
	ret += "        *(" + varName1->cppCode() + ".access(" +varName2->cppCode() + " , " + varName3->cppCode() + ")) = " + expr3->cppCode() + ";\n    }\n}\n";
    }
    return ret;
}

// Stmt
Stmt::~Stmt() {

}

// DeclStmt
DeclStmt::DeclStmt(Decl *d) {
    decl = d;
}

DeclStmt::~DeclStmt() {

}

std::string DeclStmt::unparse() {
    std::string ret;
    if(decl != NULL) {
	ret += decl->unparse();
    } else {
	printf("Failed: DeclStmt::unparse().\n");
    }
    return ret;
}

std::string DeclStmt::cppCode() {
    std::string ret;
    if(decl != NULL) {
        ret += decl->cppCode();
    } else {
	printf("Failed: DeclStmt::cppCode().\n");
    }
    return ret;
}

// StmtStmts
StmtStmts::StmtStmts(Stmts *ss) {
    statements = ss;
}

StmtStmts::~StmtStmts() {

}

std::string StmtStmts::unparse() {
    std::string ret;
    if(statements != NULL) {
        ret += " { " + statements->unparse() + " } ";
    } else {
	printf("Failed: StmtStmts::unparse().\n");
    }
    return ret;
}

std::string StmtStmts::cppCode() {
    std::string ret;
    if(statements != NULL) {
        ret += "{ \n" + statements->cppCode() + " }\n";
    } else {
	printf("Failed: StmtStmts::cppCode().\n");
    }
    return ret;
}

// IfStmt
IfStmt::IfStmt(Expr *e, Stmt *s) {
    expr = e;
    statement = s;
}

IfStmt::~IfStmt() {

}

std::string IfStmt::unparse() {
    std::string ret;
    if(expr != NULL && statement != NULL) {
        ret += " if ( " + expr->unparse() + " ) " + statement->unparse();
    } else {
	printf("Failed: IfStmt::unparse().\n");
    }
    return ret;
}

std::string IfStmt::cppCode() {
    std::string ret;
    if(expr != NULL && statement != NULL) {
        ret += "if ( ";
	ret += expr->cppCode();
	ret += " )\n";
	ret += statement->cppCode();
    } else {
	printf("Failed: IfStmt::cppCode().\n");
    }
    return ret;
}

// IfElseStmt
IfElseStmt::IfElseStmt(Expr *e, Stmt *s1, Stmt *s2) {
    expr = e;
    statement1 = s1;
    statement2 = s2;
}

IfElseStmt::~IfElseStmt() {

}

std::string IfElseStmt::unparse() {
    std::string ret;
    if(expr != NULL && statement1 != NULL && statement2 != NULL) {
        ret += " if ( " + expr->unparse() + " ) " + statement1->unparse();
        ret += " else " + statement2->unparse();
    } else {
	printf("Failed: IfElseStmt::unparse().\n");
    }
    return ret;
}

std::string IfElseStmt::cppCode() {
    std::string ret;
    ret += " if ( " + expr->cppCode() + " ) \n" + statement1->cppCode() + "\n";
    ret += " else " + statement2->cppCode();
    return ret;
}

// PrintStmt
PrintStmt::PrintStmt(Expr *e) {
    expr = e;
}

PrintStmt::~PrintStmt() {

}

std::string PrintStmt::unparse() {
    std::string ret;
    if(expr != NULL) {
        ret += "print ( " + expr->unparse() + " ) ; ";
    } else {
	printf("Failed: PrintStmt::unparse().\n");
    }
    return ret;
}
// according to sample files, output should be cout for print. 
std::string PrintStmt::cppCode() {
    std::string ret;
    if(expr != NULL) {
        ret += "cout << ";
	ret += expr->cppCode();
	ret += " ;\n";
    } else {
	printf("Failed: PrintStmt::cppCode().\n");
    }
    return ret;
}

// ForLoopStmt
ForLoopStmt::ForLoopStmt(VarNameExpr *name, Expr *e1, Expr *e2, Stmt *s) {
    varName = name;
    expr1 = e1;
    expr2 = e2;
    statement = s;
}

ForLoopStmt::~ForLoopStmt() {

}

std::string ForLoopStmt::unparse() {
    std::string ret;
    if(varName != NULL && expr1 != NULL && expr2 != NULL && statement != NULL) {
        ret += "for ( " + varName->unparse() + " = " + expr1->unparse() + " : ";
        ret += expr2->unparse() + " ) " + statement->unparse();
    } else {
	printf("Failed: ForLoopStmt::unparse().\n");
    }
    return ret;
}
//check sample files 5 and 6 for syntax. 
std::string ForLoopStmt::cppCode() {
	//TODO
    std::string ret;
    if(varName != NULL && expr1 != NULL && expr2 != NULL && statement != NULL) {
	ret += "for (" + varName->cppCode() + " = " + expr1->cppCode() + "; ";
	ret += varName->cppCode() + " <= " + expr2->cppCode() + "; ";
	ret += varName->cppCode() + " ++ )" + statement->cppCode(); 
    } else {
	printf("Failed: ForLoopStmt::cppCode().\n");
    }
    return ret;
}

// WhileStmt
WhileStmt::WhileStmt(Expr *e, Stmt *s) {
    expr = e;
    statement = s;
}

WhileStmt::~WhileStmt() {

}

std::string WhileStmt::unparse() {
    std::string ret;
    if(expr != NULL && statement != NULL) {
        ret += "while ( " + expr->unparse() + " ) " + statement->unparse();
    } else {
	printf("Failed: WhileStmt::unparse().\n");
    }
    return ret;
}

std::string WhileStmt::cppCode() {
    std::string ret;
    if(expr != NULL && statement != NULL) {
        ret += "while ( ";
	ret += expr->cppCode();
	ret += " )\n";
	ret += statement->cppCode();
    } else {
	printf("Failed: WhileStmt::cppCode().\n");
    }
    return ret;
}

// SemiColonStmt
SemiColonStmt::SemiColonStmt() {

}

SemiColonStmt::~SemiColonStmt() {

}

std::string SemiColonStmt::unparse() {
    std::string ret;
    ret += " ; ";
    return ret;
}

std::string SemiColonStmt::cppCode() {
    std::string ret;
    ret += " ; ";
    return ret;
}

// VarStmt

VarStmt::VarStmt(VarNameExpr *name, Expr *e) {
    varName = name;
    expr1 = NULL;
    expr2 = NULL;
    expr3 = e;
}
// NOTE: Stmt ::= varName '=' Expr3 ';'  | varName '[' Expr1 ',' Expr2 ']' '=' Expr3 ';'
VarStmt::VarStmt(VarNameExpr *name, Expr *e1, Expr *e2, Expr *e3) {
    varName = name;
    expr1 = e1;
    expr2 = e2;
    expr3 = e3;
}

VarStmt::~VarStmt() {

}

std::string VarStmt::unparse() {
    std::string ret;
    ret += varName->unparse();
    if(expr1 == NULL && expr2 == NULL && expr3 != NULL && varName != NULL) {
	ret += " = " + expr3->unparse() + " ; ";
    } else if(expr1 != NULL && expr2 != NULL && expr3 != NULL && varName != NULL) {
	ret += " [ " + expr1->unparse() + " , " + expr2->unparse() + " ] = " + expr3->unparse() + " ; ";
    } else {
        printf("Failed: VarStmt::unparse().\n");
    }
    return ret;
}

std::string VarStmt::cppCode() {
    std::string ret;
    if(expr1 == NULL && expr2 == NULL && expr3 != NULL && varName != NULL) {
	ret += varName->cppCode() + " = " + expr3->cppCode() + ";\n";
    } else if(expr1 != NULL && expr2 != NULL && expr3 != NULL && varName != NULL) {
	ret += "*(" + varName->cppCode() + ".access( " + expr1->cppCode() + " , " + expr2->cppCode() + " )) = " + expr3->cppCode() + " ;\n";
    } else {
	printf("Failed: VarStmt::cppCode().\n");
    }
    return ret;
}

// Expr
Expr::~Expr() {

}

// VarNameExpr
VarNameExpr::VarNameExpr(std::string c) {
    name = c;
}

VarNameExpr::~VarNameExpr() {

}

std::string VarNameExpr::unparse() {
    std::string ret;
    ret += name;
    return ret;
}

std::string VarNameExpr::cppCode() {
    std::string ret;
    ret += name;
    return ret;
}


// NormExpr
NormExpr::NormExpr(Expr *e) {
    expr = e;
}

NormExpr::~NormExpr() {

}

std::string NormExpr::unparse() {
    std::string ret;
    if(expr != NULL) {
        ret += " ( " + expr->unparse() + " ) ";
    } else {
	printf("Failed: NormExpr::unparse().\n");
    }
    return ret;
}

std::string NormExpr::cppCode() {
    std::string ret;
    if(expr != NULL) {
        ret += " ( " + expr->cppCode() + " ) ";
    } else {
	printf("Failed: NormExpr::cppCode().\n");
    }
    return ret;
}

// CallExpr
CallExpr::CallExpr(VarNameExpr *name, Expr *e) {
    varName = name;
    expr = e;
}

CallExpr::~CallExpr() {

}

std::string CallExpr::unparse() {
    std::string ret;
    if(varName != NULL && expr != NULL) {
	ret += varName->unparse() + " ( " + expr->unparse() + " ) ";
    } else {
	printf("Failed: CalLExpr::unparse().\n");
    }
    return ret;
}

std::string CallExpr::cppCode() {
    std::string ret;

    // Handle Matrix class member fucntion calls
    if( varName->name.compare("numRows") == 0 ||
        varName->name.compare("numCols") == 0 ) {
        ret += expr->unparse() + "." + varName->unparse() + "() ";
        //printf("%s", ret.c_str());
        return ret;
    }

    if(varName != NULL && expr != NULL) {
        ret += varName->cppCode() + " ( " + expr->cppCode() + " ) ";
    } else {
	printf("Failed: CallExpr::cppCode().\n");
    }
    //printf("%s", ret.c_str());
    return ret;
}

// MatrixREFExpr
MatrixREFExpr::MatrixREFExpr(VarNameExpr *name, Expr *e1, Expr *e2) {
    varName = name;
    expr1 = e1;
    expr2 = e2;
}

MatrixREFExpr::~MatrixREFExpr() {

}

std::string MatrixREFExpr::unparse() {
    std::string ret;
    if(varName != NULL && expr1 != NULL && expr2 != NULL) {
    	ret += varName->unparse() + " [ " + expr1->unparse() + " , " + expr2->unparse() + " ] ";
    } else {
	printf("Failed: MatrixREFExpr::unparse().\n");
    }
    return ret;
}

std::string MatrixREFExpr::cppCode() {
    std::string ret;
    if(varName != NULL && expr1 != NULL && expr2 != NULL) {
        ret += "*( " + varName->cppCode() + ".access(" + expr1->cppCode() + " , " + expr2->cppCode() + "))";
    } else {
	printf("Failed: MatrixREFExpr::cppCode().\n");
    }
    return ret;
}

// BinaryExpr
BinaryExpr::BinaryExpr(Expr *l, std::string op, Expr *r) {
    left = l;
    operation = op;
    right = r;
}

BinaryExpr::~BinaryExpr() {

}

std::string BinaryExpr::unparse() {
    std::string ret;
 
	/*ret += left->unparse();
	if((const char*)operation == "*") {
		ret += " * ";
	} else if((const char*)operation == "/") {
		ret += " / ";
	} else if((const char*)operation == "+") {
		ret += " + ";
	} else if((const char*)operation == "-") {
		ret += " - ";
	} else if((const char*)operation == ">") {
		ret += " > ";
	} else if((const char*)operation == ">=") {
		ret += " >= ";
	} else if((const char*)operation == "<") {
		ret += " < ";
	} else if((const char*)operation == "<=") {
		ret += " <= ";
	} else if((const char*)operation == "==") {
		ret += " == ";
	} else if((const char*)operation == "!=") {
		ret += " != ";
	} else if((const char*)operation == "&&") {
		ret += " && ";
	} else if((const char*)operation == "||") {
		ret += " || ";
	}
	ret += right->unparse();*/

        ret += left->unparse() + " " + this->operation.c_str() + " " + right->unparse();
    return ret;
}

std::string BinaryExpr::cppCode() {
    std::string ret;
    ret += "(" + left->cppCode() + " " + this->operation.c_str() + " " + right->cppCode() + ")";
//    ret += left->cppCode();
//    ret += " " + this->operation.c_str() + " ";
//    ret += right->cppCode();
    return ret;
}

// ConstExpr
ConstExpr::ConstExpr(int ii) {
    i = ii;
    constType = 0;
}
ConstExpr::ConstExpr(float fi) {
    f = fi;
    constType = 1;
}
ConstExpr::ConstExpr(bool v) {
    b = v;
    constType = 2;
}
ConstExpr::ConstExpr(std::string s) {
    constString = s;
    constType = 3;
}
ConstExpr::~ConstExpr() {

}

std::string ConstExpr::unparse() {
    std::string ret;
	switch(constType) {
	case 0:
		ret += boost::lexical_cast<std::string>(i);
		break;
	case 1:
		ret += boost::lexical_cast<std::string>(f);
		break;
	case 2:
		if(b == true) {
       		    ret += "true";
		} else {
		    ret += "false";
		}
		break;	
	case 3:
		ret += constString;
		break;
        default:
            printf("Failed: ConstExpr::unparse()\n");
            break;
	}
    return ret;
}

std::string ConstExpr::cppCode() {
    std::string ret;
    switch(constType) {
        case 0:
            ret += boost::lexical_cast<std::string>(i);
            break;
        case 1:
            ret += boost::lexical_cast<std::string>(f);
            break;
        case 2:
            if(b == true) {
                ret += "true";
            } else {
                ret += "false";
            }
            break;
        case 3:
            ret += constString;
            break;
	default:
            printf("Failed: ConstExpr::cppCode()\n");
            break;
    }
    return ret;
}

// LetExpr
LetExpr::LetExpr(Stmts *ss, Expr *e) {
    statements = ss;
    expr = e;
}

LetExpr::~LetExpr() {

}

std::string LetExpr::unparse() {
    std::string ret;
    if(statements != NULL && expr != NULL) {
        ret += " let " + statements->unparse();
        ret += " in " + expr->unparse() + " end ";
    } else {
	printf("Failed: LetExpr::unparse().\n");
    }
    return ret;
}
//check sample file 6 for syntax of the let. 
std::string LetExpr::cppCode() {
    std::string ret;
    ret += "({ " + statements->cppCode();
    ret += "    " + expr->cppCode() + " ; \n    })";
    return ret;
}

// IfExpr
IfExpr::IfExpr(Expr *e1, Expr *e2, Expr *e3) {
    expr1 = e1;
    expr2 = e2;
    expr3 = e3;
}

IfExpr::~IfExpr() {

}

std::string IfExpr::unparse() {
    std::string ret;
    if(expr1 != NULL && expr2 != NULL && expr3 != NULL) {
        ret += " if " + expr1->unparse();
        ret += " then " + expr2->unparse();
        ret += " else " +  expr3->unparse();
    } else {
	printf("Failed: IfExpr::unparse().\n");
    }
    return ret;
}

std::string IfExpr::cppCode() {
    std::string ret;
    if(expr1 != NULL && expr2 != NULL && expr3 != NULL) {
        ret += "\n";
        ret += "if( ";
	ret += expr1->cppCode();
	ret += " ) {\n";
        ret += "    ";
	ret += expr2->cppCode() + " ; ";
	ret += "\n";
        ret += "} else {\n";
        ret += expr3->cppCode() + " ; ";
	ret += "\n";
        ret += "}\n";
    } else {
        printf("Failed: IfExpr::cppCode().\n");
    }
    return ret;
}

// NotExpr
NotExpr::NotExpr(Expr *e) {
    if(e != NULL) {
        expr = e;
    } else {
        printf("Failed: NotExpr constructor. Expression parameter was NULL.\n");
    }
}

NotExpr::~NotExpr() {

}

std::string NotExpr::unparse() {
    std::string ret;
    if(expr != NULL) {
        ret += "!" + expr->unparse();
    } else {
	printf("Failed: NotExpr::unparse().\n");
    }
    return ret;
}

std::string NotExpr::cppCode() {
    std::string ret;
    if(expr != NULL) {
        ret += "!";
	ret += expr->cppCode();
    } else {
        printf("Failed: NotExpr::cppCode().\n");
    }
    return ret;
}


