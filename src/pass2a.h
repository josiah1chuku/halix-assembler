#pragma once
// =============================================================================
// pass2a.h — Pass 2a: Mnemonic Validation
// Checks every instruction token inside .BEGIN/.END against the IOT.
// If any unknown mnemonic is found, ctx.hadError is set and the caller
// must abort — no .hlx should be produced for invalid mnemonics.
// =============================================================================

#include "context.h"
#include <string>
#include <vector>

void pass2a(const std::vector<std::string>& sourceLines, AssemblerContext& ctx);
