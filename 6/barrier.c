#include "barrier.h"
#include <stdio.h>

typedef struct barrier_t barrier_t;

static int var = 0;
static barrier_t barrier;

static unsigned int all_threads_reached(barrier_t* barrier);
void barrier_init(barrier_t *barrier, int count);
void barrier_wait(barrier_t *barrier, int id);
void* thread_func(void* args);

void barrier_init(barrier_t *barrier, int count)
{
	barrier->count = count;

	int i;
	for(i=0; i<count; i++)
	{
		barrier->reached[i] = 0;
	}

	pthread_mutex_init(&barrier->mutex, NULL);

	for(i=0; i<count; i++)
	{
		pthread_cond_init(&barrier->cond[i], NULL);
	}	
}

void barrier_wait(barrier_t *barrier, int id)
{
	pthread_mutex_lock(&barrier->mutex);

	barrier->reached[id] = 1;

	// check if all the threads have reached.
	
	if(all_threads_reached(barrier))
	{
		int i;
		for(i=0; i<barrier->count; i++)
		{
			pthread_cond_signal(&barrier->cond[i]);
		}
		pthread_mutex_unlock(&barrier->mutex);
	}
	// otherwise put this thread to sleep
	else
	{
		while(!all_threads_reached(barrier))
		{
			pthread_cond_wait(&barrier->cond[id], &barrier->mutex);
			pthread_mutex_unlock(&barrier->mutex);
		}		
	}
}

void* thread_func(void* args)
{
	int thread_id = *((int*)args);
	printf("%d %d\n", thread_id, var++);
	barrier_wait(&barrier, thread_id);
	printf("%d %d\n", thread_id, var++);
}

int main()
{
	barrier_init(&barrier, BARRIER_MAX_THREADS);

	int i;
	int indexes[BARRIER_MAX_THREADS];
	for(i=0; i<BARRIER_MAX_THREADS; i++)
	{
		indexes[i] = i;
	}

	pthread_t p[BARRIER_MAX_THREADS];
	for(i=0; i<BARRIER_MAX_THREADS; i++)
	{
		pthread_create(&p[i], NULL, &thread_func, &indexes[i]);
	}

	for (i = 0; i < BARRIER_MAX_THREADS; i++)
	{
		pthread_join(p[i], NULL);
	}
}

static unsigned int all_threads_reached(barrier_t* barrier)
{
	int i;
	for(i=0; i<barrier->count; i++)
	{
		if(barrier->reached[i] != 1)
		{
			return 0;
		}
	}
	return 1;
}


