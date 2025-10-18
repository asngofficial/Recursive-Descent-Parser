#!/bin/bash

# --- Configuration ---
# Assuming your compiled C/C++ program is named 'parser'
PARSER_EXEC="./parser"
INPUT_FILE="front.in.txt"
TEST_CASES_FILE="test_cases.txt"
TOTAL_TESTS=0
PASSED_TESTS=0

echo "--- Starting Parser Test Suite ---"
echo ""

# Check if the parser executable exists
if [ ! -f "$PARSER_EXEC" ]; then
    echo "Error: Parser executable '$PARSER_EXEC' not found."
    echo "Please ensure you have compiled parser.c and named the output file 'parser'."
    exit 1
fi

# Read test cases line by line
while IFS= read -r line; do
    # Skip comments and empty lines
    if [[ "$line" =~ ^# || -z "$line" ]]; then
        echo "$line"
        continue
    fi

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # 1. Write the test expression to the input file that the C program reads
    echo -n "$line" > "$INPUT_FILE"

    echo "--- Test $TOTAL_TESTS: Expression: [$line]"
    
    # 2. Run the parser
    # We redirect output to the console for real-time visualization of the parse tree steps
    $PARSER_EXEC
    
    # Check if the parser completed successfully (based on exit code 0)
    if [ $? -eq 0 ]; then
        echo "RESULT: PASSED (Successful parse tree traversal)."
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo "RESULT: FAILED (Non-zero exit code or error during parsing)."
    fi
    
    echo ""

done < "$TEST_CASES_FILE"

# --- Summary ---
echo "--- Test Summary ---"
echo "Total Tests Run: $TOTAL_TESTS"
echo "Tests Passed:    $PASSED_TESTS"
echo "Tests Failed:    $((TOTAL_TESTS - PASSED_TESTS))"
echo "--------------------"

# Clean up input file
rm -f "$INPUT_FILE"
