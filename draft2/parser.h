#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

#include "types.h"
#include "tokens.h"

std::vector<Object>
parse_all(std::istream& in,
		  std::unordered_map<std::string, Object>& table);
Object parse(Token tkn,
			 std::istream& in,
			 std::unordered_map<std::string, Object>& table);
Object parse_id(Token tkn,
				std::unordered_map<std::string, Object>& table);
Object make_symbol(std::string id,
				   std::unordered_map<std::string, Object>& table);
Object parse_bool(Token tkn);
Object parse_num(Token tkn);
Object parse_char(Token tkn);
Object parse_string(Token tkn);
Object parse_special(Token tkn,
					 std::istream& in,
					 std::unordered_map<std::string, Object>& table);
Object make_list(std::vector<Object> objects);
Object make_pair(Object car, Object cdr);


#endif // PARSER_H
