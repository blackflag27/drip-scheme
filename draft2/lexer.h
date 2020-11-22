#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>

#include "tokens.h"

Token next_token(std::istream& in);
Token next_number(std::string buf, std::istream& in);
std::string next_exponent(std::istream& in);
Token next_char(std::istream& in);
Token next_string(std::istream& in);
Token next_identifier(std::istream& in);
void eat_comment(std::istream& in);

#endif // LEXER_H
