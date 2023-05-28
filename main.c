#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <math.h>

// ���� ����
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define PLAYER 'P'
#define BLANK ' ' // ' ' ���ϸ� ������ �������� 
#define MONSTER 'A'

#define ESC 0x1b //  ESC ������ ����

#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.

#define UP  0x48 // Up key�� 0xe0 + 0x48 �ΰ��� ���� ���´�.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define WIDTH 120
#define HEIGHT 30

int Delay = 100; // 100 msec delay, �� ���� ���̸� �ӵ��� ��������.
long long frame_count = 0;
int p1_frame_sync = 10;
int mon_frame_sync = 10;

int keep_moving = 1; // 1:����̵�, 0:��ĭ���̵�.
int time_out = 30; // ���ѽð�
int flag;

int golds[WIDTH][HEIGHT] = { 0 }; // 1�̸� Gold �ִٴ� ��
int goldinterval = 3; // GOLD ǥ�� ����
int called[2];
int attack = 100;
int life = 3;

static int pOldx = 2, pOldy = 1, pNewx = 2, pNewy = 1;

//static int mon_ox, mon_oy; // monster ��ǥ
//static int mon_nx, mon_ny; // monster�� ���ο� ��ǥ

typedef struct
{
	int mon_ox, mon_oy;
	int mon_nx, mon_ny;
	int Num;
} Monster;

Monster monsters[6];
int monsterNum = 1;

enum MapList
{
	Fst_MAP = 0,
	Snd_MAP = 1,
};

int map[30][120];
int map1[30][120] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
	{1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,0,4,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,4,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,99,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};
int map2[30][120] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
	{1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,99,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

int hidden_index; // Hidden ȭ�� ��ȣ 0 or 1
HANDLE scr_handle[2]; // ȭ�� ���� ����

void removeCursor(void) { // Ŀ���� �Ⱥ��̰� �Ѵ�

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //���� ���ϴ� ��ġ�� Ŀ�� �̵�
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API �Լ��Դϴ�. �̰� ���ʿ� �����
}

void scr_clear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	// hidden screen�� clear�Ѵ�.
	// WIDTH*2 * HEIGHT ���� [�Ӽ�]���� ������ ���� ��Ȯ�� ���ƾ� �Ѵ�.
	// ��, ȭ�� �Ӽ����� �ʺ�(W)=80, ����(H)=25��� Ư�� ���ڴ� 2ĭ�� �̹Ƿ� WIDTH=40, HEIGHT=25�̴�.
	FillConsoleOutputCharacter(scr_handle[hidden_index], ' ', WIDTH * 2 * HEIGHT, Coor, &dw);
}

void printscr(char* str)
{
	DWORD dw;
	// hidden screen�� gotoxy �Ǿ��ٰ� �����ϰ� print
	WriteFile(scr_handle[hidden_index], str, strlen(str), &dw, NULL);
}

void putstar(int x, int y, char ch)
{
	gotoxy(x, y);
	putchar(ch);
}
void erasestar(int x, int y)
{
	gotoxy(x, y);
	putchar(BLANK);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}
// ȭ�� ������ ���ϴ� �������� �����Ѵ�.
void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

int calcDist(int Ax, int Ay, int Bx, int By)
{
	float dx, dy, result;
	dx = Ax - Bx;
	dy = Ay - By;

	result = sqrt(dx * dx + dy * dy);
	return result;
}

void playerAttack(int Ay, int Ax)
{
	int i, flag = 0;

	for (i = 1; i < 6; i++)
	{
		if (calcDist(Ax, Ay, monsters[i].mon_ox, monsters[i].mon_oy) < 3 && monsters[i].Num != 0)
		{
			erasestar(monsters[i].mon_nx, monsters[i].mon_ny);
			monsters[i].Num = 0;
			gotoxy(0, 28);
			textcolor(BLACK, WHITE);
			flag = 1;
		}
	}
	if (flag)
	{
		printf("���� ����! ���Ͱ� �׾����ϴ�! ���� ���� �� : %d", monsterNum - 2);
		monsterNum--;
		flag = 0;
	}
}

