// =============================================================================
// pass2a.cpp — Pass 2a: Mnemonic Validation
// =============================================================================

#include "pass2a.h"
#include "utils.h"

using std::string;
using std::vector;

void pass2a(const vector<string>& sourceLines, AssemblerContext& ctx) {
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

        // Determine the mnemonic token (skip optional "Label:" prefix).
        string mnemonic;
        if (tokens[0].back() == ':') {
            if (tokens.size() < 2) {
                ctx.logError(lineNo,
                    "Label with no instruction: " + tokens[0]);
                continue;
            }
            mnemonic = tokens[1];
        } else {
            mnemonic = tokens[0];
        }

        if (!ctx.instructionTable.count(mnemonic))
            ctx.logError(lineNo,
                "Unknown instruction mnemonic: " + mnemonic);
    }
}
