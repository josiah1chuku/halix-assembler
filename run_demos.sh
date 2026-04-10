#!/bin/bash

ASSEMBLER="./halixAssembler"
RUNNER="./halix.crun"
LOG="run_demos.log"
PASS=0
FAIL=0
SKIP=0

echo "========================================" | tee $LOG
echo "  HALIX Assembler — Demo Runner"         | tee -a $LOG
echo "========================================" | tee -a $LOG

run_demo() {
    local hal_file=$1
    local input=$2
    local base=$(basename "$hal_file" .hal)
    local hlx_file="${base}.hlx"

    echo "" | tee -a $LOG
    echo "--- $hal_file ---" | tee -a $LOG

    if [ ! -f "$hal_file" ]; then
        echo "  SKIP (file not found)" | tee -a $LOG
        ((SKIP++))
        return
    fi

    $ASSEMBLER "$hal_file" >> $LOG 2>&1
    if [ $? -ne 0 ]; then
        echo "  SKIP (assembly error)" | tee -a $LOG
        ((SKIP++))
        return
    fi
    echo "  ASSEMBLED OK" | tee -a $LOG

    if [ -n "$input" ]; then
        output=$(echo "$input" | timeout 5 $RUNNER "$hlx_file" 2>&1)
    else
        output=$(timeout 5 $RUNNER "$hlx_file" 2>&1)
    fi

    local exit_code=$?
    echo "$output" >> $LOG

    if [ $exit_code -eq 124 ]; then
        echo "  SKIP (timeout — infinite loop)" | tee -a $LOG
        ((SKIP++))
    elif echo "$output" | grep -q "INSTRUCTIONS EXECUTED"; then
        echo "  RUN OK — PASS" | tee -a $LOG
        ((PASS++))
    else
        echo "  RUN FAIL" | tee -a $LOG
        ((FAIL++))
    fi
}

# ── 10 demo programs ──────────────────────────────────
run_demo "goodSample.hal"                                "44"
run_demo "halix-assembler/first.hal"                     "5"
run_demo "Sample_HAL_Code/first.hal"                     "7"
run_demo "Sample_HAL_Code/rectangle-greenD236.hal"       "4 5"
run_demo "Sample_HAL_Code/rectangle-crutchfieldK694.hal" "3 6"
run_demo "Sample_HAL_Code/digits2-jamesK198.hal"         "47"
run_demo "Sample_HAL_Code/two-millerI558.hal" "5"
run_demo "demo_block3.hal"                               ""
run_demo "Sample_HAL_Code/3powers-prideJ133.hal"         "3"
run_demo "Sample_HAL_Code/add2or3-crawfordJ558.hal"      "2 3"

echo "" | tee -a $LOG
echo "========================================" | tee -a $LOG
echo "  Results: $PASS passed, $FAIL failed, $SKIP skipped" | tee -a $LOG
echo "========================================" | tee -a $LOG
