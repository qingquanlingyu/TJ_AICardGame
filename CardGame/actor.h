#pragma once
#include "includes.h"


class actor
{
private:
	//�ڲ�����
	//��׼������actor.cpp��
	int hp;//Ѫ��
	int armor;//����
	int cardnum;//��ʼ��������ʵ���Ͼ���������
	int attack;//�������˺�ֵ
	int defend;//�����ĵ���ֵ
	int bloodsuck;//��Ѫֵ���˺���ظ����
	int probability[CARD_NUM + 1];//�鿨���ʣ���CARD_NUM + 1����Ҫ���һ��Ϊ0�����һ��Ϊ100
	int card[CARD_NUM];
	

	//�ڲ�����
	void extract();//�ҵĻغϣ��鿨��
public:
	friend class scene;

	actor();//���캯��
	void setprobability(const int p[CARD_NUM]);
	//���ֿ��Ʋ���������Ϊvirtual�������ְҵ����
	void start(actor* op, int kind);//����ӿڣ���ʼ�غ�
	void act_attack(actor* op);//���й�������
	void act_defend();//���з�������
	void act_destroy(actor* op);//�����Ƽײ���
	void act_bloodsuck(actor* op);//������Ѫ����
};