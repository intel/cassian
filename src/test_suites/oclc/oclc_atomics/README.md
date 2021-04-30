# oclc_atomics

## Overview
Test suite for 
[atomic functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#atomic-functions) 
OpenCL C functionality.

## Tests

### `ATOMIC_VAR_INIT`
* Status: TODO
* Goal: Verify that program-scope atomic variable can be initialized.
* Description: 
  * Initialize program-scope atomic variable.
  * Run OpenCL C kernel and read variable into a buffer using `atomic_load`.
* Expectations: Variable is initialized with a given value.

### `atomic_init` signatures
* Status: DONE
* Goal: Verify that atomic variables can be initialized using all possible `atomic_init` signatures.
* Description:
  * `global` memory type:
    * Pass buffer of global atomic variables as a kernel argument.
    * Each work-item initializes single atomic variable with a given value.
  * `local` memory type:
    * Declare array of atomic variables in local address space.
    * Each work-item initializes single atomic variable with a given value.
    * Each work-item reads local variable into a buffer.
* Expectations: All atomic variables are initialized with requested values.
* Parameters:
  * Memory type: `global`, `local`.
  * Atomic type: `int`, `uint`, `long`, `ulong`, `float`, `double`, `intptr_t`, `uintptr_t`, `size_t`, `ptrdiff_t`.

### `atomic_work_item_fence` signatures
* Status: TODO
* Goal: Verify that all possible signatures of `atomic_work_item_fence` are supported.
* Description: Run kernel with a given fence function.
* Expectations: Kernel can be built and executed successfully.
* Parameters:
  * Memory flag: `CLK_GLOBAL_MEM_FENCE`, `CLK_LOCAL_MEM_FENCE`, `CLK_IMAGE_MEM_FENCE`.
  * Memory order: `memory_order_relaxed`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`, `memory_order_seq_cst`.
  * Memory scope: `memory_scope_work_group`, `memory_scope_device`, `memory_scope_all_svm_devices`.

### `atomic_store` signatures
* Status: DONE
* Goal: Verify that atomic variables can be updated using all possible `atomic_store` signatures.
* Description:
  * `global` memory type:
    * Pass buffer of global atomic variables as a kernel argument.
    * Each work-item stores a value into a single atomic variable.
  * `local` memory type:
    * Declare array of atomic variables in local address space.
    * Each work-item stores a value into a single atomic variable.
    * Each work-item reads local variable into a buffer.
* Expectations: All atomic variables are updated with requested values.
* Parameters:
  * Memory type: `global`, `local`.
  * Atomic type: `int`, `uint`, `long`, `ulong`, `float`, `double`, `intptr_t`, `uintptr_t`, `size_t`, `ptrdiff_t`.
  * Function type: `implicit`, `explicit`.
  * Memory order: `memory_order_relaxed`, `memory_order_release`, `memory_order_seq_cst`.
  * Memory scope: `memory_scope_work_group`, `memory_scope_device`, `memory_scope_all_svm_devices`.

### `atomic_load` signatures
* Status: DONE
* Goal: Verify that atomic variables can be read using all possible `atomic_load` signatures.
* Description:
  * `global` memory type:
    * Pass buffer of global atomic variables as a kernel argument.
    * Each work-item loads a value from a single atomic variable.
  * `local` memory type:
    * Declare array of atomic variables in local address space.
    * Each work-item stores a value into a single atomic variable.
    * Each work-item loads local variable into a buffer.
* Expectations: All atomic variables are read with requested values.
* Parameters:
  * Memory type: `global`, `local`.
  * Atomic type: `int`, `uint`, `long`, `ulong`, `float`, `double`, `intptr_t`, `uintptr_t`, `size_t`, `ptrdiff_t`.
  * Function type: `implicit`, `explicit`.
  * Memory order: `memory_order_relaxed`, `memory_order_acquire`, `memory_order_seq_cst`.
  * Memory scope: `memory_scope_work_group`, `memory_scope_device`, `memory_scope_all_svm_devices`.

### `atomic_exchange` signatures
* Status: TODO
* Goal: Verify that atomic variables can be replaced using all possible `atomic_exchange` signatures.
* Description:
  * `global` memory type:
    * Pass buffer of global atomic variables as a kernel argument.
    * Each work-item exchanges a single atomic variable.
  * `local` memory type:
    * Declare array of atomic variables in local address space.
    * Each work-item loads a value into a single atomic variable.
    * Each work-item exchanges a single atomic variable.
    * Each work-item loads local variable into a buffer.
* Expectations:
  * All atomic variables are exchanged with a requested value.
  * All fetched values represent values before the exchange operation.
* Parameters:
  * Memory type: `global`, `local`.
  * Atomic type: `int`, `uint`, `long`, `ulong`, `float`, `double`, `intptr_t`, `uintptr_t`, `size_t`, `ptrdiff_t`.
  * Function type: `implicit`, `explicit`.
  * Memory order: `memory_order_relaxed`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`, `memory_order_seq_cst`.
  * Memory scope: `memory_scope_work_group`, `memory_scope_device`, `memory_scope_all_svm_devices`.

### `atomic_compare_exchange` signatures
* Status: TODO
* Goal: Verify that atomic variables can be replaced using all possible `atomic_compare_exchange` signatures.
* Description:
  * `global` memory type:
    * Pass buffer of global atomic variables as a kernel argument.
    * Each work-item compares and exchanges a single atomic variable.
  * `local` memory type:
    * Declare array of atomic variables in local address space.
    * Each work-item loads a value into a single atomic variable.
    * Each work-item compares and exchanges a single atomic variable.
    * Each work-item loads local variable into a buffer.
* Expectations:
  * All atomic variables are exchanged with a requested value.
  * All fetched values represent values before the exchange operation.
* Parameters:
  * Memory type: `global`, `local`.
  * Atomic type: `int`, `uint`, `long`, `ulong`, `float`, `double`, `intptr_t`, `uintptr_t`, `size_t`, `ptrdiff_t`.
  * Function type: `implicit`, `explicit`.
  * Failure memory order: `memory_order_relaxed`, `memory_order_acquire`,`memory_order_seq_cst`.
  * Success memory order: `memory_order_relaxed`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`, `memory_order_seq_cst`.
  * Memory scope: `memory_scope_work_group`, `memory_scope_device`, `memory_scope_all_svm_devices`.
  * Comparison type: `weak`, `strong`.
  * Comparison result: `success`, `failure`.

### `atomic_fetch` signatures
* Status: DONE
* Goal: Verify that atomic variables can be fetched and modified using all possible `atomic_fetch` signatures.
* Description:
  * `global` memory type:
    * Pass buffer of global atomic variables as a kernel argument.
    * Each work-item fetches and modifies a single atomic variable.
  * `local` memory type:
    * Declare array of atomic variables in local address space.
    * Each work-item loads a value into a single atomic variable.
    * Each work-item fetches and modifies a single atomic variable.
    * Each work-item loads local variable into a buffer.
* Expectations:
  * All atomic variables are updated by a given operation.
  * All fetched values represent values before a given operation.
* Parameters:
  * Memory type: `global`, `local`.
  * Atomic operation: `add`, `sub`, `or`, `xor`, `and`, `min`, `max`.
  * Atomic type: `int`, `uint`, `long`, `ulong`, `intptr_t`, `uintptr_t`, `size_t`, `ptrdiff_t`.
  * Function type: `implicit`, `explicit`.
  * Memory order: `memory_order_relaxed`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`, `memory_order_seq_cst`.
  * Memory scope: `memory_scope_work_group`, `memory_scope_device`, `memory_scope_all_svm_devices`.

### `atomic_fetch` special values
* Status: TODO
* Goal: Verify that atomic fetch operations return valid results when operating for type-specific special values.
* Description: TODO
* Expectations: TODO

### `ATOMIC_FLAG_INIT`
* Status: TODO
* Goal: Verify that program-scope atomic flag variable can be initialized.
* Description: 
  * Initialize program-scope atomic flag variable.
  * Run OpenCL C kernel and read variable into a buffer using `atomic_load`.
* Expectations: Variable is initialized to 0.

### `atomic_flag_test_and_set` signatures
* Status: TODO
* Goal: Verify that atomic flag variables can be set using all possible `atomic_flag_test_and_set` signatures.
* Description:
  * `global` memory type:
    * Pass buffer of global atomic flag variables as a kernel argument.
    * Each work-item sets a single atomic flag variable.
  * `local` memory type:
    * Declare array of atomic flag variables in local address space.
    * Each work-item sets a single atomic flag variable.
    * Each work-item loads local variable into a buffer.
* Expectations:
  * All atomic flag variables are set.
* Parameters:
  * Memory type: `global`, `local`.
  * Function type: `implicit`, `explicit`.
  * Memory order: `memory_order_relaxed`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`, `memory_order_seq_cst`.
  * Memory scope: `memory_scope_work_group`, `memory_scope_device`, `memory_scope_all_svm_devices`.

### `atomic_flag_clear` signatures
* Status: TODO
* Goal: Verify that atomic flag variables can be cleared using all possible `atomic_flag_clear` signatures.
* Description:
  * `global` memory type:
    * Pass buffer of global atomic flag variables as a kernel argument.
    * Each work-item clears a single atomic flag variable.
  * `local` memory type:
    * Declare array of atomic flag variables in local address space.
    * Each work-item clears a single atomic flag variable.
    * Each work-item loads local variable into a buffer.
* Expectations:
  * All atomic flag variables are cleared.
* Parameters:
  * Memory type: `global`, `local`.
  * Function type: `implicit`, `explicit`.
  * Memory order: `memory_order_relaxed`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`, `memory_order_seq_cst`.
  * Memory scope: `memory_scope_work_group`, `memory_scope_device`, `memory_scope_all_svm_devices`.

### Atomic contention
* Status: TODO
* Goal: Verify that atomic operations can be performed by multiple work-items on the same atomic variable.
* Description: TODO
* Expectations: TODO

### Memory order
* Status: TODO
* Goal: Verify that acquire/release semantics are properly handled.
* Description: TODO
* Expectations: TODO

### Memory scope
* Status: TODO
* Goal: Verify that atomic operations are synchronized according to the memory scope.
* Description: TODO
* Expectations: TODO
