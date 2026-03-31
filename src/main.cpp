// =============================================================================
// main.cpp — HALIX Assembler v4.0  (modular build)
// Entry point: parses CLI, loads config, runs all five passes in order.
//
// Exit codes:
//   0  success — .hlx, .lst, .log produced
//   1  usage error or file I/O failure
//   2  errors detected in passes 0-2a (no .hlx produced)
//   3  errors detected in pass 3     (no .hlx produced)
// =============================================================================

#include "config.h"
#include "context.h"
#include "logger.h"
#include "pass0.h"
#include "pass1.h"
#include "pass2a.h"
#include "pass2b.h"
#include "pass3.h"
#include "pass4.h"
#include "utils.h"

#include <iostream>

using std::cerr;
using std::cout;
using std::string;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: ./halixAssembler <source.hal> [config.opcode]\n";
        return 1;
    }

    const string srcFile    = argv[1];
    const string configFile = (argc >= 3) ? argv[2] : "halix.opcode";
    const string base       = getBaseName(srcFile);

    AssemblerContext ctx;

    // ── Load instruction opcode table ─────────────────────────────────────────
    if (!loadConfig(configFile, ctx)) return 1;

    // ── Read source file once — shared across all passes ─────────────────────
    std::vector<string> sourceLines;
    if (!readSourceFile(srcFile, sourceLines)) {
        cerr << "ERROR: Cannot open source file: " << srcFile << "\n";
        return 1;
    }

    cout << "=== HALIX Assembler v4.0 ===\n"
         << "Source : " << srcFile << "\n\n";

    // ── Pass 0: Directive Validation ──────────────────────────────────────────
    cout << "Pass 0: Validating directives...\n";
    pass0(sourceLines, ctx);

    if (!ctx.allocFound) {
        ctx.logWarning(0,
            ".ALLOC directive not found — defaulting to 10 data cells "
            "(legacy behaviour)");
        ctx.allocCount = 10;
    }

    // ── Pass 1: Data Symbol Table ─────────────────────────────────────────────
    cout << "Pass 1: Building Data Symbol Table...\n";
    pass1(sourceLines, ctx);

    // ── Pass 2a: Mnemonic Validation ──────────────────────────────────────────
    cout << "Pass 2a: Validating instruction mnemonics...\n";
    pass2a(sourceLines, ctx);

    if (ctx.hadError) {
        cerr << "\nERRORS found — assembly aborted after Pass 2a.\n";
        writeLog(base, ctx);
        return 2;
    }

    // ── Pass 2b: Instruction Label Symbol Table ────────────────────────────────
    cout << "Pass 2b: Building Instruction Label Symbol Table...\n";
    pass2b(sourceLines, ctx);

    // ── Pass 3: Operand Validation ────────────────────────────────────────────
    cout << "Pass 3: Validating operands...\n";
    pass3(sourceLines, ctx);

    if (ctx.hadError) {
        cerr << "\nERRORS found — assembly aborted after Pass 3.\n";
        writeLog(base, ctx);
        return 3;
    }

    // ── Pass 4: Machine Code Generation ──────────────────────────────────────
    cout << "Pass 4: Generating machine code...\n";
    pass4(sourceLines, base, ctx);

    // ── Write log and display symbol tables ───────────────────────────────────
    writeLog(base, ctx);

    cout << "\nAssembly successful.\n"
         << "Output : " << base << ".hlx\n"
         << "Listing: " << base << ".lst\n"
         << "Log    : " << base << ".log\n";

    printSymbolTables(ctx);
    return 0;
}
