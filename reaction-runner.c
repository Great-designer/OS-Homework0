//这个文件测试reaction.c中的实现，无需修改它。请通过Piazza或电子邮件(cs140ta@cs)向我们报告任何错误。
//请注意，通过这些测试并不能保证您的代码是正确的或符合给定的规范，只能说明非常接近正确答案。
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>

#include "reaction.c"

volatile int water_molecules = 0;
volatile int hydrogen_atoms_bonded = 0;
volatile int oxygen_atoms_bonded = 0;

void make_water()
{
	__sync_add_and_fetch(&water_molecules, 1);
}

void *hydrogen_thread(void *arg)
{
	struct reaction *reaction = (struct reaction*)arg;
	reaction_h(reaction);
	__sync_add_and_fetch(&hydrogen_atoms_bonded, 1);
	return NULL;
}

void *oxygen_thread(void *arg)
{
	struct reaction *reaction = (struct reaction*)arg;
	reaction_o(reaction);
	__sync_add_and_fetch(&oxygen_atoms_bonded, 1);
	return NULL;
}

void alarm_handler(int foo)
{
	fprintf(stderr, "Error: Failed to complete after 10 seconds. Something's wrong, or your system is terribly slow.\n");
	exit(1);
}

#ifndef MIN
#define MIN(_x,_y) ((_x) < (_y)) ? (_x) : (_y)
#endif

//这里创建了一组线程来模拟氢和氧分子。希望大自然母亲在被问到的时候能足够体面地把它们联系在一起。
int main(int argc, char** argv)
{
	struct reaction reaction;
	reaction_init(&reaction);
	if (argc != 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 100)
	{
		fprintf(stderr, "usage: %s percentageOfHydrogenAtoms\n e.g.: %s 50\n", argv[0], argv[0]);
		exit(1);
	}
	srandom(getpid() ^ time(NULL));
	signal(SIGALRM, alarm_handler);
	alarm(10);
	int i;
	const int total_atoms = 200;
	int hydrogen_atoms = 0;
	int oxygen_atoms = 0;
	int hydrogen_pct = atoi(argv[1]);
	for (i = 0; i < total_atoms; i++)
	{
		pthread_t tid;
		int ret;
		if ((random() % 100) < hydrogen_pct)
		{
			hydrogen_atoms++;
			ret = pthread_create(&tid, NULL, hydrogen_thread, &reaction);
		}
		else
		{
			oxygen_atoms++;
			ret = pthread_create(&tid, NULL, oxygen_thread, &reaction);
		}
		if (ret != 0)
		{
			//如果失败了，也许我们超出了系统的限制。试着减少“总原子数”。
			perror("pthread_create");
			exit(1);
		}
	}
	int expected_molecules = MIN(hydrogen_atoms / 2, oxygen_atoms);
	printf("Created %d H and %d O atoms (%.1f%% H), expecting %d H2O molecules\n",hydrogen_atoms, oxygen_atoms, (double)hydrogen_atoms / total_atoms * 100.0,expected_molecules);
	//等待预期的反应完成。
	while (water_molecules != expected_molecules);
	//再等一会儿，以防实现有问题，我们看到太多的反应。
	usleep(100000);
	if (water_molecules != expected_molecules)
	{
		fprintf(stderr, "Error: expected %d H20 molecules, but %d were created!\n",expected_molecules, water_molecules);
		exit(1);
	}
	if (hydrogen_atoms_bonded != (expected_molecules * 2))
	{
		fprintf(stderr, "Error: expected %d reaction_h() calls to return, but %d did instead!\n", expected_molecules * 2,hydrogen_atoms_bonded);
		exit(1);
	}
	if (oxygen_atoms_bonded != expected_molecules)
	{
		fprintf(stderr, "Error: expected %d reaction_o() calls to return, but %d did instead!\n", expected_molecules, oxygen_atoms_bonded);
		exit(1);
	}
	printf("Looks good!\n");
	return 0;
}
