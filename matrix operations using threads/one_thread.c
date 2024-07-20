#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#define max 20
#define first_file "a.txt"
#define second_file "b.txt"
#define result_file "c1.txt"
int a[max][max];
int b[max][max];
int c[max][max];
int arows, acols,brows,bcols,crows,ccols;

void read_from_file_a()
{
    FILE *file;
    char filename[] = first_file;
    int f=0;
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening first file t %s\n", filename);
        exit(1);
    }
    else{
       fscanf(file, "row=%d col=%d", &arows, &acols);
        for (int i = 0; i < arows; i++) {
        for (int j = 0; j < acols; j++) {
            fscanf(file, "%d", &a[i][j]);
        }
        }
        
    }
    crows=arows;
    fclose(file);
}
void read_from_file_b()
{
    FILE *file;
    char filename[] = second_file;
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening second file  %s\n", filename);
        exit(1);
    }
    else{
        fscanf(file, "row=%d col=%d", &brows, &bcols);
        for (int i = 0; i < brows; i++) {
        for (int j = 0; j < bcols; j++) {
            fscanf(file, "%d", &b[i][j]);
        }
        }
    }
    ccols=bcols;
    fclose(file);
}
void check_dimention()
{
     if(acols != brows)
    {
        printf("Error not matched dimentions\n");
        exit(0);
    }
}
void matrix_mul()
{
    int i,j,k,mul=0;
    for(i=0;i<arows;i++)
    {
        for(j=0;j<bcols;j++)
        {
            for(k=0;k<acols;k++)
            {
               mul+=a[i][k]*b[k][j];
            }
            c[i][j]=mul;
            mul=0;
        }
    }

}
void write_result_in_file()
{
    FILE *file;
    char filename[] = result_file;
    file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(file, "Error opening result file %s\n", filename);
        exit(0);
    }
    else{
    fprintf(file , "<one thread>\nrows=%d cols=%d\n",crows, ccols);
    for (int i = 0; i < crows; i++) {
        for (int j = 0; j < ccols; j++) {
            fprintf(file,"%d ", c[i][j]);
        }
        fprintf(file,"\n");
    }
    }
   fclose(file);
}
int main()
{
    struct timeval stop, start;

    gettimeofday(&start, NULL); //start checking time
    //----------------------
    read_from_file_a();
    read_from_file_b();
    check_dimention();
    matrix_mul();
    write_result_in_file();
    //---------------------------
    gettimeofday(&stop, NULL); //end checking time
    
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
}
// gcc one_thread.c -o one_thread.o && ./one_thread.o