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


void improved_dgbmv(long m, long n, long kl, long ku, double alpha, double *bA, long lda, double *x, long incx, double beta, double *y, long incy)
{
    long lenx, leny, kx, ky, iy,  kup1, i, jx, j, k, start;
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
        double *yp = NULL;
        if (incy == 1) {
            if (beta == 0) {
                yp = y;
                for (i = 1; i <= leny; i++) {
                    *yp = 0;
                    yp++;
                }
            } else {
                *yp = Y(1);
                for (i = 1; i <= leny; i++) {
                    *yp = beta * *yp;
                    yp++;
                }
            }
        } else {
            iy = ky;
            if (beta == 0) {
                yp = y;
                for (i = 1; i <= leny; i++) {
                    *yp = 0;
                    yp += incy;
                }
            } else {
                yp = y;
                for (i = 1; i <= leny; i++) {
                    *yp = beta * *yp;
                    yp += incy;
                }
            }
        }
    }

    if (alpha == 0)
        return;

    kup1 = ku + 1;
    jx = kx;

    double *xp = NULL;
    double *yp = NULL;
    double *ap = NULL;
    if (incy == 1) {
        xp = &X(jx);
        ap = &bA[0];
        for (j = 1; j <= n; ++j) {
            if (*xp != 0) {
                temp = alpha * *xp;
                k = kup1 - j;

                start = MAX(1, j - ku);
                ap = &A(k + start, j);
                yp = &Y(start);

                for (i = MAX(1, j - ku); i <= MIN(m, j + kl); ++i) {
                    *yp += temp * *ap;
                    yp++;
                    ap++;
                }
            }
            xp += incx;
        }
    } else {
        xp = &X(jx);
        ap = &bA[0];
        for (j = 1; j <= n; ++j) {
            if (*xp != 0) {
                temp = alpha * *xp;
                k = kup1 - j;
                iy = ky;
                start = MAX(1, j - ku);
                ap = &A(k + start, j);
                yp = &Y(iy);
                for (i = MAX(1, j - ku); i <= MIN(m, j + kl); ++i) {
                    *yp += temp * *ap;
                    yp += incy;
                    ap++;
                }
            }
            xp += incx;
            if (j > ku)
                ky += incy;
        }
    }
    return;
}


void dgbmv(long m, long n, long kl, long ku, double alpha, double *bA, long lda,
        double *x, long incx, double beta, double *y, long incy)
{
    long lenx, leny, kx, ky, iy,  kup1, i, jx, j, k;
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

int check_equal(double *a, long size_a, double *b, long size_b) {
    int ok = 0;
    if (size_a != size_b)
        return ok;
    long i = 0;
    for (i = 0; i < size_a; i++) {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 1)
	printf("./main flags(0 sau 1)\n");


    long m = 300000000, n = 300000000, i, lda = 1, incx = 1, incy = 2;
    long kl = 0, ku = 0, alpha = 2, beta = 10, diff_sec, diff_usec;
    int flags = atoi(argv[1]);
    struct timeval tvstart, tvstop;
    long k = 0;
    double *a, *x, *y, *yb, *yc;

    a = malloc(lda * n * sizeof(double));
    x = malloc(n * incx * sizeof(double));
    y = malloc(m * incy * sizeof(double));
    yb = malloc(m * incy * sizeof(double));
    yc = malloc(m * incy * sizeof(double));


    get_random_values(a, lda * n);
    get_random_values(x, n * incx);
    get_random_values(y, m * incy);

    for (i = 0; i < m * incy; i++) {
	    yb[i] = a[i];
	    yc[i] = a[i];
    }

    gettimeofday(&tvstart, NULL);
    dgbmv(m, n, kl, ku, alpha, a, lda, x, incx, beta, y, incy);
    gettimeofday(&tvstop, NULL);

    diff_sec = tvstop.tv_sec - tvstart.tv_sec;
    if (diff_sec > 0)
	    diff_usec = tvstop.tv_usec + 1000000 - tvstart.tv_usec;
    else
	    diff_usec = tvstop.tv_usec - tvstart.tv_usec;

    printf("%ld\n", diff_sec * 1000 + diff_usec / 1000);


    if (flags == 0) {
	    gettimeofday(&tvstart, NULL);
	    improved_dgbmv(m, n, kl, ku, alpha, a, lda, x, incx, beta, yc, incy);
	    gettimeofday(&tvstop, NULL);

	    diff_sec = tvstop.tv_sec - tvstart.tv_sec;
	    if (diff_sec > 0)
		    diff_usec = tvstop.tv_usec + 1000000 - tvstart.tv_usec;
	    else
		    diff_usec = tvstop.tv_usec - tvstart.tv_usec;

	    printf("%ld\n", diff_sec * 1000 + diff_usec / 1000);


	    gettimeofday(&tvstart, NULL);
	    cblas_dgbmv(102, 111, m, n, kl, ku, alpha, a, lda, x, incx, beta, yb, incy);
	    gettimeofday(&tvstop, NULL);
	    diff_sec = tvstop.tv_sec - tvstart.tv_sec;
	    if (diff_sec > 0)
		    diff_usec = tvstop.tv_usec + 1000000 - tvstart.tv_usec;
	    else
		    diff_usec = tvstop.tv_usec - tvstart.tv_usec;
	    printf("%ld\n", diff_sec * 1000 + diff_usec / 1000);
    }


    free(a);
    free(x);
    free(y);
    free(yb);
    free(yc);
    return 0;
}
