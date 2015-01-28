#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "regex.h"
#include "scanner.h"
#include <stdio.h>
using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s ;
    void test_setup_code ( ) {
        s = new Scanner() ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You may need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */

    // Here you should place tests for these other functions.

	/// Get String Length Test ///
	// getStringLength(const char* s)
	void test_getStringLength() {
		const char *test1 = "one";
		int test1N = s->getStringLength(test1);
		TS_ASSERT_EQUALS(test1N, 3);
	}
	void test_getStringLength02() {
		const char *test2 = "";
		int test2N = s->getStringLength(test2);
		TS_ASSERT_EQUALS(test2N, 0);
	}

	/// Token List From String Tests ///
	// Token *tokenListFromString(const char* lineOfFile)
	void test_tokenListFromString() {
		const char *testStr = "Int";
		Token *testToken = s->tokenListFromString(testStr);
		TS_ASSERT_EQUALS(testToken->terminal, intKwd);
		//TS_ASSERT_EQUALS(testToken->next, NULL);
	}
	void test_tokenListFromString02() {
		const char *testStr = "Int Int";
		Token *testToken = s->tokenListFromString(testStr);
		TS_ASSERT_EQUALS(testToken->terminal, intKwd);
		TS_ASSERT(testToken->next);
		// second int
		testToken = testToken->next;
		TS_ASSERT_EQUALS(testToken->terminal, intKwd);
		TS_ASSERT(!testToken->next);
	}

        void test_tokenListFromString03() {
                const char *testStr = "Int Float";
                Token *testToken = s->tokenListFromString(testStr);
                TS_ASSERT_EQUALS(testToken->terminal, intKwd);
                TS_ASSERT(testToken->next);
                // next is float
                testToken = testToken->next;
                TS_ASSERT_EQUALS(testToken->terminal, floatKwd);
                TS_ASSERT(!testToken->next);
        }

        void test_tokenListFromString04() {
                const char *testStr = "123 Matrix";
                Token *testToken = s->tokenListFromString(testStr);
                TS_ASSERT_EQUALS(testToken->terminal, intConst);
                TS_ASSERT(testToken->next);
                // next is matrixKwd
                testToken = testToken->next;
                TS_ASSERT_EQUALS(testToken->terminal, matrixKwd);
                TS_ASSERT(!testToken->next);
        }


	/// Append Token Lists Tests ///
	// void apendTokenLists(Token *first, Token *append)
	void test_appendTokenLists0() {
		const char *testStr1 = "Float";
		Token *list1 = s->tokenListFromString(testStr1);

		const char *testStr2 = "Int";
		Token *list2 = s->tokenListFromString(testStr2);

		s->appendTokenLists(list1, list2);
		Token *cNode = list1;
		TS_ASSERT(cNode);
		// Float
		TS_ASSERT_EQUALS(cNode->terminal, floatKwd);
		TS_ASSERT(cNode->next);		//ptr != NULL
		// Int
		cNode = cNode->next;
		TS_ASSERT_EQUALS(cNode->terminal, intKwd);
		TS_ASSERT(!cNode->next);	// ptr == NULL 
	}

	void test_appendTokenLists1() {
		const char *testStr1 = "Float Int";
		Token *list1 = s->tokenListFromString(testStr1);

		const char *testStr2 = "Int Float";
		Token *list2 = s->tokenListFromString(testStr2);

		s->appendTokenLists(list1, list2);
		Token *cNode = list1;
		TS_ASSERT(cNode);
		// first Float
		TS_ASSERT_EQUALS(cNode->terminal, floatKwd);
		TS_ASSERT(cNode->next);		//ptr != NULL
		// first Int
		cNode = cNode->next;
		TS_ASSERT_EQUALS(cNode->terminal, intKwd);
		TS_ASSERT(cNode->next);		// ptr != NULL
		// second Int
		cNode = cNode->next;
		TS_ASSERT_EQUALS(cNode->terminal, intKwd);
		TS_ASSERT(cNode->next);		// ptr != NULL
		// second Float
		cNode = cNode->next;
		TS_ASSERT_EQUALS(cNode->terminal, floatKwd);
		TS_ASSERT(!cNode->next);	// ptr = NULL
	}

        void test_appendTokenLists2() {
                const char *testStr1 = "< 5";
                Token *list1 = s->tokenListFromString(testStr1);

                const char *testStr2 = "!= Float";
                Token *list2 = s->tokenListFromString(testStr2);

                s->appendTokenLists(list1, list2);
                Token *cNode = list1;
                TS_ASSERT(cNode);
                // <
                TS_ASSERT_EQUALS(cNode->terminal, lessThan);
                TS_ASSERT(cNode->next);         //ptr != NULL
                // 5
                cNode = cNode->next;
                TS_ASSERT_EQUALS(cNode->terminal, intConst);
                TS_ASSERT(cNode->next);         // ptr != NULL
                // !=
                cNode = cNode->next;
                TS_ASSERT_EQUALS(cNode->terminal, notEquals);
                TS_ASSERT(cNode->next);         // ptr != NULL
                // Float
                cNode = cNode->next;
                TS_ASSERT_EQUALS(cNode->terminal, floatKwd);
                TS_ASSERT(!cNode->next);        // ptr = NULL
        }

    /* Below is one of the tests for these components in the project
       solution created by your instructor..  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */
	////////////////////////////////////////////////////////
	/// //// TEST FOR EACH TOKEN TYPE -- 40 OF THEM //// ///
	////////////////////////////////////////////////////////
	//// MUST HAVE THE FOLLOWING FOR EVERY TERMINAL TYPE ///
	////////////////////////////////////////////////////////
	void test_terminal_intKwd () {
		regex_t testR = s->regexVector[0].regex;
		const char *testStr = "Int";
		int nmc = matchRegex(&testR, testStr);
		TS_ASSERT_EQUALS(nmc, 3);

		Token *tokenTest = s->tokenListFromString(testStr);
		TS_ASSERT(tokenTest);
		TS_ASSERT_EQUALS(tokenTest->lexeme, "Int");
		TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[0].type);
		TS_ASSERT(!tokenTest->next);
	}

        void test_terminal_floatKwd () {
                regex_t testR = s->regexVector[1].regex;
                const char *testStr = "Float";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 5);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "Float");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[1].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_boolKwd () {
                regex_t testR = s->regexVector[2].regex;
                const char *testStr = "Bool";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 4);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "Bool");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[2].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_trueKwd () {
                regex_t testR = s->regexVector[3].regex;
                const char *testStr = "True";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 4);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "True");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[3].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_falseKwd () {
                regex_t testR = s->regexVector[4].regex;
                const char *testStr = "False";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 5);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "False");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[4].type);
                TS_ASSERT(!tokenTest->next);
        }


        void test_terminal_stringKwd () {
                regex_t testR = s->regexVector[5].regex;
                const char *testStr = "Str";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 3);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "Str");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[5].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_matrixKwd () {
                regex_t testR = s->regexVector[6].regex;
                const char *testStr = "Matrix";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 6);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "Matrix");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[6].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_letKwd () {
                regex_t testR = s->regexVector[7].regex;
                const char *testStr = "let";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 3);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "let");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[7].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_inKwd () {
                regex_t testR = s->regexVector[8].regex;
                const char *testStr = "in";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 2);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "in");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[8].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_endKwd () {
                regex_t testR = s->regexVector[9].regex;
                const char *testStr = "end";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 3);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "end");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[9].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_ifKwd () {
                regex_t testR = s->regexVector[10].regex;
                const char *testStr = "if";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 2);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "if");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[10].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_thenKwd () {
                regex_t testR = s->regexVector[11].regex;
                const char *testStr = "then";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 4);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "then");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[11].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_elseKwd () {
                regex_t testR = s->regexVector[12].regex;
                const char *testStr = "else";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 4);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "else");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[12].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_forKwd () {
                regex_t testR = s->regexVector[13].regex;
                const char *testStr = "for";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 3);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "for");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[13].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_whileKwd () {
                regex_t testR = s->regexVector[14].regex;
                const char *testStr = "while";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 5);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "while");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[14].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_printKwd () {
                regex_t testR = s->regexVector[15].regex;
                const char *testStr = "print";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 5);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "print");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[15].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_intConst () {
                regex_t testR = s->regexVector[16].regex;
                const char *testStr = "123456";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 6);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "123456");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[16].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_floatConst () {
                regex_t testR = s->regexVector[17].regex;
                const char *testStr = "123.456";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 7);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "123.456");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[17].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_stringConst () {
                regex_t testR = s->regexVector[18].regex;
                const char *testStr = "\"hello\"";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 7);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "\"hello\"");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[18].type);
                TS_ASSERT(!tokenTest->next);
        }


        void test_terminal_variableName () {
                regex_t testR = s->regexVector[19].regex;
                const char *testStr = "currentToken";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 12);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "currentToken");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[19].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_leftParen () {
                regex_t testR = s->regexVector[20].regex;
                const char *testStr = "(";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "(");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[20].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_rightParen () {
                regex_t testR = s->regexVector[21].regex;
                const char *testStr = ")";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, ")");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[21].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_leftCurly () {
                regex_t testR = s->regexVector[22].regex;
                const char *testStr = "{";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "{");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[22].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_rightCurly () {
                regex_t testR = s->regexVector[23].regex;
                const char *testStr = "}";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "}");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[23].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_leftSquare () {
                regex_t testR = s->regexVector[24].regex;
                const char *testStr = "[";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "[");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[24].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_rightSquare () {
                regex_t testR = s->regexVector[25].regex;
                const char *testStr = "]";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "]");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[25].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_comma () {
                regex_t testR = s->regexVector[26].regex;
                const char *testStr = ",";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, ",");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[26].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_semiColon () {
                regex_t testR = s->regexVector[27].regex;
                const char *testStr = ";";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, ";");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[27].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_colon () {
                regex_t testR = s->regexVector[28].regex;
                const char *testStr = ":";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, ":");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[28].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_assign () {
                regex_t testR = s->regexVector[29].regex;
                const char *testStr = "=";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "=");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[29].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_plusSign () {
                regex_t testR = s->regexVector[30].regex;
                const char *testStr = "+";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "+");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[30].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_star () {
                regex_t testR = s->regexVector[31].regex;
                const char *testStr = "*";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "*");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[31].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_dash () {
                regex_t testR = s->regexVector[32].regex;
                const char *testStr = "-";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "-");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[32].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_forwardSlash () {
                regex_t testR = s->regexVector[33].regex;
                const char *testStr = "/";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "/");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[33].type);
                TS_ASSERT(!tokenTest->next);
        }

       void test_terminal_lessThan () {
                regex_t testR = s->regexVector[34].regex;
                const char *testStr = "<";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "<");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[34].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_lessThanEqual () {
                regex_t testR = s->regexVector[35].regex;
                const char *testStr = "<=";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 2);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "<=");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[35].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_greaterThan () {
                regex_t testR = s->regexVector[36].regex;
                const char *testStr = ">";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, ">");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[36].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_greaterThanEqual () {
                regex_t testR = s->regexVector[37].regex;
                const char *testStr = ">=";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 2);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, ">=");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[37].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_equalsEquals () {
                regex_t testR = s->regexVector[38].regex;
                const char *testStr = "==";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 2);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "==");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[38].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_notEquals () {
                regex_t testR = s->regexVector[39].regex;
                const char *testStr = "!=";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 2);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "!=");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[39].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_andOp () {
                regex_t testR = s->regexVector[40].regex;
                const char *testStr = "&&";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 2);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "&&");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[40].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_orOp () {
                regex_t testR = s->regexVector[41].regex;
                const char *testStr = "||";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 2);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "||");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[41].type);
                TS_ASSERT(!tokenTest->next);
        }

        void test_terminal_notOp () {
                regex_t testR = s->regexVector[42].regex;
                const char *testStr = "!";
                int nmc = matchRegex(&testR, testStr);
                TS_ASSERT_EQUALS(nmc, 1);

                Token *tokenTest = s->tokenListFromString(testStr);
                TS_ASSERT(tokenTest);
                TS_ASSERT_EQUALS(tokenTest->lexeme, "!");
                TS_ASSERT_EQUALS(tokenTest->terminal, s->regexVector[42].type);
                TS_ASSERT(!tokenTest->next);
        }

	////////////////////////////////////////////////////////









    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the 
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                //printf("priblem: %s\n",currentToken->lexeme.c_str());
				return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned. 
    */
    void scanFileNoLexicalErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {
            if (currentToken->terminal != ts[termIndex]) {
            printf("%i: %i should be %i\n",termIndex,currentToken->terminal , ts[termIndex]);
			fflush(stdout);
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    void test_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // When a lexical error occurs, the scanner creates a token with a 
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors ( ) {
        Token *tks = s->scan ("$&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // A test for scanning numbers and a variable.
    void test_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { intConst, variableName, floatConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( tks, 4, ts ) ) ;
    }



    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexeme
       are correct.
     */

    void test_scan_bad_syntax_good_tokens ( ) {
        const char *filename = "../samples/bad_syntax_good_tokens.dsl" ;
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

                   matrixKwd,



            semiColon, comma, colon,
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 

            equalsEquals, lessThanEqual, 
            greaterThanEqual, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,


            intKwd, floatKwd,  stringKwd,  whileKwd,

            endOfFile
       } ;
        int count = 39; 
        TS_ASSERT ( sameTerminals ( tks, count, ts ) ) ;
    }

    void test_scan_sample_forestLoss ( ) {
        scanFileNoLexicalErrors ("../samples/forest_loss_v2.dsl") ;
    }



} ;
