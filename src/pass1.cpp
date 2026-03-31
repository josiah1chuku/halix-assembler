// =============================================================================
// pass1.cpp — Pass 1: Data Symbol Table Construction
// =============================================================================

#include "pass1.h"
#include "utils.h"

using std::string;
using std::vector;

void pass1(const vector<string>& sourceLines, AssemblerContext& ctx) {
    // If no .ALLOC was found, treat the whole file as data-first (legacy).
    bool inData  = !ctx.allocFound;
    int  address = 0;
    int  lineNo  = 0;

    for (const string& raw : sourceLines) {
        lineNo++;
        string line   = stripComment(raw);
        auto   tokens = tokenize(line);
        if (tokens.empty()) continue;

        // ── Directive gates ───────────────────────────────────────────────────
        if (tokens[0] == ".ALLOC") { inData = true;  continue; }
        if (tokens[0] == ".BEGIN") { inData = false; continue; }
        if (tokens[0] == ".END") {
            // After .END continue scanning (data-first layout support).
            inData = !ctx.allocFound;
            continue;
        }
        if (!inData) continue;

        // ── Expect: varName .DATA [=value] ────────────────────────────────────
        if (tokens.size() < 2 || tokens[1] != ".DATA") {
            ctx.logError(lineNo,
                "Expected '[name] .DATA [=val]', got: " + line);
            continue;
        }

        const string& varName = tokens[0];

        if (!isValidIdentifier(varName)) {
            ctx.logError(lineNo, "Invalid variable name: " + varName);
            continue;
        }
        if (ctx.dataSymbolTable.count(varName)) {
            ctx.logError(lineNo, "Duplicate variable name: " + varName);
            continue;
        }
        if (ctx.allocCount > 0 && address >= ctx.allocCount) {
            ctx.logError(lineNo,
                "Too many .DATA declarations — .ALLOC only reserved "
                + std::to_string(ctx.allocCount) + " cells");
            continue;
        }

        // ── Optional initializer ──────────────────────────────────────────────
        int  value    = 0;
        bool hasValue = false;

        if (tokens.size() == 3) {
            const string& init = tokens[2];
            if (!isValidImmediate(init)) {
                ctx.logError(lineNo, "Invalid .DATA initializer: " + init);
            } else {
                int parsed;
                if (!parseInt(init.substr(1), parsed))
                    ctx.logError(lineNo,
                        "Initializer value out of range: " + init);
                else {
                    value    = parsed;
                    hasValue = true;
                }
            }
        }

        ctx.dataSymbolTable[varName] = {address, value, hasValue};
        address++;
    }
}
