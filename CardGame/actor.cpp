#include "actor.h"

const int BASE_HP = 20;
const int BASE_STARTCARD = 4;
const int BASE_ATTACK = 2;
const int BASE_DEFEND = 3;
const int BASE_BLOODSUCK = 1;
actor::actor()
{
	hp = BASE_HP;
	cardnum = BASE_STARTCARD;
	armor = 0;
	attack = BASE_ATTACK;
	defend = BASE_DEFEND;
	bloodsuck = BASE_BLOODSUCK;

	memset(probability, 0, (CARD_NUM + 2) * sizeof(int));
	probability[CARD_NUM + 1] = 100;
	memset(card, 0, CARD_NUM * sizeof(int));
	for (int i = 0; i < cardnum; i++)
		extract();
}
void actor::act_attack(actor* op)
{
	if (card[ATTACKCARD] > 0)
		card[ATTACKCARD]--;
	if (op->armor >= attack)
		op->armor -= attack;
	else
	{
		op->hp -= (attack - op->armor);
		op->armor = 0;
	}
}
void actor::act_defend()
{
	if (card[DEFENDCARD] > 0)
		card[DEFENDCARD]--;
	armor += defend;
}
void actor::act_destroy(actor* op)
{
	if (card[DESTORYCARD] > 0)
		card[DESTORYCARD]--;
	op->armor = 0; 
}
void actor::act_bloodsuck(actor* op)
{
	if (card[BLOODSUCKCARD] > 0)
		card[BLOODSUCKCARD]--;
	op->hp--;
	if (hp<BASE_HP)
		hp++;
}

void actor::setprobability(const int p[CARD_NUM])
{
	probability[0] = 0;
	for (int i = 1; i <= CARD_NUM; i++)
		probability[i] = probability[i - 1] + p[i - 1];
}
void actor::extract()
{
	int tmp = rand() % 100;
	for (int i = 1; i <= CARD_NUM + 1; i++)
		if (tmp >= probability[i - 1] && tmp < probability[i])
		{
			card[i - 1]++;
			break;
		}
}
void actor::start(actor* op, int kind)
{
	switch(kind)
	{
	case ATTACKCARD:
		act_attack(op);
		break;
	case DEFENDCARD:
		act_defend();
		break;
	case DESTORYCARD:
		act_destroy(op);
		break;
	case BLOODSUCKCARD:
		act_bloodsuck(op);
		break;
	default:
		cout << "´íÎó£ºÎ´ÖªÅÆÐÍ£¡" << endl;
	}
}