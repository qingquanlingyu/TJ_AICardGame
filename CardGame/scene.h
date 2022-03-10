#pragma once
#include "actor.h"

const int MAXVALUE = 999999999;

struct node//�������
{
	bool target;
	int php;
	int parmor;
	int ahp;
	int aarmor;
	int cardkind;
};

class scene//�������ã�����������(����������Ϊ���¾�cocox2d)
{
private:
	bool next;//trueΪ��ң�falseΪai
	int depth;//�����������
	actor* player;
	actor* ai;
	stack<node> last;
public:
	scene(actor*a, actor*b);
	void initialize();//�����ѡ���Ⱥ��֣�����ӵ��1���ʼ����
	void start();//�ⲿ�ӿڣ���ʼ��Ϸ
	int selectcard();//�������ѡ�����
	void drawscene();//չʾ��Ϸ����
	bool GameOver();//��Ϸ����

	int evolution();//��������
	//�����������������Ѫ��-AI���Ѫ��Ϊ����������������ΪMAX��AI���ΪMIN

	void StartRound(int kind, int newcard);
	void UndoRound(int newcard);
	//����С������
	int MinMaxSearch(int d);//cardkind���ڴ������õ������ų���
	//AlphaBeta����
	int AlphaBetaSearch(int d, int alpha, int beta);

	void OutputCard(int kind);//��ʾ����Ч��
};
