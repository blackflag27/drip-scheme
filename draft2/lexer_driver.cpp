#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#include "lexer.h"
#include "tokens.h"

using namespace std;

string print_kind(Kind kind)
{
	switch (kind) {
		case Kind::Identifier:
			return "Identifier";
		case Kind::Boolean:
			return "Boolean";
		case Kind::Number:
			return "Number";
		case Kind::Character:
			return "Character";
		case Kind::String:
			return "String";
		case Kind::Special:
			return "Special";
		case Kind::Eof:
			return "Eof";
	}
}

int main()
{
	bool quit = false;

	while (!quit) {
		string line;
		getline(cin, line);

		if (line == ",q") {
			quit = true;
			break;
		}

		istringstream in(line);

		try {
			Token tkn = next_token(in);
			while (tkn.kind != Kind::Eof) {
				cout << tkn.value
					 << ": "
					 << print_kind(tkn.kind)
					 << endl;
				tkn = next_token(in);
			}
		} catch (const runtime_error& e) {
			cout << "Syntax Error: " << e.what() << endl;
		}
	}

	return 0;
}
