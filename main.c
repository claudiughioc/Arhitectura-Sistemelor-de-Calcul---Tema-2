#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <cblas.h>

#define RANGE       100
#define X(I)        x[(I)-1]
#define Y(I)        y[(I)-1]
#define A(I, J)     bA[(I) - 1 + ((J) - 1) * lda]
#define MIN(X,Y) ((X) < (Y) ?  (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ?  (X) : (Y))

double nA[5][4] = { {1, 1, 1, 0},
    {2, 2, 2, 2},
    {3, 3, 3, 3},
    {4, 4, 4, 4},
    {0, 5, 5, 5}};


void get_random_values(double *matrix, long size)
{
    int i;
    unsigned int seed = (unsigned int) time(NULL);
    srand(seed);

    for (i = 0; i < size; i++)
        matrix[i] = ((double) rand() / (double) RAND_MAX) * RANGE;

    return;
}

void display_matrix(double *matrix, long size)
{
    int i;
    printf("\nContinut matrice : \n");
    for (i = 0; i < size; i++) {
        printf("%lf ", matrix[i]);
    }
    printf("\n");
}


void dgbmv(int m, int n, int kl, int ku, double alpha, double *bA, int lda,
        double *x, int incx, double beta, double *y, int incy)
{
    int lenx, leny, kx, ky, iy,  kup1, i, jx, j, k;
    double temp;
    /* Sanity check */
    if ((m <= 0) || (n <= 0) || (kl < 0) || (ku < 0) || (lda <= 0) ||
            (lda < (kl + ku + 1)) || (incx == 0) || (incy == 0) ||
            ((alpha == 0) && (beta == 1)))
        return;

    lenx = n;
    leny = m;

    if (incx > 0)
        kx = 1;
    else
        kx = 1 - (lenx - 1) * incx;

    if (incy > 0)
        ky = 1;
    else
        ky = 1 - (leny - 1) * incy;

    if (beta != 1) {
        if (incy == 1) {
            if (beta == 0)
                for (i = 1; i <= leny; i++)
                    Y(i) = 0;
            else
                for (i = 1; i <= leny; i++)
                    Y(i) = beta * Y(i);
        } else {
            iy = ky;
            if (beta == 0)
                for (i = 1; i <= leny; i++) {
                    Y(iy) = 0;
                    iy += incy;
                }
            else
                for (i = 1; i <= leny; i++) {
                    Y(iy) = beta * Y(iy);
                    iy += incy;
                }
        }
    }

    if (alpha == 0)
        return;

    kup1 = ku + 1;
    jx = kx;


    if (incy == 1) {
        for (j = 1; j <= n; ++j) {
            if (X(jx) != 0) {
                temp = alpha * X(jx);
                k = kup1 - j;
                for (i = MAX(1, j - ku); i <= MIN(m, j + kl); ++i) {
                    Y(i) += temp * A(k + i, j);
                }
            }
            jx += incx;
        }
    } else {
        for (j = 1; j <= n; ++j) {
            if (X(jx) != 0) {
                temp = alpha * X(jx);
                iy = ky;
                k = kup1 - j;
                for (i = MAX(1, j - ku); i <= MIN(m, j + kl); ++i) {
                    Y(iy) += temp * A(k + i, j);
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


double **to_band_storage(int n, int m, int lda, int kl, int ku, double **a)
{
    int u, v, i, j;
    double **b = malloc(lda * sizeof(double *));
    if (b == NULL)
        return b;
    for (i = 0; i < lda; i++) {
        b[i] = malloc(m * sizeof(double));
        if (b[i] == NULL) {
            free(b);
            return NULL;
        }
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            if (j >= MAX(0, i - kl) && j < MIN(n, i + kl)) {
                u = i + ku - j;
                v = j;
                if ( u >= 0 && u < lda)
                    b[u][v] = a[i][j];
            }
        }
    }

    return b;
}

int main(void)
{
    /*
    int m = 5, n = 4, i, lda = 8, incx = 1, incy = 2;
    int kl = 3, ku = 2, alpha = 2, beta = 10;
    double bA[32] = {0, 0, 1, 2, 3, 4, 0, 0,
        0, 1, 2, 3, 4, 5, 0, 0,
        1, 2, 3, 4, 5, 0, 0, 0,
        2, 3, 4, 5, 0, 0, 0, 0};
    double x[4] = {1, 2, 3, 4};
    double y[10] = {1, 0, 2, 0, 3, 0, 4, 0, 5, 0};
    */

    long m = 10000000, n = 10000000, i, lda = 3, incx = 1, incy = 2;
    long kl = 0, ku = 0, alpha = 2, beta = 10i, diff_sec, diff_usec;
    struct timeval tvstart, tvstop;
    double *a = malloc(lda * n * sizeof(double));
    double *x = malloc(n * incx * sizeof(double));
    double *y = malloc(m * incy * sizeof(double));
    

    get_random_values(a, lda * n);
    get_random_values(x, n * incx);
    get_random_values(y, m * incy);

    //display_matrix(a, lda * n);


    printf("----Start attempt for my function----\n");
    gettimeofday(&tvstart, NULL);
    dgbmv(m, n, kl, ku, alpha, a, lda, x, incx, beta, y, incy);
    gettimeofday(&tvstop, NULL);

    diff_sec = tvstop.tv_sec - tvstart.tv_sec;
    diff_usec;
    if (diff_sec > 0)
        diff_usec = tvstop.tv_usec + 1000000 - tvstart.tv_usec;
    else
        diff_usec = tvstop.tv_usec - tvstart.tv_usec;

    printf("Diferenta in secunde %ld\n", diff_sec);
    printf("Diferenta in usecunde %ld\n", diff_usec);
    printf("Durata in milisecunde %ld\n", diff_sec * 1000 + diff_usec / 1000);
    printf("----end of attempt, start BLAS----\n");



    gettimeofday(&tvstart, NULL);
    cblas_dgbmv(102, 111, m, n, kl, ku, alpha, bA, lda, x, incx, beta, y, incy);
    gettimeofday(&tvstop, NULL);
    diff_sec = tvstop.tv_sec - tvstart.tv_sec;
    diff_usec;
    if (diff_sec > 0)
        diff_usec = tvstop.tv_usec + 1000000 - tvstart.tv_usec;
    else
        diff_usec = tvstop.tv_usec - tvstart.tv_usec;
    printf("Diferenta in secunde %ld\n", diff_sec);
    printf("Diferenta in usecunde %ld\n", diff_usec);
    printf("Durata in milisecunde %ld\n", diff_sec * 1000 + diff_usec / 1000);
    printf("----Apel BLAS dgbmv----\n");


    /* Afisare vector final y 
    printf("\nContinut vector y:\n");
    for (i = 0; i < incy * m; i++)
        printf("%lf, ", y[i]);
    printf("\n");
    */
    free(a);
    return 0;
}
