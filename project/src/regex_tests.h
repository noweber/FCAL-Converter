#include <cxxtest/TestSuite.h>
#include "regex.h"

using namespace std ;

class RegexTestSuite : public CxxTest::TestSuite 
{
public:

    // Tests for makeRegex and matchRegex
    // --------------------------------------------------
    /* These tests ensure that the makeRegex and matchRegex work as
       expected.  These tests are independent from the scanner or
       WordCount application.
     */

    void test_make_matchRegex_match ( void ) {
        regex_t *re = makeRegex ("^[0-9]+") ;
        TS_ASSERT (re) ;
        int numMatchedChars = matchRegex (re, "123 ") ;
        TS_ASSERT (numMatchedChars == 3) ;
    }

    void test_make_matchRegex_no_match ( void ) {
        regex_t *re = makeRegex ("^[0-9]+") ;
        TS_ASSERT (re) ;
        int numMatchedChars = matchRegex (re, " 123 ") ;
        TS_ASSERT (numMatchedChars == 0) ;
    }

    void test_make_matchRegex_match_string_copy ( void ) {
        regex_t *re = makeRegex ("^[0-9]+") ;
        TS_ASSERT (re) ;
        const char *text = "123 ";
        int numMatchedChars = matchRegex (re, text) ;
        TS_ASSERT (numMatchedChars == 3) ;
        std::string lex(text, numMatchedChars) ;
        TS_ASSERT_EQUALS (lex, "123");
    }

} ;
