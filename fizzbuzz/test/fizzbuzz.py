# --- Imports --- #
import os
import sys
import subprocess

# --- Constants --- #
PROJECT_ROOT = os.environ.get('STORPOOL_PROJECT_ROOT')
if PROJECT_ROOT is None:
    print("Please run the following command from the root directory: source project_configure")
    sys.exit(1)

FIZZBUZZ_C_BIN_PATH = PROJECT_ROOT + "/build/fizzbuzz/fizzbuzz"

def _string_diff(str1: str, str2: str) -> list:
    max_len = max(len(str1), len(str2))
    differences = []
    for i in range(max_len):
        c1 = str1[i] if i < len(str1) else None
        c2 = str2[i] if i < len(str2) else None
        if c1 != c2:
            print(f"\033[91m{c2}\033[0m", end='')
            differences.append((c1, c2))
        else:
            print(f"{c1}", end='')

    return differences

def main():
    fizz_buzz_pattern = ""
    for i in range(1, 101):
        if i % 15 == 0:
            fizz_buzz_pattern += "AB"
        elif i % 3 == 0:
            fizz_buzz_pattern += "A"
        elif i % 5 == 0:
            fizz_buzz_pattern += "B"
        else:
            fizz_buzz_pattern += f"{i}"

    if not os.path.isfile(FIZZBUZZ_C_BIN_PATH):
        print(f"Missing binary file: {os.path.basename(FIZZBUZZ_C_BIN_PATH)}")
        print("Build the project from the root directory using sp_project_build")
        return 1

    result = subprocess.run([FIZZBUZZ_C_BIN_PATH], capture_output=True, text=True)
    fizz_buzz_c_pattern = result.stdout.replace('\n', "")
    print(f"C binary {FIZZBUZZ_C_BIN_PATH} output:\n{fizz_buzz_c_pattern}")

    if (fizz_buzz_pattern != fizz_buzz_c_pattern):
        print("FizzBuzz pattern - mismatch")
        print(fizz_buzz_pattern)
        print(fizz_buzz_c_pattern)
        diffs = _string_diff(fizz_buzz_pattern, fizz_buzz_c_pattern)
        print("")
    else:
        print("Ok\n")

    return 0

if __name__ == '__main__':
    sys.exit(main())
