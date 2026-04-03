// =============================================================================
// pass0.cpp -- Pass 0: Directive Validation
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

        // -- .ALLOC
        if (first == ".ALLOC") {
            allocSeen       = true;
            inData          = true;
            ctx.allocFound  = true;
            if (tokens.size() < 2) {
                ctx.logErrorWithPointer(lineNo,
                    ".ALLOC missing size argument -- example: .ALLOC 5",
                    raw, ".ALLOC");
            } else {
                int n;
                if (!parseInt(tokens[1], n))
                    ctx.logErrorWithPointer(lineNo,
                        ".ALLOC size is not an integer: " + tokens[1],
                        raw, tokens[1]);
                else
                    ctx.allocCount = n;
            }
            continue;
        }

        // -- .BEGIN
        if (first == ".BEGIN") {
            if (inCode)
                ctx.logErrorWithPointer(lineNo,
                    "Second .BEGIN found without intervening .END",
                    raw, ".BEGIN");
            inData = false;
            inCode = true;
            continue;
        }

        // -- .END
        if (first == ".END") {
            if (!inCode)
                ctx.logErrorWithPointer(lineNo,
                    ".END without matching .BEGIN",
                    raw, ".END");
            inCode = false;
            continue;
        }

        // -- .DATA or .BLOCK before .ALLOC
        if (allocExistsInFile && !allocSeen
                && tokens.size() >= 2
                && (tokens[1] == ".DATA" || tokens[1] == ".BLOCK"))
            ctx.logErrorWithPointer(lineNo,
                ".DATA/.BLOCK declared before .ALLOC -- .ALLOC must come first",
                raw, tokens[1]);

        // -- Unknown directive inside data section
        if (inData && tokens.size() >= 2
                && tokens[1] != ".DATA" && tokens[1] != ".BLOCK")
            ctx.logErrorWithPointer(lineNo,
                "Unknown directive in DATA section: " + tokens[1],
                raw, tokens[1]);

        // -- Unknown top-level directive
        if (!inData && !inCode && !first.empty() && first[0] == '.'
                && first != ".ALLOC" && first != ".BEGIN" && first != ".END")
            ctx.logErrorWithPointer(lineNo,
                "Unknown directive: " + first, raw, first);
    }

    // -- Unclosed .BEGIN block
    if (inCode)
        ctx.logError(lineNo,
            "Missing .END -- reached end of file inside .BEGIN block");
}
