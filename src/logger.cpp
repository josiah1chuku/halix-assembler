// =============================================================================
// logger.cpp — Log writer and symbol table display
// =============================================================================

#include "logger.h"

#include <fstream>
#include <iomanip>
#include <iostream>

using std::string;

// ---------------------------------------------------------------------------
void writeLog(const string& base, const AssemblerContext& ctx) {
    std::ofstream log(base + ".log");
    int errCount = static_cast<int>(ctx.errorLog.size());
    int wrnCount = static_cast<int>(ctx.warnLog.size());

    if (errCount == 0 && wrnCount == 0) {
        log << "Assembly completed with NO errors.\n";
        return;
    }
    if (errCount > 0) {
        log << "Assembly completed with " << errCount << " error(s):\n\n";
        for (const string& e : ctx.errorLog) log << e << "\n";
    }
    if (wrnCount > 0) {
        if (errCount > 0) log << "\n";
        log << wrnCount << " warning(s):\n\n";
        for (const string& w : ctx.warnLog) log << w << "\n";
    }
}

// ---------------------------------------------------------------------------
void printSymbolTables(const AssemblerContext& ctx) {
    std::cout << "\n--- Data Symbol Table ---\n";
    std::cout << std::left << std::setw(14) << "Name"
                           << std::setw(8)  << "Addr" << "Value\n";
    for (const auto& kv : ctx.dataSymbolTable)
        std::cout << std::setw(14) << kv.first
                  << std::setw(8)  << kv.second.address
                  << (kv.second.hasValue ? std::to_string(kv.second.value) : "-")
                  << "\n";

    std::cout << "\n--- Instruction Label Table ---\n";
    std::cout << std::left << std::setw(14) << "Label" << "Addr\n";
    for (const auto& kv : ctx.labelTable)
        std::cout << std::setw(14) << kv.first << kv.second << "\n";
}
