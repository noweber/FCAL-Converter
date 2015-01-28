/* ExtToken: an extension of the Token class with new methods for parsing 
   (led, nud, lbp) and describing the token.

   Author: Eric Van Wyk

   Last modified: March 4, 2013.
*/

#ifndef EXTTOKEN_H
#define EXTTOKEN_H

#include "scanner.h"
#include "parser.h"

class ExtToken {
public:
    ExtToken (Parser *p, Token *t) 
        : lexeme(t->lexeme), terminal(t->terminal), parser(p) { }
    ExtToken (Parser *p, Token *t, std::string d) 
        : lexeme(t->lexeme), terminal(t->terminal), parser(p), descStr(d) { }

    virtual ~ExtToken () { } ;

    virtual ParseResult nud () {
        throw ( parser->makeErrorMsg (parser->currToken->terminal) ) ;
    }

    virtual ParseResult led (ParseResult left) {
        throw ( parser->makeErrorMsg (parser->currToken->terminal) ) ;
    }
    std::string lexeme ;
    tokenType terminal ;
    ExtToken *next ;
    Parser *parser;

    virtual int lbp() { return 0 ; }
    virtual std::string description() { return descStr ; }

private:
    ExtToken () : parser(NULL) { } 
    std::string descStr ;
} ;

ExtToken *extendToken (Parser *p, Token *tokens) ;
ExtToken *extendTokenList (Parser *p, Token *tokens) ;

/* For each terminal symbol that will play some unique role in the
   semantic analysis of the program, we need a unique subclass of
   ExtToken.
 */
class NotOpToken : public ExtToken {
public:
    NotOpToken (Parser *p, Token *t) : ExtToken(p,t) { }
    //TODO make real function
    ParseResult nud () { return parser->parseNotExpr(); }
    std::string description() { return "notOp"; }
} ;



// True Kwd
class TrueKwdToken : public ExtToken {
public:
    TrueKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseTrueKwd (); }
    std::string description() { return "true const"; }
} ;

// False Kwd
class FalseKwdToken : public ExtToken {
public:
    FalseKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseFalseKwd (); }
    std::string description() { return "false const"; }
} ;

// Int Const
class IntConstToken : public ExtToken {
public:
    IntConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseIntConst (); }
    std::string description() { return "int const"; }
} ;

// Float Const
class FloatConstToken : public ExtToken {
public:
    FloatConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseFloatConst (); }
    std::string description() { return "float const"; }
} ;

// String Const
class StringConstToken : public ExtToken {
public:
    StringConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseStringConst (); }
    std::string description() { return "string const"; }
} ;

// Char Const
class CharConstToken : public ExtToken {
public:
    CharConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseCharConst (); }
    std::string description() { return "char const"; }
} ;

// Variable Name
class VariableNameToken : public ExtToken {
public:
    VariableNameToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseVariableName (); }
    std::string description() { return "variable name"; }
} ;

class IfToken:public ExtToken{
    public:
    IfToken (Parser *p, Token *t) : ExtToken(p,t) { };
    ParseResult nud () { return parser->parseIfExpr () ; }
    std::string description() { return "'if'"; }
    int lbp() { return 80; }
};
class LetToken:public ExtToken{
    public:
    LetToken (Parser *p, Token *t) : ExtToken(p,t) { };
    ParseResult nud () { return parser->parseLetExpr () ; }
    std::string description() { return "'let'"; }
    int lbp() { return 80; }
};
    
// Left Paren
class LeftParenToken : public ExtToken {
public:
    LeftParenToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseNestedExpr () ; }
    std::string description() { return "'('"; }
    int lbp() { return 80; }
} ;

// Plus Sign
class PlusSignToken : public ExtToken {
public:
    PlusSignToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseAddition (left) ; 
    }
    std::string description() { return "'+'"; }
    int lbp() { return 50; }
} ;

// Star
class StarToken : public ExtToken {
public:
    StarToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseMultiplication (left) ; 
    }
    std::string description() { return "'*'"; }
    int lbp() { return 60; }
} ;

// Dash
class DashToken : public ExtToken {
public:
    DashToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseSubtraction (left) ; 
    }
    std::string description() { return "'-'"; }
    int lbp() { return 50; }
} ;

// ForwardSlash
class ForwardSlashToken : public ExtToken {
public:
    ForwardSlashToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseDivision (left) ; 
    }
    std::string description() { return "/"; }
    int lbp() { return 60; }
} ;

// Relational Op
class RelationalOpToken : public ExtToken {
public:
    RelationalOpToken (Parser *p, Token *t, std::string d) : ExtToken(p,t,d) { }
    ParseResult led (ParseResult left) {
        return parser->parseRelationalExpr (left) ; 
    }
    int lbp() { return 30; }
} ;


// End of File
class EndOfFileToken : public ExtToken {
public:
    EndOfFileToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "end of file"; }
} ;


