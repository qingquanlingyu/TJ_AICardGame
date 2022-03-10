#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <conio.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <windows.h>
#include <vector>
#include <queue>
#include <map>
#include <stack>
#include <mmsystem.h>
#include <graphics.h>
#include<dsound.h>
#pragma comment(lib,"winmm.lib")
using namespace std;

//#define DEBUG
//搜索类型：1 极大极小   0 Alpha-Beta
#define SEARCHKIND 1

#define SEARCHDEPTH 6
//debug模式下最好为1，非debug模式下可为6或更高
//使用O2优化+Release模式后，Alpha-Beta搜索最多甚至允许到9

#define ATTACKCARD 0
#define DEFENDCARD 1
#define DESTORYCARD 2
#define BLOODSUCKCARD 3

//卡牌种类数
const int CARD_NUM = 4;
const int CARD_PROBABILITY[CARD_NUM] = { 40,30,12,18 };

/*
游戏说明：
游戏为双方明牌！

20血量，初始4张牌，先出再摸
后手可以拥有1点护甲


卡牌系统：
攻击：对对方造成2点伤害
防御：为自己叠加3点护甲
破甲：破坏对方的所有护甲
吸血：无视护甲对对方造成1点伤害，回复自身1点血量
*/