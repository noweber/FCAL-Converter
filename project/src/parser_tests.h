#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include "extToken.h"
#include "parser.h"
#include "parseResult.h"

#include <sstream>

using namespace std ;

class ParserTestSuite : public CxxTest::TestSuite 
{
public:
    Scanner *s ;
    Parser *p ;
    void test_setup_code ( ) {
        s = new Scanner() ;
        p = new Parser() ;
    }

    void test_parse_bad_syntax ( ) {
        const char *text 
          = readInputFromFile ( "../samples/bad_syntax_good_tokens.dsl" )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        TS_ASSERT ( ! pr.ok ) ;
    }
    void test_parse_sample_1 ( ) {
        const char *filename = "../samples/sample_1.dsl" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors ; 
        TSM_ASSERT ( msg , pr.ok );
    }
    void test_parse_sample_2 ( ) {
        const char *filename = "../samples/sample_2.dsl" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors ; 
        TSM_ASSERT ( msg , pr.ok );
    }
    void test_parse_sample_3 ( ) {
        const char *filename = "../samples/sample_3.dsl" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors ; 
        TSM_ASSERT ( msg , pr.ok );
    }
    void test_parse_sample_4 ( ) {
        const char *filename = "../samples/sample_4.dsl" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors ; 
        TSM_ASSERT ( msg , pr.ok );
    }
    void test_parse_sample_5 ( ) {
        const char *filename = "../samples/sample_5.dsl" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors ; 
        TSM_ASSERT ( msg , pr.ok );
    }
     void test_parse_mysample ( ) {
        const char *filename = "../samples/mysample.dsl" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors ; 
        TSM_ASSERT ( msg , pr.ok );
    }
    void test_parse_forestLossV2 ( ) {
        const char *filename = "../samples/forest_loss_v2.dsl" ;
        const char *text = readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        ParseResult pr = p->parse ( text ) ;
        string msg (filename) ;
        msg += "\n" + pr.errors ; 
        TSM_ASSERT ( msg , pr.ok );
    }
 
	/*void xtest_parseExpr() {
	const char *text = "1+1";
	//TS assert



	}*/


} ;
