#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX 10240
#define NUM_THREADS  10

int n1,n2;
char *s1,*s2;
FILE *fp;
int countArray[NUM_THREADS]={0};


//read input file and generate string s1/s2 and length n1/n2
int readf(FILE *fp)
{
	if((fp=fopen("strings.txt", "r"))==NULL){
		printf("ERROR: can't open string.txt!\n");
		return 0;
	}
	s1=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory!\n");
		return -1;
	}
	s2=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory\n");
		return -1;
	}
	/*read s1 s2 from the file*/
	s1=fgets(s1, MAX, fp);
	s2=fgets(s2, MAX, fp);
	n1=strlen(s1);  /*length of s1*/
	n2=strlen(s2)-1; /*length of s2*/

	if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
		return -1;
	return 0;
}

// Function to count substrings for a specific thread
int num_substring(int t)
{
    int segment_size = n1 / NUM_THREADS;
    int start = t * segment_size;
    int end = (t == NUM_THREADS - 1) ? n1 - n2 : (t + 1) * segment_size - n2;
    int count = 0;

    for (int i = start; i <= end; i++)
    {
        int j;
        for (j = 0; j < n2; j++)
        {
            if (s1[i + j] != s2[j])
                break;
        }
        if (j == n2)
            count++;
    }

    return count;
}
void *calSubStringThread(void *threadid){
    long tid = (long)threadid;
    printf("This is thread %ld, ", tid);
    int num = num_substring(tid);
    printf("find num of is: %d\n", num);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int t, rc;
    int totalNum = 0;

	readf(fp);

	for(t=0; t<NUM_THREADS; t++){
        rc = pthread_create(&threads[t], NULL, calSubStringThread, (void *) (size_t)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t=0; t<NUM_THREADS; t++){
        pthread_join(threads[t], NULL);
    }

 	printf("The number of substrings is: %d\n", totalNum);
	return 1;
}
https://kevinsuo.github.io/teaching/2023Fall/3502/Project/project2.pdf










