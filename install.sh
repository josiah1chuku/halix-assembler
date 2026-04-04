#!/bin/bash
# HALIX Assembler -- One-line installer
# Usage: curl -sSL https://raw.githubusercontent.com/josiah1chuku/halix-assembler/main/install.sh | bash

echo "========================================"
echo "  HALIX Assembler -- Avenger Assembler"
echo "  CEN5075, FAMU, 2026"
echo "========================================"
echo ""
echo "Cloning repository..."
git clone https://github.com/josiah1chuku/halix-assembler.git
cd halix-assembler

echo "Compiling assembler..."
g++ -std=c++17 -O2 -o halixAssembler halixAssembler.cpp

echo "Adding to PATH..."
echo "export PATH=\"\$PATH:$(pwd)\"" >> ~/.bashrc
export PATH="$PATH:$(pwd)"

echo ""
echo "========================================"
echo "  Installation complete!"
echo "  Version: $(./halixAssembler --version)"
echo "========================================"
echo ""
echo "Run: halixAssembler --help"
echo "Run: halixAssembler --man"
echo "Run: halixAssembler myprogram.hal"
