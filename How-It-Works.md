Drip can be run in two modes: Compile or Compile-and-Go.

Compilation follows these steps:

- Source files are run through m4 optionally with the Drippy macro set
- Tokenization
- Parsing
- Generation
- Compilation, Linking, etc... (depends on backend)
- Optionally Execution
