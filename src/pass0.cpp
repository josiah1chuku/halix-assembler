// =============================================================================
// pass0.cpp — Pass 0: Directive Validation
// =============================================================================

#include "pass0.h"
#include "utils.h"

using std::string;
using std::vector;

void pass0(const vector<string>& sourceLines, AssemblerContext& ctx) {
    bool allocExistsInFile = sourceContainsDirective(sourceLines, ".ALLOC");
    bool inData  = false;
    bool inCode  = false;
    bool allocSeen = false;
    int  lineNo  = 0;

    for (const string& raw : sourceLines) {
        lineNo++;
        string line   = stripComment(raw);
        auto   tokens = tokenize(line);
        if (tokens.empty()) continue;
        const string& first = tokens[0];

        // ── .ALLOC ───────────────────────────────────────────────────────────
        if (first == ".ALLOC") {
            allocSeen       = true;
            inData          = true;
            ctx.allocFound  = true;
            if (tokens.size() < 2) {
                ctx.logError(lineNo,
                    ".ALLOC missing size argument — example: .ALLOC 5");
            } else {
                int n;
                if (!parseInt(tokens[1], n))
                    ctx.logError(lineNo,
                        ".ALLOC size is not an integer: " + tokens[1]);
                else
                    ctx.allocCount = n;
            }
            continue;
        }

        // ── .BEGIN ───────────────────────────────────────────────────────────
        if (first == ".BEGIN") {
            if (inCode)
                ctx.logError(lineNo,
                    "Second .BEGIN found without intervening .END");
            inData = false;
            inCode = true;
            continue;
        }

        // ── .END ─────────────────────────────────────────────────────────────
        if (first == ".END") {
            if (!inCode)
                ctx.logError(lineNo, ".END without matching .BEGIN");
            inCode = false;
            continue;
        }

        // ── .DATA before .ALLOC ───────────────────────────────────────────────
        if (allocExistsInFile && !allocSeen
                && tokens.size() >= 2 && tokens[1] == ".DATA")
            ctx.logError(lineNo,
                ".DATA declared before .ALLOC — .ALLOC must come first");

        // ── Unknown directive inside data section ─────────────────────────────
        if (inData && tokens.size() >= 2 && tokens[1] != ".DATA")
            ctx.logError(lineNo,
                "Unknown directive in DATA section: " + tokens[1]);

        // ── Unknown top-level directive ───────────────────────────────────────
        if (!inData && !inCode && !first.empty() && first[0] == '.'
                && first != ".ALLOC" && first != ".BEGIN" && first != ".END")
            ctx.logError(lineNo, "Unknown directive: " + first);
    }

    // ── Unclosed .BEGIN block ─────────────────────────────────────────────────
    if (inCode)
        ctx.logError(lineNo,
            "Missing .END — reached end of file inside .BEGIN block");
}
