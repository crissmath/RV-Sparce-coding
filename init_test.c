#include <stdio.h>
#include <math.h>


//prototype
void sparse_multiply(
    int rows, int cols, const double *A, const double *x,
    int *out_nnz, double *values, int *col_indices, int *row_ptrs,
    double *y)
{
    int nnz = 0; // counter for nozero elements 
    row_ptrs[0] = 0; // init ptr in 0


    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            double val = A[ i * cols + j];
            // check no zero
            if( val != 0.0){
                values[nnz] = val;      // get the value 
                col_indices[nnz] = j;   // save position
                nnz++;                  // inc cnt
            }
        }
        row_ptrs[i + 1] = nnz;          // calculate the numbers of positions init rows
    }
    *out_nnz = nnz; // save in output variable

    //  y = A * x
    for(int i = 0; i < nnz; i++){
        double row_sum = 0.0;
        for(int k = row_ptrs[i]; k < row_ptrs[i + 1]; k++){
            row_sum += values[k] * x[col_indices[k]];     // Dotproduct
        }
        y[i] = row_sum;
    }
}// end func




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

    int rows  = 4;
    int cols  = 4;
    
    // number rows
    // number of cols
    // ptr --> A 
    // ptr --> x

    sparse_multiply(rows, cols, A, x, &nnz, values, col_ind, row_ptr, y);


    // Test init
    printf("----- Format CSR Generate----\n");
    printf(" numbers of NNZ: %d\n", nnz);

    printf("Values = [");
    for(int i = 0; i<nnz; i++) printf(" %.1f", values[i]);
    printf(" ]\n");

    printf("indices = [");
    for(int i = 0; i<nnz; i++) printf(" %d", col_ind[i]);
    printf(" ]\n");

    printf("ptr = [");
    for(int i = 0; i <nnz; i++) printf(" %d", row_ptr[i]);
    printf(" ]\n");

    printf("y = [");
    for(int i = 0; i <nnz; i++) printf(" %.1f", y[i]);
    printf(" ]\n");

    
    /*
    ====== Fast check manual =====

    A  = 12.0, 0.0, 26.0, 0.0,
          0.0, 0.0,  0.0, 0.0,
          0.0, 19.0, 0.0, 0.0,
          0.0, 14.0, 0.0, 7.0

    x   = 1, 2, 3, 4

    y[0] = (12 * 1) + (26 * 3) = 12 + 78 = 90
    y[1] =                                  0
    y[2] = ( 9 * 2) =                      18 
    y[3] = (14 * 2) + (7 * 4)  = 28 + 28 = 56           

    y = [ 90, 0, 18, 56]
    

    */


}// end main 