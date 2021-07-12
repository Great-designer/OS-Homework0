//对CS140 PS1（2012年冬季）兼容Pintos API的pthread(3)调用的普通包装。
//在Unix/Linux/OSX下运行，只需在代码中包含#include此头文件，并使用“-lpthread”标志进行编译，例如：cc -o myBinary myCode.c -lpthread
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

//确保锁和条件变量在使用前被初始化。
#define LOCK_COND_INIT_MAGIC 0x4a3acd94

struct lock
{
	int init;
	pthread_mutex_t mutex;
};

struct condition
{
	int init;
	pthread_cond_t cond;
};

//如果其中任何一个没有用过，就把它挂在-Wall。
static void lock_init(struct lock *lock) __attribute__((unused));
static void lock_acquire(struct lock *lock) __attribute__((unused));
static void lock_release(struct lock *lock) __attribute__((unused));
static void cond_init(struct condition *cond) __attribute__((unused));
static void cond_wait(struct condition *cond, struct lock *lock) __attribute__((unused));
static void cond_signal(struct condition *cond, struct lock *lock) __attribute__((unused));
static void cond_broadcast(struct condition *cond, struct lock *lock) __attribute__((unused));

static void __assert(int satisfied, const char *message)
{
	if (!satisfied)
	{
		fprintf(stderr, "FATAL ERROR: %s\n", message);
		exit(1);
	}
}

static void lock_init(struct lock *lock)
{
	lock->init = LOCK_COND_INIT_MAGIC;
	pthread_mutex_init(&lock->mutex, NULL);
}

static void lock_acquire(struct lock *lock)
{
	__assert(lock->init == LOCK_COND_INIT_MAGIC,"lock_acquire used before lock was initialised!");
	pthread_mutex_lock(&lock->mutex);
}

static void lock_release(struct lock *lock)
{
	__assert(lock->init == LOCK_COND_INIT_MAGIC,"lock_release used before lock was initialised!");
	__assert(pthread_mutex_trylock(&lock->mutex) != 0,"lock_release on unlocked lock!");
	pthread_mutex_unlock(&lock->mutex);
}

static void cond_init(struct condition *cond)
{
	cond->init = LOCK_COND_INIT_MAGIC;
	pthread_cond_init(&cond->cond, NULL);
}

static void cond_wait(struct condition *cond, struct lock *lock)
{
	//断言锁已被持有。
	//它应该由这个调用者持有。
	//这个简单的检查不会捕获所有错误的调用（其他线程也可以持有它，不一定是调用者），但至少它是一个开始……
	__assert(pthread_mutex_trylock(&lock->mutex) != 0,"cond_wait not called with lock held!");
	__assert(cond->init == LOCK_COND_INIT_MAGIC,"cond_wait used before cond was initialised!");
	__assert(lock->init == LOCK_COND_INIT_MAGIC,"cond_wait used before lock was initialised!");
	pthread_cond_wait(&cond->cond, &lock->mutex);
}

static void cond_signal(struct condition *cond, struct lock *lock)
{
	//请参阅cond_wait()中的注释。
	__assert(pthread_mutex_trylock(&lock->mutex) != 0,"cond_signal not called with lock held!");
	__assert(cond->init == LOCK_COND_INIT_MAGIC,"cond_signal used before cond was initialised!");
	__assert(lock->init == LOCK_COND_INIT_MAGIC,"cond_signal used before lock was initialised!");
	pthread_cond_signal(&cond->cond);
}

static void cond_broadcast(struct condition *cond, struct lock *lock)
{
	//请参阅cond_wait()中的注释。
	__assert(pthread_mutex_trylock(&lock->mutex) != 0,"cond_broadcast not called with lock held!");
	__assert(cond->init == LOCK_COND_INIT_MAGIC,"cond_broadcast used before cond was initialised!");
	__assert(lock->init == LOCK_COND_INIT_MAGIC,"cond_broadcast used before lock was initialised!");
	pthread_cond_broadcast(&cond->cond);
}
