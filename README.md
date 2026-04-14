# HALIX Assembler v1.2.0
### Avenger Assembler Team — CEN5075 Systems Software Engineering, FAMU

A five-pass assembler for the Halix v12 instruction set, written in C++.  
Translates Halix assembly language (`.hal`) source files into machine code (`.hlx`).

---

## Team Members
- Alissa Forde
- Dunkley
- Kimieka
- Kiros
- Kalab M.
- Josiah Chuku

---

## Installation (Codio / Linux) — No Compiling Needed

Download the latest release binaries with two commands:

```bash
wget https://github.com/josiah1chuku/halix-assembler/releases/download/v1.2.0/halixAssembler
wget https://github.com/josiah1chuku/halix-assembler/releases/download/v1.2.0/halix.opcode
chmod +x halixAssembler
```

> **Both files must stay in the same directory.** The assembler reads `halix.opcode` at runtime.

---

## Usage

```bash
./halixAssembler yourfile.hal
```

This produces three output files:
- `yourfile.hlx` — machine code loaded by the Halix runtime
- `yourfile.lst` — assembler listing showing source and object code side by side
- `yourfile.log` — error and warning messages

---

## CLI Options

```bash
./halixAssembler --help       # Show usage instructions
./halixAssembler --version    # Show version number
./halixAssembler --man        # Show full manual
```

---

## Example

```bash
./halixAssembler first.hal
halix.crun first.hlx
```

---

## Building from Source (MSYS2 / Linux)

```bash
git clone https://github.com/josiah1chuku/halix-assembler.git
cd halix-assembler
g++ -std=c++17 -Wall -o halixAssembler halixAssembler.cpp
```

---

## Running the Test Suite

```bash
bash run_all_tests.sh
```

Current status: **78 tests passing**.

---

## Features (v1.2.0)
Every push to GitHub automatically:
1. Spins up a fresh Ubuntu Linux machine
2. Installs g++ and compiles the assembler
3. Runs all 78 tests
4. Assembles a demo program

- Five-pass assembly pipeline (Pass 0 – Pass 4)
- Supports all 54 Halix v12 instructions loaded from `halix.opcode`
- `.BLOCK` directive — allocates N consecutive data memory cells
- CLI flags: `--help`, `--version`, `--man`
- Unused variable warnings
- Pointer-style error messages with `^` indicator under offending token
- Generates `.hlx`, `.lst`, and `.log` output files
- 77 automated tests across all passes

---

## Project Structure

```
halix-assembler/
├── halixAssembler.cpp   # Entry point (unity build)
├── halix.opcode         # Instruction config file (required at runtime)
├── src/
│   ├── pass0.cpp        # Directive validation
│   ├── pass1.cpp        # Data symbol table
│   ├── pass2.cpp        # Mnemonic validation
│   ├── pass3.cpp        # Operand validation
│   └── pass4.cpp        # Machine code generation
├── include/
│   ├── types.h
│   ├── context.h
│   ├── utils.h
│   ├── logger.h
│   └── config.h
├── tests/               # 77 test cases
└── run_all_tests.sh     # Test runner
```

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| v1.0 | March 18, 2026 | Initial release, 36 tests passing |
| v1.2.0 | April 2026 | .BLOCK directive, CLI flags, pointer errors, 78 tests |

---

## Repository

[https://github.com/josiah1chuku/halix-assembler](https://github.com/josiah1chuku/halix-assembler)
