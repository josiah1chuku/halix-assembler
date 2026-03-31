#pragma once
// =============================================================================
// config.h — loadConfig declaration
// Reads halix.opcode and populates ctx.instructionTable (the IOT).
// =============================================================================

#include "context.h"
#include <string>

// Parse configFile (e.g. "halix.opcode") and fill ctx.instructionTable.
// Returns true on success; prints to cerr and returns false on any error.
bool loadConfig(const std::string& configFile, AssemblerContext& ctx);
