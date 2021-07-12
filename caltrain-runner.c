//这个文件测试caltrain.c中的实现，无需修改它。请通过Piazza或电子邮件(cs140ta@cs)向我们报告任何错误。
//请注意，通过这些测试并不能保证您的代码是正确的或符合给定的规范，只能说明非常接近正确答案。
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>

#include "caltrain.c"

//已完成的乘客线程计数（即station_wait_for_train已返回），并正在等待station_on_board()调用。
volatile int threads_completed = 0;

void *passenger_thread(void *arg)
{
	struct station *station = (struct station*)arg;
	station_wait_for_train(station);
	__sync_add_and_fetch(&threads_completed, 1);
	return NULL;
}

struct load_train_args
{
	struct station *station;
	int free_seats;
};

volatile int load_train_returned = 0;

void *load_train_thread(void *args)
{
	struct load_train_args *ltargs = (struct load_train_args*)args;
	station_load_train(ltargs->station, ltargs->free_seats);
	load_train_returned = 1;
	return NULL;
}

const char* alarm_error_str;
int alarm_timeout;

void _alarm(int seconds, const char *error_str)
{
	alarm_timeout = seconds;
	alarm_error_str = error_str;
	alarm(seconds);
}

void alarm_handler(int foo)
{
	fprintf(stderr, "Error: Failed to complete after %d seconds. Something's wrong, or your system is terribly slow. Possible error hint: [%s]\n",alarm_timeout, alarm_error_str);
	exit(1);
}

#ifndef MIN
#define MIN(_x,_y) ((_x) < (_y)) ? (_x) : (_y)
#endif

//这里创建一组线程来模拟到达的火车和乘客。
int main()
{
	struct station station;
	station_init(&station);
	srandom(getpid() ^ time(NULL));
	signal(SIGALRM, alarm_handler);
	//如果没有候车乘客，请确保station_load_train()立即返回。
	_alarm(1, "station_load_train() did not return immediately when no waiting passengers");
	station_load_train(&station, 0);
	station_load_train(&station, 10);
	_alarm(0, NULL);
	//创建一堆“乘客”，每个人都有自己的线程。
	int i;
	const int total_passengers = 1000;
	int passengers_left = total_passengers;
	for (i = 0; i < total_passengers; i++)
	{
		pthread_t tid;
		int ret = pthread_create(&tid, NULL, passenger_thread, &station);
		if (ret != 0)
		{
			//如果失败了，也许我们超出了系统的限制。尝试减少“总乘客”。
			perror("pthread_create");
			exit(1);
		}
	}
	//如果没有空位，请确保station_load_train()立即返回。
	station_load_train(&station, 0);
	_alarm(0, NULL);
	//大量随机测试。
	int total_passengers_boarded = 0;
	const int max_free_seats_per_train = 50;
	int pass = 0;
	while (passengers_left > 0)
	{
		_alarm(2, "Some more complicated issue appears to have caused passengers not to board when given the opportunity");
		int free_seats = random() % max_free_seats_per_train;
		printf("Train entering station with %d free seats\n", free_seats);
		load_train_returned = 0;
		struct load_train_args args = { &station, free_seats };
		pthread_t lt_tid;
		int ret = pthread_create(&lt_tid, NULL, load_train_thread, &args);
		if (ret != 0)
		{
			perror("pthread_create");
			exit(1);
		}
		int threads_to_reap = MIN(passengers_left, free_seats);
		int threads_reaped = 0;
		while (threads_reaped < threads_to_reap)
		{
			if (load_train_returned)
			{
				fprintf(stderr, "Error: station_load_train returned early!\n");
				exit(1);
			}
			if (threads_completed > 0)
			{
				if ((pass % 2) == 0)
					usleep(random() % 2);
				threads_reaped++;
				station_on_board(&station);
				__sync_sub_and_fetch(&threads_completed, 1);
			}
		}
		//再等一会儿。给station_load_train()一个返回的机会，并确保没有其他乘客上车。一秒钟应该是很长的时间，但如果你在一个严重超载的系统上，这可能需要调整。
		for (i = 0; i < 1000; i++)
		{
			if (i > 50 && load_train_returned)
				break;
			usleep(1000);
		}
		if (!load_train_returned)
		{
			fprintf(stderr, "Error: station_load_train failed to return\n");
			exit(1);
		}
		while (threads_completed > 0)
		{
			threads_reaped++;
			__sync_sub_and_fetch(&threads_completed, 1);
		}
		passengers_left -= threads_reaped;
		total_passengers_boarded += threads_reaped;
		printf("Train departed station with %d new passenger(s) (expected %d)%s\n",threads_reaped, threads_to_reap,(threads_to_reap != threads_reaped) ? " *****" : "");
		if (threads_to_reap != threads_reaped)
		{
			fprintf(stderr, "Error: Too many passengers on this train!\n");
			exit(1);
		}
		pass++;
	}
	if (total_passengers_boarded == total_passengers)
	{
		printf("Looks good!\n");
		return 0;
	}
	else
	{
		//我不认为这是可达的，但以防万一。
		fprintf(stderr, "Error: expected %d total boarded passengers, but got %d!\n",total_passengers, total_passengers_boarded);
		return 1;
	}
}
