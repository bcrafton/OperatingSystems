struct stat;
struct rtcdate;

// Mutual exclusion lock.
struct pthread_mutex_t {
  uint locked;       // Is the lock held?
  
  // For debugging:
  //char *name;        // Name of lock.
  //struct cpu *cpu;   // The cpu holding the lock.
  //uint pcs[10];      // The call stack (an array of program counters)
                     // that locked the lock.
};

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(char*, int);
int mknod(char*, short, short);
int unlink(char*);
int fstat(int fd, struct stat*);
int link(char*, char*);
int mkdir(char*);
int chdir(char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int clone(void *, int);
int join(void);

// ulib.c
int stat(char*, struct stat*);
char* strcpy(char*, char*);
void *memmove(void*, void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, char*, ...);
char* gets(char*, int max);
uint strlen(char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

//thread.c
int pthread_create(void *(*)(void*), void *); 
int pthread_join(void);

//threadlock.c
void pthread_mutex_init(struct pthread_mutex_t*);
void pthread_mutex_lock(struct pthread_mutex_t*);
void pthread_mutex_unlock(struct pthread_mutex_t*);

