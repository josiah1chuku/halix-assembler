#pragma once
// =============================================================================
// pass0.h — Pass 0: Directive Validation
// Validates .ALLOC / .BEGIN / .END structure before any symbol work begins.
// Populates ctx.allocFound and ctx.allocCount.
// Logs errors for: unknown directives, missing .ALLOC args, wrong ordering,
// double .BEGIN, .END without .BEGIN, unclosed .BEGIN block.
// =============================================================================

#include "context.h"
#include <string>
#include <vector>

void pass0(const std::vector<std::string>& sourceLines, AssemblerContext& ctx);
