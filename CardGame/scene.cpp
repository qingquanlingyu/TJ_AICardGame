#include "scene.h"

 const int BASE_HP = 20;
scene::scene(actor* a, actor* b)
{
	player = a;
	ai = b;
	depth = SEARCHDEPTH;
}

int scene::evolution()
{
	return (player->hp - ai->hp);
}

void scene::StartRound(int kind, int newcard)
{
	if (next)
	{
		player->card[newcard]++;
		node a;
		a.php = player->hp;
		a.parmor = player->armor;
		a.ahp = ai->hp;
		a.aarmor = ai->armor;
		a.target = false;
		a.cardkind = kind;
		last.push(a);
		player->start(ai, kind);
	}
	else
	{
		ai->card[newcard]++;
		node a;
		a.php = player->hp;
		a.parmor = player->armor;
		a.ahp = ai->hp;
		a.aarmor = ai->armor;
		a.target = true;
		a.cardkind = kind;
		last.push(a);
		ai->start(player, kind);
	}
	next = !next;
}
void scene::UndoRound(int newcard)
{
#ifdef DEBUG
	if (last.empty())
	{
		cout << "last栈为空！" << endl;
		return;
	}
#endif //DEBUG
	node a = last.top();
	last.pop();
	if (a.target)
	{
		player->hp = a.php;
		player->armor = a.parmor;
		ai->hp = a.ahp;
		ai->armor = a.aarmor;
		ai->card[a.cardkind]++;
		ai->card[newcard]--;
	}
	else
	{
		player->hp = a.php;
		player->armor = a.parmor;
		ai->hp = a.ahp;
		ai->armor = a.aarmor;
		player->card[a.cardkind]++;
		player->card[newcard]--;
	}
	next = !next;
}

bool scene::GameOver()
{
	if (ai->hp <= 0 || player->hp <= 0)
		return true;
	return false;
}
int scene::MinMaxSearch(int d)
{
	int bestvalue;
	if (GameOver())
	{
#ifdef DEBUG
		cout << "搜索到游戏结束，返回评估值" << evolution() << endl;
#endif //DEBUG
		return evolution();
	}
	if (d <= 0)
	{
#ifdef DEBUG
		cout << "搜索到层数为零，返回" << evolution() << endl;
#endif //DEBUG
		return evolution();
	}
	if (next)//轮到player行动，MAX
	{
#ifdef DEBUG
		cout << "分析MAX层" << endl;
#endif //DEBUG
		bestvalue = -MAXVALUE;
		for (int i = 0; i < CARD_NUM; i++)
			if (player->card[i] > 0)
			{
#ifdef DEBUG
				cout << "进入MAX层分支：" << i << endl;
#endif //DEBUG
				double value = 0;
				for (int j = 0; j < CARD_NUM; j++)//机会博弈中的CHANCE层
				{
#ifdef DEBUG
					cout << "进入MAX层分支：" << i << "——chance层分支：" << j << endl;
#endif //DEBUG
					StartRound(i, j);
					int tmpvalue = MinMaxSearch(d - 1);
					value += double(player->probability[j+1]- player->probability[j])/100 * tmpvalue;//机会博弈求期望极大极小值
					UndoRound(j);
				}
				if (int(value) > bestvalue)
				{
					bestvalue = int(value);
#ifdef DEBUG
					cout << "更新bestvalue" << endl;
#endif //DEBUG
				}
			}
	}
	else//轮到ai行动，MIN
	{
		bestvalue = MAXVALUE;
#ifdef DEBUG
		cout << "分析MIN层" << endl;
#endif //DEBUG
		for (int i = 0; i < CARD_NUM; i++)
			if (ai->card[i] > 0)
			{
				double value = 0;
#ifdef DEBUG
				cout << "进入MIN层分支：" << i << endl;
#endif //DEBUG
				for (int j = 0; j < CARD_NUM; j++)//机会博弈中的CHANCE层
				{
#ifdef DEBUG
					cout << "进入MIN层分支：" << i << "——chance层分支：" << j << endl;
#endif //DEBUG
					StartRound(i, j);
					int tmpvalue = MinMaxSearch(d - 1);
					value += double(player->probability[j + 1] - player->probability[j]) / 100 * tmpvalue;//机会博弈求期望极大极小值
					UndoRound(j);
				}
				if (int(value) < bestvalue)
				{
					bestvalue = int(value);
#ifdef DEBUG
					cout << "更新bestvalue" << endl;
#endif //DEBUG
				}			
			}
	}
#ifdef DEBUG
	cout << "该节点搜索结束，返回评估值" << bestvalue << endl;
#endif //DEBUG
	return bestvalue;
}

