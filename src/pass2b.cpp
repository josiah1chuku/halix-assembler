// =============================================================================
// pass2b.cpp — Pass 2b: Instruction Label Symbol Table Construction
// =============================================================================

#include "pass2b.h"
#include "utils.h"

using std::string;
using std::vector;

void pass2b(const vector<string>& sourceLines, AssemblerContext& ctx) {
    bool inCode  = false;
    int  address = 0;
    int  lineNo  = 0;

    for (const string& raw : sourceLines) {
        lineNo++;
        string line   = stripComment(raw);
        auto   tokens = tokenize(line);
        if (tokens.empty()) continue;

        if (tokens[0] == ".BEGIN") { inCode = true;  continue; }
        if (tokens[0] == ".END")   { inCode = false; continue; }
        if (!inCode) continue;

        string mnemonic;
        string labelName;

        if (tokens[0].back() == ':') {
            labelName = tokens[0].substr(0, tokens[0].size() - 1);
            if (tokens.size() < 2) continue;   // label-only line — caught by pass2a
            mnemonic = tokens[1];
        } else {
            mnemonic = tokens[0];
        }

        // Skip lines with unknown mnemonics (pass2a already logged them).
        if (!ctx.instructionTable.count(mnemonic)) continue;

        // Register the label at the current IM address.
        if (!labelName.empty()) {
            if (!isValidIdentifier(labelName))
                ctx.logError(lineNo, "Invalid label name: " + labelName);
            else if (ctx.labelTable.count(labelName))
                ctx.logError(lineNo, "Duplicate label: " + labelName);
            else
                ctx.labelTable[labelName] = address;
        }

        address++;
    }
}
