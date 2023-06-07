# E20 Assembler

A simple assembler for E20 assembly language.

## Table of Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Functionality](#functionality)


## Introduction

This repository contains a C++ implementation of an E20 Assembler. It is designed to convert E20 assembly instructions into corresponding machine code. The program operates by parsing the provided assembly file, interpreting the instructions, and generating corresponding machine code. This utility can be a crucial tool for any developer working with E20 assembly language, providing an automated method to translate their assembly code into machine language.


## Installation

1. Clone the repository: `git clone https://github.com/username/repository.git`
2. Navigate into the directory: `cd repository`

## Usage

1. Compile the program with the following command: `g++ -o asm asm.cpp`
2. Run the compiled program with an assembly file as input: `./asm inputfile.s`

## Functionality

The E20 Assembler supports the following operations:

- Add: `add`
- Subtract: `sub`
- And: `and`
- Or: `or`
- Set Less Than: `slt`
- Jump Register: `jr`
- Set Less Than Immediate: `slti`
- Load Word: `lw`
- Store Word: `sw`
- Jump Equal: `jeq`
- Add Immediate: `addi`
- Jump: `j`
- Jump And Link: `jal`
- Move Immediate: `movi`
- No Operation: `nop`
- Halt: `halt`
- Fill: `.fill`

It supports a standard set of 8 registers (`$0` - `$7`), as well as memory labels for jumps and branches.
