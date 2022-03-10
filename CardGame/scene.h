#pragma once
#include "actor.h"

const int MAXVALUE = 999999999;

struct node//搜索结点
{
	bool target;
	int php;
	int parmor;
	int ahp;
	int aarmor;
	int cardkind;
};

class scene//场景配置，本质是搜索(这样起名是为了致敬cocox2d)
{
private:
	bool next;//true为玩家，false为ai
	int depth;//基础搜索深度
	actor* player;
	actor* ai;
	stack<node> last;
public:
	scene(actor*a, actor*b);
	void initialize();//由玩家选择先后手，后手拥有1点初始防御
	void start();//外部接口，开始游戏
	int selectcard();//人类玩家选择出牌
	void drawscene();//展示游戏场景
	bool GameOver();//游戏结束

	int evolution();//评估函数
	//评估函数以人类玩家血量-AI玩家血量为基础，因此人类玩家为MAX，AI玩家为MIN

	void StartRound(int kind, int newcard);
	void UndoRound(int newcard);
	//极大极小搜索用
	int MinMaxSearch(int d);//cardkind用于传搜索得到的最优出牌
	//AlphaBeta搜索
	int AlphaBetaSearch(int d, int alpha, int beta);

	void OutputCard(int kind);//显示出牌效果
};
