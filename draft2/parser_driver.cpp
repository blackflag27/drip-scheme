#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "types.h"
#include "tokens.h"
#include "lexer.h"
#include "parser.h"

using namespace std;

int main()
{
	bool quit = false;
	unordered_map<string, Object> table;

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
				vector<Object> parses = parse_all(in, table);
				cout << "OK" << endl;

				tkn = next_token(in);
			}
		} catch (const runtime_error& e) {
			cout << "Syntax Error: " << e.what() << endl;
		}
	}

	return 0;
}
