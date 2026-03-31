// =============================================================================
// logger.cpp — Log writer and symbol table display
// =============================================================================

#include "logger.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

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
    // Sort DST by address for readable display
    using DstEntry = std::pair<int, std::pair<std::string, DataSymbol>>;
    std::vector<DstEntry> sorted;
    for (const auto& kv : ctx.dataSymbolTable)
        sorted.push_back({kv.second.address, {kv.first, kv.second}});
    std::sort(sorted.begin(), sorted.end(),
        [](const DstEntry& a, const DstEntry& b){ return a.first < b.first; });

    std::cout << "\n--- Data Symbol Table ---\n";
    std::cout << std::left << std::setw(6)  << "Addr"
                           << std::setw(14) << "Name" << "Value\n";
    for (const auto& entry : sorted) {
        const auto& ds = entry.second.second;
        std::cout << std::setw(6)  << ds.address
                  << std::setw(14) << entry.second.first
                  << (ds.hasValue ? std::to_string(ds.value) : "-")
                  << "\n";
    }

    // Sort label table by address
    std::vector<std::pair<int,std::string>> labels;
    for (const auto& kv : ctx.labelTable)
        labels.push_back({kv.second, kv.first});
    std::sort(labels.begin(), labels.end());

    std::cout << "\n--- Instruction Label Table ---\n";
    std::cout << std::left << std::setw(6) << "Addr" << "Label\n";
    for (const auto& entry : labels)
        std::cout << std::setw(6) << entry.first << entry.second << "\n";
}
