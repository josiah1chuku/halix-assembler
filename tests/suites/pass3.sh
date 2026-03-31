# suites/pass3.sh — Pass 3: Operand Validation
should_run "P3" || return 0
echo ""; echo -e "${CYAN}--- Pass 3: Operand Validation ---${RESET}"

run_test "TC-P3-01" "p3/tc_p3_01.hal" 0 "NO errors"
run_test "TC-P3-02" "p3/tc_p3_02.hal" 0 "NO errors"
run_test "TC-P3-03" "p3/tc_p3_03.hal" 0 "NO errors"
run_test "TC-P3-04" "p3/tc_p3_04.hal" 3 "Undefined variable"
run_test "TC-P3-05" "p3/tc_p3_05.hal" 3 "label not found"
run_test "TC-P3-06" "p3/tc_p3_06.hal" 3 "requires an operand"
run_test "TC-P3-07" "p3/tc_p3_07.hal" 3 "takes no operand"
run_test "TC-P3-08" "p3/tc_p3_08.hal" 3 "immediate operand"
run_test "TC-P3-09" "p3/tc_p3_09.hal" 0 "NO errors"
run_test "TC-P3-10" "p3/tc_p3_10.hal" 0 "NO errors"
