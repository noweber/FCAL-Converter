/* Authors:	Nicholas Weber	3408684
		Amy Le		4520434
*/

#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <regex.h>
#include <string>

class Token;
class Scanner;
class RegexNode;


// This enumerated type is used to keep track of what kind of
// construct was matched.
enum tokenEnumType {

	intKwd, floatKwd, boolKwd,
	trueKwd, falseKwd, stringKwd, matrixKwd,
	letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd,
	forKwd, whileKwd, printKwd,

	// Constants
	intConst, floatConst, stringConst,

	// Names
	variableName ,

	// Punctuation
	leftParen, rightParen,
	leftCurly, rightCurly,
	leftSquare, rightSquare,

	comma, semiColon, colon,

	//Operators
	assign,
	plusSign, star, dash, forwardSlash,
	lessThan, lessThanEqual, greaterThan, greaterThanEqual,
	equalsEquals, notEquals,
	andOp, orOp, notOp,

	// Special terminal types
	endOfFile ,
	lexicalError
} ;
typedef enum tokenEnumType tokenType ;


class RegexNode {
	public:
	// Ctor
	RegexNode(regex_t r, tokenEnumType t);

	regex_t regex;
	tokenEnumType type;
	
};


// Below you need to write your class definitions for Token and Scanner.
class Token {

	public:
	// Constructors
	//Token();
	//Token(char *text, tokenEnumType tType);	// Does not force recursion as the next one does.
	Token(char *text, tokenEnumType tType, Token *nextT);
	//Token(char *text, tokenEnumType tType, Token *nextT, Token *prevT, int len, int line, int column);

	// Destructor
	//~Token();

	// Required Fields
	char *lexeme;			// String containing the text matched
	tokenEnumType terminal;		// Enumerated type of the token matched
	Token *next;			// Pointer to next token within the linked list

	// Other Fields
	//Token *previous;		// Pointer to the previous token within the linked list (creates doubly linked list)
	int lexLength;			// Length of the lexeme string (not including the terminated character)
	//int lineFoundAt;		// The line of the text file this token was found at.
	//int columnFoundAt;		// The column of the text file this token was found at.
};

class Scanner {

	public:
	// Constructor
	Scanner();

	// Destructor
	//~Scanner();

	// The scan function returns a pointer the the first token
	// in a linked list that it creates from the input file.
	Token *scan(const char* text);

	// Vector to store all of the regex strings...
	// Their indexes correspond to the tokenEnumType order.
	//std::vector<regex_t> regexVector;
	std::vector<RegexNode> regexVector;

	// Initializes regexVector with all necessary regular expressions.
	void init();

	// Creates a linked list of tokens from a string.  Returns a pointer
	// to the first Token in the list.  (Use to create appendable sub lists)
	Token *tokenListFromString(const char *lineOfFile);

	// Appends two linked lists of tokens together.  This function
	// merely walks through the first list and assigns the last element's
	// 'next' point to the start of the second list.  Returns -1 for error.
	int appendTokenLists(Token *first, Token *append);

	// The consumeWSAC function returns the integer amount to increase
	// the file index by based on how many spaces and comments it matches.
	// The whiteSpace, blockComment, and lineComment regexes are defined
	// within the consumeWSAC function body (scanner.cpp)
	int consumeWhiteSpacesAndComments(const char *text);

	// Copies n chars from one string into another, then appends '\0'.
	char* nCharCopy(char *into, const char *from, int n);

	// Retrieves the enum that matches int r.
	tokenEnumType getEnum(int r);

	// Returns the number of characters excluding the terminating character.
	int getStringLength(const char* s);
};

#endif /* SCANNER_H */
