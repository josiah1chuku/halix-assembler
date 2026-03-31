#pragma once
// =============================================================================
// pass2b.h — Pass 2b: Instruction Label Symbol Table Construction
// Scans the CODE section and populates ctx.labelTable.
// Each "Label:" prefix is bound to the IM address of its instruction.
// Validates: identifier format, no duplicate labels.
// Must run AFTER pass2a so unknown mnemonics do not consume IM addresses.
// =============================================================================

#include "context.h"
#include <string>
#include <vector>

void pass2b(const std::vector<std::string>& sourceLines, AssemblerContext& ctx);
