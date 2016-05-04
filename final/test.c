#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "param.h"

#define NUM_THREADS 5

struct pthread_mutex_t lock;

void *
function(void* arg)
{
  pthread_mutex_lock((struct pthread_mutex_t *)&lock);
  int i;
  int* x = (int*)arg;
  for(i=0; i<100000000; i++)
  {
    (*x)++;
  }
  printf(1, "%d\n", *x);
  pthread_mutex_unlock((struct pthread_mutex_t *)&lock);
  return 0;
}

int
main(int argc, char *argv[])
{
  pthread_mutex_init((struct pthread_mutex_t *)&lock);
  int x = 0;
  int i;
  for(i=0; i<NUM_THREADS; i++)
  {
    pthread_create(function, (void *)&x);
  }
  
  for(i=0; i<NUM_THREADS; i++)
  {
    pthread_join();
  }
  printf(1, "%d\n", x);

  exit();
}



