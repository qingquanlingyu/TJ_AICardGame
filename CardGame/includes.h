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
//�������ͣ�1 ����С   0 Alpha-Beta
#define SEARCHKIND 1

#define SEARCHDEPTH 6
//debugģʽ�����Ϊ1����debugģʽ�¿�Ϊ6�����
//ʹ��O2�Ż�+Releaseģʽ��Alpha-Beta���������������9

#define ATTACKCARD 0
#define DEFENDCARD 1
#define DESTORYCARD 2
#define BLOODSUCKCARD 3

//����������
const int CARD_NUM = 4;
const int CARD_PROBABILITY[CARD_NUM] = { 40,30,12,18 };

/*
��Ϸ˵����
��ϷΪ˫�����ƣ�

20Ѫ������ʼ4���ƣ��ȳ�����
���ֿ���ӵ��1�㻤��


����ϵͳ��
�������ԶԷ����2���˺�
������Ϊ�Լ�����3�㻤��
�Ƽף��ƻ��Է������л���
��Ѫ�����ӻ��׶ԶԷ����1���˺����ظ�����1��Ѫ��
*/