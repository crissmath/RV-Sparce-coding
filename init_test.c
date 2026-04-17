#include <stdio.h>
#include <math.h>


//prototype
void sparse_multiply(
    int rows, int cols, const double *A, const double *x,
    int *out_nnz, double *values, int *col_indices, int *row_ptrs,
    double *y)
{




}




int main(){

    // input : A 
    double A[16] = {
        12.0, 0.0, 26.0, 0.0,
         0.0, 0.0,  0.0, 0.0,
         0.0, 19.0, 0.0, 0.0,
         0.0, 14.0, 0.0, 7.0
    };

    // input: x 
    double x[4] ={1.0, 2.0, 3.0, 4.0}; 

    // buffers CSR size = 16 for test 
    double values[16];
    int col_ind[16];
    int row_ptr[5]; // rows + 1
    int nnz = 0;
    double y[4];

    int rows = 4;
    int cols  = 4;
    
    // number rows
    // number of cols
    // ptr --> A 
    // ptr --> x

    sparse_multiply(rows, cols, A, x, &nnz, values, col_ind, row_ptr, y);


    // Test init
    



}// end main 