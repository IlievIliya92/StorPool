# FizzBuzz

```{contents}
:depth: 3
```

## Task description

Write a program that outputs the numbers from 1 to 100. If the number is divisible
by 3, instead of it print “A”, if it’s divisible by 5 - “B”, if it’s divisible
by 15 - “AB”. The catch is to write it without any conditionals/ifs,
including for/while loops.

## Implementation

The program processes one number at a time generating an event `print_events_t`

```{eval-rst}
..  autocenumerator:: main.c::print_events_t
    :members:
```

The resulting `event` (bitmask) is used to invoke a function pointer from `print_handlers`
which executes the required handler.

## Tests

The test *fizzbuzz/test/fizzbuzz.py* generates the fizzbuzz pattern and compares it
with the output of the binary `fizzbuzz`.

**Test output**

```console
C binary /home/iliya/Work/StorPool/StorPool/build/fizzbuzz/fizzbuzz output:
12A4BA78AB11A1314AB1617A19BA2223AB26A2829AB3132A34BA3738AB41A4344AB4647A49BA5253AB56A5859AB6162A64BA6768AB71A7374AB7677A79BA8283AB86A8889AB9192A94BA9798AB
Ok
```
