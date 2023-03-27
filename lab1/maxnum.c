
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
/*
 * For the OpenMP version, the one with four threads, you need to do the following (Note: Some details are left on purpose for you to figure out):
• Load the whole file in an array of characters (i.e. string). Assume its name is: list[]
• Each thread will be responsible for N/4 characters. You need to take care of the
corner case when N is not divisible by 4.
• Assume N/4 = step
• Thread 0 will take care of characters from 0 to step-1. Thread 1 will take care of
characters from step to 2*step – 1, and so one.
• Let each thread have its own array of four int where it puts the count of the four
characters.
• At the end, combine the four arrays into one.
• Finally, pick the largest one and print the output.

 x occurred the most y times of a total of z characters.
 * */



int main(int argc, char *argv[])
{
    int count = 0; // the number of characters in the file
    int threads_count = 0; //thread number
    char *buffer = (char*) malloc(100000000 * sizeof(char));
    char x = 'a'; // the char occurs most frequent
    int y = 0; // how many times x occurred
    int map[4] = {0,0,0,0}; // hashmap store occurrence of each letter
    int i = 0;
    size_t bytes_read;
    FILE * fp;

    if(argc != 4 ){
        printf("usage: ./maxnum N num filenmae\n");
        printf("N: thread number \n");
        printf("num: the number of characters in the file \n");
        printf("name: the name of the file to be generated\n");
        exit(1);
    }

    if( (fp = fopen(argv[3],"r")) == NULL )
    {
        printf("Cannot create the file %s\n", argv[2]);
        exit(1);
    }
    count = atoi(argv[2]);
    threads_count = atoi(argv[1]);
    bytes_read = fread(buffer, sizeof(char), count, fp); // read fime into buffer
    buffer[bytes_read] = '\0'; // set end

    // parallel program start here
    if (threads_count == 0) {
        //sequential
        while(buffer[i] != '\0') {
            map[buffer[i] - 'a']++;
            ++i;
        }
    }
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
    return 0;
}
