#!/bin/bash
# =============================================================================
# run_all_tests.sh  —  HALIX Assembler Full Test Suite
# =============================================================================
# Modular design: each pass group is sourced from tests/suites/<pass>.sh
# Works on: Ubuntu (Colab / CI), MSYS2 UCRT64 (Windows local)
# Usage:  bash tests/run_all_tests.sh [--pass P0] [--filter PATTERN]
# =============================================================================

# ── Resolve script location so tests run from any working directory ──────────
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
ASSEMBLER="$REPO_ROOT/halixAssembler"

# ── CLI options ──────────────────────────────────────────────────────────────
FILTER_PASS=""      # --pass P0  runs only Pass 0 suite
FILTER_PATTERN=""   # --filter TC-P4  runs only tests matching pattern

while [[ $# -gt 0 ]]; do
    case "$1" in
        --pass)    FILTER_PASS="$2";    shift 2 ;;
        --filter)  FILTER_PATTERN="$2"; shift 2 ;;
        *) echo "Unknown option: $1"; exit 1 ;;
    esac
done

# ── Globals ───────────────────────────────────────────────────────────────────
PASS=0; FAIL=0; TOTAL=0
FAILED_IDS=()

# ── Colour helpers (disabled when not a TTY) ─────────────────────────────────
if [ -t 1 ]; then
    GREEN='\033[0;32m'; RED='\033[0;31m'; YELLOW='\033[1;33m'
    CYAN='\033[0;36m'; RESET='\033[0m'
else
    GREEN=''; RED=''; YELLOW=''; CYAN=''; RESET=''
fi

# =============================================================================
# Core test primitives
# =============================================================================

# run_test ID FILE EXPECTED_EXIT EXPECTED_MSG_FRAGMENT
#   Runs the assembler on FILE, checks exit code and .log content.
run_test() {
    local id="$1" file="$2" expect_exit="$3" expect_msg="$4"

    # Apply filters
    [[ -n "$FILTER_PATTERN" && "$id" != *"$FILTER_PATTERN"* ]] && return
    TOTAL=$((TOTAL + 1))

    local abs_file="$SCRIPT_DIR/$file"
    if [[ ! -f "$abs_file" ]]; then
        echo -e "  ${RED}SKIP${RESET}  $id — file not found: $file"
        FAIL=$((FAIL + 1)); FAILED_IDS+=("$id"); return
    fi

    # Run assembler from a temp dir so output files don't clutter the repo
    local tmpdir; tmpdir=$(mktemp -d)
    local basename; basename=$(basename "$file" .hal)
    cp "$abs_file" "$tmpdir/${basename}.hal"
    cp "$REPO_ROOT/halix.opcode" "$tmpdir/" 2>/dev/null || true

    pushd "$tmpdir" > /dev/null
    "$ASSEMBLER" "${basename}.hal" > /dev/null 2>&1
    local actual_exit=$?
    local actual_msg; actual_msg=$(cat "${basename}.log" 2>/dev/null)
    popd > /dev/null
    rm -rf "$tmpdir"

    local ok=1
    if [[ "$actual_exit" -ne "$expect_exit" ]]; then
        echo -e "  ${RED}FAIL${RESET}  $id — exit: expected=$expect_exit got=$actual_exit"
        echo    "         log: $actual_msg"
        ok=0
    elif [[ -n "$expect_msg" ]] && ! echo "$actual_msg" | grep -q "$expect_msg"; then
        echo -e "  ${RED}FAIL${RESET}  $id — expected msg fragment: '$expect_msg'"
        echo    "         got: $actual_msg"
        ok=0
    fi

    if [[ "$ok" -eq 1 ]]; then
        echo -e "  ${GREEN}PASS${RESET}  $id"
        PASS=$((PASS + 1))
    else
        FAIL=$((FAIL + 1)); FAILED_IDS+=("$id")
    fi
}

# run_test_file ID FILE LINE EXPECTED_LINE_CONTENT
#   Runs the assembler on FILE, checks that line LINE of the .hlx equals EXPECTED.
run_test_file() {
    local id="$1" file="$2" expect_line="$3" expect_content="$4"

    [[ -n "$FILTER_PATTERN" && "$id" != *"$FILTER_PATTERN"* ]] && return
    TOTAL=$((TOTAL + 1))

    local abs_file="$SCRIPT_DIR/$file"
    if [[ ! -f "$abs_file" ]]; then
        echo -e "  ${RED}SKIP${RESET}  $id — file not found: $file"
        FAIL=$((FAIL + 1)); FAILED_IDS+=("$id"); return
    fi

    local tmpdir; tmpdir=$(mktemp -d)
    local basename; basename=$(basename "$file" .hal)
    cp "$abs_file" "$tmpdir/${basename}.hal"
    cp "$REPO_ROOT/halix.opcode" "$tmpdir/" 2>/dev/null || true

    pushd "$tmpdir" > /dev/null
    "$ASSEMBLER" "${basename}.hal" > /dev/null 2>&1
    local actual_line; actual_line=$(sed -n "${expect_line}p" "${basename}.hlx" 2>/dev/null)
    popd > /dev/null
    rm -rf "$tmpdir"

    if [[ "$actual_line" == "$expect_content" ]]; then
        echo -e "  ${GREEN}PASS${RESET}  $id"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}FAIL${RESET}  $id — .hlx line $expect_line"
        echo    "         expected: '$expect_content'"
        echo    "         got:      '$actual_line'"
        FAIL=$((FAIL + 1)); FAILED_IDS+=("$id")
    fi
}

