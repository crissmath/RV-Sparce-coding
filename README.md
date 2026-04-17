# RV-Sparse: Coding Challenge

## Challenge

Implement the sparse_multiply function that:

* Scans a row-major matrix A and identifies its non-zero elements.
* Extracts them into Compressed Sparse Row (CSR) format using caller-provided buffers.
* Computes the matrix-vector product y = A * x using the extracted CSR data.
* Writes the result directly into a caller-provided output buffer.

**Critical Constraint**: Your function must perform zero dynamic memory allocation. All memory is pre-allocated by the caller.

## Test Harness

The function prototype and test harness is given in the file challenge.c which you can find here. The placeholder for the implementation of the function is also given there, marked by the TODO comment.

To test your implementation, execute the following.

```bash
>>> gcc -o run challenge.c -lm
>>> ./run
```

## Submission

Host your solution in a public Git repository with brief documentation. Submit a PDF with the link to your Git repository.

## Problem Description

In real-world applications—ranging from graph analytics and social network modeling to linear programming and large-scale simulations—data is rarely dense. Most coefficients in these massive datasets are zero. Using standard General Matrix Multiply (GEMM) kernels on such data is highly inefficient, as it wastes computational cycles and memory bandwidth processing redundant zero-value operations.


## 💻 Implementación en C (Explicada)
