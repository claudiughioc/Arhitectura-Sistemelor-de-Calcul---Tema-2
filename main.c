#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RANGE       100
#define X(I)        x[(I)-1]
#define Y(I)        y[(I)-1]
#define A(I, J)     bA[(I) - 1 + ((J) - 1) * lda]
#define N           5
#define M           4
#define MIN(X,Y) ((X) < (Y) ?  (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ?  (X) : (Y))

double nA[5][4] = { {1, 1, 1, 0},
        {2, 2, 2, 2},
        {3, 3, 3, 3},
        {4, 4, 4, 4},
        {0, 5, 5, 5}};
double bA[32] = {0, 0, 1, 2, 3, 4, 0, 0,
                0, 1, 2, 3, 4, 5, 0, 0,
                1, 2, 3, 4, 5, 0, 0, 0,
                2, 3, 4, 5, 0, 0, 0, 0};
double x[4] = {1, 2, 3, 4};
double y[10] = {1, 0, 2, 0, 3, 0, 4, 0, 5, 0};

double **get_random_values(int n, int m, double **matrix)
{
    int i, j;
    unsigned int seed = (unsigned int) time(NULL);
    srand(seed);

    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            matrix[i][j] = ((double) rand() / (double) RAND_MAX) * RANGE;

    return matrix;
}

void free_general_matrix(int n, int m, double **a)
{
    int i = 0;
    for (i = 0; i < n; i++)
        free(a[i]);
    free(a);
}

void display_matrix(int n, int m, double **a)
{
    int i, j;
    printf("\nContinut matrice : \n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            printf("%lf ", a[i][j]);
        printf("\n");
    }
}


void dgbmv(int m, int n, int kl, int ku, double alpha, double *Ax, int lda,
    double *xx, int incx, double beta, double *yy, int incy)
{
    int lenx, leny, kx, ky, iy, ix, kup1, i, jx, j, k;
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
    int n = 5, m = 4, i, lda = 8;
    int kl = 4, ku = 3;

    double **a = malloc(n * sizeof(double *));
    double **b;

    if (!a)
        perror("Error while allocating a\n");
    for (i = 0; i < n; i++) {
        a[i] = malloc(m * sizeof(double));
        if (!a[i])
            perror("Error while allocating array in a\n");
    }

    a = get_random_values(n, m, a);
    display_matrix(n, m, a);

    b = to_band_storage(n, m, lda, kl, ku, a);
    display_matrix(lda, m, b);
    

    dgbmv(5, 4, 3, 2, 2, NULL, 8, NULL, 1, 10, NULL, 2);

    /* Afisare vector final y */
    printf("\nContinut vector y:\n");
    for (i = 0; i < 10; i++)
        printf("%lf, ", y[i]);
    printf("\n");

    free_general_matrix(n, m, a);
    return 0;
}
