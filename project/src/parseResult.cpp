/* ParseResult: a simple object for holding the result of a parse.

   Author: Eric Van Wyk

   Last modified: March 5, 2013.
*/

#include "parseResult.h"

using namespace std ;

ParseResult::ParseResult ( ) {
    errors = "" ;
    ok = true ;
    ast = NULL ; 
}
