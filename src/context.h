#pragma once
// =============================================================================
// context.h — AssemblerContext
// Central state object passed by reference through every pass.
// Replaces all global variables; enables clean unit-testing per pass.
// =============================================================================

#include "types.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>

struct AssemblerContext {
    // ── Instruction Opcode Table (IOT) — populated by loadConfig ─────────────
    std::map<std::string, InstrInfo>  instructionTable;

    // ── Instruction Label Symbol Table (ILST) — populated by pass2b ──────────
    std::map<std::string, int>        labelTable;

    // ── Data Symbol Table (DST) — populated by pass1 ─────────────────────────
    std::map<std::string, DataSymbol> dataSymbolTable;

    // ── .ALLOC state ──────────────────────────────────────────────────────────
    int  allocCount = 0;
    bool allocFound = false;

    // ── Error / warning accumulation ─────────────────────────────────────────
    std::vector<std::string> errorLog;
    std::vector<std::string> warnLog;
    std::set<std::string>    indexedBlocks;
    bool hadError = false;

    // ── Logging helpers ───────────────────────────────────────────────────────

    void logError(int lineNo, const std::string& msg) {
        std::string entry = "  LINE " + std::to_string(lineNo) + ": ERROR - " + msg;
        std::cerr << entry << "\n";
        errorLog.push_back(entry);
        hadError = true;
    }

    void logWarning(int lineNo, const std::string& msg) {
        std::string entry = (lineNo > 0)
            ? "  LINE " + std::to_string(lineNo) + ": WARNING - " + msg
            : "  WARNING - " + msg;
        std::cerr << entry << "\n";
        warnLog.push_back(entry);
    }


    void logErrorWithPointer(int lineNo, const std::string& msg,
                             const std::string& sourceLine,
                             const std::string& token) {
        std::string entry = "  LINE " + std::to_string(lineNo) + ": ERROR - " + msg;
        std::cerr << entry << "\n";
        size_t pos = sourceLine.find(token);
        if (pos != std::string::npos) {
            std::cerr << "         " << sourceLine << "\n";
            std::cerr << "         " << std::string(pos, ' ') << "^\n";
        }
        errorLog.push_back(entry);
        hadError = true;
    }

    // Wipe all state — useful for unit tests that reuse a context object.
    void reset() {
        instructionTable.clear();
        labelTable.clear();
        dataSymbolTable.clear();
        allocCount = 0;
        allocFound = false;
        errorLog.clear();
        warnLog.clear();
        indexedBlocks.clear();
        hadError = false;
    }
};
