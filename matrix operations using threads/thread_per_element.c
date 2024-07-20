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
#define result_file "c3.txt"
int a[max][max];
int b[max][max];
int c[max][max];
int arows, acols,brows,bcols,crows,ccols;
typedef struct dimention{
    int row;
    int col;
}Dimen;
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
void * mul_per_element(void* arguments){
    int MUL = 0;
    Dimen current_element = *(Dimen *)arguments;
    int i;
    for(i=0 ; i < acols ; i++){
            MUL+= a[current_element.row][i] *b[i][current_element.col];
        }
        c[current_element.row][current_element.col] =MUL;
        MUL = 0;
    }
void thread_fn()
{
    struct timeval stop, start;
    gettimeofday(&start, NULL); 
    //----------------------
    int element=0;
   pthread_t thread[arows*bcols];
    for(int i = 0 ; i < arows ; i++){
        for(int j=0;j<bcols;j++)
        {
        Dimen *args = malloc(sizeof(Dimen));
        args->row=i;
        args->col=j;
        pthread_create(&thread[element] , NULL , &mul_per_element, args);
        element++;
        
        }
    }
    for(int i = 0 ; i < arows; i++){
        pthread_join(thread[i] , NULL);
    }
    printf("number of threads = %d \n",element);
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
    fprintf(file , "<thread per element>\nrows=%d cols=%d\n",crows, ccols);
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
//gcc tread_per_element.c -o tread_per_element.o -pthread && ./tread_per_element.o