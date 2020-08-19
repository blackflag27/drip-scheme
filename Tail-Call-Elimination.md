Drip Scheme doesn't always completely follow the standard in that it's support for tail call elimination depends on it's backend.

For example in C++ tail call elimination is completely up to the compiler. (Though maybe with gotos I can do this myself)
I should also consider trampolining, and chicken style stack checking.
