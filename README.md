# EEP0 Disassembler

[![Run on Repl.it](https://repl.it/badge/github/samuelpswang/eep0-disassembler)](https://repl.it/github/samuelpswang/eep0-disassembler)

**TRY IT: 1) Click Replit badge; 2) Click RUN button in Replit.**

## Intro
Disassembler for 16-bit EEP0 CPU designed by ([Professer Clarke at Imperial EEE Department](https://www.imperial.ac.uk/people/t.clarke)).

## Command Line Interface
### Direct Instructions
Type in your machine code directly and get the assembly code. See below for example.
```
> 0x33EE
ADD R0, #238

> 0b1011100001101101
STR R2, [#109]

> 0x
Input Error: input is not valid machine code
```

### Input File
Type `input` for program to read your machine code from `input.txt`, the assembly code will be in `output.txt`. Remember to create an `input.txt` beforehand otherwise you will get `File Not Found Error`. See below for example.
```
> input
Done: output.txt written
```

## Generate All Possible Commands
1. Comment all code under `// Instructions` & `// Command Line` comments.
2. Uncommment all code under `// For Verification Purposes Only`.
3. Compile & run `./main`.

## Release
* `Version: 1.0.0`
* `Date: 2022/02/01`
