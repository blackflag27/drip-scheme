CXX = clang++
CFLAGS = -std=c++14 -Wall -Wextra -g

lexer_driver: lexer_driver.o lexer.o char_predicates.o
	$(CXX) $^ -o $@

parser_driver: parser_driver.o parser.o lexer.o char_predicates.o
	$(CXX) $^ -o $@

parser_driver.o parser .o lexer_driver.o lexer.o char_predicates.o: %.o: %.cpp
	$(CXX) $(CFLAGS) -c $^

