# Counting

```{contents}
:depth: 3
```

## Task description

As input to your program, there is a binary file containing 1 billion 32-bit integer numbers,
stored as uint32_t. The size of the file is 4,000,000,000 bytes.

*а)* count the unique numbers. For example:

```console
given input -> output
0x100 0x100 0xfff 0xfff -> 2 unique numbers
0x100 0x100 0x100 0x100 -> 1 unique number
0x100 0x100 0x800 0xfff -> 3 unique numbers
```

*b)* count how many numbers are seen ONLY once. For example:

```console
given input -> output
0x100 0x100 0xfff 0xfff -> 0 numbers seen only once
0x100 0x100 0x100 0x100 -> 0 numbers seen only once
0x100 0x100 0x800 0xfff -> 2 numbers seen only once
```

## Implementation

The program reads the contents of a binary file passed as an argument `--input_file`
and stores the number of occuarance of each 12 bit value into a frequency table `numbers`.

The `numbers` array is evaluated after the entire file has been read.

```{note}
If the file is smaller than 12 bits (2 bytes) it is discarted due to insuficient data.
Leftover bits from files with size is not multiple of 12 bits are not processed and considered
insucient data since they are not enough to form a valid 12 bit value.
```

## Tests

The test *counting/test/counting.py* generates 5 predefined patterns repeated `n`
number of times and compares the ouput from the binary `counting` with the expected values
for the input patterns.

**Test output**

```console
Test pattern: 0x123, 0x456, 0x789, 0xABC, 0xDEF, repeated: 1
C binary /home/iliya/Work/StorPool/StorPool/build/counting/counting output:
5 unique numbers
5 numbers seen only once

Test pattern: 0x123, 0x456, 0x789, 0xABC, 0xDEF, repeated: 2
C binary /home/iliya/Work/StorPool/StorPool/build/counting/counting output:
5 unique numbers
0 numbers seen only once

Test pattern: 0x123, 0x456, 0x789, 0xABC, 0xDEF, repeated: 1000
C binary /home/iliya/Work/StorPool/StorPool/build/counting/counting output:
5 unique numbers
0 numbers seen only once

Test pattern: 0x123, 0x123, 0xABC, 0xABC, 0xDEF, repeated: 1
C binary /home/iliya/Work/StorPool/StorPool/build/counting/counting output:
3 unique numbers
1 numbers seen only once

Test pattern: , repeated: 1
C binary /home/iliya/Work/StorPool/StorPool/build/counting/counting output:
0 unique numbers
0 numbers seen only once

Ok
```
