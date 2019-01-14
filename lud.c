#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset, memcpy
#include <math.h> // pow

// Declaration of variables
long long int n, i, j, k; // n - matrix size
float *A, *L, *U, *LU;
int exist = 1, choice; // choice - user input, exist - LU exists or not

// Function declarations

void randMat(); // Generste random nxn matrix
void getMat(); // Manually input matrix 
void printMat(); // Print a matrix
void compute(); // Factorize matrix to L*U
void matMul(); // Matrix multiplication
float norm(); // Compute norm of a matrix
void test(); // testing of module
void Free(); // Frees allocated memory

int main(int argc, char* argv[])
{    
    // nxn is matrix size
    printf("Enter input matrix size: ");
    scanf("%lld", &n);

    // Memory allocation for matrices
    // A is input matrix
    // L, U are lower and upper trianlgular matrices
    // LU is L*U
    A = (float*)malloc(n*n*sizeof(float));
    L = (float*)malloc(n*n*sizeof(float));
    U = (float*)malloc(n*n*sizeof(float));
    LU = (float*)malloc(n*n*sizeof(float));

    // Enter choice of input matrix generation
    // 0: random generator
    // 1: maual input
    printf("Enter 1 (for manual input of matrix) or 0 (randomized matrix generator): ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 0: randMat(); break;
        case 1: getMat(); break;
        default: printf("\nWrong choice.\n"); return 0;
    }

    // Init values
    memset(L, 0, sizeof(L));
    memset(LU, 0, sizeof(LU));
    memcpy(U, A, sizeof(A)*n*n);

    // Check and factorize A
    compute();  
    // Compute error
    matMul(L, U);
    float error = norm(A, LU);

    if(exist == 1) // LU decomposition exists
    {
        if(n <= 10) // Print matrices only if size is small
        {
            printf("\nInput matrix is:\n");
            printMat(A);

            printf("\nU matrix is:\n");  
            printMat(U);
            printf("\nL matrix is:\n");  
            printMat(L);
            
            printf("\nL*U matrix is:\n"); 
            printMat(LU);            
        }

        printf("\nError is: %f\n", error);                 
    }
        
    else // LU decomposition does not exist, encountered division by 0 in process
    {
        printf("\nLU decomposition does not exist for input matrix.\n");
    }

    //Free();
    test();
    
    return 0;
}

// Function definitions 

void randMat()
{
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
            A[i*n+j] = (float)rand()/(float)(RAND_MAX/100)-50;
}

void getMat()
{
    printf("\n");
    for(i=0; i<n; i++)
    {
        printf("Enter row %lld:\n", i+1);
        for(j=0; j<n; j++)
            scanf("%f", &A[i*n+j]);
    }
}

void printMat(float *mat)
{
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
            printf("%f ", mat[i*n+j]);
        printf("\n");
    }
}

void compute()
{
    // Diagonal of L matrix - constraint
    for(long long int i=0; i<n; i++)
        L[i*n+i] = 1;

    // Elementary row operations to make U
    for(long long int i=1; i<n; i++)
        for(long long int j=0; j<i; j++)
        {
            float X;
            if(U[j*n+j] != 0) // Checking if LU exists
            {
                X = U[i*n+j]/U[j*n+j];
            }             
            else
            {
                exist = -1;
                return;
            }

            L[i*n+j] = X;
            for(long long int k=0; k<n; k++)
                    U[i*n+k] = U[i*n+k] - U[j*n+k]*X;
        }            
}

void matMul(float *L, float *U)
{
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
            for(k=0; k<n; k++)
                LU[i*n+j] += L[i*n+k]*U[k*n+j];
}

float norm(float *A, float *B)
{
    float s = 0;    
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
            s += pow((A[i*n+j]-B[i*n+j]), 2);

    return pow(s, 0.5);
}

void test()
{
    float avg=0, max=-1, min=1000;
    int count = 0;

    for(long long int i=1; i<182; i++)
    {
        n = i;

        A = (float*)malloc(n*n*sizeof(float));
        L = (float*)malloc(n*n*sizeof(float));
        U = (float*)malloc(n*n*sizeof(float));
        LU = (float*)malloc(n*n*sizeof(float));

        randMat();
        memset(L, 0, sizeof(L));
        memset(LU, 0, sizeof(LU));
        memcpy(U, A, sizeof(A)*n*n);
        compute();

        if(exist == 1)
        {
            matMul(L, U);

            float err = norm(A, LU);
            count++;
            //printf("%f ", err);
          
            if(err > max)
                max = err;
            
            if(err < min)
                min = err;

            avg += err;
        }
            
        //Free();
    }
    avg /= count;
    printf("\nError: Min = %f, Max = %f, Average = %f\n", min, max, avg);
}

void Free()
{
    free(A);
    free(L);
    free(U);
    free(LU);    
}