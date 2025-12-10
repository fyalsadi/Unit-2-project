#!/bin/bash
# Bash shell script to run tests.
# Runs the test and checks that the program output contains a line
# matching a provided text pattern (useful for menu item outputs).

echo "#####################################"
echo "       d[o_0]b ELEC2645 Bot          "
echo "#####################################"
echo "Running tests..."
echo

echo "Checking for errors..."

# User input
# Each input string is passed to the program's stdin
inputs=(
  $'5\n100\nb\n6\n'
)

# Expected patterns to search for in the program output (regex/ERE)
# Example: 'Area of circle' or '^Area of circle' or 'Sum : [0-9]+'
expected_patterns=(
  'Area of circle with radius'
)

expected=(
  'Area of circle with radius 100.00 = 31415.90'
)



failed=0

for i in "${!inputs[@]}"; do
  input="${inputs[$i]}"
  pattern="${expected_patterns[$i]}"

  # run program, capture stdout+stderr
  output=$(echo "$input" | ./main.out 2>&1)
  status=$?

  # remove CRs that can break matching (Windows line endings)
  clean=$(printf '%s' "$output" | tr -d '\r')

  if [ $status -ne 0 ]; then
    echo "Fail: crashed (exit $status) for input -> $(echo "$input" | tr '\n' ' ')"
    echo "  Raw output: $clean"
    failed=1
    continue
  fi

  # find the first line that matches the expected pattern (extended regex)
  matching_line=$(printf '%s\n' "$clean" | grep -E -m1 -- "$pattern" || true)

  if [ -n "$matching_line" ]; then

  if [ "$matching_line" == "${expected[$i]}" ]; then
    echo "Pass [Line $((i+1))]: '${expected[$i]}'"
  else
    echo "Fail [Line $((i+1))]: Got '$matching_line', Expected '${expected[$i]}'"
    failed=1
  fi
  
  else
    echo "Fail: $(echo "$input" | tr '\n' ' ')"
    echo "  No line matched pattern: $pattern"
    echo "  Raw output: $clean"
    failed=1
  fi
done

echo
if [ $failed -eq 0 ]; then
  echo "#####################################"
  echo "     d[o_0]b All tests passed :D    "
  echo "#####################################"
  exit 0
else
  echo "#####################################"
  echo "     d[o_0]b Some tests failed :(   "
  echo "#####################################"
  exit 1
fi