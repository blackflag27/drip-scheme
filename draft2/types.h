#ifndef TYPES_H
#define TYPES_H

/*
 * All objects are represented as a tagged union of pointers
 * Many implementations of scheme include various optimizations
 * that make integer arithmetic not require a pointer dereference but
 * for the sake of simplicity my implementation will not.
 *
 * Only a subset of Scheme standard data types are implemented
 * these are:
 *
 * Integers (64 bit signed only)
 * Floats (double precision only)
 * Characters (only 8 bit ASCII characters with the high bit ignored)
 * Booleans (True and False)
 * Void (While technically not in the standard this is what functions
 *		 that are only useful for their side effect return)
 * Empty list
 * End Of File
 * Pairs (Pairs are also used to implement lists, which are not
 *		  a seperate type)
 * Symbols (Like strings but can only include the same characters as
 *			identifiers, and they are interned so that every symbol
 *			with the same value, are stored in the same location
 *			for cheap comparison)
 * Strings
 * Ports (File I/O)
 * Procedures (Rest parameters are not implemented however)
*/

#include <string>
#include <fstream>

struct Pair;

enum class Type {
	Integer,
	Float,
	Character,
	Boolean,
	Void,
	Empty,
	Eof,
	Pair,
	Symbol,
	String,
	Port,
	Procedure
};

struct Procedure;

struct Object {
	Type type;
	union {
		int i;	
		double d;
		char c;
		bool b;
		// Void, Empty, and EOF are represented as tags only
		Pair* p;
		std::string* s; // Used for both Symbols and Strings
						// the tag is used to determine which is which
		std::fstream* po;
		Procedure* pr;
	};
};

struct Pair {
	Object car;
	Object cdr;
};

// Each Environment struct represents a frame of the environment, 
// the complete environment is represented by the most recent frame,
// with links to the parent frames. Each frame is essentially a scope.

struct Environment {
    Environment* parent;
	std::unordered_map<std::string, Object> table;
};

struct Procedure {
	Environment* closure; // Procedures capture outside variables they
						  // reference when they are created.
	int arity;
	Object *parameters; // These should be symbols. The reason this
						 // is an array of Objects instead of strings
						 // is so they can be cheaply compared to
						 // mentions of the symbols in the body of the
						 // function.
	Object body; // the body is an Object because code and data are not
				 // seperate in scheme. Code is represented as pairs of
				 //	symbols, etc...
};

#endif // TYPES_H
