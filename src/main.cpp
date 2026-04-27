// =============================================================================
// main.cpp -- HALIX Assembler v1.5.0  (modular build)
// Entry point: parses CLI, loads config, runs all five passes in order.
//
// Exit codes:
//   0  success -- .hlx, .hll, .log produced
//   1  usage error or file I/O failure
//   2  errors detected in passes 0-2a (no .hlx produced)
//   3  errors detected in pass 3     (no .hlx produced)
// =============================================================================

#include "config.h"
#include "context.h"
#include "logger.h"
#include "pass0.h"
#include "pass1.h"
#include "pass2a.h"
#include "pass2b.h"
#include "pass3.h"
#include "pass4.h"
#include "utils.h"

#include <iostream>

using std::cerr;
using std::cout;
using std::string;

// =============================================================================
// printHelp -- short usage summary (--help)
// =============================================================================
static void printHelp() {
    cout <<
"Usage:\n"
"  ./halixAssembler <source.hal> [halix.opcode]\n"
"  ./halixAssembler --help\n"
"  ./halixAssembler --man\n"
"  ./halixAssembler --version\n"
"\n"
"Arguments:\n"
"  <source.hal>     Halix Assembly Language source file to assemble.\n"
"  [halix.opcode]   Optional opcode config file (default: halix.opcode).\n"
"\n"
"Output files (written to current directory):\n"
"  <base>.hlx       Machine code -- run with halix.crun\n"
"  <base>.hll       Assembly listing with addresses and machine words\n"
"  <base>.log       Error and warning log\n"
"\n"
"Exit codes:\n"
"  0   Assembly successful\n"
"  1   Usage error or file I/O failure\n"
"  2   Errors in passes 0-2a (no .hlx produced)\n"
"  3   Errors in pass 3     (no .hlx produced)\n"
"\n"
"Quick Start:\n"
"  1. Write a .hal file (see --man for format)\n"
"  2. Assemble it:    ./halixAssembler myprogram.hal\n"
"  3. Run it:         ./halix.crun myprogram.hlx\n"
"\n"
"Common Directives:\n"
"  .ALLOC N         Reserve N data memory cells (must come first)\n"
"  name .DATA       Declare a variable (uninitialised)\n"
"  name .DATA =val  Declare and initialise a variable\n"
"  name .BLOCK N    Declare an array of N consecutive cells\n"
"  .BEGIN           Start of code section\n"
"  .END             End of code section\n"
"\n"
"Common Instructions:\n"
"  READ  var        Read integer input into variable\n"
"  WRITE var        Print variable to screen\n"
"  LOAD  var        Load variable into accumulator (ACC)\n"
"  STORE var        Store ACC into variable\n"
"  ADD   var        ACC = ACC + var\n"
"  SUB   var        ACC = ACC - var\n"
"  MULT  var        ACC = ACC * var\n"
"  HALT             Stop program execution\n"
"\n"
"Examples:\n"
"  ./halixAssembler tests/first.hal\n"
"  ./halixAssembler tests/first.hal halix.opcode\n"
"\n"
"Run --man for full documentation and tutorial.\n";
}

