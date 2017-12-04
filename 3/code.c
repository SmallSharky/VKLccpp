#include <stdio.h>
//#include <conio.h>
#include <math.h>
#include <stdlib.h>
#define N 1024

int * find_min(int ** M, int n) {
    int i = 0;
    int min_i = 0, min_j = 0;
    while(i<n) {
        int j = 0;
        while(j<n) {
            if(M[i][j]<M[min_i][min_j]) {
                min_i = i;
                min_j = j;
            }
            j++;
        }
        i++;
    }
    return &(M[min_i][min_j]);
}

int main (int argc, char ** argv)
{
    int b[N];
    int n=0;

    int i=0;
    int j=0;
    int f=1;
    int ind1=0;
    int ind2=0;


    if(argc<2) {
        printf("File not specified\n");
        printf("Please, specify it as program argument\n");
        return -1;
    }

    FILE * fp3 = NULL;
    fp3=fopen(argv[1],"r");
    if(!fp3)
    {
        printf("no file\n");
        return -1;
    }
    fscanf (fp3, "%i\n", &n);
    int** a = (int **)malloc(n*sizeof(int *));
    for(int i=0; i<n; i++)
        a[i] = (int *)malloc(n*sizeof(int));
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
            fscanf(fp3, "%d",&a[i][j]);
    }
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
            printf(" %d ",a[i][j]);
        printf("\n");
    }

    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
        {
            if(i==(n-j-1))
                f=f*(a[i][j]);
        }
    }
    printf("\n s=%i",f);
    printf("\n");
    int * a_min_ptr = find_min(a, n);
    *a_min_ptr = f;

    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
            printf(" %d ",a[i][j]);
        printf("\n");
    }
    getchar();
    return 0;
}