void showPlayer(int ox, int oy, int nx, int ny)
{
	gotoxy(ox, oy);
	putchar(BLANK);
	gotoxy(nx, ny);
	textcolor(BLACK, WHITE);
	putchar(PLAYER);
}

void showMon(int ox, int oy, int nx, int ny) 
{
	gotoxy(ox, oy);
	putchar(BLANK);
	gotoxy(nx, ny);
	textcolor(RED2, WHITE);
	putchar(MONSTER);
	textcolor(BLACK, WHITE);
}

void monsterAttack()
{
	life--;
	gotoxy( 0, 29);
	textcolor(BLACK, WHITE);
	printf("������ ������ �޾ҽ��ϴ�. ������� �پ��ϴ�. ���� ���� : %d", life);
}

void moveMonster(int tmpMonsterNum)
{
	if (calcDist(pOldx, pOldy, monsters[tmpMonsterNum].mon_ox, monsters[tmpMonsterNum].mon_oy) < 3 && frame_count % (mon_frame_sync * 2) == 0)
	{
		monsterAttack();
	}
	else if (calcDist(pOldx, pOldy, monsters[tmpMonsterNum].mon_ox, monsters[tmpMonsterNum].mon_oy) < 15)
	{
		if ((pOldx - monsters[tmpMonsterNum].mon_ox) > 0)
		{
			if (map1[monsters[tmpMonsterNum].mon_ny][monsters[tmpMonsterNum].mon_nx/2 + 1] != 1)
				monsters[tmpMonsterNum].mon_nx += 1;
		}
		else if ((pOldx - monsters[tmpMonsterNum].mon_ox) < 0)
		{
			if (map1[monsters[tmpMonsterNum].mon_oy][monsters[tmpMonsterNum].mon_nx/2 - 1] != 1)
				monsters[tmpMonsterNum].mon_nx -= 1;
		}
		if ((pOldy - monsters[tmpMonsterNum].mon_oy) > 0)
		{
			if (map1[monsters[tmpMonsterNum].mon_ny + 1][monsters[tmpMonsterNum].mon_ox/2] != 1)
				monsters[tmpMonsterNum].mon_ny += 1;
		}
		else if ((pOldy - monsters[tmpMonsterNum].mon_oy) < 0)
		{
			if (map1[monsters[tmpMonsterNum].mon_ny - 1][monsters[tmpMonsterNum].mon_ox/2] != 1)
				monsters[tmpMonsterNum].mon_ny -= 1;
		}
		showMon(monsters[tmpMonsterNum].mon_ox, monsters[tmpMonsterNum].mon_oy, monsters[tmpMonsterNum].mon_nx, monsters[tmpMonsterNum].mon_ny);
		monsters[tmpMonsterNum].mon_ox = monsters[tmpMonsterNum].mon_nx;
		monsters[tmpMonsterNum].mon_oy = monsters[tmpMonsterNum].mon_ny;
	}
}

