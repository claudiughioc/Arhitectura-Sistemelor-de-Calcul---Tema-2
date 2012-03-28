#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RANGE       100
#define N           5
#define M           4
#define MIN(X,Y) ((X) < (Y) ?  (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ?  (X) : (Y))

double bA[5][4] = { {1, 1, 1, 0},
        {2, 2, 2, 2},
        {3, 3, 3, 3},
        {4, 4, 4, 4},
        {0, 5, 5, 5}};
double A[8][4] = {{0, 0, 1, 2},
        {0, 1, 2, 3},
        {1, 2, 3, 4},
        {2, 3, 4, 5},
        {3, 4, 5, 0},
        {4, 5, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}};
double x[4] = {1, 2, 3, 4};
double y[10] = {1, 0, 2, 0, 3, 0, 4, 0, 5, 0};

/*
double **generate_general_matrix(int n, int m)
{
    int i, j;
    unsigned int seed = (unsigned int) time(NULL);
    srand(seed);

    double **A = malloc(n * sizeof(double *));
    if (!A)
        perror("Error while allocating A\n");
    for (i = 0; i < n; i++) {
        A[i] = malloc(m * sizeof(double));
        if (!A[i])
            perror("Error while allocating array in A\n");
    }

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            general_A[i][j] = ((double) rand() / (double) RAND_MAX) * RANGE;


    return NULL;
}*/

void free_general_matrix(int n, int m, double **A)
{
    int i = 0;
    for (i = 0; i < n; i++)
        free(A[i]);
    free(A);
}

/*
void display_matrix(int n, int m, double **a)
{
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            printf("%lf ", general_A[i][j]);
        printf("\n");
    }
}*/




void dgbmv(int m, int n, int kl, int ku, double alpha, double **Ax, int lda,
    double *xx, int incx, double beta, double *yy, int incy)
{
    int lenx, leny, kx, ky, iy, ix, kup1, i, jx, j, k;
    double temp;
    /* Sanity check */
    if ((m <= 0) || (n <= 0) || (kl < 0) || (ku < 0) || (lda <= 0) ||
        (lda < (kl + ku + 1)) || (incx = 0) || (incy == 0) ||
        ((alpha == 0) && (beta == 1)))
        return;
    
    lenx = n;
    leny = m;

    if (incx > 0)
        kx = 0;
    else
        kx = - lenx * incx;

    if (incy > 0)
        ky = 0;
    else
        ky = - leny * incy;

    if (beta != 1) {
        if (incy == 1) {
            if (beta == 0)
                for (i = 0; i < leny; i++)
                    y[i] = 0;
            else
                for (i = 0; i < leny; i++)
                    y[i] = beta * y[i];
        } else {
            iy = ky;
            if (beta == 0)
                for (i = 0; i < leny; i++) {
                    y[iy] = 0;
                    iy += incy;
                }
            else
                for (i = 0; i < leny; i++) {
                    y[iy] = beta * y[iy];
                    iy += incy;
                }
        }
    }

    if (alpha == 0)
        return;

    kup1 = ku + 1;
    jx = kx;

    if (incy == 1) {
        for (j = 1; j < n; j++) {
            if (x[jx] != 0) {
                temp = alpha * x[jx];
                k = kup1 - j;
                for (i = MAX(1, j - ku); i < MIN(m, j + kl); i++)
                    y[i] += temp * A[k + i][j];
            }
            jx += incx;
        }
    } else {
        for (j = 1; j < n; j++) {
            if (x[jx] != 0) {
                temp = alpha * x[jx];
                iy = ky;
                k = kup1 - j;
                for (i = MAX(1, j - ku); i < MIN(M, j + kl); i++) {
                    y[iy] += temp * A[k + i][j];
                    iy += incy;
                }
            }
            jx += incx;
            if (j > ku)
                ky += incy;
        }
    }
    return;
}

int main(void)
{
    int n, m;
    //generate_general_matrix(5, 4);
    //display_matrix(5, 4, NULL);

    dgbmv(5, 4, 3, 2, 2, NULL, 8, NULL, 1, 10, NULL, 2);

    int i = 0;
    for (i = 0; i < 10; i++)
        printf("%lf, ", y[i]);

    return 0;
}
