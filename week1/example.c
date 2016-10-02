#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include<sched.h>
#include<string.h>
#define NUM_THREADS 4

/* Create 4 threads.
 * In each thread keep track of approx time that it executed 
 * by using a counter.
*/  

void *threadFunc(void* arg)
{
	int id =  (int*)arg;
	for(int i = 0; i < 5; i++)
	{
	  printf("threadFunc(): %d\n", id);
	}
	pthread_exit(NULL);
}

// Check CPU affinity - ensure it can run on all available units
int getAffinity()
{
  pid_t pid = getpid();
  cpu_set_t my_set;
  CPU_ZERO(&my_set);
  int count = 0;
  //sched_getaffinity() writes the affinity mask of the process whose ID is pid into the cpu_set_t structure pointed to by mask. 
  //The cpusetsize argument specifies the size (in bytes) of mask. If pid is zero, then the mask of the calling process is returned.
  //Return Value: On success, sched_getaffinity() return 0. On error, -1 is returned, and errno is set appropriately.     
  if(!sched_getaffinity(0, sizeof(my_set), &my_set)) 
  {
    for (int j = 0; j < CPU_SETSIZE; ++j)
    {
      if (CPU_ISSET(j, &my_set))
      {
        ++count;
      }
    }
    printf("getAffinity(): pid %d affinity has %d CPUs\n", pid, count);
  }
  return count;
}

int main(void)
{	
  //Check if the number of threads created can run on all available units.
  if(getAffinity() != NUM_THREADS)
  {
    printf("Number of threads to be created is not equal to number of available units\n");
  }
  pthread_t threads[NUM_THREADS]; 
  for(int i = 0; i < NUM_THREADS; i++) 
  {
    printf("Created thread number: %d\n", i);
    if(pthread_create(&threads[i], NULL, threadFunc, (void*)i))
    {
      printf("Error in pthread_create\n");
    }
  }
  for(int i = 0; i < NUM_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