void playerMove(unsigned char ch)
{
	//static int oldx = 2, oldy = 1, newx = 2, newy = 1;
	static unsigned char last_ch = 0;
	int move_flag = 0, tmpX = pOldx;

	if (called[0] == 0) { // ó�� �Ǵ� Restart
		pOldx = 2, pOldy = 1, pNewx = 2, pNewy = 1;
		putstar(pOldx, pOldy, PLAYER);
		called[0] = 1;
		last_ch = 0;
		ch = 0;
	}
	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	if (pOldx % 2 != 0)
		tmpX = pOldx - 1;
	else
		tmpX = pOldx;


	switch (ch) {
	case UP:
		if (map[pOldy - 1][tmpX / 2] != 1 && pOldy > 0)
			pNewy = pOldy - 1;
		move_flag = 1;
		break;
	case DOWN:
		if (map[pOldy + 1][tmpX / 2] != 1 && pOldy < HEIGHT - 1)
			pNewy = pOldy + 1;
		move_flag = 1;
		break;
	case LEFT:
		if (map[pOldy][tmpX / 2 - 1] != 1 && pOldx > 0)
			pNewx = pOldx - 2;
		move_flag = 1;
		break;
	case RIGHT:
		if (map[pOldy][tmpX / 2 + 1] != 1 && pOldx < WIDTH - 1)
			pNewx = pOldx + 2;
		move_flag = 1;
		break;
	case 32:	
	{
		if (map[pOldy][tmpX / 2] == 99 && monsterNum - 1 == 0)
		{
			flag = 1;
			cls(BLACK, WHITE);
			gotoxy(0, 0);
			printf("��");
		}
		else
			playerAttack(pOldy, tmpX);
	}
		break;
	}
	if (move_flag) {
		textcolor(BLACK, WHITE);
		erasestar(pOldx, pOldy); // ������ ��ġ�� * �� �����
		putstar(pNewx, pNewy, PLAYER); // ���ο� ��ġ���� * �� ǥ���Ѵ�.
		removeCursor();
		pOldx = pNewx; // ������ ��ġ�� ����Ѵ�.
		pOldy = pNewy;
	}
}

//���� �ʱ�ȭ, ó�� ���۰� Restar�� ȣ��
void init_game()
{
	pOldx = 2, pOldy = 1, pNewx = 2, pNewy = 1;
	life = 3;
	attack = 100;
	monsterNum = 1;
}

int map33[30][120];
void DrawMap()
{
	int x, y, count, randomIndex;
	for (y = 0; y < HEIGHT; y++) 
	{
		for (x = 0; x < WIDTH; x++) 
		{
			if (x % 2 == 0 || y % 2 == 0)
				map33[y][x] = 1;
			else
				map33[y][x] = 0;
		}
	}

	for (y = 0; y < HEIGHT; y++)
	{
		count = 1;
		for (x = 0; x < WIDTH; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			if (y == HEIGHT - 2 && x == WIDTH - 2)
				continue;

			if (y == HEIGHT - 2)
			{
				map33[y][x + 1] = 0;
				continue;
			}

			if (x == WIDTH - 2)
			{
				map33[y + 1][x] = 0;
				continue;
			}

			if (rand() % 2 == 0)
			{
				map33[y][x + 1] = 0;
				count++;
			}
			else
			{
				randomIndex = rand() % count;
				map33[y + 1][x - randomIndex * 2] = 0;
				count = 1;
			}

		}
	}
	scr_clear();
	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++)
		{
			if (map33[y][x] != 0)
				gotoxy(x * 2, y);

			if (map33[y][x] == 0)
			{
				printf("  ");
			}
			else if (map33[y][x] == 1)
			{
				textcolor(BLACK, WHITE);
				printf("��");
			}
		}
	}
}

void DrawMap1() 
{
	static int i = 0;
	int x, y;
	scr_clear();

	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			map[y][x] = map1[y][x];
		}
	}

	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			if (map[y][x] != 0)
				gotoxy(x * 2, y);
			switch (map[y][x]) {
			case 0:
				printf("  ");
				break;
			case 1:
				textcolor(BLACK, WHITE);
				printf("��");
				break;
			case 2:
				if ((frame_count / 4) % 2 == 1) { // 4~7..12~15.. 20~23(Ȧ����°..)
					textcolor(BLUE2, WHITE);
					printf("��");
				}
				else { // 8~11 .. 16~19.. 24~27 ... (¦����°)
					textcolor(RED2, WHITE);
					printf("��");
				}
				break;
			case 3:
				textcolor(CYAN2, WHITE);
				printf("��");
				break;
			case 4:
				monsters[monsterNum].Num = monsterNum;
				monsters[monsterNum].mon_ox = x * 2;
				monsters[monsterNum].mon_oy = y;
				monsters[monsterNum].mon_ny = y;
				monsters[monsterNum].mon_nx = x * 2;
				monsterNum++;
				break;
			case 99:
				textcolor(BLUE2, WHITE);
				printf("@");
			}
		}
	}
}

