// Authors:	Nicholas Weber	3408684
//		Amy Le		4520434
#include "scanner.h"
#include <stdio.h>
#include <string.h> 	// strcpy
#include <string>
#include <stdlib.h>
#include "readInput.h"
#include "regex.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

// REGEX NODE //
RegexNode::RegexNode(regex_t r, tokenEnumType t) {
	this->regex = r;
	this->type = t;
}


/// SCANNER ///
// Constructor
Scanner::Scanner() {
	// Test init() by verifying that each item in regexVector works as intended
	this->init();
}

// Destructor

// Methods
Token* Scanner::scan(const char *fileText) {
	// Returns a pointer the first Token in a linked list.
	Token *firstNodeMainList = NULL;
	int fileLength = 0;
	fileLength = strlen(fileText);
	firstNodeMainList = tokenListFromString(fileText);
	Token *lastNode = new Token("", endOfFile, NULL);
	int append = appendTokenLists(firstNodeMainList, lastNode);
	if(append <= 0) {
		return lastNode;
	}
	return firstNodeMainList;
}

Token* Scanner::tokenListFromString(const char* lineOfFile) {
	// Declare the token pointer to be returned -- A Link to the List
	Token *listNode = NULL;
	// Determine the length of the text in the function argument
	int lineLength = this->getStringLength(lineOfFile);
	if(lineLength == 0 || lineOfFile == NULL) {
		return NULL;
	}
	int numMatchedChars = 0;
	int maxNumMatchedChars = 0;
	tokenEnumType tempTerminal = lexicalError;

	// Consume the white space and increment the pointer
	lineOfFile += consumeWhiteSpacesAndComments(lineOfFile);
	// Line of blank spaces check
	if(strlen(lineOfFile) == 0 || lineOfFile[0] == '\0') {
		return NULL;
	}

	// Check for the next regex match
	int r = 0;
	int size = this->regexVector.size();
	regex_t *temp = NULL;
	while (r < size) {
		temp = &this->regexVector[r].regex;
		numMatchedChars = matchRegex(temp, lineOfFile);
		// Check if the number of characters matched is greater than the
		// longest match found so far...
		if(numMatchedChars > maxNumMatchedChars) {
			maxNumMatchedChars = numMatchedChars;
			tempTerminal = this->regexVector[r].type;
		}
		r++;
	}

	// Copy the lexeme for when the Token is made
	int lexSize = maxNumMatchedChars;
	if(lexSize == 0) {
		lexSize = 1;
	}
	const char *tokenLexeme = (char *)calloc(sizeof(char), (lexSize+1));

	char *tempStr = const_cast<char *>(tokenLexeme);
	if(maxNumMatchedChars > 0) {
		tokenLexeme = this->nCharCopy(tempStr, lineOfFile, maxNumMatchedChars);
	} else {
		tokenLexeme = this->nCharCopy(tempStr, lineOfFile, 1);
	}

	char *tempLexeme = const_cast<char *>(tokenLexeme);
	// After we've found the largest regex match...
	// Increment the pointer within the line and build the Token
	// recursively through the end of the line.
	if(maxNumMatchedChars > 0) {			// Regex was matched
		lineOfFile += maxNumMatchedChars;
		//printf("Token created for %s lexeme.\n", tempLexeme);
		if(lineOfFile[0] != '\0') {
			listNode = new Token(tempLexeme, tempTerminal, tokenListFromString(lineOfFile));
		} else {
			listNode = new Token(tempLexeme, tempTerminal, NULL);
		}
	} else { 					// Lexical error
		lineOfFile += 1;
		if(lineOfFile[0] != '\0') {
			listNode = new Token(tempLexeme, lexicalError, tokenListFromString(lineOfFile));
		} else {
			listNode = new Token(tempLexeme, lexicalError, NULL);
		}
	}
	//free(tokenLexeme);
	return listNode;
}

int Scanner::appendTokenLists(Token *first, Token *append) {
	Token *firstList = first;
	Token *listToAppend = append;
	if(firstList == NULL && listToAppend == NULL) {
		return -1;
	}
	else if( firstList == NULL && listToAppend != NULL) {
		// This occurs when the first sub-list is generated in scan()
		firstList = listToAppend;
		return 0;
	}
	else {
		Token *cNode = firstList;
		// Walk through firstList until the end is reached
		while(cNode->next != NULL) {
			cNode = cNode->next;
		}
		// Append listToAppend to the end of firstList
		cNode->next = listToAppend;
		return 1;
	}
	return -1;
}

