# suites/pass0.sh — Pass 0: Directive Validation
should_run "P0" || return 0
echo ""; echo -e "${CYAN}--- Pass 0: Directive Validation ---${RESET}"

run_test "TC-P0-01" "p0/tc_p0_01.hal" 0 "NO errors"
run_test "TC-P0-02" "p0/tc_p0_02.hal" 2 "Unknown directive"
run_test "TC-P0-03" "p0/tc_p0_03.hal" 2 "missing size argument"
run_test "TC-P0-04" "p0/tc_p0_04.hal" 2 "not an integer"
run_test "TC-P0-05" "p0/tc_p0_05.hal" 2 "before .ALLOC"
run_test "TC-P0-06" "p0/tc_p0_06.hal" 2 "Second .BEGIN"
run_test "TC-P0-07" "p0/tc_p0_07.hal" 2 "without matching .BEGIN"
run_test "TC-P0-08" "p0/tc_p0_08.hal" 2 "Missing .END"
run_test "TC-P0-09" "p0/tc_p0_09.hal" 2 "before .ALLOC"
