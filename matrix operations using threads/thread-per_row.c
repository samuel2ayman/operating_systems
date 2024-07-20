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
#define result_file "c2.txt"
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
        fprintf(stderr, "Error opening first file %s\n", filename);
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
        fprintf(stderr, "Error opening second file %s\n", filename);
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
void * mul_per_row(void* arguments){
   int MUL = 0;
    int row = *(int *)arguments;
    for(int i = 0 ; i < bcols ; i++){
        for(int j = 0 ; j < acols ; j++){
            MUL+= a[row][j] *b[j][i];
        }
        c[row][i] =MUL;
        MUL = 0;
    }
}
void thread_fn()
{
    struct timeval stop, start;
    gettimeofday(&start, NULL); 
    //----------------------
    int i;
   pthread_t thread[arows];
    for( i = 0 ; i < arows ; i++){
        int* arg = malloc(sizeof(int)*20);
        *arg = i;
        pthread_create(&thread[i] , NULL , &mul_per_row, arg) ;
        }
    
    printf("number of threads = %d \n",i);
    for(int i = 0 ; i < arows; i++){
        pthread_join(thread[i] , NULL);
    }
    //---------------------------
    gettimeofday(&stop, NULL); 
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
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
    fprintf(file , "<thread per row>\nrows=%d cols=%d\n",crows, ccols);
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
    read_from_file_a();
    read_from_file_b();
    check_dimention();
    thread_fn();
    write_result_in_file();
}
//gcc thread_per_row.c -o thread_per_row.o -pthread && ./thread_per_row.o