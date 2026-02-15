# Elash Developement Roadmap

## 0. Project Setup

- [x] Initialize repository
- [ ] Configure build system
- [ ] Add LLVM C bindings as dependency
- [x] Create compiler entry point
- [ ] Implement source file loader
- [ ] Implement diagnostic system (line/column + spans)
- [ ] Setup automated tests


## 1. Lexer

- [x] Define token types
- [x] Implement identifier and keyword recognition
- [x] Implement numeric literals (int, float)
- [x] Implement string literals (with escapes)
- [x] Implement operator parsing (single and multi-character)
- [x] Implement comment handling
- [x] Attach position metadata to tokens
- [ ] Add lexer test suite

## 2. Preprocessor

- [x] Define preprocessor directive syntax
- [x] Implement preprocessing phase as a token-stream transform
- [ ] Implement file inclusion directive
- [ ] Resolve relative and module-based include paths
- [ ] Prevent recursive inclusion
- [ ] Implement function-like macros
- [ ] Implement macro expansion engine
- [ ] Handle nested macro expansion
- [ ] Preserve correct source locations for diagnostics after expansion
- [ ] Implement conditional compilation directives
- [ ] Evaluate constant expressions in conditionals
- [ ] Support predefined compiler macros
- [ ] Integrate preprocessor output with lexer
- [ ] Add preprocessor test suite (includes, macros, conditionals, edge cases)


## 3. Parser - AST

- [ ] Define AST node structures
- [ ] Implement expression parser with precedence handling
- [ ] Implement variable declarations
- [ ] Implement function declarations
- [ ] Implement blocks
- [ ] Implement control flow statements
- [ ] Implement return statements
- [ ] Implement top-level/module parsing
- [ ] Attach source spans to AST nodes
- [ ] Add parser test suite


## 4. Semantic Analysis

- [ ] Implement symbol table
- [ ] Implement scope handling
- [ ] Implement built-in functions
- [ ] Perform name resolution
- [ ] Detect duplicate declarations
- [ ] Detect undefined identifiers
- [ ] Define internal type representation
- [ ] Implement expression type checking
- [ ] Validate function calls
- [ ] Validate return types
- [ ] Implement implicit and explicit conversions
- [ ] Add semantic test suite


## 5. AST → LLVM IR Lowering

- [ ] Initialize LLVM context, module, and IR builder
- [ ] Define mapping from language types to LLVM types
- [ ] Lower primitive types
- [ ] Lower function signatures
- [ ] Create LLVM function definitions
- [ ] Create entry blocks
- [ ] Lower local variables using alloca in entry block
- [ ] Lower arithmetic operations
- [ ] Lower comparisons
- [ ] Lower branching (if / loops)
- [ ] Lower function calls
- [ ] Lower return instructions
- [ ] Handle global variables
- [ ] Verify LLVM module
- [ ] Emit LLVM IR (.ll)
- [ ] Emit object file


## 6. Linking

- [ ] Configure LLVM target triple
- [ ] Create target machine
- [ ] Emit object files
- [ ] Invoke system linker
- [ ] Produce final executable


## 7. Optimizations (LLVM Passes)

- [ ] Configure pass manager
- [ ] Define optimization levels (O0, O1, O2)
- [ ] Run optimization pipeline
- [ ] Verify module after passes
- [ ] Benchmark optimized vs non-optimized builds


## 8. CLI

- [ ] Implement command parsing
- [ ] Implement useful flags
- [ ] Implement optimization flags
- [ ] Implement colored diagnostics
- [ ] Return proper exit codes


## 9. Modules & Multi-file Compilation

- [ ] Implement import resolution
- [ ] Support multi-file parsing
- [ ] Share symbol table across modules
- [ ] Implement symbol visibility rules
- [ ] Detect circular imports
- [ ] Merge modules into single LLVM module before emission


## 10. Standard Library
- [ ] `io` module
- [ ] `fs` module
- [ ] `math` module
- [ ] `collections` module
- [ ] `time` module
- [ ] `rand` module
- [ ] `thread` module
- [ ] `net` module
- 

## 11. Stability & Hardening

- [ ] End-to-end compilation tests
- [ ] Invalid program test suite
- [ ] Large file compilation test
- [ ] Memory usage checks
- [ ] Regression tests