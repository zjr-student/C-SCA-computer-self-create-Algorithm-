# MAI Language — v2.0

**MAI (Math Abstract Intelligence)** is a fully encoded mathematical language designed as the underlying representation layer for the **C-SCA** project.

---

## Overview

MAI maps every programming and mathematical concept—keywords, operators, variables, constants, functions—to **fixed-length numeric codes**.

This design enables extremely simple decoding and is optimized for AI-driven structure discovery, not for human readability.

> **Philosophy:** No exceptions. Everything is encoded.

---

## Design Principles

### 1. Everything Is Encoded
- All elements (variables, constants, operators, functions, keywords) are mapped to numeric codes.
- **No "keep as-is" exceptions.**

### 2. Fixed‑Length Encoding
- Every code is exactly **2 digits** (00–99).
- The decoder always reads **exactly 2 digits** per token.

### 3. Table‑Driven Decoding
- Decoding logic:
  1. Read 2 digits
  2. Look up the table
  3. Output the corresponding symbol
- **No branching, no type inference, no special cases.**

---

## Encoding Table (v2.0)

| Code | Meaning | Category |
|------|---------|----------|
| 00   | 0       | Constant |
| 01   | 1       | Constant |
| 02   | 2       | Constant |
| 03   | 3       | Constant |
| 04   | 4       | Constant |
| 05   | 5       | Constant |
| 06   | 6       | Constant |
| 07   | 7       | Constant |
| 08   | 8       | Constant |
| 09   | 9       | Constant |
| 10   | +       | Arithmetic |
| 11   | -       | Arithmetic |
| 12   | *       | Arithmetic |
| 13   | /       | Arithmetic |
| 14   | %       | Arithmetic |
| 15   | =       | Assignment |
| 16   | sqrt    | Math |
| 17   | sin     | Math |
| 18   | cos     | Math |
| 19   | log     | Math |
| 20   | exp     | Math |
| 21   | abs     | Math |
| 22   | floor   | Math |
| 23   | ceil    | Math |
| 24   | tan     | Math |
| 25   | asin    | Math |
| 26   | acos    | Math |
| 27   | atan    | Math |
| 28   | sinh    | Math |
| 29   | cosh    | Math |
| 30   | tanh    | Math |
| 31   | gamma   | Math |
| 32   | erf     | Math |
| 33   | factorial | Math |
| 34   | gcd     | Math |
| 35   | lcm     | Math |
| 36   | rand    | Math |
| 37   | randint | Math |
| 38   | pi      | Constant |
| 39   | e       | Constant |
| 40   | ^       | Operator |
| 41   | !=      | Logic |
| 42   | ==      | Logic |
| 43   | <       | Logic |
| 44   | >       | Logic |
| 45   | <=      | Logic |
| 46   | >=      | Logic |
| 47   | and     | Logic |
| 48   | or      | Logic |
| 49   | not     | Logic |
| 50   | if      | Control |
| 51   | else    | Control |
| 52   | for     | Control |
| 53   | while   | Control |
| 54   | break   | Control |
| 55   | continue| Control |
| 56   | def     | Control |
| 57   | return  | Control |
| 58   | (       | Delimiter |
| 59   | )       | Delimiter |
| 60   | ,       | Delimiter |
| 61   | ;       | Delimiter |
| 62   | True    | Constant |
| 63   | False   | Constant |
| 64   | None    | Constant |
| 65   | in      | Operator |
| 66   | A       | Variable seed |
| 67   | B       | Variable seed |
| 68   | C       | Variable seed |
| 69   | D       | Variable seed |
| 70   | E       | Variable seed |
| 71–99| Reserved| Future use |

---

## Variable Encoding

Variables are **not** assigned a single code. Instead, they are composed from the **5 variable seeds** (66–70):

| Seed | Meaning |
|------|---------|
| 66   | A       |
| 67   | B       |
| 68   | C       |
| 69   | D       |
| 70   | E       |

A variable name is formed by **combining seeds**:

| Human Readable | MAI Encoding |
|----------------|---------------|
| A              | 66            |
| B              | 67            |
| AA             | 66 66         |
| AB             | 66 67         |
| ABA            | 66 67 66      |
| ...            | ...           |

> **No digits are used in variable names.**  
> Variables are pure combinations of the 5 seed codes.

---

## Decoder Logic

The decoder is **extremely simple**:

