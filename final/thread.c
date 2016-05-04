#include "types.h"
#include "mmu.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "fs.h"
#include "proc.h"
#include "x86.h"

int
pthread_create(void * (*start_routine)(void *), void * arg){
	// a thread needs its own stack
  void *user_stack = malloc(PGSIZE);
	// call clone with the stack, and return the pid  
  int pid = clone(user_stack, PGSIZE);
  
  // if this is the child, then do the function
  if (pid == 0)
  {
		// call the thread function with argument
    (*start_routine)(arg);
		// free the stack after complete
    free(user_stack);
		// exit so parent can take care of zombie.
    exit();
  }
  // otherwise return the actual pid
  else
  {
    return pid;
  }
}

int
pthread_join(void)
{
  return join();
}

