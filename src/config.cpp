// =============================================================================
// config.cpp — loadConfig implementation
// Format of each non-comment line in halix.opcode:
//   OPCODE #MNEMONIC#
// =============================================================================

#include "config.h"
#include "utils.h"

#include <fstream>
#include <iostream>
#include <sstream>

using std::string;
using std::cerr;
using std::cout;

bool loadConfig(const string& configFile, AssemblerContext& ctx) {
    std::ifstream in(configFile);
    if (!in.is_open()) {
        cerr << "ERROR: Cannot open config file: " << configFile << "\n";
        return false;
    }

    string line;
    int lineNo = 0;

    while (std::getline(in, line)) {
        lineNo++;

        std::istringstream ss(line);
        int opcode;
        string mnemonicRaw;

        if (!(ss >> opcode >> mnemonicRaw)) continue;

        // Strip # delimiters from #MNEMONIC#
        if (mnemonicRaw.size() >= 2 &&
            mnemonicRaw.front() == '#' &&
            mnemonicRaw.back() == '#') {
            mnemonicRaw = mnemonicRaw.substr(1, mnemonicRaw.size() - 2);
        } else {
            continue;
        }

        string mnemonic = mnemonicRaw;

        // Derive operand type from opcode
        OperandType opType;
        if (opcode == 1  || opcode == 2  || opcode == 3  ||
            opcode == 4  || opcode == 5  || opcode == 6  ||
            opcode == 7  || opcode == 8  || opcode == 9  ||
            opcode == 15 || opcode == 18 || opcode == 19 ||
            opcode == 20 || opcode == 21 || opcode == 22 ||
            opcode == 23 || opcode == 30 || opcode == 35 ||
            opcode == 58 || opcode == 59) {
            opType = OP_DATA;
        } else if (opcode >= 24 && opcode <= 29) {
            opType = OP_IMM;
        } else if (opcode == 10 || opcode == 11 ||
                   opcode == 12 || opcode == 32) {
            opType = OP_LABEL;
        } else {
            opType = OP_NONE;
        }

        ctx.instructionTable[mnemonic] = {opcode, opType};
    }

    cout << "Loaded " << ctx.instructionTable.size()
         << " instructions from " << configFile << "\n";
    return true;
}
