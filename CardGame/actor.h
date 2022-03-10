#pragma once
#include "includes.h"


class actor
{
private:
	//内部变量
	//基准数据在actor.cpp中
	int hp;//血量
	int armor;//护甲
	int cardnum;//初始手牌数，实际上就是手牌数
	int attack;//攻击的伤害值
	int defend;//防御的叠甲值
	int bloodsuck;//吸血值，伤害与回复相等
	int probability[CARD_NUM + 1];//抽卡概率，有CARD_NUM + 1个，要求第一个为0，最后一个为100
	int card[CARD_NUM];
	

	//内部函数
	void extract();//我的回合，抽卡！
public:
	friend class scene;

	actor();//构造函数
	void setprobability(const int p[CARD_NUM]);
	//各种卡牌操作，设置为virtual便于设计职业特性
	void start(actor* op, int kind);//对外接口，开始回合
	void act_attack(actor* op);//进行攻击操作
	void act_defend();//进行防御操作
	void act_destroy(actor* op);//进行破甲操作
	void act_bloodsuck(actor* op);//进行吸血操作
};