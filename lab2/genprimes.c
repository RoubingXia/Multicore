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

int findNext(int thread_num, int thread_count, int n, int cur, int* candidates) {
    // This function should find the next unchecked prime number for thread_num
    // Algorithm: 1. Start from cur + 1, iterate the candidates every time meet a candidates[i] == 1, do count++
    //            2. When count == (thread_count - thread_num), that means this is the next start prime for thread_num
    int count = 0;
    int dest = thread_count - thread_num;
    int next = cur;
    while(count < dest) {
        next++;
        if (candidates[next] == 1) {
            count++;
        }
    }
    return next == cur ? -1 : next;// -1 means no primes left for check
}
void getPrimesM(int n, int* size, int threads_count, int* res) {
    // each thread hand le a chunk of input range
    int* candidates = (int*)malloc(sizeof(int)*(n + 1));
    int res_count = 0; // total prime numbers
    int step = n / threads_count;
    // get the starting point for each thread
    int len1 = 0;// length of start_points
    int* start_points = getPrimes(600, &len1); // find the first half primes, use them as starting points for each thread
    int local_count[threads_count]; // use to count the number of primes in each chunk
    #pragma omp parallel num_threads(threads_count)
    {
        int tid = omp_get_thread_num();
        int start = tid * step;
        int end = (tid == threads_count - 1) ? n : (tid + 1) * step;
        local_count[tid] = 0;// initialize own counter
        for (int i = start; i <= end; ++i) {
            candidates[i] = 1;
        }
        // all threads should wait after all numbers get initialized to 1
        #pragma omp barrier
        // each thread fill different part of the table, e.g if two threads process N = 25, t0 will start with 2, fill
        // all numbers that are dividable by 2, and t1 start from 3, when t0 finished with 2, it should call findNext to
        // get the number 5, and check all numbers dividable by 5. before t0 call findNext should it wait?
        int first_prime = start_points[tid];
        while(first_prime != -1) {
            for (int p = first_prime; p <= ((n + 1) / 2); ++p) {
                if (candidates[p] == 1) {
                    for (int j = p; j <= n; j += p) {
                        if (j == p) continue;
                        candidates[j] = 0;
                    }
                }
            }
            // findNext prime
            first_prime = findNext(tid, threads_count, n, first_prime, candidates);
        }

        // wait till the table been filled completely, then calculate the count
        #pragma omp barrier
        for (int i = start; i <= end; ++i) {
            if (i < 2) continue;
            if (candidates[i] == 1) local_count[tid]++;
        }
        // get total count
        #pragma omp barrier
        #pragma omp single
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
    }
    free(start_points);
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
    free(res);
    return 0;
}
