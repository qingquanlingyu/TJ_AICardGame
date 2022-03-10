#include "actor.h"
#include "scene.h"

int main()
{
	ios::sync_with_stdio(false);
	srand((unsigned)time(NULL));
	while (1)
	{
		//标题+音乐
		MOUSEMSG mmessage;
		system("cls");
		initgraph(1000, 600, EW_SHOWCONSOLE);
		setbkcolor(WHITE);
		cleardevice();
		FlushMouseMsgBuffer();
		PlaySound(TEXT("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\theme.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		IMAGE title;
		loadimage(&title, _T("D:\\homework\\大二下\\AI\\CardGame\\CardGame\\static\\title.jpg"));
		putimage(0, 0, &title);
		while (1)
		{
			Sleep(1);
			if (PeekMouseMsg(&mmessage, true))
			{
				if (mmessage.mkLButton)
				{
					if (mmessage.x >= 322 && mmessage.x <= 658 && mmessage.y >= 343 && mmessage.y <= 406)
						break;//开始游戏
					else if (mmessage.x >= 322 && mmessage.x <= 658 && mmessage.y >= 469 && mmessage.y <= 532)
					{//结束游戏
						closegraph();
						return 0;
					}
				}
			}
		}
		PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
		cleardevice();
		actor* player = new actor();
		player->setprobability(CARD_PROBABILITY);
		actor* ai = new actor();
		ai->setprobability(CARD_PROBABILITY);
		scene* s = new scene(player, ai);
		s->start();
		delete player;
		delete ai;
	}
	return 0;
}