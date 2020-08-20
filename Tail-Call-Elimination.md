Drip Scheme doesn't always completely follow the standard in that it's support for tail call elimination depends on it's backend.

For example in C++ tail call elimination is can be done either with compiler optimizations with -foptimize-sibling-calls, with trampolining, or with Appel's method a la chicken scheme.

In Lua, it depends on the implementation used. The default lua backend, the reference implementation of Lua 5.4 tail call elimination is present.