void Scanner::init() {
        // Initializes regexVector with all of the necessary regular expressions.

        // 0
	this->regexVector.push_back(RegexNode(*makeRegex("^Int"), intKwd));

        // 1
	this->regexVector.push_back(RegexNode(*makeRegex("^Float"), floatKwd));

	// 2
	this->regexVector.push_back(RegexNode(*makeRegex("^Bool"), boolKwd));

	// 3
	this->regexVector.push_back(RegexNode(*makeRegex("^True"), trueKwd));

	//4
	this->regexVector.push_back(RegexNode(*makeRegex("^False"), falseKwd));

        // 5
	this->regexVector.push_back(RegexNode(*makeRegex("^Str"), stringKwd));

        // 6
	this->regexVector.push_back(RegexNode(*makeRegex("^Matrix"), matrixKwd));

        // 7
	this->regexVector.push_back(RegexNode(*makeRegex("^let"), letKwd));

        // 8
	this->regexVector.push_back(RegexNode(*makeRegex("^in"), inKwd));

        // 9
	this->regexVector.push_back(RegexNode(*makeRegex("^end"), endKwd));

        // 10
	this->regexVector.push_back(RegexNode(*makeRegex("^if"), ifKwd));

        // 11
	this->regexVector.push_back(RegexNode(*makeRegex("^then"), thenKwd));

        // 12
	this->regexVector.push_back(RegexNode(*makeRegex("^else"), elseKwd));

        // 13
	this->regexVector.push_back(RegexNode(*makeRegex("^for"), forKwd));

        // 14
	this->regexVector.push_back(RegexNode(*makeRegex("^while"), whileKwd));

        // 15
	this->regexVector.push_back(RegexNode(*makeRegex("^print"), printKwd));

        // 16
	this->regexVector.push_back(RegexNode(*makeRegex("^[0-9]+"), intConst));

        // 17
	this->regexVector.push_back(RegexNode(*makeRegex("^[0-9]+\\.[0-9]+"), floatConst));

        // 18
	this->regexVector.push_back(RegexNode(*makeRegex("^\"([^\'\"\\\\]|\\\\.)*\""), stringConst));

        // 19
	this->regexVector.push_back(RegexNode(*makeRegex("^(\\_*[a-zA-Z0-9]*)+"), variableName));

        // 20
	this->regexVector.push_back(RegexNode(*makeRegex("^\\("), leftParen));

        // 21
	this->regexVector.push_back(RegexNode(*makeRegex("^\\)"), rightParen));

        // 22
	this->regexVector.push_back(RegexNode(*makeRegex("^\\{"), leftCurly));

        // 23
	this->regexVector.push_back(RegexNode(*makeRegex("^\\}"), rightCurly));

        // 24
	this->regexVector.push_back(RegexNode(*makeRegex("^\\["), leftSquare));

        // 25
	this->regexVector.push_back(RegexNode(*makeRegex("^\\]"), rightSquare));

        // 26
	this->regexVector.push_back(RegexNode(*makeRegex("^\\,"), comma));

        // 27
	this->regexVector.push_back(RegexNode(*makeRegex("^\\;"), semiColon));

        // 28
	this->regexVector.push_back(RegexNode(*makeRegex("^\\:"), colon));

        // 29
	this->regexVector.push_back(RegexNode(*makeRegex("^\\="), assign));

        // 30
	this->regexVector.push_back(RegexNode(*makeRegex("^\\+"), plusSign));

        // 31
	this->regexVector.push_back(RegexNode(*makeRegex("^\\*"), star));

        // 32
	this->regexVector.push_back(RegexNode(*makeRegex("^\\-"), dash));

        // 33
	this->regexVector.push_back(RegexNode(*makeRegex("^\\/"), forwardSlash));

        // 34
	this->regexVector.push_back(RegexNode(*makeRegex("^<"), lessThan));

        // 35
	this->regexVector.push_back(RegexNode(*makeRegex("^<="), lessThanEqual));

        // 36
	this->regexVector.push_back(RegexNode(*makeRegex("^>"), greaterThan));

        // 37
	this->regexVector.push_back(RegexNode(*makeRegex("^>="), greaterThanEqual));

        // 38
	this->regexVector.push_back(RegexNode(*makeRegex("^\\=="), equalsEquals));

        // 39
	this->regexVector.push_back(RegexNode(*makeRegex("^\\!="), notEquals));

        // 40
	this->regexVector.push_back(RegexNode(*makeRegex("^\\&&"), andOp));

        // 41
	this->regexVector.push_back(RegexNode(*makeRegex("^\\|\\|"), orOp));

        // 42
	this->regexVector.push_back(RegexNode(*makeRegex("^!"), notOp));
}


int Scanner::consumeWhiteSpacesAndComments(const char *text) {
	regex_t whiteSpace = *makeRegex ("^[\n\t\r ]+");
	regex_t blockComment = *makeRegex ("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
	regex_t lineComment = *makeRegex ("^//[^\n]*\n");

	int numMatchedChars = 0;
    	int totalNumMatchedChars = 0;
	int stillConsumingWhiteSpace;
	    do {
		stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

		// Try to match white space
		numMatchedChars = matchRegex (&whiteSpace, text) ;
		totalNumMatchedChars += numMatchedChars ;
		if (numMatchedChars > 0) {
		    text = text + numMatchedChars ;
		    stillConsumingWhiteSpace = 1 ;
		}

		// Try to match block comments
		numMatchedChars = matchRegex (&blockComment, text) ;
		totalNumMatchedChars += numMatchedChars ;
		if (numMatchedChars > 0) {
		    text = text + numMatchedChars ;
		    stillConsumingWhiteSpace = 1 ;
		}

		// Try to match single-line comments
		numMatchedChars = matchRegex (&lineComment, text) ;
		totalNumMatchedChars += numMatchedChars ;
		if (numMatchedChars > 0) {
		    text = text + numMatchedChars ;
		    stillConsumingWhiteSpace = 1 ;
		}
	    }
	    while ( stillConsumingWhiteSpace ) ;

	    return totalNumMatchedChars ;
}

int Scanner::getStringLength(const char *string) {
	int length = 0;
	if(string == NULL) {
		return 0;
	}
	int i = 0;
	while(string[i] != '\0') {
		length++;
		i++;
	}
	return length;
}

char* Scanner::nCharCopy(char *into, const char *from, int n) {
	int index = 0;
	while(index < n) {
		if(*(from + index) == '\0') {
			return into;
		}
		*(into + index) = *(from + index);
		index++;
	}
	*(into + index) = '\0';
	return into;
}


/// TOKEN ///
// Constructors
Token::Token(char *text, tokenEnumType tType, Token *ptr) {
        this->lexeme = text;    // May need a string copy here
        this->terminal = tType;
        this->next = ptr;
	//-- TODO: implement getStringLength function
        //this->lexLength = getStringLength(lexeme);      // may need a derefence here
}