void main()
{
	unsigned char ch;
	int i, RBack, RText;
	char flag;
	srand(time(NULL));

	FILE* input = NULL;
	wchar_t script[100];

	cls(BLACK, WHITE);
	input = fopen("start.txt", "r");
	if (input == NULL)
	{
		printf("����");
		fclose(input);
		return 1;
	}

	fgetwc(input, "%s", script);
	for (i = 0; i < 100; i++)
	{
		script[i] = fgetwc(input);
		if (script[i] == EOF)
			break;
	}

	fclose(input);

	gotoxy(0, 10);
	//for (i = 0; i < 100; i++)
	//{
	//	printf("%lc", script[i]);
	//	Sleep(50);
	//}
	
START:
	cls(BLACK, WHITE);
	init_game();

	while (1)
	{
		textcolor(YELLOW2, BLACK);
		for (i = 0; i < 3; i++)
		{
			gotoxy(rand() % 120, rand() % 15);
			printf("*");
		}
		flag = _kbhit();
		if (flag != NULL)
			break;
		RText = rand() % 15 + 1;

		textcolor(RText, BLACK);
		gotoxy(22, 5);
		printf("������  ��   ������          ��         ���     ����  ��  ��\n");
		gotoxy(22, 6);
		printf("    ��      ��           ��        ��  ��    ������  ��      ��  ��\n");
		gotoxy(22, 7);
		printf("  ��  ��    ��           ��      ��      ��   ��    ��   ��      ����\n");
		gotoxy(22, 8);
		printf("��      ��  �� ��������  ��������   ���     ����  ��  ��\n");
		gotoxy(22, 9);
		printf("            ��       ��              ��          ��              ��  ��\n");
		gotoxy(22, 10);
		printf("                     ��              ��      ������\n");

		textcolor(WHITE, BLACK);
		gotoxy(40, 20);
		printf("�����Ϸ��� �ƹ�Ű�� ��������");
		Sleep(200);
	}

	cls(WHITE, BLACK);
	DrawMap1();
	showPlayer(pOldx, pOldy, pNewx, pNewy);
	
	while (1)
	{
		if (_kbhit() == 1)
		{
			ch = _getch();
			if (ch == SPECIAL1 || ch == SPECIAL2 || ch == 32)
			{
				ch = _getch();
				// Player1�� ����Ű�� �����δ�.
				switch (ch)
				{
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
				case 32:
					playerMove(ch);//player1�� ���� ��ȯ
					break;
				default: // ���� ��ȯ�� �ƴϸ�
					playerMove(0);
				}
			}
			gotoxy(0, 0);
			printf("%d %d", pOldx, pOldy);
		}
		for (i = 1; i < 6; i++)
		{
			if (monsters[i].Num != 0 && frame_count % mon_frame_sync == 0)
			{
				moveMonster(i);
			}
		}

		gotoxy(102, 0);
		textcolor(BLACK, WHITE);
		printf("  ���ݷ�  : %d\n", attack);
		gotoxy(102, 1);
		printf("���� ���� :");
		for (i = 0; i < 3; i++)
			printf("  ");
		gotoxy(113, 1);
		textcolor(RED2, WHITE);
		for (i = 0; i < life; i++)
			printf("��");

		if (life == 0)
		{
			cls(BLACK, WHITE);
			break;
		}
		removeCursor();
		Sleep(Delay);
		frame_count++;
	}

	while (1)
	{
		textcolor(WHITE, BLACK);
		gotoxy(52, 12);
		printf("** Game Over **");
		gotoxy(45, 15);
		printf("Hit (R) to Restart (Q) to Quit");
		//Sleep(300);
		if (_kbhit()) 
		{
			ch = _getch();
			if (ch == 'r' || ch == 'q')
				break;
		}
	}

	if (ch == 'r')
		goto START;
	gotoxy(0, HEIGHT - 1);
}