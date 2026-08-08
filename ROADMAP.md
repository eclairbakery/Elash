# Elash v0.2.0 Roadmap

## Language Features
### Statements
- [ ] Init-statements in `if` and `while`
- [ ] Post-iteration statement in `while` loop
- [x] Initializers as `return` values

### Type system
- [ ] Enum types
- [ ] Union types
- [ ] Optional types
- [ ] Read-only types
- [ ] Write-only types

### Operators
- [ ] Sizeof operator (`sizeof`)
- [ ] Alignof operator (`alignof`)
- [ ] Offsetof operator (`offsetof`)

- [ ] Bitcast operator (`bitcast`)

- [ ] Optional fallback operator (`??`)
- [ ] Optional map operator (`?:`)
- [ ] Optional unwrap operator (`!`)
- [ ] Optional member operator (`?.`)

### Driver/CLI
- [x] `-I[src/sys] <name>=<path>` flag
- [x] `--no-corelib` and `--no-stdlib` flags
- [ ] Predefined modes (`release`, `debug`)
- [ ] Invoke system linker
- [ ] Produce final executable
- [ ] Don't report warnings from system headers

## Preprocessor
- [ ] Implement file inclusion directive
- [ ] Resolve scoped include paths
- [ ] Prevent recursive inclusion
- [ ] Implement embed directive
- [ ] Support preprocessor variables and constatns
- [ ] Support preprocessor functions
- [ ] Implement macro expansion engine
- [ ] Implement macro expansion result rescan
- [ ] Preserve correct source locations after expansion
- [ ] Implement conditional compilation directives
- [ ] Implement loops (`#while`, `#for`)
- [ ] Support predefined preprocessor functions and macros
- [ ] Builtin constants (`ELC_MODE`, `ELC_VERSION`, `ELC_OPTLVL`)

## Library
### Corelib
- [ ] `str` module

### Stdlib
- [ ] `io` module
- [ ] `mem` module

## Lowering
- [ ] Perform early MIR optimizations
- [ ] Attach DWARF debug info
- [ ] Attach source spans to MIR structures

### Optimizations
#### LLVM passes
- [ ] Configure pass manager
- [ ] Define optimization levels (O0, O1, O2)
- [ ] Run optimization pipeline
- [ ] Verify module after passes

#### Frontend
- [ ] Lower `T&?` and `T[&]?` to just `T*` in MIR
- [ ] Omit null checks in release mode

## Documentation
### Compiler internals
- [ ] Lexer docs
- [ ] Parser docs
- [ ] Binder docs
- [ ] Lowerer docs
- [ ] Srcdoc docs

### The Language
- [ ] Setup initial language documentation site
- [ ] Document elash basics

## Stability & Hardening
- [ ] Preproc test suite
- [ ] Parser test suite
- [x] Unparser test suite
- [ ] Binder test suite
- [ ] Lowerer test suite
- [x] Fuzz tests
