# RV-Sparse: CSR Matrix-Vector Multiplication

This repository contains my solution for the **RV-Sparse Coding Challenge**. The project implements a high-performance kernel for Sparse Matrix-Vector Multiplication (SpMV) using the Compressed Sparse Row (CSR) format, specifically designed for systems with strict memory constraints and potential for hardware acceleration.

## 📌 Problem Overview

In real-world applications—ranging from graph analytics and social network modeling to scientific simulations—data matrices are predominantly sparse. Using standard General Matrix Multiply (**GEMM**) kernels on such data is highly inefficient, as it wastes computational cycles and memory bandwidth processing redundant zero-value operations.

This implementation addresses these inefficiencies by:

1. **Compressing** a dense row-major matrix into a CSR structure.
2. **Computing** the result vector $y$ by only iterating over non-zero entries, reducing the complexity from $O(m \times n)$ to $O(NNZ)$.

## 🛠 Technical Implementation

### The CSR Algorithm

The solution is divided into two distinct logical phases:

1. **Extraction & Compression:** Scans the input matrix $A$, identifies non-zero coefficients, and populates the `values`, `col_indices`, and `row_ptrs` buffers.
2. **SpMV Computation:** Performs the product $y = A \times x$. For each row $i$, the result is computed as:
   $$y_i = \sum_{k=row\_ptr[i]}^{row\_ptr[i+1]-1} values[k] \times x[col\_indices[k]]$$

### Design Constraints

- **Zero Dynamic Allocation:** In compliance with the challenge requirements, the function performs **no `malloc` calls**. It operates strictly on caller-provided buffers, making it ideal for bare-metal RISC-V implementations and embedded systems.
- **Precision:** Uses `double` precision for all floating-point calculations, achieving a maximum error of `0.00e+00` across test iterations.

- **Spatial Locality:** The CSR format ensures that matrix values and column indices are accessed sequentially, maximizing cache hits.

## 📂 How to Build and Run

To compile the test harness and verify the implementation:

```bash
gcc -o run challenge.c -lm
./run
```

## ✅ Test Results

The implementation successfully passed the full suite of 100 randomized iterations:

```bash
Iter 99 [ 34x 35, density=0.36, nnz= 446]: PASS (Max error: 0.00e+00)
All tests passed! (100/100 iterations passed)
```

**Developed as part of the RISC-V Mentorship Program Coding Challenge.**
