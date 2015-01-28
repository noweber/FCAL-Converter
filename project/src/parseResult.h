/* ParseResult: a simple object for holding the result of a parse.

   Author: Eric Van Wyk

   Last modified: March 5, 2013.
*/

#ifndef PARSER_RESULT_H
#define PARSER_RESULT_H

#include "AST.h"

#include <string>


class ParseResult {
public:
    ParseResult() ;
    std::string errors ;
    Node *ast ;
    bool ok ;
} ;

#endif /* PARSER_RESULT_H */
