# suites/pass2a.sh — Pass 2a: Mnemonic Validation
should_run "P2a" || return 0
echo ""; echo -e "${CYAN}--- Pass 2a: Mnemonic Validation ---${RESET}"

run_test "TC-P2a-01" "p2a/tc_p2a_01.hal" 0 "NO errors"
run_test "TC-P2a-02" "p2a/tc_p2a_02.hal" 2 "Unknown instruction"
run_test "TC-P2a-03" "p2a/tc_p2a_03.hal" 2 "Unknown instruction"
run_test "TC-P2a-04" "p2a/tc_p2a_04.hal" 0 "NO errors"
run_test "TC-P2a-05" "p2a/tc_p2a_05.hal" 2 "Unknown instruction"
run_test "TC-P2a-06" "p2a/tc_p2a_06.hal" 2 "Unknown instruction"
