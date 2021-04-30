# oclc_storage_class_specifiers

## Overview
Test suite for [storage class specifiers](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#storage-class-specifiers) OpenCL C functionality.

The goal of those tests is to verify the use of storage class specifiers: `typedef`, `extern` and `static`.

## Tests

### Storage-class specifiers on program scope variables
* Status: DONE
* Goal: Test if all of the storage-class specifiers works properly on program-scope variables.
* Description:
  * `typedef` storage-class specifier. Declares a program-scope variable type.
  * `static` storage-class specifier. Defines a program-scope variable.
  * `extern` storage-class specifier. Externs a program-scope variable.
* Expectations: Kernel is successfully compiled and ran. Kernel's output is the same as the reference.

### Storage-class specifiers on functions
* Status: TODO
* Goal: Test if all of the storage-class specifiers works properly on functions.
* Description:
  * `typedef` storage-class specifier. Declares a type definition of a function declaration.
  * `static` storage-class specifier. Defines a static function.
  * `extern` storage-class specifier. Externs a function.
* Expectations: Kernel is successfully compiled and ran. Kernel's output is the same as the reference.

### Storage-class specifiers on local variables
* Status: TODO
* Goal: Test if all of the storage-class specifiers works properly on local variables.
* Description:
  * `typedef` storage-class specifier. Declares a local variable type.
  * `static` storage-class specifier. Checks a static keyword on local variables.
  * `extern` storage-class specifier. Externs a local variable.
* Expectations: Kernel is successfully compiled and ran. Kernel's output is the same as the reference.

### Storage-class specifiers on variables/functions from another file
* Status: BLOCKED
* Goal: Test if storage-class specifiers is working with a definition in another file.
* Description:
  * `static` storage-class specifier. Make sure static variables/functions are only seen in one translation unit.
  * `extern` storage-class specifier. Extern a program-scope variable that was defined in a different source.
* Expectations: Kernel is successfully compiled and ran or throws.
