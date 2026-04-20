# HALIX Assembler

A production-grade, multi-pass C++ assembler for the Halix v25 instruction set.

Built by the **Avenger Assembler Team** for CEN5075 Systems Software Engineering at FAMU.

---

## Team

- Alissa Forde
- Dunkley Kimieka
- Kiros Kalab M.
- Josiah Chuku

---

## Features

- 61 instructions from the Halix v25 instruction set
- 5-pass assembly pipeline
- Data-driven opcode table
- Clean error messages with line numbers
- Three output files per assembly: .hlx, .lst, .log
- Full CI/CD on GitHub Actions
- 78 unit tests + 46 acceptance tests
- Zero CodeQL security vulnerabilities

---

## Installation

One command on Codio or Ubuntu:

curl -fsSL https://raw.githubusercontent.com/josiah1chuku/halix-assembler/main/install.sh | bash

---

## Quick Start

./halixAssembler myprogram.hal
./halix_v25.crun myprogram.hlx

---

## Architecture

| Pass | Purpose |
|------|---------|
| Pass 0 | Validate directives |
| Pass 1 | Build Data Symbol Table |
| Pass 2a | Validate instruction mnemonics |
| Pass 2b | Build Instruction Label Symbol Table |
| Pass 3 | Validate operands |
| Pass 4 | Generate machine code |

---

## Links

- [GitHub Repository](https://github.com/josiah1chuku/halix-assembler)
- [Latest Release](https://github.com/josiah1chuku/halix-assembler/releases)
- [GitHub Actions CI](https://github.com/josiah1chuku/halix-assembler/actions)

---

Version 1.5.0 · April 2026
