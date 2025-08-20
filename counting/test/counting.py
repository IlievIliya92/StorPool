# --- Imports --- #
import os
import sys
import subprocess

# --- Constants --- #
PROJECT_ROOT = os.environ.get('STORPOOL_PROJECT_ROOT')
if PROJECT_ROOT is None:
    print("Please run the following command from the root directory: source project_configure")
    sys.exit(1)

COUNTING_C_BIN_PATH = PROJECT_ROOT + "/build/counting/counting"

def generate_12bit_pattern_bytes(pattern, repeat_count):
    if pattern >= (1 << 12) or pattern < 0:
        raise ValueError("Pattern must be a 12-bit integer (0 to 4095)")

    bitstream = 0
    bit_len = 0
    output_bytes = bytearray()

    for _ in range(repeat_count):
        bitstream = (bitstream << 12) | pattern
        bit_len += 12

        while bit_len >= 8:
            bit_len -= 8
            byte = (bitstream >> bit_len) & 0xFF
            output_bytes.append(byte)

    if bit_len > 0:
        byte = (bitstream << (8 - bit_len)) & 0xFF
        output_bytes.append(byte)

    return output_bytes, bit_len

def concat_12bit_outputs(output1, bits1, output2, bits2):
    combined_bitstream = 0
    combined_len = 0

    # Append bits from first output
    bits_in_output1 = len(output1) * 8 if bits1 == 0 else (len(output1) - 1) * 8 + bits1
    for i in range(bits_in_output1):
        combined_bitstream = (combined_bitstream << 1) | ((output1[i // 8] >> (7 - (i % 8))) & 1)
        combined_len += 1

    # Append bits from second output
    bits_in_output2 = len(output2) * 8 if bits2 == 0 else (len(output2) - 1) * 8 + bits2
    for i in range(bits_in_output2):
        combined_bitstream = (combined_bitstream << 1) | ((output2[i // 8] >> (7 - (i % 8))) & 1)
        combined_len += 1

    # Pack back into bytes
    result = bytearray()
    bits_left = combined_len
    while bits_left >= 8:
        bits_left -= 8
        byte = (combined_bitstream >> bits_left) & 0xFF
        result.append(byte)
    if bits_left > 0:
        byte = (combined_bitstream & ((1 << bits_left) - 1)) << (8 - bits_left)
        result.append(byte)

    return result, bits_left

def main():
    output_bytes = bytearray()

    # 12-bit patterns
    patterns = [
        {
            "test_values": [0x123, 0x456, 0x789, 0xABC, 0xDEF],
            "repeat_cnt": 1,
            "expected_output": "5 unique numbers\n5 numbers seen only once\n"
        },
        {
            "test_values": [0x123, 0x456, 0x789, 0xABC, 0xDEF],
            "repeat_cnt": 2,
            "expected_output": "5 unique numbers\n0 numbers seen only once\n"
        },
        {
            "test_values": [0x123, 0x456, 0x789, 0xABC, 0xDEF],
            "repeat_cnt": 1000,
            "expected_output": "5 unique numbers\n0 numbers seen only once\n"
        },
        {
            "test_values": [0x123, 0x123, 0xABC, 0xABC, 0xDEF],
            "repeat_cnt": 1,
            "expected_output": "3 unique numbers\n1 numbers seen only once\n"
        },
        {
            "test_values": [],
            "repeat_cnt": 1,
            "expected_output": "0 unique numbers\n0 numbers seen only once\n"
        }
    ]

    if not os.path.isfile(COUNTING_C_BIN_PATH):
        print(f"Missing binary file: {os.path.basename(COUNTING_C_BIN_PATH)}")
        print("Build the project from the root directory using sp_project_build")
        return 1

    for i, pattern in enumerate(patterns):
        final_bytes = bytearray()
        leftover_bits = 0

        test_values = pattern.get('test_values')
        repeat_cnt = pattern.get('repeat_cnt')
        expected_output = pattern.get('expected_output')
        for test_value in test_values:
            out, bits = generate_12bit_pattern_bytes(test_value, repeat_cnt)
            if not final_bytes:
                final_bytes = out
                leftover_bits = bits
            else:
                final_bytes, leftover_bits = concat_12bit_outputs(final_bytes,
                    leftover_bits, out, bits)

        # Write to file
        filename = f"counting_test_pattern_{i}.bin"
        with open(filename, "wb") as f:
            f.write(final_bytes)

        result = subprocess.run([COUNTING_C_BIN_PATH, '-i', filename],
                capture_output=True, text=True)
        print(f"Test pattern: {', '.join(f'0x{x:03X}' for x in test_values)}, repeated: {repeat_cnt}")
        print(f"C binary {COUNTING_C_BIN_PATH} output:\n{result.stdout}")
        counting_c_bin_result = result.stdout
        if (counting_c_bin_result != expected_output):
            print("Error: Unexpected output")
            print(f"C binary output:\n{counting_c_bin_result}\nExpected output:\n{expected_output}")
            return 1

    print("Ok\n")

    return 0

if __name__ == '__main__':
    sys.exit(main())
