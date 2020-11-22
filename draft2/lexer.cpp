#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <cctype>

#include "lexer.h"
#include "tokens.h"
#include "char_predicates.h"

// Only a subset of Scheme's syntax is implemented.
// The following syntactic features are unsupported:
//
// Quasiquotation
// Dot notation for improper lists
// ... as a special identifier (dropped because macros are
//                              unimplemented)
// Non base 10 number literals.
// Complex numbers.
// Exponent markers other than e.
//
// The following nonstandard features are implemented:
// Tab is considered whitespace

using namespace std;

Token next_token(istream& in)
{
	char c;	
	char nx;

	while (in.get(c)) {
		switch ((c)) {
			case '(':
				return {"(", Kind::Special};
			case ')':
				return {")", Kind::Special};
			case '\'':
				return {"'", Kind::Special};
			case '.':
				in.get(nx);
				if (in.fail()) {
					throw runtime_error("Expected a digit after .");
				} else if (is_digit(nx)) {
					in.unget();
					return next_number(".", in);
				} else {
					throw runtime_error("Expected a digit after .");
				}
			case '#':
				in.get(nx);
				if (in.fail()) {
					throw runtime_error("Invalid expression.");
				} else {
					switch (nx) {
						case 't':
							return {"t", Kind::Boolean};
						case 'f':
							return {"f", Kind::Boolean};
						case '\\':
							return next_char(in);
						default:
							throw runtime_error("Invalid expression.");
					}
				}
			case ' ':
				// Eat the whitespace
			case '\t':
			case '\n':
				break;
			case ';':
				eat_comment(in);
				break;
			case '"':
				return next_string(in);
			case '+':
				in.get(nx);
				if (in.eof()) {
					return {"+", Kind::Identifier};
				} else if (in.fail()) {
					throw runtime_error("Invalid number.");
				} else if (is_delimiter(nx)) {
					in.unget();
					return {"+", Kind::Identifier};
				} else if (nx == '.') {
					return next_number("+.", in);
				} else if (is_digit(nx)) {
					in.unget();
					return next_number("+", in);
				} else {
					throw runtime_error("Invalid number.");
				}
			case '-':
				in.get(nx);
				if (in.eof()) {
					return {"-", Kind::Identifier};
				} else if (in.fail()) {
					throw runtime_error("Invalid number.");
				} else if (is_delimiter(nx)) {
					in.unget();
					return {"-", Kind::Identifier};
				} else if (nx == '.') {
					return next_number("-.", in);
				} else if (is_digit(nx)) {
					in.unget();
					return next_number("-", in);
				} else {
					throw runtime_error("Invalid number.");
				}
			default:
				if (is_digit(c)) {
					in.unget();
					return next_number("", in);
				} else if (is_initial(c)) {
					in.unget();
					return next_identifier(in);
				} else {
					throw runtime_error("Invalid expression.");
				}
		}
	}

	if (in.eof()) {
		return {"", Kind::Eof};
	} else {
		throw runtime_error("Input failure");
	}
}


Token next_number(string buf, istream& in)
{
	// Possible values of buf (# means one or more digits)
	// ""
	// "."
	// "+"
	// "+#."
	// "-"
	// "-#."
	
	char c;
	char nx;
	
	int dot_count = count(buf.begin(), buf.end(), '.');

	if (dot_count > 1) {
		throw runtime_error(
				"Invalid character in number literal.");
	} else if (dot_count == 1) {
		while (in.get(c)) {
			if (is_delimiter(c)) {
				in.unget();
				return {buf, Kind::Number};
			} else if (c == 'e') {
				in.get(nx);

				if (in.fail()) {
					throw runtime_error("Input failure.");
				} else if ((nx == '+') || nx == '-') {
					buf.push_back(c);
					buf.push_back(nx);
					buf.append(next_exponent(in));
					return {buf, Kind::Number};
				} else if (is_digit(nx)) {
					in.unget();
					buf.push_back(c);
					buf.append(next_exponent(in));
					return {buf, Kind::Number};
				} else {
					throw runtime_error
						("Invalid character in number literal.");
				}
			} else if (is_digit(c)) {
				buf.push_back(c);
			} else {
				throw runtime_error
					("Invalid character in number literal.");
			}
		}

		if (in.eof()) {
			// Special cases for +. and -.
			if ((buf == "+.") or (buf == "-.")) {
				throw runtime_error(
						"Expected digit in number literal.");
			} else {
				return {buf, Kind::Number};
			}
		} else {
			throw runtime_error("Input failure.");
		}
	} else {
		while (in.get(c)) {
			if (is_delimiter(c)) {
				in.unget();
				return {buf, Kind::Number};
			} else if (c == 'e') {
				buf.push_back(c);
				buf.append(next_exponent(in));
				return {buf, Kind::Number};
			} else if (c == '.') {
				buf.push_back(c);
				return next_number(buf, in);
			} else if (is_digit(c)) {
				buf.push_back(c);
			} else {
				throw runtime_error
					("Invalid character in number literal.");
			}
		}

		if (in.eof()) {
			return {buf, Kind::Number};
		} else {
			throw runtime_error("Input failure.");
		}
	}
}

