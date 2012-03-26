#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define RANGE       100
#define N           5
#define M           4

double general_A[5][4] = { {1, 1, 1, 0},
        {2, 2, 2, 2},
        {3, 3, 3, 3},
        {4, 4, 4, 4},
        {0, 5, 5, 5}};


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
}

void free_general_matrix(int n, int m, double **A)
{
    int i = 0;
    for (i = 0; i < n; i++)
        free(A[i]);
    free(A);
}

void display_matrix(int n, int m, double **a)
{
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            printf("%lf ", general_A[i][j]);
        printf("\n");
    }
}

int main(void)
{
    int n, m;
    generate_general_matrix(5, 4);
    display_matrix(5, 4, NULL);

    return 0;
}
