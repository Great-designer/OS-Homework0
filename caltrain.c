#include "pintos_thread.h"

struct station//车站结构体
{
	struct lock Lock;
	struct condition mayLeave;//车等乘客
	struct condition Arrived;//乘客等车
	int Waiting;//在车站等待的乘客数
	int Available;//火车上的空座位数
	int Boarding;//上车中，车上还没坐好的人
	int inStation;//车在车站，车站有车
};

void station_init(struct station *station)
{
	lock_init(&station->Lock);
	lock_init(&station->Lock);
	cond_init(&station->mayLeave);
	cond_init(&station->Arrived);
	station->Waiting=0;
	station->Boarding=0;
	station->inStation=0;
}

void station_load_train(struct station *station, int count)//列车到达车站并打开车门
{
	lock_acquire(&station->Lock);
	station->inStation=1;//进站
	station->Boarding=0;//没人上车
	station->Available=count;//记录空座位数
	if(station->Available>0&&station->Waiting>0)//还有空座位，车站有乘客，就通知乘客
	{
		cond_broadcast(&station->Arrived,&station->Lock);
	}
	while(station->Available>0&&(station->Waiting>0||station->Boarding>0))//有空座位，且有未上车或未坐下的乘客，就要等待
	{
		cond_wait(&station->mayLeave,&station->Lock);
	}
	station->Available=0;//没有空座位
	station->inStation=0;//出站 
	lock_release(&station->Lock);
}

void station_wait_for_train(struct station *station)//乘客到达车站
{
	lock_acquire(&station->Lock);
	station->Waiting++;//车站人数增加
	while(!station->inStation||station->Boarding>=station->Available)//车站没车，并且没坐好的人达到了空座位数或更多，就要等
	{
		cond_wait(&station->Arrived,&station->Lock);
	}
	station->Boarding++;//上车
	station->Waiting--;//车站人数减少
	lock_release(&station->Lock);
}

void station_on_board(struct station *station)//乘客就座
{
	lock_acquire(&station->Lock);
	station->Available--;//空座位数减少
	station->Boarding--;//没坐好的人数减少
	if(station->Available==0||station->Boarding==0)//如果没有空座位，或者所有人都坐好了，就通知发车
	{
		cond_signal(&station->mayLeave,&station->Lock);
	}
	lock_release(&station->Lock);
}