# run_test_no_hlx ID FILE
#   Asserts that the assembler does NOT produce a .hlx file (error cases).
run_test_no_hlx() {
    local id="$1" file="$2"

    [[ -n "$FILTER_PATTERN" && "$id" != *"$FILTER_PATTERN"* ]] && return
    TOTAL=$((TOTAL + 1))

    local abs_file="$SCRIPT_DIR/$file"
    if [[ ! -f "$abs_file" ]]; then
        echo -e "  ${RED}SKIP${RESET}  $id — file not found: $file"
        FAIL=$((FAIL + 1)); FAILED_IDS+=("$id"); return
    fi

    local tmpdir; tmpdir=$(mktemp -d)
    local basename; basename=$(basename "$file" .hal)
    cp "$abs_file" "$tmpdir/${basename}.hal"
    cp "$REPO_ROOT/halix.opcode" "$tmpdir/" 2>/dev/null || true

    pushd "$tmpdir" > /dev/null
    "$ASSEMBLER" "${basename}.hal" > /dev/null 2>&1
    local hlx_exists=0
    [[ -f "${basename}.hlx" ]] && hlx_exists=1
    popd > /dev/null
    rm -rf "$tmpdir"

    if [[ "$hlx_exists" -eq 0 ]]; then
        echo -e "  ${GREEN}PASS${RESET}  $id"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}FAIL${RESET}  $id — .hlx was produced but should NOT have been"
        FAIL=$((FAIL + 1)); FAILED_IDS+=("$id")
    fi
}

# section LABEL — print a section header, respecting --pass filter
section() {
    local label="$1" pass_tag="$2"
    [[ -n "$FILTER_PASS" && "$pass_tag" != "$FILTER_PASS" ]] && return 0
    echo ""
    echo -e "${CYAN}--- $label ---${RESET}"
    return 0  # always return 0 — callers use this as a gate
}

# should_run PASS_TAG — returns 0 (true) if this pass should run
should_run() {
    [[ -z "$FILTER_PASS" || "$FILTER_PASS" == "$1" ]]
}

# =============================================================================
# Print header
# =============================================================================
echo "========================================"
echo "  HALIX Assembler — Full Test Suite"
echo "  Assembler : $ASSEMBLER"
echo "  Test root : $SCRIPT_DIR"
echo "========================================"

# Verify assembler binary exists
if [[ ! -x "$ASSEMBLER" ]]; then
    echo -e "${RED}ERROR: Assembler not found or not executable: $ASSEMBLER${RESET}"
    echo "Compile first:  g++ -std=c++17 -o halixAssembler halixAssembler.cpp"
    exit 1
fi

# =============================================================================
# SOURCE EACH PASS SUITE
# Each suite file defines only run_test / run_test_file calls.
# =============================================================================

for suite in \
    "suites/pass0.sh" \
    "suites/pass1.sh" \
    "suites/pass2a.sh" \
    "suites/pass2b.sh" \
    "suites/pass3.sh" \
    "suites/pass4.sh" \
    "suites/integration.sh"
do
    suite_file="$SCRIPT_DIR/$suite"
    if [[ -f "$suite_file" ]]; then
        # shellcheck source=/dev/null
        source "$suite_file"
    else
        echo -e "${YELLOW}WARNING: Suite file not found: $suite_file${RESET}"
    fi
done

# =============================================================================
# Summary
# =============================================================================
echo ""
echo "========================================"
printf "  Results: %d passed, %d failed, %d total\n" $PASS $FAIL $TOTAL
echo "========================================"

if [[ ${#FAILED_IDS[@]} -gt 0 ]]; then
    echo -e "${RED}  Failed tests:${RESET}"
    for id in "${FAILED_IDS[@]}"; do
        echo "    - $id"
    done
fi

if [[ "$FAIL" -eq 0 ]]; then
    echo -e "${GREEN}  ALL TESTS PASSED${RESET}"; exit 0
else
    echo -e "${RED}  SOME TESTS FAILED${RESET}"; exit 1
fi
