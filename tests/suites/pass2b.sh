# suites/pass2b.sh — Pass 2b: Instruction Label Symbol Table
should_run "P2b" || return 0
echo ""; echo -e "${CYAN}--- Pass 2b: Label Symbol Table ---${RESET}"

run_test "TC-P2b-01" "p2b/tc_p2b_01.hal" 0 "NO errors"
run_test "TC-P2b-02" "p2b/tc_p2b_02.hal" 0 "NO errors"
run_test "TC-P2b-03" "p2b/tc_p2b_03.hal" 3 "Duplicate label"
run_test "TC-P2b-04" "p2b/tc_p2b_04.hal" 3 "label not found"
run_test "TC-P2b-05" "p2b/tc_p2b_05.hal" 2 "Label with no instruction"
