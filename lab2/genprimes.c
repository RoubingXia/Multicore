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
    int* candidates = (int*)malloc(sizeof(int)*(n + 1));
    for (int i = 0; i <= n; ++i) {
        candidates[i] = 1;
    }
    for (int p = 2; p <= ((n + 1) / 2); ++p) {
        if (candidates[p] == 1) {
            for (int j = p; j <= n; j += p) {
                if (j == p) continue;
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
    if (!res) {
        printf("allocate failed");
        exit(2);
    }
    int k = 0;
    for (int i = 2; i <= n; ++i) {
        if (candidates[i] == 1) {
            res[k++] = i;
        }
    }

    return res;
}


void getPrimesM(int n, int* size, int threads_count, int* res) {
    // each thread hand le a chunk of input range
    int* candidates = (int*)malloc(sizeof(int)*(n + 1));
    int res_count = 0; // total prime numbers
    int step = n / threads_count;
    // get the starting point for each thread
    int len1 = 0;// length of start_points
    int* start_points = getPrimes(600, &len1); // find the first 100 primes, use them as starting points for each thread
    int local_count[threads_count]; // use to count the number of primes in each chunk
    #pragma omp parallel num_threads(threads_count)
    {
        int tid = omp_get_thread_num();
        int start = tid * step;
        int end = (tid == threads_count - 1) ? n : (tid + 1) * step;
        int first_prime = start_points[tid]; // the first prime in current chunk
        for (int i = start; i <= end; ++i) {
            candidates[i] = 1;
        }
        for (int p = first_prime; p <= ((end + 1) / 2); ++p) {
            if (candidates[p] == 1) {
                for (int j = p; j <= end; j += p) {
                    if (j == p) continue;
                    candidates[j] = 0;
                }
            }
        }
        for (int i = start; i <= end; ++i) {
            if (i < 2) continue;
            if (candidates[i] == 1) local_count[tid]++;
        }
        // get total count
        #pragma omp critical
        {
            for (int i = 0; i < threads_count; ++i) {
                res_count += local_count[i];
            }
            res = (int*)malloc(sizeof(int) * res_count);
            // copy to res, how to parallelize this part?
            int k = 0;
            for (int i = 2; i <= n; ++i) {
                if (candidates[i] == 1) {
                    res[k++] = i;
                }
            }
        }
        /*
        // Wait so, ever thread will have a right res_count
        #pragma omp barrier
        int k = ;
        for (int i = 2; i <= n; ++i) {
            if (candidates[i] == 1) {
                res[k++] = i;
            }
        }
        */
    }


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
    int threads_count = atoi(argv[2]);
    int primeN = atoi(argv[1]);
    int size_n = 0;
    int* res;
    // get start time
    tstart = omp_get_wtime();
    // parallel program start here
    if (threads_count == 0) {
        //sequential
        res = getPrimes(primeN, &size_n);

    }
    else {
        getPrimesM(primeN, &size_n, threads_count, res);
    }
    ttaken = omp_get_wtime() - tstart;
    printf("Time take for the main part: %f\n", ttaken);
    write_helper("output", res, size_n);
    return 0;
}
