// =============================================================================
// pass3.cpp — Pass 3: Operand Validation
// =============================================================================

#include "pass3.h"
#include "utils.h"

using std::string;
using std::vector;

void pass3(const vector<string>& sourceLines, AssemblerContext& ctx) {
    bool inCode = false;
    int  lineNo = 0;

    for (const string& raw : sourceLines) {
        lineNo++;
        string line   = stripComment(raw);
        auto   tokens = tokenize(line);
        if (tokens.empty()) continue;

        if (tokens[0] == ".BEGIN") { inCode = true;  continue; }
        if (tokens[0] == ".END")   { inCode = false; continue; }
        if (!inCode) continue;

        // Determine mnemonic and where the operand token starts.
        string mnemonic;
        int    opStart;

        if (tokens[0].back() == ':') {
            if (tokens.size() < 2) continue;
            mnemonic = tokens[1];
            opStart  = 2;
        } else {
            mnemonic = tokens[0];
            opStart  = 1;
        }

        // Skip unknown mnemonics — pass2a already logged them.
        if (!ctx.instructionTable.count(mnemonic)) continue;

        OperandType opType    = ctx.instructionTable.at(mnemonic).opType;
        bool        hasOperand = (static_cast<int>(tokens.size()) > opStart);

        // ── OP_NONE: must have no operand ─────────────────────────────────────
        if (opType == OP_NONE) {
            if (hasOperand)
                ctx.logError(lineNo,
                    mnemonic + " takes no operand, found: " + tokens[opStart]);
            continue;
        }

        // ── Non-NONE: must have an operand ────────────────────────────────────
        if (!hasOperand) {
            ctx.logError(lineNo,
                mnemonic + " requires an operand but none was given");
            continue;
        }

        const string& operand = tokens[opStart];

        if (opType == OP_IMM) {
            if (!isValidImmediate(operand))
                ctx.logError(lineNo,
                    mnemonic + " requires immediate operand (=number), got: "
                    + operand);

        } else if (opType == OP_LABEL) {
            if (!ctx.labelTable.count(operand))
                ctx.logError(lineNo,
                    mnemonic + " branch target label not found: " + operand);

        } else {  // OP_DATA — variable name OR inline immediate
            if (!operand.empty() && operand[0] == '=') {
                if (!isValidImmediate(operand))
                    ctx.logError(lineNo, "Malformed immediate: " + operand);
            } else if (!ctx.dataSymbolTable.count(operand)) {
                ctx.logError(lineNo, "Undefined variable: " + operand);
            }
        }
    }
}
