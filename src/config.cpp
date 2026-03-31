// =============================================================================
// config.cpp — loadConfig implementation
// Format of each non-comment line in halix.opcode:
//   MNEMONIC  OPCODE  OPERAND_TYPE
// where OPERAND_TYPE ∈ { NONE, IMM, LABEL, DATA }
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

        // Strip inline comments
        auto pos = line.find('#');
        if (pos != string::npos) line = line.substr(0, pos);

        std::istringstream ss(line);
        string mnemonic, opcodeStr, opTypeStr;
        if (!(ss >> mnemonic >> opcodeStr >> opTypeStr)) continue;

        int opcode;
        if (!parseInt(opcodeStr, opcode)) {
            cerr << "CONFIG LINE " << lineNo
                 << ": Invalid opcode for " << mnemonic << "\n";
            return false;
        }

        OperandType opType;
        if      (opTypeStr == "NONE")  opType = OP_NONE;
        else if (opTypeStr == "IMM")   opType = OP_IMM;
        else if (opTypeStr == "LABEL") opType = OP_LABEL;
        else if (opTypeStr == "DATA")  opType = OP_DATA;
        else {
            cerr << "CONFIG LINE " << lineNo
                 << ": Unknown operand type '" << opTypeStr << "'\n";
            return false;
        }

        ctx.instructionTable[mnemonic] = {opcode, opType};
    }

    cout << "Loaded " << ctx.instructionTable.size()
         << " instructions from " << configFile << "\n";
    return true;
}
