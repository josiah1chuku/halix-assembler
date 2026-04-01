#pragma once
// =============================================================================
// types.h — Shared types, constants, and POD structs
// All other modules include this; nothing here depends on anything else.
// =============================================================================

constexpr int UNINIT_SENTINEL = 9999;  // halix.crun sentinel for uninitialised data

// Operand categories — used by both config loader and pass3/pass4.
enum OperandType { OP_NONE, OP_IMM, OP_LABEL, OP_DATA };

// One entry in the Instruction Opcode Table (IOT), loaded from halix.opcode.
struct InstrInfo {
    int         opcode;
    OperandType opType;
};

// One entry in the Data Symbol Table (DST), built by pass1.
// blockSize = 1 for .DATA entries, N for .BLOCK N entries.
struct DataSymbol {
    int  address;
    int  value;
    bool hasValue;
    int  blockSize = 1;   // 1 for .DATA, N for .BLOCK N
};
