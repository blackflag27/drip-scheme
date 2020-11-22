#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <sstream>

#include "parser.h"
#include "types.h"
#include "tokens.h"
#include "lexer.h"

// IMPORTANT NOTICE:
// This implementation ignores memory deallocation, and
// garbage collection. Nontrivial program will most likely crash
// from running out of memory.

using namespace std;

vector<Object> parse_all(istream& in,
						 unordered_map<string, Object>& table)
{
	vector<Object> objects;

	try {
			Object o = parse(tkn, in, table);

			objects.push_back(o);

			tkn = next_token(in);
		}
	} catch (const runtime_error& e) {
		cout << "Syntax Error: " << e.what() << endl;
	}

	return objects;
}

Object parse(Token tkn,
			 istream& in,
			 unordered_map<string, Object>& table)
{
	switch (tkn.kind) {
		case Kind::Identifier:
			return parse_id(tkn, table);
		case Kind::Boolean:
			return parse_bool(tkn);
		case Kind::Number:
			return parse_num(tkn);
		case Kind::Character:
			return parse_char(tkn);
		case Kind::String:
			return parse_string(tkn);
		case Kind::Special:
			return parse_special(tkn, in, table);
	}
}

Object parse_id(Token tkn, unordered_map<string, Object>& table)
{
	return make_symbol(tkn.value, table);
}

Object make_symbol(string id, unordered_map<string, Object>& table)
{
	if (table.count(id) == 1) {
		return table[id];
	} else {
		Object o;
		o.type = Type::Symbol;

		string* s = new string(id);
		o.s = s;

		table[id] = o;
		return o;
	}
}

Object parse_bool(Token tkn)
{
	Object o;
	o.type = Type::Boolean;

	if (tkn.value == "f") {
		o.b = false;
		return o;
	} else {
		o.b = true;
		return o;
	}
}

Object parse_num(Token tkn)
{
	Object o;

	istringstream numval1(tkn.value);
	istringstream numval2(tkn.value);
	double d;
	int i;

	// A little hackery.
	numval1 >> d;
	numval2 >> i;

	if (d != i) {
		o.type = Type::Float;
		o.d = d;

		return o;
	} else {
		o.type = Type::Integer;
		o.i = i;

		return o;
	}
}

Object parse_char(Token tkn)
{
	Object o;
	o.type = Type::Character;
	
	o.c = tkn.value.front();

	return o;
}

Object parse_string(Token tkn)
{
	Object o;
	o.type = Type::String;

	string* s = new string(tkn.value);
	o.s = s;

	return o;
}

Object parse_special(Token tkn,
					 istream& in,
					 unordered_map<string, Object>& table)
{
	switch (tkn.value) {
		case "(":
			vector<Object> objects;

			Token tkn = next_token(in);
			while ((tkn.kind != Kind::Special) &&
				   (tkn.value != ")")) {
				Object o = parse(tkn, in, table);

				objects.push_back(o);

				tkn = next_token(in);
			}
			
			return make_list(objects);
		case ")":
			throw runtime_error("Expected ( before ).");
		case "'":
			vector<Object> objects;
			objects.push_back(make_symbol("quote", table));

			Token tkn = next_token(in);
			if (tkn.kind == Kind::Eof) {
				throw runtime_error("Expected token after '");
			} else {
				Object o = parse(tkn, in, table);
				objects.push_back(o);
			}

			return make_list(objects):
	}
}

Object make_list(vector<Object> objects)
{
	Object empty;
	empty.type = Type::Empty;

	// check for off by one errors
	Object list;
	int last_idx = objects.size() - 1;

	for (int idx = last_idx; idx == 0; --idx) {
		if (idx == last_idx) {
			list = make_pair(objects[idx], empty);
		} else if (idx == 0) {
			return make_pair(objects[idx], list);
		} else {
			list = make_pair(objects[idx], list);
		}
	}
}

Object make_pair(Object car, Object cdr)
{
	Object o;
	o.type == Type::Pair;

	Pair* p = new Pair;
	p->car = car;
	p->cdr = cdr;

	o.p = p;

	return o;
}

