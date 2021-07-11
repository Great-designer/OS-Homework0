#include "pintos_thread.h"

struct station//��վ�ṹ��
{
	struct lock Lock;
	struct condition mayLeave;//���ȳ˿�
	struct condition Arrived;//�˿͵ȳ�
	int Waiting;//�ڳ�վ�ȴ��ĳ˿���
	int Available;//���ϵĿ���λ��
	int Boarding;//�ϳ��У����ϻ�û���õ���
	int inStation;//���ڳ�վ����վ�г�
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

void station_load_train(struct station *station, int count)//�г����ﳵվ���򿪳���
{
	lock_acquire(&station->Lock);
	station->inStation=1;//��վ
	station->Boarding=0;//û���ϳ�
	station->Available=count;//��¼����λ��
	if(station->Available>0&&station->Waiting>0)//���п���λ����վ�г˿ͣ���֪ͨ�˿�
	{
		cond_broadcast(&station->Arrived,&station->Lock);
	}
	while(station->Available>0&&(station->Waiting>0||station->Boarding>0))//�п���λ������δ�ϳ���δ���µĳ˿ͣ���Ҫ�ȴ�
	{
		cond_wait(&station->mayLeave,&station->Lock);
	}
	station->Available=0;//û�п���λ
	station->inStation=0;//��վ 
	lock_release(&station->Lock);
}

void station_wait_for_train(struct station *station)//�˿͵��ﳵվ
{
	lock_acquire(&station->Lock);
	station->Waiting++;//��վ��������
	while(!station->inStation||station->Boarding>=station->Available)//��վû��������û���õ��˴ﵽ�˿���λ������࣬��Ҫ��
	{
		cond_wait(&station->Arrived,&station->Lock);
	}
	station->Boarding++;//�ϳ�
	station->Waiting--;//��վ��������
	lock_release(&station->Lock);
}

void station_on_board(struct station *station)//�˿;���
{
	lock_acquire(&station->Lock);
	station->Available--;//����λ������
	station->Boarding--;//û���õ���������
	if(station->Available==0||station->Boarding==0)//���û�п���λ�����������˶������ˣ���֪ͨ����
	{
		cond_signal(&station->mayLeave,&station->Lock);
	}
	lock_release(&station->Lock);
}
