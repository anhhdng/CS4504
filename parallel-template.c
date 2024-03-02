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



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 10

char *s1;
char *s2;
int n1,n2;

pthread_mutex_t mutex;
pthread_t threads[NUM_THREADS];
int thread_id[NUM_THREADS];
int count_for_each_thread[NUM_THREADS];

void *count(void *arg) {
    int start = *(int*)(arg);
    // start is the starting point for that index
    for(int i=start;i<n1;i+=NUM_THREADS){ 
        if(i+n2-1>=n1){
            break;
        }
        int flag = 0;
        for(int j=0;j<n2;j++){ // the inner loop just checks whether the substring of length n2 starting from index i is equal to s2
            if(s1[i+j]!=s2[j]){
                flag = 1;
                break;
            }
        }
        if(!flag){
            // if an equal substring is found increment the count for that thread
            count_for_each_thread[start]++;
        }
    } 
}

int main(int argc, char *argv[]) {
    // input is taken from user.
    scanf("%d %d",&n1,&n2);
    s1 = (char *)malloc(n1*sizeof(char)+1);
    s2 = (char *)malloc(n2*sizeof(char)+1);
    scanf("%s",s1); 
    scanf("%s",s2);
    pthread_mutex_init(&mutex, NULL);
    for(int i=0;i<NUM_THREADS;i++){
        thread_id[i]=i;
    }
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, count, &thread_id[i]);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }
    int sum = 0;
    for(int i=1;i<=10;i++){
        printf("The number of substrings thread %d find is %d\n",i,count_for_each_thread[i-1]);
        sum+=count_for_each_thread[i-1];
    }
    printf("Total substrings is %d\n", sum);
}







