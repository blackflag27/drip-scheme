#ifndef TOKENS_H
#define TOKENS_H

#include <string>

enum class Kind {
	Identifier,
	Boolean,
	Number,
	Character,
	String,
	Special, // Includes things like ( and )
	Eof // Signifies the last token in a stream
};

struct Token {
	std::string value;
	Kind kind;
};

#endif // TOKENS_H
