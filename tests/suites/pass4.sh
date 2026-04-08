# suites/pass4.sh — Pass 4: Machine Code Generation (.hlx content checks)
# .hlx legacy format: line1=codeSize, line2=first instruction, ...
should_run "P4" || return 0
echo ""; echo -e "${CYAN}--- Pass 4: Machine Code Generation ---${RESET}"

run_test      "TC-P4-01"     "p4/tc_p4_01.hal" 0 "NO errors"
run_test_file "TC-P4-01-hlx" "p4/tc_p4_01.hal" 2 "0300"

run_test      "TC-P4-02"     "p4/tc_p4_02.hal" 0 "NO errors"
run_test_file "TC-P4-02-hlx" "p4/tc_p4_02.hal" 2 "0300"

run_test      "TC-P4-03"     "p4/tc_p4_03.hal" 0 "NO errors"
run_test_file "TC-P4-03-hlx" "p4/tc_p4_03.hal" 2 "2442"

run_test "TC-P4-04" "p4/tc_p4_04.hal" 0 "warning"
run_test_file "TC-P4-04-hlx" "p4/tc_p4_04.hal" 2 "3500"

run_test      "TC-P4-05"     "p4/tc_p4_05.hal" 0 "NO errors"
run_test_file "TC-P4-05-hlx" "p4/tc_p4_05.hal" 2 "9900"

run_test "TC-P4-06" "p4/tc_p4_06.hal" 0 "warning"
run_test_file "TC-P4-06-hlx" "p4/tc_p4_06.hal" 4 "9999"

run_test      "TC-P4-07"     "p4/tc_p4_07.hal" 0 "NO errors"
run_test_file "TC-P4-07-hlx" "p4/tc_p4_07.hal" 3 "1203"

run_test      "TC-P4-08"     "p4/tc_p4_08.hal" 0 "NO errors"
run_test_file "TC-P4-08-hlx" "p4/tc_p4_08.hal" 2 "0300"
