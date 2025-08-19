# --- Imports --- #
import os
import sys
import json
import argparse
import subprocess

from collections import Counter

# --- Constants --- #
ANALYSIS_C_BIN_PATH = "./../build/analysis"
INPUT_JSON_FILE = "./data_sample.json"

def _parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-f",
        "--input_file",
        help="Input file (.json)",
        type=str,
        default=INPUT_JSON_FILE
    )

    return parser.parse_args()

def main():
    args = _parse_args()

    if not os.path.isfile(ANALYSIS_C_BIN_PATH):
        print(f"Missing binary file: {os.path.basename(ANALYSIS_C_BIN_PATH)}")
        print("Build the project from the root directory using cmake")
        return 1

    with open(args.input_file, 'r') as file:
        test_data = json.load(file)

    model_counts = Counter(item['model'] for item in test_data)

    result = subprocess.run([ANALYSIS_C_BIN_PATH, '-i', args.input_file],
        capture_output=True, text=True)
    counter_dict = {}
    for line in result.stdout.split('\n')[2:-2]:
        key, value = line.split(':')
        counter_dict[key.strip()] = int(value.strip())

    model_counts_c = Counter(counter_dict)
    all_keys = set(model_counts.keys()) | set(model_counts_c.keys())
    # Compute and display differences for both of the outputs
    differences = {}
    for key in all_keys:
        diff = model_counts.get(key, 0) - model_counts_c.get(key, 0)
        if diff != 0:
            differences[key] = diff

    print(differences)

    return 0

if __name__ == '__main__':
    sys.exit(main())
