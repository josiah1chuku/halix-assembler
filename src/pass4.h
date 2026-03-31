#pragma once
// =============================================================================
// pass4.h — Pass 4: Machine Code Generation
// Translates validated source into the legacy halix.crun .hlx format and
// produces an annotated .lst listing file.
//
// .hlx format (line numbers):
//   1           codeSize
//   2..N+1      4-digit machine words  (opcode*100 + addr)
//   N+2         dataSize
//   N+3..end    4-digit data values    (uninit → 9999)
// =============================================================================

#include "context.h"
#include <string>
#include <vector>

void pass4(const std::vector<std::string>& sourceLines,
           const std::string&              base,
           const AssemblerContext&         ctx);
