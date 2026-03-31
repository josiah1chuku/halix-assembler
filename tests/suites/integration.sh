# suites/integration.sh — Happy-path & error-path end-to-end tests
should_run "INT" || return 0
echo ""; echo -e "${CYAN}--- Integration: Happy Path ---${RESET}"

run_test "TC-INT-01" "integration/tc_int_01.hal" 0 "NO errors"
run_test "TC-INT-02" "integration/tc_int_02.hal" 0 "NO errors"
run_test "TC-INT-03" "integration/tc_int_03.hal" 0 "NO errors"
run_test "TC-INT-04" "integration/tc_int_04.hal" 0 "NO errors"
run_test "TC-INT-05" "integration/tc_int_05.hal" 0 "NO errors"
run_test_file "TC-INT-05-hlx" "integration/tc_int_05.hal" 4 "9900"
run_test "TC-INT-06" "integration/tc_int_06.hal" 0 "NO errors"
# .hlx: codeSize=2 (line1), LOAD(line2), HALT(line3), dataSize=2(line4),
#       init=0005(line5), uninit=9999(line6)
run_test_file "TC-INT-06-hlx" "integration/tc_int_06.hal" 6 "9999"

echo ""; echo -e "${CYAN}--- Integration: Error Path ---${RESET}"

run_test     "TC-ERR-01" "integration/tc_err_01.hal" 2 "Unknown instruction"
run_test_no_hlx "TC-ERR-01-nohlx" "integration/tc_err_01.hal"
run_test     "TC-ERR-02" "integration/tc_err_02.hal" 2 "Missing .END"
# TC-ERR-03: instructions outside .BEGIN treated as bad DATA lines → exit 2
run_test     "TC-ERR-03" "integration/tc_err_03.hal" 2 "without matching .BEGIN"
run_test     "TC-ERR-04" "integration/tc_err_04.hal" 2 "Duplicate variable"
run_test     "TC-ERR-05" "integration/tc_err_05.hal" 3 "label not found"
# TC-ERR-06: empty file → exit 0 with a warning (no crash, no .hlx)
run_test     "TC-ERR-06" "integration/tc_err_06.hal" 0 ""
