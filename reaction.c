#include "pintos_thread.h"

// 转发声明。这个函数是在reaction-runner.c中实现的，但您不必关心它的作用。
// 只要确保在reaction_o()/reaction_h()中适当地调用它。
void make_water();

struct reaction//一个反应容器中
{
	struct lock Lock;
	struct condition Arrival;//H原子到达变量
	struct condition Occurred;//O原子产生变量
	int Atoms;//H原子总数。有可能H总数少于2
	int Needed;//O原子需要的H原子数。有可能O总数少于1
};

void reaction_init(struct reaction *reaction)
{
	lock_init(&reaction->Lock);
	cond_init(&reaction->Arrival);
	cond_init(&reaction->Occurred);
	reaction->Atoms=0;
	reaction->Needed=0;
}

void reaction_h(struct reaction *reaction)//每个H原子调用函数
{
	lock_acquire(&reaction->Lock);//上锁
	reaction->Atoms++;//获得一个H原子，反应容器中H原子总数增加
	cond_signal(&reaction->Arrival,&reaction->Lock);//唤醒一个H原子到达变量上的线程
	while(reaction->Needed==0)//只要不需要H原子
	{
		cond_wait(&reaction->Occurred,&reaction->Lock);//等待O原子产生变量，等待需要H原子
	}
	reaction->Needed--;//只要需要H原子，就消耗自己
	lock_release(&reaction->Lock);//解锁
}

void reaction_o(struct reaction *reaction)//每个O原子调用函数
{
	lock_acquire(&reaction->Lock);//上锁
	while(reaction->Atoms<2)//只要反应容器中H原子数少于2个
	{
		cond_wait(&reaction->Arrival,&reaction->Lock);//等待H原子到达变量，等待H原子数多于2个
	}
	reaction->Needed+=2;//需要H原子数增加2
	reaction->Atoms-=2;//H原子总数减少2
	cond_broadcast(&reaction->Occurred,&reaction->Lock);//唤醒所有O原子产生变量上的线程
	make_water();//执行化学反应
	lock_release(&reaction->Lock);//解锁
}
