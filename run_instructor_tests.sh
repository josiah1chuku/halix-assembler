#!/bin/bash
PASS=0
FAIL=0
TOTAL=0

echo "========================================"
echo "  Instructor HAL Programs — Assembly Test"
echo "========================================"
echo ""

for f in ~/workspace/halix-assembler/instructor_tests/*.hal; do
    TOTAL=$((TOTAL + 1))
    name=$(basename "$f")
    result=$(./halixAssembler "$f" 2>&1 | grep -E "Assembly successful|ERROR")
    if echo "$result" | grep -q "Assembly successful"; then
        echo "  PASS  $name"
        PASS=$((PASS + 1))
    else
        echo "  FAIL  $name"
        echo "$result" | grep "ERROR" | head -2 | sed 's/^/         /'
        FAIL=$((FAIL + 1))
    fi
done

echo ""
echo "========================================"
printf "  Results: %d passed, %d failed, %d total\n" $PASS $FAIL $TOTAL
echo "========================================"
