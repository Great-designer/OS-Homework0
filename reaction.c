#include "pintos_thread.h"

// ת�������������������reaction-runner.c��ʵ�ֵģ��������ع����������á�
// ֻҪȷ����reaction_o()/reaction_h()���ʵ��ص�������
void make_water();

struct reaction//һ����Ӧ������ 
{
	struct lock Lock;
	struct condition Arrival;//Hԭ�ӵ������ 
	struct condition Occurred;//Oԭ�Ӳ������� 
	int Atoms;//Hԭ���������п���H��������2 
	int Needed;//Oԭ����Ҫ��Hԭ�������п���O��������1 
};

void reaction_init(struct reaction *reaction)
{
	lock_init(&reaction->Lock);
	cond_init(&reaction->Arrival);
	cond_init(&reaction->Occurred);
	reaction->Atoms=0;
	reaction->Needed=0;
}

void reaction_h(struct reaction *reaction)//ÿ��Hԭ�ӵ��ú���
{
	lock_acquire(&reaction->Lock);//���� 
	reaction->Atoms++;//���һ��Hԭ�ӣ���Ӧ������Hԭ���������� 
	cond_signal(&reaction->Arrival,&reaction->Lock);//����һ��Hԭ�ӵ�������ϵ��߳� 
	while(reaction->Needed==0)//ֻҪ����ҪHԭ�� 
	{
		cond_wait(&reaction->Occurred,&reaction->Lock);//�ȴ�Oԭ�Ӳ����������ȴ���ҪHԭ��
	}
	reaction->Needed--;//ֻҪ��ҪHԭ�ӣ��������Լ� 
	lock_release(&reaction->Lock);//���� 
}

void reaction_o(struct reaction *reaction)//ÿ��Oԭ�ӵ��ú���
{
	lock_acquire(&reaction->Lock);//���� 
	while(reaction->Atoms<2)//ֻҪ��Ӧ������Hԭ��������2�� 
	{
		cond_wait(&reaction->Arrival,&reaction->Lock);//�ȴ�Hԭ�ӵ���������ȴ�Hԭ��������2�� 
	}
	reaction->Needed+=2;//��ҪHԭ��������2 
	reaction->Atoms-=2;//Hԭ����������2 
	cond_broadcast(&reaction->Occurred,&reaction->Lock);//��������Oԭ�Ӳ��������ϵ��߳� 
	make_water();//ִ�л�ѧ��Ӧ 
	lock_release(&reaction->Lock);//���� 
}