string next_exponent(istream& in)
{
	string buf = "";
	char c;

	while (in.get(c)) {
		if (is_digit(c)) {
			buf.push_back(c);
		} else if (is_delimiter(c)) {
			in.unget();
			return buf;
		} else {
			throw runtime_error
				("Invalid character in number literal.");
		}
	}

	if (in.eof()) {
		return buf;
	} else {
		throw runtime_error("Input failure.");
	}
}

Token next_char(istream& in)
{
	char c;
	char nx;

	in.get(c);
	
	if (in.fail()) {
		throw runtime_error("Incomplete character literal.");
	} else if (c == 's') { // Handle the special literal #\space
		in.get(nx);
		if (in.fail()) {
			throw runtime_error("Incomplete character literal.");
		} else if (nx == 'p') {
			in.get(nx);
			if (in.fail()) {
				throw runtime_error("Incomplete character literal.");
			} else if (nx == 'a') {
				in.get(nx);
				if (in.fail()) {
					throw runtime_error
						("Incomplete character literal.");
				} else if (nx == 'c') {
					in.get(nx);
					if (in.fail()) {
						throw runtime_error
							("Incomplete character literal.");
					} else if (nx == 'e') {
						return {" ", Kind::Character};
					} else {
						throw runtime_error
							("Invalid character literal.");
					}
				} else {
					throw runtime_error ("Invalid character literal.");
				}
			} else {
				throw runtime_error	("Invalid character literal.");
			}
		} else {
			throw runtime_error ("Invalid character literal.");
		}
	} else if (c == 'n') { // Handle the special literal #\newline
		in.get(nx);
		if (in.fail()) {
			throw runtime_error("Incomplete character literal.");
		} else if (nx == 'e') {
			in.get(nx);
			if (in.fail()) {
				throw runtime_error("Incomplete character literal.");
			} else if (nx == 'w') {
				in.get(nx);
				if (in.fail()) {
					throw runtime_error
						("Incomplete character literal.");
				} else if (nx == 'l') {
					in.get(nx);
					if (in.fail()) {
						throw runtime_error
							("Incomplete character literal.");
					} else if (nx == 'i') {
						in.get(nx);
						if (in.fail()) {
							throw runtime_error
								("Incomplete character literal.");
						} else if (nx == 'n') {
							in.get(nx);
							if (in.fail()) {
								throw runtime_error
									("Incomplete character literal.");
							} else if (nx == 'e') {
								return {"\n", Kind::Character};
							} else {
								throw runtime_error
									("Invalid character literal.");
							}
						} else {
							throw runtime_error
								("Invalid character literal.");
						}
					} else {
						throw runtime_error
							("Invalid character literal.");
					}
				} else {
					throw runtime_error("Invalid character literal.");
				}
			} else {
				throw runtime_error("Invalid character literal.");
			}
		} else {
			throw runtime_error("Invalid character literal.");
		}
	} else if (isgraph(static_cast<unsigned char>(c))) {
		// cast to unsigned char because isgraph exhibits undefined
		// behavior when c is not in the range of 0 - 127
		in.get(nx);
		if (in.eof()) {
			return {string(1, c), Kind::Character};
		} else if (in.fail()) {
			throw runtime_error("Input failure.");
		} else if (is_delimiter(nx)) {
			in.unget();
			return {string(1, c), Kind::Character};
		} else {
			throw runtime_error("Invalid character literal.");
		}
	} else {
		throw runtime_error("Invalid character literal.");
	}
}

Token next_string(istream& in)
{
	string buf = "";
	char c = 0;
	char nx = 0;

	while (in.get(c)) {
		if (c == '"') {
			return {buf, Kind::String};
		} else if (c == '\\') {
			nx = in.get();
			if (in.fail()) {
				throw runtime_error("Invalid string literal.");
			} else if ((nx == '"') || (nx == '\\')) {
				buf.push_back(nx);
			} else {
				throw runtime_error("Invalid string literal.");
			}
		} else if (isgraph(static_cast<unsigned char>(c)) ||
				   isspace(static_cast<unsigned char>(c))) {
			buf.push_back(c);
		} else {
			throw runtime_error("Invalid string literal.");
		}
	}

	throw runtime_error("Input failure");
		// Can this branch only be reached by 
		// eof or input error???
}

Token next_identifier(istream& in)
{
	string buf = "";
	char c;

	while (in.get(c)) {
		if (is_delimiter(c)) {
			in.unget();
			return {buf, Kind::Identifier};
		} else if (is_initial(c)) {
			buf.push_back(c);
		} else if (is_digit(c)) {
			buf.push_back(c);
		} else if ((c == '+') ||
				   (c == '-') ||
				   (c == '.') ||
				   (c == '@')) {
			buf.push_back(c);
		} else {
			throw runtime_error("Invalid identifier.");
		}
	}

	if (in.eof()) {
		return {buf, Kind::Identifier};
	} else {
		throw runtime_error("Invalid identifier.");
	}
}

void eat_comment(istream& in)
{
	string garbage_buf;

	getline(in, garbage_buf);

	return;
}
