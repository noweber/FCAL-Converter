CXX_DIR = ../lib/cxxtest
CXXTEST = $(CXX_DIR)/bin/cxxtestgen
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -Wall -g

# Program files.
readInput.o:	readInput.cpp readInput.h
	g++ $(FLAGS) -c readInput.cpp 

regex.o:	regex.cpp regex.h
	g++ $(FLAGS) -c regex.cpp

scanner.o:	scanner.cpp scanner.h regex.h
	g++ $(FLAGS) -c scanner.cpp

parser.o:	parser.cpp parser.h extToken.h scanner.h
	g++ $(FLAGS) -c parser.cpp

extToken.o:	extToken.cpp extToken.h parser.h
	g++ $(FLAGS) -c extToken.cpp

parseResult.o:	parseResult.cpp parseResult.h
	g++ $(FLAGS) -c parseResult.cpp

AST.o:	AST.cpp AST.h
	g++ $(FLAGS) -c AST.cpp

# Testing files and targets.
run-tests:	regex_tests scanner_tests parser_tests ast_tests codegeneration_tests 
	./regex_tests
	./scanner_tests
	./parser_tests
	./ast_tests
	./codegeneration_tests

regex_tests:	regex_tests.cpp regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -o regex_tests regex.o regex_tests.cpp

regex_tests.cpp:	regex.h regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cpp regex_tests.h

scanner_tests:	scanner_tests.cpp scanner.o regex.o readInput.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o scanner_tests \
		scanner.o regex.o readInput.o scanner_tests.cpp

scanner_tests.cpp:	scanner.o scanner_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cpp scanner_tests.h

parser_tests:	parser_tests.cpp parser.o readInput.o scanner.o extToken.o parseResult.o regex.o AST.o
	g++ $(FLAGS) -I$(CXX_DIR) -o parser_tests \
		parser.o readInput.o scanner.o extToken.o parseResult.o regex.o AST.o parser_tests.cpp

parser_tests.cpp:	parser.o parser_tests.h readInput.h extToken.h parseResult.h scanner.h
	$(CXXTEST) $(CXXFLAGS) -o parser_tests.cpp parser_tests.h

ast_tests:	ast_tests.cpp AST.o scanner.o parser.o regex.o readInput.o extToken.o parseResult.o
	g++ $(FLAGS) -I$(CXX_DIR) -o ast_tests \
		AST.o parser.o readInput.o scanner.o extToken.o parseResult.o regex.o ast_tests.cpp

ast_tests.cpp:	AST.o ast_tests.h readInput.h extToken.h parseResult.h scanner.h
	$(CXXTEST) $(CXXFLAGS) -o ast_tests.cpp ast_tests.h

codegeneration_tests:	codegeneration_tests.cpp AST.o parser.o scanner.o readInput.o regex.o parseResult.o extToken.o
	g++ $(FLAGS) -I$(CXX_DIR) -o codegeneration_tests readInput.o scanner.o parser.o AST.o regex.o parseResult.o extToken.o codegeneration_tests.cpp

codegeneration_tests.cpp:	codegeneration_tests.h AST.h parser.h scanner.h readInput.h regex.h parseResult.h extToken.h
	$(CXXTEST) --error-printer -o codegeneration_tests.cpp codegeneration_tests.h

clean:
	rm -Rf *.o \
		regex_tests regex_tests.cpp \
		scanner_tests scanner_tests.cpp \
		parser_tests parser_tests.cpp \
		ast_tests ast_tests.cpp \
		codegeneration_tests codegeneration_tests.cpp