/*

// Left Angle
class LeftAngleToken : public ExtToken {
public:
    LeftAngleToken (Parser *p, Token *t) : ExtToken(p,t) { }

    ParseResult led (ParseResult left) {
        return parser->parseRelationalExpr (left) ; 
    }

    std::string description() { return "'('"; }
    int lbp() { return 40; }
} ;

// Right Angle
class RightAngleToken : public ExtToken {
public:
    RightAngleToken (Parser *p, Token *t) : ExtToken(p,t) { }

    ParseResult led (ParseResult left) {
        return parser->parseRelationalExpr (left) ; 
    }

    std::string description() { return "'('"; }
    int lbp() { return 40; }
} ;



// In Kwd
class InKwdToken : public ExtToken {
public:
    InKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'In'"; }
} ;

// End Kwd
class EndKwdToken : public ExtToken {
public:
    EndKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'End'"; }
} ;

// If Kwd
class IfKwdToken : public ExtToken {
public:
    IfKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseIfThenElse (); }
    std::string description() { return "'if'"; }
} ;

// Then Kwd
class ThenKwdToken : public ExtToken {
public:
    ThenKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'then'"; }
} ;

// Else Kwd
class ElseKwdToken : public ExtToken {
public:
    ElseKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'else'"; }
} ;

// Print Kwd
class PrintKwdToken : public ExtToken {
public:
    PrintKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parsePrintExpr (); }
    std::string description() { return "'print'"; }
} ;

// Read Kwd
class ReadKwdToken : public ExtToken {
public:
    ReadKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseReadExpr (); }
    std::string description() { return "'read'"; }
} ;

// Write Kwd
class WriteKwdToken : public ExtToken {
public:
    WriteKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseWriteExpr (); }
    std::string description() { return "'write'"; }
} ;

// Integer Kwd
class IntegerKwdToken : public ExtToken {
public:
    IntegerKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'Integer'"; }
} ;

// Float Kwd
class FloatKwdToken : public ExtToken {
public:
    FloatKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'Float'"; }
} ;

// Boolean Kwd
class BooleanKwdToken : public ExtToken {
public:
    BooleanKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'Boolean'"; }
} ;

// String Kwd
class StringKwdToken : public ExtToken {
public:
    StringKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'String'"; }
} ;

// True Kwd
class TrueKwdToken : public ExtToken {
public:
    TrueKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseTrueKwd (); }
    std::string description() { return "'True'"; }
} ;

// False Kwd
class FalseKwdToken : public ExtToken {
public:
    FalseKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseFalseKwd (); }
    std::string description() { return "'False'"; }
} ;

// Head Kwd
class HeadKwdToken : public ExtToken {
public:
    HeadKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseHeadExpr (); }
    std::string description() { return "'Head'"; }
} ;

// Tail Kwd
class TailKwdToken : public ExtToken {
public:
    TailKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseTailExpr (); }
    std::string description() { return "'Tail'"; }
} ;

// Null Kwd
class NullKwdToken : public ExtToken {
public:
    NullKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseNullExpr (); }
    std::string description() { return "'Null'"; }
} ;

// Map Kwd
class MapKwdToken : public ExtToken {
public:
    MapKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseMapExpr (); }
    std::string description() { return "'Map'"; }
} ;

// Filter Kwd
class FilterKwdToken : public ExtToken {
public:
    FilterKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseFilterExpr (); }
    std::string description() { return "filter"; }
} ;

// Fold Kwd
class FoldKwdToken : public ExtToken {
public:
    FoldKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseFoldExpr (); }
    std::string description() { return "fold"; }
} ;

// Zip Kwd
class ZipKwdToken : public ExtToken {
public:
    ZipKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseZipExpr (); }
    std::string description() { return "zip"; }
} ;


// Int Const
class IntConstToken : public ExtToken {
public:
    IntConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseIntConst (); }
    std::string description() { return "integer constant"; }
} ;

// Float Const
class FloatConstToken : public ExtToken {
public:
    FloatConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseFloatConst (); }
    std::string description() { return "floating point constant"; }
} ;

// String Const
class StringConstToken : public ExtToken {
public:
    StringConstToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseStringConst (); }
    std::string description() { return "string constant"; }
} ;





// Percent
class PercentToken : public ExtToken {
public:
    PercentToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseModulus (left) ; 
    }
    std::string description() { return "%"; }
    int lbp() { return 60; }
} ;


// Plus Plus
class PlusPlusToken : public ExtToken {
public:
    PlusPlusToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseAppendExpr (left) ; 
    }
    std::string description() { return "++"; }
    int lbp() { return 40; }
} ;

// Dot Dot
class DotDotToken : public ExtToken {
public:
    DotDotToken (Parser *p, Token *t) : ExtToken(p,t) { }
//    ParseResult led (ParseResult left) {
//        return parser->parseDotDotExpr (left) ; 
//    }
    std::string description() { return ".."; }
    int lbp() { return 0 ; // 40;  
              }
} ;


// BackSlash
class BackSlashToken : public ExtToken {
public:
    BackSlashToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseLambdaExpr () ;  }
    std::string description() { return "'\'"; }
} ;




// Colon Colon
class ColonColonToken : public ExtToken {
public:
    ColonColonToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'::'"; }
} ;

// Colon
class ColonToken : public ExtToken {
public:
    ColonToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult led (ParseResult left) {
        return parser->parseConsExpr (left) ; 
    }
    std::string description() { return "':'"; }
    int lbp() { return 70; }
} ;

// Semicolon
class SemiColonToken : public ExtToken {
public:
    SemiColonToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "';'"; }
} ;
// Equals Sign
class EqualsSignToken : public ExtToken {
public:
    EqualsSignToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'='"; }
} ;

// Tuple Op
class TupleOpToken : public ExtToken {
public:
    TupleOpToken (Parser *p, Token *t) : ExtToken(p,t) { }
    ParseResult nud () { return parser->parseProjectionExpr (); }
    std::string description() { return "tuple projection operator"; }
} ;


*/


/*
// Name Kwd
class NameKwdToken : public ExtToken {
public:
    NameKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'name'"; }
} ;

// Platform Kwd
class PlatformKwdToken : public ExtToken {
public:
    PlatformKwdToken (Parser *p, Token *t) : ExtToken(p,t) { }
    std::string description() { return "'platform'"; }
} ;
*/

#endif /* EXTTOKEN_H */
