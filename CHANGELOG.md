## 0.1.2 - December 21, 2025
### Changed:
- `char` is now a recognized data type mapped to `i8`
- `float` and `f32` are now recognized data types mapped to `float`
- `double` and `f64` are now recognized data types mapped to `double`
- `dataTypeToString()` now returns unsigned integers as "i(n_bits)", following the LLVM IR standards
- AST struct now has a union of various data types as value of literals

## 0.1.1 - December 20, 2025
### Added:
- CHANGELOG.md

#### Changed:
- `int` is now a recognized data type mapped to `i32`

## 0.1 (Initial commit) - December 20, 2025
#### Added:
- AST.c
- checker.c
- eon.c
- io.c
- ir.c
- lexer.c
- main.c
- parser.c
- token.c
- types.c
- vector.c

- AST.h
- checker.h
- eon.h
- io.h
- ir.h
- lexer.h
- macros.h
- main.h
- parser.h
- token.h
- types.h
- vector.h

- main.eon

- README.md
- .gitignore
- Makefile