int scene::AlphaBetaSearch(int d, int alpha, int beta)
{
	if (GameOver())
	{
		return evolution();
#ifdef DEBUG
		cout << "搜索到游戏结束，返回评估值" << evolution() << endl;
#endif //DEBUG
	}
	if (d <= 0)
	{
#ifdef DEBUG
		cout << "搜索到层数为零，返回" << evolution() << endl;
#endif //DEBUG
		return evolution();
	}
	if (next)//轮到player行动，MAX
	{
#ifdef DEBUG
		cout << "分析MAX层" << endl;
#endif //DEBUG
		for (int i = 0; i < CARD_NUM; i++)
			if (player->card[i] > 0)
			{
#ifdef DEBUG
				cout << "进入MAX层分支：" << i << endl;
#endif //DEBUG
				double value = 0;
				for (int j = 0; j < CARD_NUM; j++)
				{
#ifdef DEBUG
					cout << "进入MAX层分支：" << i << "——chance层分支：" << j << endl;
#endif //DEBUG
					StartRound(i, j);
					int tmpvalue = AlphaBetaSearch(d - 1, alpha, beta);
					value += double(player->probability[j + 1] - player->probability[j]) / 100 * tmpvalue;//机会博弈求期望极大极小值
					UndoRound(j);
				}
				if (int(value) > alpha)
				{
#ifdef DEBUG
					cout << "更新alpha" << endl;
#endif //DEBUG
					alpha = int(value);
					if (alpha >= beta)
					{
#ifdef DEBUG
						cout << "剪枝" << endl;
#endif //DEBUG
						break;
					}
				}			
			}
		return alpha;
	}
	else//轮到ai行动，MIN
	{
#ifdef DEBUG
		cout << "分析MIN层" << endl;
#endif //DEBUG
		for (int i = 0; i < CARD_NUM; i++)
			if (ai->card[i] > 0)
			{
#ifdef DEBUG
				cout << "进入MIN层分支：" << i << endl;
#endif //DEBUG
				double value = 0;
				for (int j = 0; j < CARD_NUM; j++)
				{
#ifdef DEBUG
					cout << "进入MIN层分支：" << i << "——chance层分支：" << j << endl;
#endif //DEBUG
					StartRound(i, j);
					int tmpvalue = AlphaBetaSearch(d - 1, alpha, beta);
					value += double(player->probability[j + 1] - player->probability[j]) / 100 * tmpvalue;//机会博弈求期望极大极小值
					UndoRound(j);
				}
				if (int(value) < beta)
				{
					beta = int(value);
#ifdef DEBUG
					cout << "更新beta" << endl;
#endif //DEBUG
					if (alpha >= beta)
					{
#ifdef DEBUG
						cout << "剪枝" << endl;
#endif //DEBUG
						break;
					}
				}			
			}
		return beta;
	}
}
void scene::drawscene()
{
	cleardevice();
	FlushMouseMsgBuffer();
	IMAGE card;

	int k = 0;
	for (int i = 0; i < CARD_NUM; i++)
	{
		for (int j = 0; j < ai->card[i]; j++)
		{
			switch (i)
			{
			case ATTACKCARD:
				loadimage(&card, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\attack.jpg"));
				putimage(k*215+(k+1)*10, 14, &card);
				break;
			case DEFENDCARD:
				loadimage(&card, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\defend.jpg"));
				putimage(k * 215 + (k + 1) * 10, 14, &card);
				break;
			case DESTORYCARD:
				loadimage(&card, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\destroy.jpg"));
				putimage(k * 215 + (k + 1) * 10, 14, &card);
				break;
			case BLOODSUCKCARD:
				loadimage(&card, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\bloodsuck.jpg"));
				putimage(k * 215 + (k + 1) * 10, 14, &card);
				break;
			default:
				;
			}
			k++;
		}
	}

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 20;
	_tcscpy(f.lfFaceName, _T("黑体"));		
	f.lfQuality = ANTIALIASED_QUALITY;		
	settextstyle(&f);
	settextcolor(BLUE);
	RECT r = { 900, 50, 1000, 100 };
	drawtext(_T("HP"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r= { 900, 75, 1000, 125 };
	wstring a = to_wstring(ai->hp) + L" / " + to_wstring(BASE_HP);
	LPCTSTR str = a.c_str();
	drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 900, 125, 1000, 175 };
	drawtext(_T("Armor"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 900, 150, 1000, 200 };
	a = to_wstring(ai->armor);
	str = a.c_str();
	drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	k = 0;
	for (int i = 0; i < CARD_NUM; i++)
	{
		for (int j = 0; j < player->card[i]; j++)
		{
			switch (i)
			{
			case ATTACKCARD:
				loadimage(&card, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\attack.jpg"));
				putimage(k * 215 + (k + 1) * 10, 314, &card);
				break;
			case DEFENDCARD:
				loadimage(&card, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\defend.jpg"));
				putimage(k * 215 + (k + 1) * 10, 314, &card);
				break;
			case DESTORYCARD:
				loadimage(&card, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\destroy.jpg"));
				putimage(k * 215 + (k + 1) * 10, 314, &card);
				break;
			case BLOODSUCKCARD:
				loadimage(&card, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\bloodsuck.jpg"));
				putimage(k * 215 + (k + 1) * 10, 314, &card);
				break;
			default:
				;
			}
			k++;
		}
	}

	settextcolor(RED);
	r = { 900, 350, 1000, 400 };
	drawtext(_T("HP"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 900, 375, 1000, 425 };
	a = to_wstring(player->hp) + L" / " + to_wstring(BASE_HP);
	str = a.c_str();
	drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 900, 425, 1000, 475 };
	drawtext(_T("Armor"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	r = { 900, 450, 1000, 500 };
	a = to_wstring(player->armor);
	str = a.c_str();
	drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void scene::initialize()
{
	MOUSEMSG mmessage;
	cleardevice();
	FlushMouseMsgBuffer();
	IMAGE choose;
	loadimage(&choose, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\choose.jpg"));
	putimage(0, 0, &choose);
	while (1)
	{
		Sleep(1);
		if (PeekMouseMsg(&mmessage, true))
		{
			if (mmessage.mkLButton)
			{
				if (mmessage.x >= 192 && mmessage.x <= 352 && mmessage.y >= 422 && mmessage.y <= 482)
				{//玩家选择先手
					ai->armor++;
					next = true;
					break;
				}
				else if (mmessage.x >= 662 && mmessage.x <= 822 && mmessage.y >= 422 && mmessage.y <= 482)
				{//玩家选择后手
					player->armor++;
					next = false;
					break;
				}
			}
		}
	}
	cleardevice();
	FlushMouseMsgBuffer();
	for (int i = 0; i < player->cardnum; i++)
		player->extract();
	for (int i = 0; i < ai->cardnum; i++)
		ai->extract();
	drawscene();
}
//人类玩家选择出什么牌
int scene::selectcard()
{
	int t = 0;
	FlushMouseMsgBuffer();
	MOUSEMSG mmessage;
	bool finish = false;
	while (1)
	{
		if (finish)
			break;
		Sleep(1);
		if (PeekMouseMsg(&mmessage, true))
			if (mmessage.mkLButton)
				for (int i = 0; i <= player->cardnum; i++)
					if (mmessage.x >= i * 215 + (i + 1) * 10 && mmessage.x < (i + 1) * 225 && mmessage.y >= 314 && mmessage.y < 600)
					{
						t = i;
						finish = true;
						break;
					}
	}
	int tmp[CARD_NUM + 1] = { 0 };
	for (int i = 1; i <= CARD_NUM; i++)
	{
		tmp[i] = tmp[i - 1] + player->card[i - 1];
	}
	for (int i = 0; i < CARD_NUM; i++)
	{
		if (t >= tmp[i] && t < tmp[i + 1])
			return i;
	}
	return 0;
}
//显示出牌效果，并在控制台窗口显示数据改变情况
void scene::OutputCard(int kind)
{
	drawscene();
	if (next)
	{
		switch (kind)
		{
		case ATTACKCARD:
			cout << "玩家使用了攻击牌，对AI造成了2点伤害"  << endl;
			break;
		case DEFENDCARD:
			cout << "玩家使用了防御牌，增加了自身3点护甲" << endl;
			break;
		case DESTORYCARD:
			cout << "玩家使用了破甲牌，使AI护甲清零" << endl;
			break;
		case BLOODSUCKCARD:
			cout << "玩家使用了吸血牌，对AI血量造成1点伤害并恢复自身1点血量" << endl;
			break;
		default:
			;
		}
	}
	else
	{
		switch (kind)
		{
		case ATTACKCARD:
			cout << "AI使用了攻击牌，对玩家造成了2点伤害" << endl;
			break;
		case DEFENDCARD:
			cout << "AI使用了防御牌，增加了自身3点护甲" << endl;
			break;
		case DESTORYCARD:
			cout << "AI使用了破甲牌，使玩家护甲清零" << endl;
			break;
		case BLOODSUCKCARD:
			cout << "AI使用了吸血牌，对玩家血量造成1点伤害并恢复自身1点血量" << endl;
			break;
		default:
			;
		}
	}
	cout << endl;
	Sleep(500);
}
void scene::start()
{
	PlaySound(TEXT("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\choose.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	initialize();
	PlaySound(TEXT("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\inwar.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	while (1)
	{
		if (GameOver())
		{
			PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
			MOUSEMSG mmessage;
			if (ai->hp <= 0)
			{
				cout << "玩家胜利！" << endl;
				PlaySound(TEXT("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\victory.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				IMAGE victory;
				loadimage(&victory, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\victory.jpg"));
				putimage(0, 0, &victory);
				while (1)
				{
					Sleep(1);
					if (PeekMouseMsg(&mmessage, true))
						if (mmessage.mkLButton)
							break;
				}
			}
			else if (player->hp <= 0)
			{
				cout << "AI胜利！" << endl;
				PlaySound(TEXT("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\defeat.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
				IMAGE defeat;
				loadimage(&defeat, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\defeat.jpg"));
				putimage(0, 0, &defeat);
				while (1)
				{
					Sleep(1);
					if (PeekMouseMsg(&mmessage, true))
						if (mmessage.mkLButton)
							break;
				}
			}
			break;
		}
		if (next)
		{
			int cardkind = 0;
			drawscene();
			cardkind = selectcard();
			player->start(ai, cardkind);
			//player出牌
			OutputCard(cardkind);
			player->extract();
			drawscene();
		}
		else
		{
			int cardkind = 0;
			drawscene();
			while (!last.empty())
				last.pop();
			clock_t startt = clock();
			int bestvalue = MAXVALUE;
			bestvalue = MAXVALUE;
#ifdef DEBUG
			cout << "开始搜索" << endl;
#endif //DEBUG
			for (int i = 0; i < CARD_NUM; i++)
				if (ai->card[i] > 0)
				{
					double value = 0;
					for (int j = 0; j < CARD_NUM; j++)//机会博弈中的CHANCE层
					{
#ifdef DEBUG
						cout << "进入MIN层分支：" << i << "——chance层分支：" << j << endl;
#endif //DEBUG
						StartRound(i, j);
						int tmpvalue;
						if (SEARCHKIND)
							tmpvalue = MinMaxSearch(depth - 1);
						else
							tmpvalue = AlphaBetaSearch(depth - 1, -MAXVALUE, MAXVALUE);
						value += double(player->probability[j + 1] - player->probability[j]) / 100 * tmpvalue;//机会博弈求期望极大极小值
						UndoRound(j);
					}
					if (int(value) < bestvalue)
					{
						bestvalue = int(value);
						cardkind = i;
					}
				}

#ifdef DEBUG
			if (ai->card[cardkind] > 0)
				cout << "得到最优走法为：" << cardkind << endl;
			else
				cout << "计算错误，选择了没有的手牌" << endl;
#endif //DEBUG
			clock_t endt = clock();
			double endtime = (double)(endt - startt) / CLOCKS_PER_SEC;
			cout << "本次搜索用时:" << endtime * 1000 << "ms";	//ms为单位
#ifdef DEBUG
			cout << "(本次搜索为debug模式，搜索用时大大增加)";
#endif //DEBUG
			cout << endl;
			ai->start(player, cardkind);
			//出牌
			OutputCard(cardkind);
			ai->extract();
			drawscene();
		}
		next = !next;
	}
	PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
}