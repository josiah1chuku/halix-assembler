// =============================================================================
// pass4.cpp — Pass 4: Machine Code Generation
// =============================================================================

#include "pass4.h"
#include "utils.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

using std::string;
using std::vector;
using std::ofstream;

// ---------------------------------------------------------------------------
// Encode one machine word: opcode * 100 + address, zero-padded to 4 digits.
static string encodeMachineWord(int opcode, int addr) {
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << (opcode * 100 + addr);
    return oss.str();
}

// ---------------------------------------------------------------------------
// Resolve an operand token to its integer value / address.
// Immediates: strip '=' and parse.
// Variables:  look up in DST.
// Labels:     look up in ILST.
// Returns false only if the token is completely unresolvable (should have
// been caught by pass3 — addr stays 0 as a safe fallback).
static bool resolveOperand(const string&           operand,
                           const AssemblerContext&  ctx,
                           int&                     resolvedAddr)
{
    if (!operand.empty() && operand[0] == '=') {
        int val;
        if (!parseInt(operand.substr(1), val)) return false;
        resolvedAddr = val;
        return true;
    }
    if (ctx.dataSymbolTable.count(operand)) {
        resolvedAddr = ctx.dataSymbolTable.at(operand).address;
        return true;
    }
    if (ctx.labelTable.count(operand)) {
        resolvedAddr = ctx.labelTable.at(operand);
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
void pass4(const vector<string>&   sourceLines,
           const string&           base,
           const AssemblerContext& ctx)
{
    ofstream out(base + ".hlx");
    ofstream lst(base + ".lst");

    // ── Sort DST entries by address for deterministic output ─────────────────
    using DstEntry = std::pair<int, std::pair<string, DataSymbol>>;
    vector<DstEntry> sortedData;
    for (const auto& kv : ctx.dataSymbolTable)
        sortedData.push_back({kv.second.address, {kv.first, kv.second}});
    std::sort(sortedData.begin(), sortedData.end(),
        [](const DstEntry& a, const DstEntry& b) {
            return a.first < b.first;
        });

    // ── Listing: data section ─────────────────────────────────────────────────
    lst << "===== HALIX ASSEMBLER LISTING =====\n\n--- DATA SECTION ---\n";
    lst << std::left << std::setw(8) << "Addr"
                     << std::setw(14) << "Name" << "Value\n"
        << string(40, '-') << "\n";
    for (const auto& entry : sortedData) {
        const string&     name = entry.second.first;
        const DataSymbol& ds   = entry.second.second;
        lst << std::setw(8)  << ds.address
            << std::setw(14) << name
            << (ds.hasValue ? std::to_string(ds.value) : "(uninit)") << "\n";
    }

    // ── Collect data values (uninit → UNINIT_SENTINEL) ────────────────────────
    vector<int> dataVals;
    for (const auto& entry : sortedData)
        dataVals.push_back(entry.second.second.hasValue
                           ? entry.second.second.value
                           : UNINIT_SENTINEL);

    // ── Count valid instructions (needed for codeSize header line) ────────────
    int  codeSize = 0;
    bool counting = false;
    for (const string& raw : sourceLines) {
        auto toks = tokenize(stripComment(raw));
        if (toks.empty()) continue;
        if (toks[0] == ".BEGIN") { counting = true;  continue; }
        if (toks[0] == ".END")   { counting = false; continue; }
        if (!counting) continue;
        string mn = (toks[0].back() == ':')
                    ? (toks.size() > 1 ? toks[1] : "")
                    : toks[0];
        if (ctx.instructionTable.count(mn)) codeSize++;
    }

    // ── Write .hlx code section ───────────────────────────────────────────────
    out << codeSize << "\n";

    lst << "\n--- CODE SECTION ---\n";
    lst << std::left << std::setw(6) << "Line"
                     << std::setw(6) << "Addr"
                     << std::setw(8) << "Machine" << "Source\n"
        << string(55, '-') << "\n";

    bool inCode  = false;
    int  address = 0;
    int  lineNo  = 0;

    for (const string& raw : sourceLines) {
        lineNo++;
        string line   = stripComment(raw);
        auto   tokens = tokenize(line);
        if (tokens.empty()) continue;

        if (tokens[0] == ".BEGIN") { inCode = true;  continue; }
        if (tokens[0] == ".END")   { inCode = false; continue; }
        if (!inCode) continue;

        string mnemonic;
        int    opStart;
        if (tokens[0].back() == ':') {
            if (tokens.size() < 2) continue;
            mnemonic = tokens[1];
            opStart  = 2;
        } else {
            mnemonic = tokens[0];
            opStart  = 1;
        }
        if (!ctx.instructionTable.count(mnemonic)) continue;

        int opcode = ctx.instructionTable.at(mnemonic).opcode;
        int addr   = 0;
        if (static_cast<int>(tokens.size()) > opStart)
            resolveOperand(tokens[opStart], ctx, addr);

        string machOut = encodeMachineWord(opcode, addr);
        out << machOut << "\n";
        lst << std::setw(6) << lineNo
            << std::setw(6) << address
            << std::setw(8) << machOut
            << raw << "\n";
        address++;
    }

    // ── Write .hlx data section ───────────────────────────────────────────────
    lst << "\n===== END OF LISTING =====\n";
    out << dataVals.size() << "\n";
    for (int v : dataVals) {
        if (v < 0)
            out << "-" << std::setw(3) << std::setfill('0') << -v << "\n";
        else
            out << std::setw(4) << std::setfill('0') << v << "\n";
    }
}