// =============================================================================
// printMan -- full manual page (--man)
// =============================================================================
static void printMan() {
    cout <<
"===============================================================================\n"
"HALIX ASSEMBLER -- Manual Page\n"
"Version 4.0  |  CEN5075 Systems Software Engineering\n"
"===============================================================================\n"
"\n"
"NAME\n"
"    halixAssembler -- translates Halix Assembly Language (.hal) to\n"
"                     Halix machine code (.hlx)\n"
"\n"
"SYNOPSIS\n"
"    ./halixAssembler <source.hal> [halix.opcode]\n"
"    ./halixAssembler --help\n"
"    ./halixAssembler --man\n"
"    ./halixAssembler --version\n"
"\n"
"DESCRIPTION\n"
"    halixAssembler is a five-pass assembler for the Halix v25 instruction\n"
"    set. It reads a .hal source file, validates directives, builds symbol\n"
"    tables, validates operands, and generates .hlx machine code compatible\n"
"    with the halix.crun runtime.\n"
"\n"
"    The assembler is architecturally superior to the legacy HASM.csh:\n"
"      - In-memory symbol tables (vs disk-based temp files)\n"
"      - Data-driven opcode table loaded from halix.opcode (vs hardcoded)\n"
"      - Proper exit codes for each error class\n"
"      - Pointer-style error messages with ^ indicator\n"
"      - Unused variable warnings\n"
"      - Detailed error messages with line numbers\n"
"\n"
"OPTIONS\n"
"    <source.hal>\n"
"        Required. Path to the Halix Assembly Language source file.\n"
"\n"
"    [halix.opcode]\n"
"        Optional. Path to the opcode configuration file.\n"
"        Defaults to halix.opcode in the current directory.\n"
"        Format per line:  MNEMONIC  OPCODE  OPERAND_TYPE\n"
"        OPERAND_TYPE is one of: NONE, IMM, LABEL, DATA\n"
"\n"
"    --help\n"
"        Print a short usage summary and exit.\n"
"\n"
"    --man\n"
"        Print this full manual page and exit.\n"
"\n"
"    --version\n"
"        Print the assembler version and exit.\n"
"\n"
"TUTORIAL -- Writing Your First Halix Program\n"
"\n"
"    Step 1: Create a .hal file\n"
"\n"
"        # myfirst.hal -- adds two numbers and prints the result\n"
"        .ALLOC 3\n"
"        a    .DATA =5\n"
"        b    .DATA =3\n"
"        sum  .DATA\n"
"        .BEGIN\n"
"          LOAD  a\n"
"          ADD   b\n"
"          STORE sum\n"
"          WRITE sum\n"
"          HALT\n"
"        .END\n"
"\n"
"    Step 2: Assemble it\n"
"        ./halixAssembler myfirst.hal\n"
"\n"
"    Step 3: Run it\n"
"        ./halix.crun myfirst.hlx\n"
"\n"
"    Expected output: 8\n"
"\n"
"    Step 4: Read user input\n"
"\n"
"        # cube.hal -- reads a number and prints its cube\n"
"        .ALLOC 2\n"
"        num    .DATA\n"
"        result .DATA\n"
"        .BEGIN\n"
"          READ  num\n"
"          LOAD  num\n"
"          MULT  num\n"
"          MULT  num\n"
"          STORE result\n"
"          WRITE result\n"
"          HALT\n"
"        .END\n"
"\n"
"    Step 5: Use branching\n"
"\n"
"        # positive.hal -- prints 1 if input > 0, else prints 0\n"
"        .ALLOC 2\n"
"        num  .DATA\n"
"        zero .DATA =0\n"
"        .BEGIN\n"
"          READ  num\n"
"          LOAD  num\n"
"          CGT   zero\n"
"          BRF   NOTPOS\n"
"          SETI  =1\n"
"          SHACC\n"
"          BRANCH END\n"
"NOTPOS:   SETI  =0\n"
"          SHACC\n"
"END:      HALT\n"
"        .END\n"
"\n"
"    Step 6: Use arrays with .BLOCK\n"
"\n"
"        # array.hal -- stores 10,20,30 in an array\n"
"        .ALLOC 4\n"
"        x    .DATA =10\n"
"        arr  .BLOCK 3\n"
"        .BEGIN\n"
"          LOADA arr\n"
"          iCLR\n"
"          SETI  =10\n"
"          iSTORE\n"
"          iINCR\n"
"          SETI  =20\n"
"          iSTORE\n"
"          HALT\n"
"        .END\n"
"\n"
"HAL SOURCE FILE FORMAT\n"
"    A .hal source file consists of:\n"
"\n"
"    DIRECTIVES (data section):\n"
"        .ALLOC N          Reserve N data memory cells (must come first)\n"
"        name .DATA        Declare uninitialised variable\n"
"        name .DATA =val   Declare variable with initial value\n"
"        name .BLOCK N     Declare array of N consecutive cells\n"
"\n"
"    CODE SECTION:\n"
"        .BEGIN            Mark start of instruction section\n"
"        .END              Mark end of instruction section\n"
"\n"
"    INSTRUCTIONS:\n"
"        [label:] mnemonic [operand]\n"
"        Labels must end with ':'\n"
"        Immediate operands use '=' prefix: SETI =10\n"
"\n"
"    COMMENTS:\n"
"        Anything after '#' on a line is ignored\n"
"\n"
"FIVE-PASS PIPELINE\n"
"    Pass 0   Directive validation -- checks .ALLOC, .BEGIN, .END, .DATA, .BLOCK\n"
"    Pass 1   Data Symbol Table -- assigns DM addresses to all variables\n"
"    Pass 2a  Mnemonic validation -- checks all instruction names against IOT\n"
"    Pass 2b  Label Symbol Table -- assigns IM addresses to all labels\n"
"    Pass 3   Operand validation -- checks all operands against DST and ILST\n"
"    Pass 4   Machine code generation -- writes .hlx, .hll, .log files\n"
"\n"
"OUTPUT FILES\n"
"    <base>.hlx\n"
"        Machine code file for halix.crun. Format:\n"
"          Line 1:     number of instructions\n"
"          Lines 2..N: 4-digit machine words (opcode*100 + address)\n"
"          Line N+1:   number of data cells\n"
"          Lines N+2+: 4-digit data values (uninit = 9999)\n"
"\n"
"    <base>.hll\n"
"        Assembly listing showing source lines alongside machine words\n"
"        and addresses. Includes data and instruction symbol tables.\n"
"\n"
"    <base>.log\n"
"        Error and warning log. On success: 'Assembly completed with NO errors.'\n"
"        On failure: lists each error with line number and ^ pointer.\n"
"\n"
"EXIT CODES\n"
"    0   Assembly successful -- .hlx, .hll, .log all produced\n"
"    1   Usage error or file I/O failure -- check file path\n"
"    2   Errors in passes 0-2a -- directive or mnemonic errors\n"
"    3   Errors in pass 3 -- operand errors (undefined variable or label)\n"
"\n"
"COMMON ERRORS AND FIXES\n"
"    ERROR: Unknown directive\n"
"        Check spelling of .ALLOC, .DATA, .BLOCK, .BEGIN, .END\n"
"\n"
"    ERROR: .DATA declared before .ALLOC\n"
"        Move .ALLOC to the very first line of your file\n"
"\n"
"    ERROR: Invalid variable name\n"
"        Variable names must start with a letter or underscore\n"
"\n"
"    ERROR: Undefined variable\n"
"        Check that the variable is declared in the data section\n"
"\n"
"    ERROR: Unknown instruction mnemonic\n"
"        Check spelling -- mnemonics are case-sensitive (READ not read)\n"
"\n"
"    WARNING: variable declared but never used in code\n"
"        You declared a variable but never referenced it -- safe to ignore\n"
"        or remove the unused variable\n"
"\n"
"EXAMPLES\n"
"    Assemble a simple program:\n"
"        ./halixAssembler tests/first.hal\n"
"\n"
"    Assemble with a custom opcode file:\n"
"        ./halixAssembler tests/first.hal halix.opcode\n"
"\n"
"    Check output:\n"
"        cat first.hlx\n"
"        cat first.hll\n"
"        cat first.log\n"
"\n"
"    Run the test suite:\n"
"        bash tests/run_all_tests.sh\n"
"\n"
"    Run a single pass group:\n"
"        bash tests/run_all_tests.sh --pass P0\n"
"\n"
"INSTRUCTION SET\n"
"    Supports all 61 Halix v25 instructions loaded from halix.opcode.\n"
"    Key instruction groups:\n"
"      01-09   Memory and arithmetic (READ, WRITE, LOAD, STORE, ADD, SUB, MULT, DIV, MOD)\n"
"      10-12   Branching (BRANCH, BRT, BRF)\n"
"      13-14   Accumulator control (CLEAR, SET)\n"
"      15-17   Data operations (DOUBLE, INCR, DECR)\n"
"      18-23   Comparison (CLT, CLE, CEQ, CNE, CGE, CGT)\n"
"      24-29   Immediate arithmetic (SETI, ADDI, SUBI, MULTI, DIVI, MODI)\n"
"      30-33   Power, display, subroutine (POW, SHACC, BSUB, RET)\n"
"      34-54   Base-index addressing (LOADA, iREAD, iLOAD, iSTORE, iINCR...)\n"
"      99      HALT\n"
"\n"
"ADDING NEW INSTRUCTIONS\n"
"    No recompile needed. Edit halix.opcode and add a line:\n"
"        MNEMONIC  OPCODE  OPERAND_TYPE\n"
"    Example:\n"
"        SWAP  55  DATA\n"
"\n"
"VERSION HISTORY\n"
"    v1.0   Initial release -- 36 tests\n"
"    v1.1   Added .BLOCK directive -- 77 tests\n"
    "    v1.2   Added --help, --man, --version; unused variable warnings\n"
    "    v1.3   .BLOCK initializer list; unindexed block warnings\n"
    "    v1.4   ANSI color output\n"
    "    v1.5   CodeQL security scanning\n"
"           pointer error messages; team authors display\n"
"\n"
"AUTHORS\n"
"    Alissa Forde, Dunkley Kimieka, Kiros Kalab M., Josiah Chuku\n"
"    Avenger Assembler -- CEN5075, FAMU, 2026\n"
"\n"
"SEE ALSO\n"
"    halix.crun(1)    Halix runtime emulator\n"
"    halix.opcode     Instruction opcode configuration file\n"
"    Halix_v12_InstructionSet.pdf\n"
"\n"
"===============================================================================\n";
}

