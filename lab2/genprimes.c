#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
/*

 * */


void write_helper(char file_name[], int primes[], int n) {
    // write an array to a file
    FILE *fptr;
    fptr = fopen(file_name,"w");
    //printf("Hello, the size n is %d\n", n);
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

int* getPrimes(int n, int* size) {
    /*
    1. Generate all numbers from 2 to N.
    2. First number is 2, so remove all numbers that are multiple of 2 (i.e. 4, 6, 8, ... N). Do not
    remove the 2 itself.
    3. Following number is 3, so remove all multiple of 3 that have not been removed from the
    previous step. That will be: 9, 15, ... till you reach N.
    4. The next number that has not been crossed so far is 5. So, remove all multiple of 5 that
    have not been crossed before, till you reach N.
    5. Continue like this till floor((N+1)/2).
    6. The remaining numbers are the prime numbers.
    */
    int candidates[n + 1];
    for (int i = 0; i <= n; ++i) {
        candidates[i] = 1;
    }
    for (int p = 2; p <= (n + 1) / 2; ++p) {
        if (candidates[p] == 1) {
            for (int j = p * p; j <= n; j += p) {
                candidates[j] = 0;
            }
        }
    }
    int count = 0;
    for (int i = 2; i <= n; ++i) {
        if (candidates[i] == 1) count++;
    }
    *size = count;
    int* res = (int*) malloc(sizeof(int) * count);
    return res;
}
int* getPrimes2(int n, int* size) {
    // return the array of primes to n


    int isPrime[n + 1];
    int count = 0;
    for (int i = 0; i <= n; ++i) {
        isPrime[i] = 1;
    }
    for (int p = 2; p * p <= n; p++) {
        if (isPrime[p] == 1) {
            for (int j = p * p; j <= n; j += p) {
                isPrime[j] = 0;
            }
        }
    }
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i] == 1) {
            count++;
        }
    }
    int* res = (int*)malloc(sizeof(int) * count);
    int k = 0;
    for (int i = 2; i <= n; ++i) {

        if (isPrime[i] == 1) {
            res[k++] = i;
        }
    }
    *size = count;
    return res;
}
int main(int argc, char *argv[])
{

    double tstart = 0.0, tend=0.0, ttaken;
    if(argc != 3 ){
        printf("usage: ./genprime N t\n");
        printf("N: positive number bigger than 2 \n");
        printf("t: the number of threads and is a positive integer that does not exceed 100\n");
        exit(1);
    }
/*
 * double tstart = 0.0, tend=0.0, ttaken;
Read the input from the command line
tstart = omp_get_wtime();
Generate the prime numbers (as indicated by the algorithm above)This will be the parallel part ttaken = omp_get_wtime() - t_start;
printf(“Time take for the main part: %f\n”, ttaken);
Write the output file and exit.
 *
 * */
    int threads_count = atoi(argv[2]);
    int primeN = atoi(argv[1]);
    // get start time
    tstart = omp_get_wtime();
    // parallel program start here
    if (threads_count == 0) {
        //sequential
        //int test_arr[] = {2,3,5,7};
        //int size_n = sizeof(test_arr) / sizeof(test_arr[0]);
        int size_n = 0;
        int* res = getPrimes(primeN, &size_n);
        write_helper("output", res, size_n);
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
    ttaken = omp_get_wtime() - tstart;
    printf("Time take for the main part: %f\n", ttaken);
    return 0;
}
