# HALIX Assembler v1.5.0

**Avenger Assembler Team — CEN5075 Systems Software Engineering, FAMU, 2026**

A five-pass assembler for the Halix v25 instruction set, written in C++.  
Translates Halix Assembly Language (`.hal`) source files into machine code (`.hlx`).

![CI](https://github.com/josiah1chuku/halix-assembler/actions/workflows/ci.yml/badge.svg)

---

## Team Members

| Name | Role |
|---|---|
| Alissa Forde | Avenger Assembler Team |
| Dunkley Kimieka | Avenger Assembler Team |
| Kiros Kalab M. | Avenger Assembler Team |
| Josiah Chuku | Avenger Assembler Team |

---

## Installation (Codio / Linux) — No Compiling Needed

Download the latest release binaries with one command:

```bash
wget https://github.com/josiah1chuku/halix-assembler/releases/download/v1.5.0/halixAssembler
wget https://github.com/josiah1chuku/halix-assembler/releases/download/v1.5.0/halix_v25.opcode
chmod +x halixAssembler
```

Both files must stay in the same directory. The assembler reads `halix_v25.opcode` at runtime.

---

## Usage

```bash
./halixAssembler yourfile.hal
```

Produces three output files:

| File | Description |
|---|---|
| `yourfile.hlx` | Machine code loaded by the Halix runtime |
| `yourfile.lst` | Assembler listing showing source and object code side by side |
| `yourfile.log` | Error and warning messages |

### CLI Options

```bash
./halixAssembler --help       # Show usage instructions
./halixAssembler --version    # Show version number
./halixAssembler --man        # Show full manual
```

---

## End-to-End Example

```bash
./halixAssembler 3powers-mcnairA799.hal
./halix_v25.crun 3powers-mcnairA799.hlx
```

Input: `10` → Output: `1000` (10³) — 6 instructions executed.

See [`docs/diagrams/halix_e2e.pdf`](docs/diagrams/halix_e2e.pdf) for the full system flow diagram.

---

## Building from Source (MSYS2 UCRT64 / Linux)

> **MSYS2 users:** Always use the **UCRT64** shell — `g++` is not available in MINGW64.

```bash
git clone https://github.com/josiah1chuku/halix-assembler.git
cd halix-assembler
g++ -std=c++17 -Wall -o halixAssembler src/*.cpp
```

---

## Running the Test Suite

```bash
bash tests/run_all_tests.sh
```

**Current status: 78/78 tests passing** on both Codio and GitHub Actions CI.

---

## Features (v1.5.0)

- **Five-pass assembly pipeline** (Pass 0 – Pass 4)
- **61 Halix v25 instructions** loaded from `halix_v25.opcode` — no recompile needed to add instructions
- **ANSI colour output** — cyan/yellow/green/red pass progress and error messages
- **CodeQL security scanning** — zero vulnerabilities
- **Modular 12-file architecture** — each pass in its own `.cpp` file
- **CI/CD** — every push to GitHub automatically compiles, runs all 78 tests, and assembles a demo program
- **GitHub Releases** with pre-built Linux binaries and `install.sh` one-line installer
- Generates `.hlx`, `.lst`, and `.log` output files
- CLI flags: `--help`, `--version`, `--man`
- Unused variable warnings
- Pointer-style error messages with `^` indicator under offending token

---

## Project Structure

```
halix-assembler/
├── src/
│   ├── main.cpp         # Entry point
│   ├── pass0.cpp        # Directive validation
│   ├── pass1.cpp        # Data symbol table
│   ├── pass2a.cpp       # Mnemonic validation
│   ├── pass2b.cpp       # Instruction label symbol table
│   ├── pass3.cpp        # Operand validation
│   ├── pass4.cpp        # Machine code generation
│   ├── config.cpp       # halix_v25.opcode parser
│   ├── logger.cpp       # ANSI colour logging
│   └── utils.cpp        # Shared utilities
├── include/
│   ├── types.h
│   ├── context.h
│   ├── utils.h
│   ├── logger.h
│   └── config.h
├── tests/               # 78 unit test cases
│   └── run_all_tests.sh # Test runner
├── instructor_tests/    # 46 instructor sample programs
├── docs/
│   ├── MPD_Halix_v2.docx          # Maintenance Plan Document v2.0
│   └── diagrams/
│       ├── halix_pipeline.pdf     # 5-Pass Pipeline (TikZ)
│       ├── halix_pipeline.tex     # TikZ source
│       ├── halix_e2e.pdf          # End-to-End System Flow (TikZ)
│       └── halix_e2e.tex          # TikZ source
├── halix_v25.opcode     # Instruction config file (required at runtime)
└── halix_v25.crun       # Instructor runtime (Codio)
```

---

## Documentation

| Document | Location | Description |
|---|---|---|
| Maintenance Plan (MPD) | [`docs/MPD_Halix_v2.docx`](docs/MPD_Halix_v2.docx) | Maintenance procedures, defect log, version history |
| 5-Pass Pipeline Diagram | [`docs/diagrams/halix_pipeline.pdf`](docs/diagrams/halix_pipeline.pdf) | TikZ diagram of the assembly pipeline |
| End-to-End System Diagram | [`docs/diagrams/halix_e2e.pdf`](docs/diagrams/halix_e2e.pdf) | TikZ diagram of full system flow |

---

## Version History

| Version | Date | Changes | Tests |
|---|---|---|---|
| v1.0 | March 18, 2026 | Initial release, five-pass assembler, 54 Halix v12 instructions | 36/36 |
| v1.5.0 | April 20, 2026 | Aligned with halix_v25.opcode (61 instructions), modular 12-file architecture, ANSI colour output, CodeQL scanning, instructor test suite (46 programs) | 78/78 |

---

## Repository

**https://github.com/josiah1chuku/halix-assembler**
