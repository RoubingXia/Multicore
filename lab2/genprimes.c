#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
/*

 * */


void write_helper(char file_name[], int primes[]) {
    // write an array to a file
    FILE *fptr;
    size_t n = sizeof(primes) / sizeof(primes[0]);
    fptr = fopen(file_name,"w");
    printf("Hello, the size n is %d", n);
    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    for (int i = 0; i < n; ++i) {
        int line_num = i + 1;
        int prime = primes[i];
        fprintf(fptr,"%d %d\n",line_num, prime);
    }
    fclose(fptr);
}
int* getPrimes(int n) {
    // return the array of primes to n
    int isPrime[n + 1];
    int count = 0;
    for (int i = 0; i <= n; ++i) {
        isPrime[i] = 0;
    }
    isPrime[2] = 1;
    for (int p = 2; p < (n / 2); p = p * p) {
        for (int j = p; j < p * p; j += p) {
            isPrime[j] = 1;
        }
    }
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i] == 1) {
            count++;
        }
    }
    int res[count];
    int k = 0;
    for (int i = 2; i <= n; ++i) {

        if (isPrime[i] == 1) {
            res[k++] = i;
        }
    }
    return res;
}
int main(int argc, char *argv[])
{

    if(argc != 3 ){
        printf("usage: ./genprime N t\n");
        printf("N: positive number bigger than 2 \n");
        printf("t: the number of threads and is a positive integer that does not exceed 100\n");
        exit(1);
    }

    int threads_count = atoi(argv[2]);
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
