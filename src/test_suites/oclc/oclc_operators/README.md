# oclc_operators

## Overview
Test suite for 
[operators](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#operators) 
OpenCL C functionality.

The goal of these tests is to verify use of all defined operators on all supported scalar and vector data types. 

Tests mostly consist of below test cases given an operator and a data type are supported:
* Zero as operand;
* Positive value as operand;
* Negative value as operand;
* Floating point value as operand;
* NaN/Inf value as operand;
* Check if operator is commutative;
* Use non-uniform buffer as operand;
* Generate random value as operand;
* Force overflow of unsigned operand;
* Mix scalar and vector operands;
* Use non-uniform vector as operand;

## Tests

### Arithmetic operators
* Status: WIP
* Goal: Verify that `+`, `-`, `*`, `/`, `%` operators are handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Pre/post operators
* Status: TODO
* Goal: Verify that `++`, `--` operators are handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Relational operators
* Status: TODO
* Goal: Verify that `>`, `<`, `>=`, `<=` operators are handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Equality operators
* Status: WIP
* Goal: Verify that `==`, `!=` operators are handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Bitwise operators
* Status: TODO
* Goal: Verify that `&`, `|`, `^`, `~` operators are handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Logical operators
* Status: TODO
* Goal: Verify that `&&`, `||` operators are handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Unary logical operator
* Status: WIP
* Goal: Verify that `! ` operator is handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Ternary selection operator
* Status: TODO
* Goal: Verify that `?:` operator is handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Shift operators
* Status: TODO
* Goal: Verify that `>>`, `<<` operators are handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Sizeof operator
* Status: TODO
* Goal: Verify that `sizeof` operator is handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Comma operator
* Status: TODO
* Goal: Verify that `,` operator is handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Indirection operator
* Status: TODO
* Goal: Verify that `*` operator is handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Address operator
* Status: TODO
* Goal: Verify that `&` operator is handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### Assignment operators
* Status: TODO
* Goal: Verify that `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `<<=`, `>>=`, `&=`, `|=`, `^=` operators 
are handled properly.
* Description: Run OpenCL C kernel that uses a given operator on requested scalar and vector 
data types. Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