// =============================================================================
// main
// =============================================================================
int main(int argc, char* argv[]) {

    // ── CLI flag handling ─────────────────────────────────────────────────────
    if (argc >= 2) {
        string flag = argv[1];
        if (flag == "--help" || flag == "-h") { printHelp(); return 0; }
        if (flag == "--man"  || flag == "-m") { printMan();  return 0; }
        if (flag == "--version" || flag == "-v") { cout << "HALIX Assembler v1.5.0\n" << "Authors: Alissa Forde, Dunkley Kimieka, Kiros Kalab M., Josiah Chuku\n" << "Team:    Avenger Assembler -- CEN5075, FAMU, 2026\n"; return 0; }
    }

    if (argc < 2) {
        cout << "Usage: ./halixAssembler <source.hal> [config.opcode]\n";
        cout << "Run --help for usage summary or --man for full documentation.\n";
        return 1;
    }

    const string srcFile    = argv[1];
    const string configFile = (argc >= 3) ? argv[2] : "halix.opcode";
    const string base       = getBaseName(srcFile);

    AssemblerContext ctx;

    // ── Load instruction opcode table ─────────────────────────────────────────
    if (!loadConfig(configFile, ctx)) return 1;

    // ── Read source file once -- shared across all passes ─────────────────────
    std::vector<string> sourceLines;
    if (!readSourceFile(srcFile, sourceLines)) {
        cerr << "ERROR: Cannot open source file: " << srcFile << "\n";
        return 1;
    }

    cout << "=== HALIX Assembler v1.5.0 ===\n"
         << "    Alissa Forde, Dunkley Kimieka, Kiros Kalab M., Josiah Chuku\n"
         << "    Avenger Assembler -- CEN5075, FAMU, 2026\n"
         << "Source : " << srcFile << "\n\n";

    // ── Pass 0: Directive Validation ──────────────────────────────────────────
    cout << Color::CYAN << "Pass 0: Validating directives..." << Color::RESET << "\n";
    pass0(sourceLines, ctx);

    if (!ctx.allocFound) {
        ctx.logWarning(0,
            ".ALLOC directive not found -- defaulting to 10 data cells "
            "(legacy behaviour)");
        ctx.allocCount = 10;
    }

    // ── Pass 1: Data Symbol Table ─────────────────────────────────────────────
    cout << Color::CYAN << "Pass 1: Building Data Symbol Table..." << Color::RESET << "\n";
    pass1(sourceLines, ctx);

    // ── Pass 2a: Mnemonic Validation ──────────────────────────────────────────
    cout << Color::CYAN << "Pass 2a: Validating instruction mnemonics..." << Color::RESET << "\n";
    pass2a(sourceLines, ctx);

    if (ctx.hadError) {
        cerr << "\nERRORS found -- assembly aborted after Pass 2a.\n";
        writeLog(base, ctx);
        return 2;
    }

    // ── Pass 2b: Instruction Label Symbol Table ────────────────────────────────
    cout << Color::CYAN << "Pass 2b: Building Instruction Label Symbol Table..." << Color::RESET << "\n";
    pass2b(sourceLines, ctx);

    // ── Pass 3: Operand Validation ────────────────────────────────────────────
    cout << Color::CYAN << "Pass 3: Validating operands..." << Color::RESET << "\n";
    pass3(sourceLines, ctx);

    if (ctx.hadError) {
        cerr << "\nERRORS found -- assembly aborted after Pass 3.\n";
        writeLog(base, ctx);
        return 3;
    }

    // ── Pass 4: Machine Code Generation ──────────────────────────────────────
    cout << Color::CYAN << "Pass 4: Generating machine code..." << Color::RESET << "\n";
    // ── .BLOCK variables never indexed warning ───────────────────────────────
    for (auto& kv : ctx.dataSymbolTable) {
        if (kv.second.blockSize > 1 && !ctx.indexedBlocks.count(kv.first))
            ctx.logWarning(0, ".BLOCK variable '" + kv.first + "' is never indexed with LOADA");
    }

    pass4(sourceLines, base, ctx);

    // ── Write log and display symbol tables ───────────────────────────────────
    writeLog(base, ctx);

    cout << Color::GREEN << "\nAssembly successful." << Color::RESET << "\n"
         << "Output : " << base << ".hlx\n"
         << "Listing: " << base << ".hll\n"
         << "Log    : " << base << ".log\n";

    printSymbolTables(ctx);
    return 0;
}
