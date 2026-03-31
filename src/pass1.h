#pragma once
// =============================================================================
// pass1.h — Pass 1: Data Symbol Table Construction
// Scans the DATA section and populates ctx.dataSymbolTable.
// Each variable gets a sequential address starting at 0.
// Validates: identifier format, duplicates, overflow vs .ALLOC, initializer.
// =============================================================================

#include "context.h"
#include <string>
#include <vector>

void pass1(const std::vector<std::string>& sourceLines, AssemblerContext& ctx);
