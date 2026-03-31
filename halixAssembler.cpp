// =============================================================================
// halixAssembler.cpp  —  HALIX Assembler v4.0  (unity build)
//
// PURPOSE
// -------
// This file is the single compilation entry point.  It #includes every module
// so that the compiler processes them as one translation unit.  This means:
//
//   g++ -std=c++17 -Wall -Wextra -o halixAssembler halixAssembler.cpp
//
// ...is the only command needed — no Makefile, no separate object files.
//
// MODULARITY IS PRESERVED
// -----------------------
// Each module still lives in its own src/*.h + src/*.cpp pair with clear
// boundaries and no shared globals.  The unity build is purely a compilation
// convenience — you can switch to separate compilation at any time by just
// running:
//
//   g++ -std=c++17 -Isrc src/*.cpp -o halixAssembler
//
// or using the provided Makefile.
//
// MODULE ORDER
// ------------
// Includes must follow the dependency chain (leaves first, root last):
//
//   types.h       — enums and POD structs          (no deps)
//   context.h     — AssemblerContext               (types.h)
//   utils         — pure text helpers              (std only)
//   config        — load halix.opcode → IOT        (context, utils)
//   pass0         — directive validation           (context, utils)
//   pass1         — data symbol table              (context, utils)
//   pass2a        — mnemonic validation            (context, utils)
//   pass2b        — label symbol table             (context, utils)
//   pass3         — operand validation             (context, utils)
//   pass4         — machine code generation        (context, utils)
//   logger        — .log writer + symbol display   (context)
//   main          — orchestrates all passes        (everything)
// =============================================================================

// Headers-only modules (no matching .cpp — everything is in the .h)
#include "src/types.h"
#include "src/context.h"

// Implementation modules — include the .cpp, not the .h, so definitions
// are compiled exactly once inside this translation unit.
#include "src/utils.cpp"
#include "src/config.cpp"
#include "src/pass0.cpp"
#include "src/pass1.cpp"
#include "src/pass2a.cpp"
#include "src/pass2b.cpp"
#include "src/pass3.cpp"
#include "src/pass4.cpp"
#include "src/logger.cpp"
#include "src/main.cpp"
