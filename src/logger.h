#pragma once
// =============================================================================
// logger.h — Log writer and symbol table display
// Writes the .log file and prints DST/ILST to stdout after assembly.
// =============================================================================

#include "context.h"
#include <string>

// Write ctx.errorLog / ctx.warnLog to <base>.log.
// If both are empty, writes "Assembly completed with NO errors."
void writeLog(const std::string& base, const AssemblerContext& ctx);

// Print the Data Symbol Table and Instruction Label Table to stdout.
void printSymbolTables(const AssemblerContext& ctx);
