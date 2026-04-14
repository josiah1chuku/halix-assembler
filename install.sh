#!/bin/bash
# HALIX Assembler -- One-line installer
# Usage: curl -sSL https://raw.githubusercontent.com/josiah1chuku/halix-assembler/main/install.sh | bash

echo "========================================"
echo "  HALIX Assembler -- Avenger Assembler"
echo "  CEN5075, FAMU, 2026"
echo "========================================"
echo ""

if [ -d "halix-assembler" ]; then
    echo "Updating existing installation..."
    cd halix-assembler
    git pull origin main
else
    echo "Cloning repository..."
    git clone https://github.com/josiah1chuku/halix-assembler.git
    cd halix-assembler
fi

echo "Compiling assembler..."
make

echo "Adding to PATH..."
INSTALL_DIR=$(pwd)
grep -q "halix-assembler" ~/.bashrc || echo "export PATH=\"\$PATH:$INSTALL_DIR\"" >> ~/.bashrc
export PATH="$PATH:$INSTALL_DIR"

echo ""
echo "========================================"
echo "  Installation complete!"
echo "  $(./halixAssembler --version)"
echo "========================================"
echo ""
echo "Run: halixAssembler --version"
echo "Run: halixAssembler --help"
echo "Run: halixAssembler --man"
echo "Run: halixAssembler myprogram.hal"
