#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
/*

 * */


void write_helper(char file_name[], int primes[]) {
    // write an array to a file
    FILE *fptr;

    fptr = fopen(file_name,"w");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    for (int i = 0; i < primes.size(); ++i) {
        int line_num = i;
        int prime = primes[i];
        fprintf(fptr,"%d %d\n",line_num, prime);
    }
    fclose(fptr);
}
int main(int argc, char *argv[])
{

    if(argc != 3 ){
        printf("usage: ./genprime N t\n");
        printf("N: positive number bigger than 2 \n");
        printf("t: the number of threads and is a positive integer that does not exceed 100\n");
        exit(1);
    }

    int threads_count = argc[2];
    // parallel program start here
    if (threads_count == 0) {
        //sequential
        int test_arr[] = {2,3,5,7};
        write_helper("output", test_arr);
    }
    /*
    else {
        int step = count / threads_count;
        #pragma omp parallel num_threads(threads_count)
        {
            int tid = omp_get_thread_num();
            int start = tid * step;
            int end = (tid == threads_count - 1) ? count : (tid + 1) * step;
            int local_map[4] = {0, 0, 0, 0};
            int k = 0;
            for (k = start; k < end; k++) {
                local_map[buffer[k] - 'a']++;
            }
            //printf("I am thread %d, I process from %d to %d, my count for letter c is %d\n", tid, start, end, local_map[2]);
        #pragma omp critical
            {
                for (i = 0; i < 4; i++) {
                    map[i] += local_map[i];
                }
            }
        }
    }
    fclose(fp);
    // find max
    int j = 0;
    for (j = 0; j < 4; ++j) {
        if (map[j] > y) {
            y = map[j];
            x = (char)(j + 'a');
        }
    }
    // print output
    printf(" %c occurred the most %d times of a total of %d characters.\n", x, y, count);
    free(buffer);
    */
    return 0;
}
