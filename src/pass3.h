#pragma once
// =============================================================================
// pass3.h — Pass 3: Operand Validation
// Validates every instruction operand against DST / ILST / immediate format.
// Requires both pass1 (DST) and pass2b (ILST) to have completed successfully.
// =============================================================================

#include "context.h"
#include <string>
#include <vector>

void pass3(const std::vector<std::string>& sourceLines, AssemblerContext& ctx);
