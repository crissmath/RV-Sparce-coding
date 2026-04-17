#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void sparse_multiply(
    int rows, int cols, const double* A, const double* x,
    int* out_nnz, double* values, int* col_indices, int* row_ptrs,
    double* y
) {
    int nnz = 0;
    row_ptrs[0] = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double val = A[i * cols + j];

            if (val != 0.0) {
                values[nnz] = val;
                col_indices[nnz] = j;
                nnz++;
            }
        }
        row_ptrs[i + 1] = nnz;
    }
    *out_nnz = nnz;

    for (int i = 0; i < rows; i++) {
        double row_sum = 0.0;
        for (int k = row_ptrs[i]; k < row_ptrs[i + 1]; k++) {
            row_sum += values[k] * x[col_indices[k]];
        }
        y[i] = row_sum;
    }
}

void sparse_multiply_op1(
    int rows, int cols, const double* restrict A, const double* restrict x,
    int* restrict out_nnz, double* restrict values, int* restrict col_indices,
    int* restrict row_ptrs, double* restrict y
) {
    int nnz = 0;
    row_ptrs[0] = 0;

    const double *a = A;

    for (int i = 0; i < rows; ++i) {
        double row_sum = 0.0;

        for (int j = 0; j < cols; ++j, ++a) {
            double val = *a;

            if (val != 0.0) {
                values[nnz] = val;
                col_indices[nnz] = j;
                row_sum += val * x[j];
                ++nnz;
            }
        }

        row_ptrs[i + 1] = nnz;
        y[i] = row_sum;
    }

    *out_nnz = nnz;
}

static void fill_sparse_matrix(double* A, int rows, int cols, double density) {
    for (int i = 0; i < rows * cols; ++i) {
        double r = (double)rand() / (double)RAND_MAX;
        if (r < density) {
            A[i] = 1.0 + (rand() % 10);
        } else {
            A[i] = 0.0;
        }
    }
}

static void fill_vector(double* x, int n) {
    for (int i = 0; i < n; ++i) {
        x[i] = 1.0 + (rand() % 5);
    }
}

static double elapsed_seconds(clock_t a, clock_t b) {
    return (double)(b - a) / (double)CLOCKS_PER_SEC;
}

static int check_equal(const double* y1, const double* y2, int n) {
    for (int i = 0; i < n; ++i) {
        if (fabs(y1[i] - y2[i]) > 1e-9) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    const int rows = 2000;
    const int cols = 2000;
    const double density = 0.0005;
    const int iterations = 50;

    srand(1234);

    double* A = (double*)malloc((size_t)rows * cols * sizeof(double));
    double* x = (double*)malloc((size_t)cols * sizeof(double));

    double* values1 = (double*)malloc((size_t)rows * cols * sizeof(double));
    int* col_indices1 = (int*)malloc((size_t)rows * cols * sizeof(int));
    int* row_ptrs1 = (int*)malloc((size_t)(rows + 1) * sizeof(int));
    double* y1 = (double*)malloc((size_t)rows * sizeof(double));

    double* values2 = (double*)malloc((size_t)rows * cols * sizeof(double));
    int* col_indices2 = (int*)malloc((size_t)rows * cols * sizeof(int));
    int* row_ptrs2 = (int*)malloc((size_t)(rows + 1) * sizeof(int));
    double* y2 = (double*)malloc((size_t)rows * sizeof(double));

    int nnz1 = 0;
    int nnz2 = 0;

    if (!A || !x || !values1 || !col_indices1 || !row_ptrs1 || !y1 ||
        !values2 || !col_indices2 || !row_ptrs2 || !y2) {
        printf("Memory allocation failed\n");
        return 1;
    }

    fill_sparse_matrix(A, rows, cols, density);
    fill_vector(x, cols);

    sparse_multiply(rows, cols, A, x, &nnz1, values1, col_indices1, row_ptrs1, y1);
    sparse_multiply_op1(rows, cols, A, x, &nnz2, values2, col_indices2, row_ptrs2, y2);

    if (!check_equal(y1, y2, rows)) {
        printf("Error: results are different\n");
        free(A); free(x);
        free(values1); free(col_indices1); free(row_ptrs1); free(y1);
        free(values2); free(col_indices2); free(row_ptrs2); free(y2);
        return 1;
    }

    clock_t t1_start = clock();
    for (int it = 0; it < iterations; ++it) {
        sparse_multiply(rows, cols, A, x, &nnz1, values1, col_indices1, row_ptrs1, y1);
    }
    clock_t t1_end = clock();

    clock_t t2_start = clock();
    for (int it = 0; it < iterations; ++it) {
        sparse_multiply_op1(rows, cols, A, x, &nnz2, values2, col_indices2, row_ptrs2, y2);
    }
    clock_t t2_end = clock();

    double time_v1 = elapsed_seconds(t1_start, t1_end);
    double time_op1 = elapsed_seconds(t2_start, t2_end);

    printf("rows = %d, cols = %d, density = %.4f, iterations = %d\n",
           rows, cols, density, iterations);
    printf("nnz v1   = %d\n", nnz1);
    printf("nnz op1  = %d\n", nnz2);
    printf("time v1  = %.6f s\n", time_v1);
    printf("time op1 = %.6f s\n", time_op1);
    printf("speedup  = %.2fx\n", time_v1 / time_op1);

    free(A); free(x);
    free(values1); free(col_indices1); free(row_ptrs1); free(y1);
    free(values2); free(col_indices2); free(row_ptrs2); free(y2);

    return 0;
}
