 As with tail call elimination, the type and presence of garbage collection depends on the backend.
 
 In the case of C++, the following types are supported:
 - Manual Memory Management, with the function free e.g. (free x)
 - Reference Counting with smart_ptrs, by default all shared but through special syntactic forms can use unique and weak pointers
 - Boehm's garbage collector
 - Ravenbrook's MPS, (many variations are supported through this library, I think) either with Boehm's or without
 - Maybe someday stalin style lifetime/data flow analysis
 - Optional boost memory pools in conjunction with one of the above methods

The lua backend uses lua's builtin garbage collector
