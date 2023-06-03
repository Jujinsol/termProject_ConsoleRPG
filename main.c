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
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

// 색상 정의
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
#define BLANK ' ' // ' ' 로하면 흔적이 지워진다 
#define MONSTER 'A'
#define BULLET 'C'
#define FIRE 'W'

#define ESC 0x1b //  ESC 누르면 종료

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define WIDTH 120
#define HEIGHT 30

int Delay = 100; // 100 msec delay, 이 값을 줄이면 속도가 빨라진다.
long long frame_count = 0;
int p1_frame_sync = 10;
int mon_frame_sync = 5;
int bigMon1_frame_sync = 37;
int bigMon2_frame_sync = 53;
int bigMon3_frame_sync = 97;

int keep_moving = 1; // 1:계속이동, 0:한칸씩이동.
int time_out = 30; // 제한시간

int called[2];
int attack;
int life;

int bigMonLife = 300;
int bigPflag = 0;

int next_map = 0;

static int pOldx = 2, pOldy = 1, pNewx = 2, pNewy = 1;
int BigPlayer_x, BigPlayer_y;
int BigMon_x, BigMon_y, BigMon_life = 100;
int bigMon_fire_x, bigMon_fire_y;

static int bigMonAttack1 = 0;
static int bigMonAttack2 = 0;
static int bigMonAttack3 = 0;

int bigRain_x, bigRain_y;

//static int mon_ox, mon_oy; // monster 좌표
//static int mon_nx, mon_ny; // monster의 새로운 좌표

typedef struct
{
	int mon_ox, mon_oy;
	int mon_nx, mon_ny;
	int Num;
} Monster;

Monster monsters[6];
int monsterNum = 1;

typedef struct
{
	int life_x, life_y;
	int attack_x, attack_y;
	int eatLife, eatAttack;
} Item;

Item item[1];

int map[30][120];
int map1[30][120] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,1,1,1,1,1,1,1,1,4,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,0,4,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,10,0,0,0,0,0,0,0,0,4,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,99,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,11,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
};
int map2[30][120] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,10,1,1,1,1,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,5,0},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,99,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,11,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,4,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
};

int map3[30][120] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,1,1,1,5,0},
	{1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,5,0},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,5,0},
	{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,10,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,0,99,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,4,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
};

int map4[30][120] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,1,1,1,10,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,5,0},
	{1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,1,5,0},
	{1,1,1,1,1,1,0,0,0,0,0,0,0,1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,5,0},
	{1,1,1,0,0,0,0,1,1,1,1,99,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,5,0},
	{1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,00,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,4,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,4,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,11,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,11,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,5,0},
	{1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,5,0},
	{1,1,1,0,0,0,0,4,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,5,0},
	{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
};

int map5[30][120] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,5,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5},
	{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
};

int hidden_index; // Hidden 화면 번호 0 or 1
HANDLE scr_handle[2]; // 화면 버퍼 변수

void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void scr_clear()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	// hidden screen를 clear한다.
	// WIDTH*2 * HEIGHT 값은 [속성]에서 설정한 값과 정확히 같아야 한다.
	// 즉, 화면 속성에서 너비(W)=80, 높이(H)=25라면 특수 문자는 2칸씩 이므로 WIDTH=40, HEIGHT=25이다.
	FillConsoleOutputCharacter(scr_handle[hidden_index], ' ', WIDTH * 2 * HEIGHT, Coor, &dw);
}

void printscr(char* str)
{
	DWORD dw;
	// hidden screen에 gotoxy 되었다고 가정하고 print
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
// 화면 지우기고 원하는 배경색으로 설정한다.
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
// 일반맵 player의 공격을 위함 함수
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
		textcolor(BLACK, WHITE);
		gotoxy(100, 6);
		printf("공격 성공!");
		gotoxy(100, 7);
		printf("남은 몬스터 수 : %d", monsterNum - 2);
		monsterNum--;
		flag = 0;
	}
}
// 아이템 먹은 경우
void playerItem(int x, int y)
{
	if (map[y][x] == 10) // 별 : 공격력
	{
		if (attack > 40)
		{
			gotoxy(100, 14);
			printf("최대 공격력입니다.\n");
			return;
		}
		else
			attack += 10;
		gotoxy(100, 14);
		printf("공격력 + 10 | %d", attack);
	}
	if (map[y][x] == 11) // 하트 : 생명력
	{
		if (life > 2)
		{
			gotoxy(100, 15);
			printf("최대 생명력입니다.\n");
			return;
		}
		else
			life += 1;
		gotoxy(100, 15);
		printf("생명력 + 1 | %d", life);
	}
}
// 처음 map 호출할 때 player 보여주는 용도
void showPlayer(int ox, int oy, int nx, int ny)
{
	gotoxy(ox, oy);
	putchar(BLANK);
	gotoxy(nx, ny);
	textcolor(BLACK, WHITE);
	putchar(PLAYER);
}
// player를 따라 움직이는 monster 보여주는 용도
void showMon(int ox, int oy, int nx, int ny)
{
	gotoxy(ox, oy);
	putchar(BLANK);
	gotoxy(nx, ny);
	textcolor(RED2, WHITE);
	putchar(MONSTER);
	textcolor(BLACK, WHITE);
}
// monster의 공격. player가 생명을 잃음.
void monsterAttack()
{
	life--;
	gotoxy(100, 8);
	textcolor(BLACK, WHITE);
	printf("몬스터의 공격!");
	gotoxy(100, 9);
	printf("생명력이 줄어듭니다.");
	gotoxy(100, 10);
	printf("남은 생명 : %d", life);
}
// player 사방에 monster가 있는지 검사
int checkMonster(int Ay, int Ax)
{
	int i;

	for (i = 1; i < 6; i++) // 6마리 monster 중 하나라도 있나를 검사
	{
		// 위치가 맞아야 하고 이미 죽은 애가 아니어야 함.
		if (monsters[i].mon_ox == Ax && monsters[i].mon_oy == Ay && monsters[i].Num != 0)
		{
			return 1;
		}
	}
	return 0;
}
// monster가 따라갈 player의 위치를 검사.
int checkPlayer(int Ay, int Ax)
{
	if (pOldx == Ax && pOldy == Ay)
		return 1;
	return 0;
}
// 자동으로 player를 따라오는 monster 함수
void moveMonster(int tmpMonsterNum)
{
	if (calcDist(pOldx, pOldy, monsters[tmpMonsterNum].mon_ox, monsters[tmpMonsterNum].mon_oy) < 3 && frame_count % (mon_frame_sync % 4) == 0)
	{
		monsterAttack();
	}
	else if (calcDist(pOldx, pOldy, monsters[tmpMonsterNum].mon_ox, monsters[tmpMonsterNum].mon_oy) < 15)
	{
		if ((pOldx - monsters[tmpMonsterNum].mon_ox) > 0)
		{
			if (map[monsters[tmpMonsterNum].mon_ny][monsters[tmpMonsterNum].mon_nx / 2 + 1] != 1 && checkPlayer(monsters[tmpMonsterNum].mon_ny, monsters[tmpMonsterNum].mon_nx + 1) == 0)
				monsters[tmpMonsterNum].mon_nx += 1;
		}
		else if ((pOldx - monsters[tmpMonsterNum].mon_ox) < 0)
		{
			if (map[monsters[tmpMonsterNum].mon_oy][monsters[tmpMonsterNum].mon_nx / 2 - 1] != 1 && checkPlayer(monsters[tmpMonsterNum].mon_ny, monsters[tmpMonsterNum].mon_nx - 1) == 0)
				monsters[tmpMonsterNum].mon_nx -= 1;
		}
		if ((pOldy - monsters[tmpMonsterNum].mon_oy) > 0)
		{
			if (map[monsters[tmpMonsterNum].mon_ny + 1][monsters[tmpMonsterNum].mon_ox / 2] != 1 && checkPlayer(monsters[tmpMonsterNum].mon_ny + 1, monsters[tmpMonsterNum].mon_nx) == 0)
				monsters[tmpMonsterNum].mon_ny += 1;
		}
		else if ((pOldy - monsters[tmpMonsterNum].mon_oy) < 0)
		{
			if (map[monsters[tmpMonsterNum].mon_ny - 1][monsters[tmpMonsterNum].mon_ox / 2] != 1 && checkPlayer(monsters[tmpMonsterNum].mon_ny - 1, monsters[tmpMonsterNum].mon_nx) == 0)
				monsters[tmpMonsterNum].mon_ny -= 1;
		}
		showMon(monsters[tmpMonsterNum].mon_ox, monsters[tmpMonsterNum].mon_oy, monsters[tmpMonsterNum].mon_nx, monsters[tmpMonsterNum].mon_ny);
		monsters[tmpMonsterNum].mon_ox = monsters[tmpMonsterNum].mon_nx;
		monsters[tmpMonsterNum].mon_oy = monsters[tmpMonsterNum].mon_ny;
	}
}

void showItem(int isAttack)
{
	if (isAttack)
	{
		textcolor(BLUE2, WHITE);
		gotoxy(item[0].attack_x, item[0].attack_y);
		printf("*");
		item[0].eatAttack = 1;
		textcolor(BLACK, WHITE);
	}
	else
	{
		textcolor(RED2, WHITE);
		gotoxy(item[0].life_x, item[0].life_y);
		printf("H");
		item[0].eatLife = 1;
		textcolor(BLACK, WHITE);
	}
}

// 방향키에 따라 player가 움직임
void playerMove(unsigned char ch)
{
	static unsigned char last_ch = 0;
	int move_flag = 0, tmpX = pOldx, flag = 0;

	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	if (pOldx % 2 != 0)
		tmpX = pOldx - 1;
	else
		tmpX = pOldx;

	if (calcDist(item[0].attack_x, item[0].attack_y, tmpX, pOldy) < 7 && item[0].eatAttack == 0)
	{
		showItem(1);
	}
	if (calcDist(item[0].life_x, item[0].life_y, tmpX, pOldy) < 7 && item[0].eatLife == 0)
	{
		showItem(0);
	}
	// player 위치에 아이템이 있는가?
	if (map[pOldy][tmpX / 2] == 10 || map[pOldy][tmpX / 2] == 11)
	{
		gotoxy(tmpX, pOldy);
		printf(" ");
		playerItem(tmpX / 2, pOldy);
		map[pOldy][tmpX / 2] = 0;
	}

	switch (ch) {
	case UP:
		if (map[pOldy - 1][tmpX / 2] != 1 && pOldy > 0 && checkMonster(pOldy - 1, pOldx) == 0)
		{
			pNewy = pOldy - 1;
		}
		move_flag = 1;
		break;
	case DOWN:
		if (map[pOldy + 1][tmpX / 2] != 1 && pOldy < HEIGHT - 1 && checkMonster(pOldy + 1, pOldx) == 0)
		{
			pNewy = pOldy + 1;
		}
		move_flag = 1;
		break;
	case LEFT:
		if (map[pOldy][tmpX / 2 - 1] != 1 && pOldx > 0 && checkMonster(pOldy, pOldx - 2) == 0)
		{
			pNewx = pOldx - 2;
		}
		move_flag = 1;
		break;
	case RIGHT:
		if (map[pOldy][tmpX / 2 + 1] != 1 && pOldx < WIDTH - 1 && checkMonster(pOldy, pOldx + 2) == 0)
		{
			pNewx = pOldx + 2;
		}
		move_flag = 1;
		break;
	case 32:
	{
		if (map[pOldy][tmpX / 2] == 99 && monsterNum - 1 == 0)
		{
			next_map = 1;
		}
		else
			playerAttack(pOldy, tmpX);
	}
	break;
	}
	if (move_flag && flag == 0)
	{
		textcolor(BLACK, WHITE);
		erasestar(pOldx, pOldy); // 마지막 위치의 * 를 지우고
		putstar(pNewx, pNewy, PLAYER); // 새로운 위치에서 * 를 표시한다.
		removeCursor();
		pOldx = pNewx; // 마지막 위치를 기억한다.
		pOldy = pNewy;
	}
	else
		return;
}

void DrawBigPlayer(int x, int y, int f)
{
	if (f == 1)
	{ // 기본 자세
		gotoxy(x, y);
		printf("      ■■■");
		gotoxy(x, y + 1);
		printf("    ■■■■■");
		gotoxy(x, y + 2);
		printf("      ■■■");
		gotoxy(x, y + 3);
		printf("        ■");
		gotoxy(x, y + 4);
		printf("       ■■");
		gotoxy(x, y + 5);
		printf("    ■■■■■");
		gotoxy(x, y + 6);
		printf("  ■   ■■   ■");
		gotoxy(x, y + 7);
		printf("■     ■■     ■");
		gotoxy(x, y + 8);
		printf("      ■■■");
		gotoxy(x, y + 9);
		printf("      ■  ■");
		gotoxy(x, y + 10);
		printf("      ■  ■");
	}
	else
	{ // Boss를 공격하는 모션
		gotoxy(x, y);
		printf("      ■■■");
		gotoxy(x, y + 1);
		printf("    ■■■■■");
		gotoxy(x, y + 2);
		printf("      ■■■");
		gotoxy(x, y + 3);
		printf("■      ■");
		gotoxy(x, y + 4);
		printf("  ■   ■■");
		gotoxy(x, y + 5);
		printf("    ■■■■■");
		gotoxy(x, y + 6);
		printf("       ■■   ■");
		gotoxy(x, y + 7);
		printf("       ■■     ■");
		gotoxy(x, y + 8);
		printf("      ■■■");
		gotoxy(x, y + 9);
		printf("      ■  ■");
		gotoxy(x, y + 10);
		printf("      ■  ■");
	}
}

void EraseBigPlayer(int x, int y)
{
	gotoxy(x, y);
	printf("            ");
	gotoxy(x, y + 1);
	printf("              ");
	gotoxy(x, y + 2);
	printf("            ");
	gotoxy(x, y + 3);
	printf("          ");
	gotoxy(x, y + 4);
	printf("           ");
	gotoxy(x, y + 5);
	printf("              ");
	gotoxy(x, y + 6);
	printf("                ");
	gotoxy(x, y + 7);
	printf("                  ");
	gotoxy(x, y + 8);
	printf("            ");
	gotoxy(x, y + 9);
	printf("            ");
	gotoxy(x, y + 10);
	printf("            ");
}

void DrawBigMonster(int x, int y)
{
	gotoxy(x, y);
	printf("■■■■■■■■■■");
	gotoxy(x, y + 1);
	printf("■■■■■■■■■■");
	gotoxy(x, y + 2);
	printf("■■■■■■＠＠■■");
	gotoxy(x, y + 3);
	printf("■■■■■■■■■■");
	gotoxy(x, y + 4);
	printf("■■■■■■■■■■");
	gotoxy(x, y + 5);
	printf("■■■■■■■■");
	gotoxy(x, y + 6);
	printf("■■■■■■■■");
	gotoxy(x, y + 7);
	printf("■■■■■■■■");
	gotoxy(x, y + 8);
	printf("■■■■■■■■■■");
	gotoxy(x, y + 9);
	printf("■■■■■■■■■■");
	gotoxy(x, y + 10);
	printf("■■■■■■■■■■");
	gotoxy(x, y + 11);
	printf("  ■■■    ■■■");
	gotoxy(x, y + 12);
	printf("  ■■■    ■■■");
}

void EraseBigMonster(int x, int y)
{
	gotoxy(x, y);
	printf("                    ");
	gotoxy(x, y + 1);
	printf("                    ");
	gotoxy(x, y + 2);
	printf("                    ");
	gotoxy(x, y + 3);
	printf("                    ");
	gotoxy(x, y + 4);
	printf("                    ");
	gotoxy(x, y + 5);
	printf("                ");
	gotoxy(x, y + 6);
	printf("                ");
	gotoxy(x, y + 7);
	printf("                ");
	gotoxy(x, y + 8);
	printf("                    ");
	gotoxy(x, y + 9);
	printf("                    ");
	gotoxy(x, y + 10);
	printf("                    ");
	gotoxy(x, y + 11);
	printf("                  ");
	gotoxy(x, y + 12);
	printf("                  ");
}

void BigPlayerAttack(int Ax, int Ay)
{
	int i;
	EraseBigPlayer(Ax, Ay);
	DrawBigPlayer(Ax, Ay, 0);
	Sleep(70);
	EraseBigPlayer(Ax, Ay);
	DrawBigPlayer(Ax, Ay, 1);

	while (Ax != BigMon_x + 20)
	{
		textcolor(BLUE2, WHITE);
		putstar(Ax, Ay + 5, BULLET);
		Sleep(7);
		erasestar(Ax, Ay + 5);
		Ax -= 1;
		textcolor(BLACK, WHITE);
	}
	if (Ax >= BigMon_x + 20)
	{
		bigMonLife -= attack;
		for (i = 0; i < 5; i++)
		{
			textcolor(RED2, WHITE);
			Sleep(20);
			DrawBigMonster(BigMon_x, BigMon_y);
			textcolor(BLACK, WHITE);
		}

		textcolor(BLACK, WHITE);
		gotoxy(100, 6);
		printf("공격 성공!");
		gotoxy(100, 7);
		printf("남은 생명 : %3d", bigMonLife);

		gotoxy(2, 2);
		for (i = 0; i < 30; i++)
			printf("  ");
		gotoxy(2, 2);
		textcolor(GREEN2, WHITE);
		for (i = 0; i < bigMonLife / 10; i++)
			printf("♥");
		textcolor(BLACK, WHITE);
	}
}

void BigPlayerMove(unsigned char ch)
{
	int bigMon_frame = 0;
	switch (ch) {
	case UP:
		if (bigMonAttack1 || bigMonAttack3)
			break;
		EraseBigPlayer(BigPlayer_x, BigPlayer_y);
		BigPlayer_y -= 12;
		DrawBigPlayer(BigPlayer_x, BigPlayer_y, 1);
		if (bigMonAttack2)
		{
			while (bigMon_fire_x != 94 && bigMon_frame < 25)
			{
				putstar(bigMon_fire_x, bigMon_fire_y, FIRE);
				Sleep(5);
				erasestar(bigMon_fire_x, bigMon_fire_y);
				bigMon_fire_x += 1;
				bigMon_frame++;
			}
		}
		else
			Sleep(450);
		EraseBigPlayer(BigPlayer_x, BigPlayer_y);
		BigPlayer_y += 12;
		//DrawBigPlayer(BigPlayer_x, BigPlayer_y, 1);
		break;
	case LEFT:
		if (map5[BigPlayer_y][BigPlayer_x / 2 - 1] != 1 && BigPlayer_x != BigMon_x  + 22)
		{
			EraseBigPlayer(BigPlayer_x, BigPlayer_y);
			BigPlayer_x -= 2;
			//DrawBigPlayer(BigPlayer_x, BigPlayer_y, 1);
		}
		break;
	case RIGHT:
		if (map5[BigPlayer_y][BigPlayer_x / 2 + 9] != 1 && BigPlayer_x != BigMon_x + 20)
		{
			EraseBigPlayer(BigPlayer_x, BigPlayer_y);
			BigPlayer_x += 2;
			//DrawBigPlayer(BigPlayer_x, BigPlayer_y, 1);
		}
		break;
	case 32:
		if (bigMonAttack1 || bigMonAttack2 || bigMonAttack3)
			break;
		BigPlayerAttack(BigPlayer_x, BigPlayer_y);
		break;
	}
}

//게임 초기화, 처음 시작과 Restar때 호출
void init_game()
{
	pOldx = 2, pOldy = 1, pNewx = 2, pNewy = 1;
	life = 3;
	attack = 40;
	monsterNum = 1;
}

void DrawRain(int Ax, int Ay)
{
	textcolor(BLUE1, WHITE);
	int i, j;
	for (i = 0; i < 41; i += 20)
	{
		for (j = 0; j < 4; j ++)
		{
			gotoxy(Ax + 40 + i, Ay + j);
			putstar(Ax + 40 + i, Ay + j, 'l');
		}
	}
	textcolor(BLACK, WHITE);
}

void EraseRain(int Ax, int Ay)
{
	int i, j;
	for (i = 0; i < 41; i += 20)
	{
		for (j = 0; j < 4; j++)
		{
			gotoxy(Ax + 40 + i, Ay + j);
			putstar(Ax + 40 + i, Ay + j, '  ');
		}
	}
}

int IsBigPlayerHurt()
{
	// 맵 크기는 제한적이고 플레이어가 움직이는 공간도 제한적이다. 보스의 x좌표가 플레이어 x좌표보다 크다는 건? 공격이 먹혔다는 것
	if (bigMonAttack1 && BigMon_x + 22 > BigPlayer_x) 
	{
		bigPflag++;
		return 1;
	}
	if (bigMonAttack2 && bigMon_fire_x == BigPlayer_x && bigMon_fire_y == BigPlayer_y + 6)
	{
		bigPflag++;
		return 1;
	}
	if (bigMonAttack3 && (bigRain_x + 40 + 2 != BigPlayer_x && bigRain_x + 60 + 2 != BigPlayer_x 
			&& (bigRain_y > BigPlayer_y && bigRain_y < BigPlayer_y + 10)))
	{
		bigPflag++;
		return 1;
	}

	return 0;
}
// 데이터만 변하고 플레이어나 보스가 즉각적으로 변하지 않아 만든 함수.
// 더블 버퍼링을 사용했다면 없어도 되는 부분이다...
void UpdateScreen()
{
	unsigned char ch;
	int i;

	if (bigMonAttack2 == 1) // 보스 공격 2를 처리하는 부분
	{
		putstar(bigMon_fire_x, bigMon_fire_y, FIRE);
		Sleep(5);
		erasestar(bigMon_fire_x, bigMon_fire_y);
	}
	if (bigMonAttack3 == 1) // 보스 공격 3을 처리하는 부분
	{
		DrawRain(bigRain_x, bigRain_y);
		Sleep(50);
		EraseRain(bigRain_x, bigRain_y);
	}
	// 스크린을 변경하는 도중에도 입력을 받을 수 있어야 한다.
	// 이 부분이 없으면 보스가 공격하는 동안에는 움직일 수가 없다.
	if (kbhit() == 1) 
	{
		ch = getch();
		if (ch == SPECIAL1 || ch == SPECIAL2 || ch == 32)
		{
			ch = getch();
			switch (ch)
			{
			case UP:
			case DOWN:
			case LEFT:
			case RIGHT:
			case 32:
					BigPlayerMove(ch);
				break;
			}
		}
	}

	if (IsBigPlayerHurt() && bigPflag == 1)
	{
		life--;
		gotoxy(100, 8);
		textcolor(BLACK, WHITE);
		printf("보스의 공격!");
		gotoxy(100, 9);
		printf("생명력이 줄어듭니다.");
		gotoxy(100, 10);
		printf("남은 생명 : %d", life);
		removeCursor();
		for (i = 0; i < 5; i++)
		{
			// player가 빨간 색으로 깜빡이길 원해서 만든 곳
			textcolor(RED2, WHITE);
			Sleep(20);
			DrawBigPlayer(BigPlayer_x, BigPlayer_y, 1);
			textcolor(BLACK, WHITE);
		}
		textcolor(BLACK, WHITE);
		bigPflag = 2;
	}

	DrawBigMonster(BigMon_x, BigMon_y);
	DrawBigPlayer(BigPlayer_x, BigPlayer_y, 1);
}

void BigMonsterAttack1(int Ax, int Ay)
{
	int i;
	unsigned char ch;
	for (i = 0; i < 8; i++)
	{
		BigMon_x += i;		
		UpdateScreen();
		Sleep(100);
		EraseBigMonster(BigMon_x, BigMon_y);
	}
	for (i = 7; i >= 0; i--)
	{
		BigMon_x -= i;
		UpdateScreen();
		Sleep(100);
		EraseBigMonster(BigMon_x, BigMon_y);
	}
	DrawBigMonster(BigMon_x, BigMon_y);
	bigMonAttack1 = 0;
	bigPflag = 0;
}

void BigMonsterAttack2(int Ax, int Ay)
{
	bigMon_fire_x = Ax + 20;
	bigMon_fire_y = Ay + 8;
	while (bigMon_fire_x < 94)
	{
		UpdateScreen();
		bigMon_fire_x += 1;
	}
	bigPflag = 0;
}

void BigMonsterAttack3(int Ax, int Ay)
{
	bigRain_x = Ax;
	bigRain_y = 1;
	while (bigRain_y < 23)
	{
		UpdateScreen();
		bigRain_y += 1;
	}
	bigPflag = 0;
}

void DrawMap(int map[HEIGHT][WIDTH], int a, int b)
{
	int x, y, i, j;

	gotoxy(a, b);
	for (y = 0; y < HEIGHT -1; y++) {
		for (x = 0; x < WIDTH; x++) {
			if (map[y][x] != 0)
				gotoxy((x + a) * 2, y + b);
			switch (map[y][x]) {
			case 0:
				printf("  ");
				break;
			case 1:
				textcolor(BLACK, WHITE);
				printf("□");
				break;
			case 3:
				textcolor(CYAN2, WHITE);
				printf("■");
				break;
			case 4:
				monsters[monsterNum].Num = monsterNum;
				monsters[monsterNum].mon_ox = x * 2;
				monsters[monsterNum].mon_oy = y;
				monsters[monsterNum].mon_ny = y;
				monsters[monsterNum].mon_nx = x * 2;
				monsterNum++;
				break;
			case 5:
				textcolor(BLACK, WHITE);
				printf("|");
				break;
			case 6:
				textcolor(BLACK, WHITE);
				printf("--");
				break;
			case 8:
				BigMon_x = x * 2;
				BigMon_y = y;
				break;
			case 9:
				BigPlayer_x = x * 2;
				BigPlayer_y = y;
				break;
			case 10:
				item[0].attack_x = x * 2;
				item[0].attack_y = y;
				item[0].eatAttack = 0;
				break;
			case 11:
				item[0].life_x = x * 2;
				item[0].life_y = y;
				item[0].eatLife = 0;
				break;
			case 99:
				textcolor(BLUE2, WHITE);
				printf("@");
				break;
			}
		}
	}
}

void main()
{
	unsigned char ch;
	int i,j, RBack, RText, x, y, isBig = 0;
	char flag;
	srand(time(NULL));
	sndPlaySoundA("song.wav", SND_ASYNC | SND_NODEFAULT | SND_LOOP);

	FILE* input = NULL;
	wchar_t script[100];

	cls(BLACK, WHITE);
	input = fopen("start.txt", "r");
	if (input == NULL)
	{
		printf("실패");
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

	gotoxy(0, 11);
	for (i = 0; i < 100; i++)
	{
		printf("%lc", script[i]);
		removeCursor();
		Sleep(100);
	}

START:
	cls(BLACK, WHITE);
	init_game();
	int a = 0;
	int b = 0;
	int c = 0;

	while (1)
	{
		textcolor(YELLOW2, BLACK);
		for (i = 0; i < 3; i++)
		{
			gotoxy(rand() % 120, rand() % 15);
			printf("*");
		}
		flag = kbhit();
		if (flag != NULL)
			break;
		RText = rand() % 15 + 1;

		textcolor(RText, BLACK);
		gotoxy(23, 5);
		printf("■■■■■  ■   ■■■■■          ■         ■■     ■■■  ■  ■\n");
		gotoxy(23, 6);
		printf("    ■      ■           ■        ■  ■    ■■■■■  ■      ■  ■\n");
		gotoxy(23, 7);
		printf("  ■  ■    ■           ■      ■      ■   ■    ■   ■      ■■■\n");
		gotoxy(23, 8);
		printf("■      ■  ■ ■■■■■■■  ■■■■■■■   ■■     ■■■  ■  ■\n");
		gotoxy(23, 9);
		printf("            ■       ■              ■          ■              ■  ■\n");
		gotoxy(23, 10);
		printf("                     ■              ■      ■■■■■\n");

		textcolor(WHITE, BLACK);
		gotoxy(45, 20);
		printf("시작하려면 아무키나 누르세요");
		gotoxy(22, 18);
		printf("↑");
		gotoxy(19, 19);
		printf("← ↓ →");
		gotoxy(18, 21);
		printf("방향키 이동");
		gotoxy(91, 18);
		printf("[___________]");
		gotoxy(90, 21);
		printf("스페이스바 공격");
		removeCursor();
		Sleep(200);
	}

	for (i = 1; i < 6; i++)
	{
		cls(WHITE, BLACK);
		switch (i)
		{
		case 1:
			for (y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					map[y][x] = map1[y][x];
				}
			}
			monsterNum = 0;
			next_map = 0;
			DrawMap(map,0,0);
			showPlayer(2, 1, 2, 1);
			gotoxy(0, 0);
			break;
		case 2:
			for (y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					map[y][x] = map2[y][x];
				}
			}
			monsterNum = 0;
			next_map = 0;
			DrawMap(map,0,0);
			pOldx = 2;
			pOldy = 1;
			pNewx = 2;
			pNewy = 1;
			gotoxy(0, 0);
			break;
		case 3:
			for (y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					map[y][x] = map3[y][x];
				}
			}
			monsterNum = 0;
			next_map = 0;
			DrawMap(map,0,0);
			pOldx = 2;
			pOldy = 1;
			pNewx = 2;
			pNewy = 1;
			gotoxy(0, 0);
			break;
		case 4:
			for (y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					map[y][x] = map4[y][x];
				}
			}
			monsterNum = 0;
			next_map = 0;
			DrawMap(map,0,0);
			pOldx = 2;
			pOldy = 1;
			pNewx = 2;
			pNewy = 1;
			gotoxy(0, 0);
			break;
		case 5:
			for (y = 0; y < HEIGHT; y++) {
				for (x = 0; x < WIDTH; x++) {
					map[y][x] = map5[y][x];
				}
			}
			isBig = 1;
			next_map = 0; 
			DrawMap(map,0,0);
			DrawBigMonster(BigMon_x, BigMon_y);
			DrawBigPlayer(BigPlayer_x, BigPlayer_y, 1);
			gotoxy(2, 1);
			textcolor(BLACK, WHITE);
			printf("<몬스터 남은 생명>");
			gotoxy(2, 2);
			for (j = 0; j < 30; j++)
				printf("  ");
			gotoxy(2, 2);
			textcolor(GREEN2, WHITE);
			for (j = 0; j < bigMonLife / 10; j++)
				printf("♥");
			textcolor(BLACK, WHITE);
			gotoxy(0, 0);
			break;
		}

		while (1)
		{
			if (kbhit() == 1)
			{
				ch = getch();
				if (ch == SPECIAL1 || ch == SPECIAL2 || ch == 32)
				{
					ch = getch();
					// Player1은 방향키로 움직인다.
					switch (ch)
					{
					case UP:
					case DOWN:
					case LEFT:
					case RIGHT:
					case 32:
						if (isBig == 0)
							playerMove(ch);//player1만 방향 전환
						else
						{
							BigPlayerMove(ch);
							UpdateScreen();
						}
						break;
					default: // 방향 전환이 아니면
						playerMove(0);
					}
				}
			}
			if (next_map)
				break;

			gotoxy(100, 1);
			textcolor(BLACK, WHITE);
			printf("공격력 : %d\n", attack);
			gotoxy(100, 2);
			printf("남은 생명 : ");
			for (j = 0; j < 3; j++)
				printf("  ");
			gotoxy(112, 2);
			textcolor(RED2, WHITE);
			for (j = 0; j < life; j++)
				printf("♥");

			textcolor(BLACK, WHITE);
			gotoxy(100, 4);
			printf("--------------------");
			gotoxy(100, 12);
			printf("--------------------");

			if (life <= 0)
			{
				cls(BLACK, WHITE);
				goto END;
			}
			// 보스맵이 아니라면
			if (isBig == 0)
			{
				for (j = 1; j < 6; j++)
				{
					// 몬스터가 자유롭게 움직일 수 있도록 한다
					if (monsters[j].Num != 0 && frame_count % mon_frame_sync == 0)
					{
						moveMonster(j);
					}
				}
			}
			else // 보스맵이라면
			{
				// 정해둔 프레임마다 보스가 공격을 할 수 있도록 설정한다
				if (frame_count % bigMon1_frame_sync == 36)
				{
					bigMonAttack1 = 1;
					BigMonsterAttack1(BigMon_x, BigMon_y);
					UpdateScreen();
				}
				if (frame_count % bigMon2_frame_sync == 52)
				{
					bigMonAttack2 = 1;
					BigMonsterAttack2(BigMon_x, BigMon_y);
					bigMonAttack2 = 0;
				}
				if (frame_count % bigMon3_frame_sync == 92)
				{
					bigMonAttack3 = 1;
					BigMonsterAttack3(BigMon_x, BigMon_y);
					bigMonAttack3 = 0;
				}

				if (bigMonLife <= 0)
					goto WIN;
			}
			removeCursor();
			Sleep(Delay);
			frame_count++;
		}
	}

END:
	while (1)
	{
		textcolor(WHITE, BLACK);
		gotoxy(52, 12);
		printf("** Game Over **");
		gotoxy(45, 15);
		printf("Hit (R) to Restart (Q) to Quit");
		//Sleep(300);
		if (kbhit())
		{
			ch = getch();
			if (ch == 'r' || ch == 'q')
				break;
		}
	}

	if (ch == 'r')
		goto START;
	gotoxy(0, HEIGHT - 1);

WIN:
	while (1)
	{
		cls(BLACK, WHITE);
		RText = rand() % 15 + 1;
		textcolor(RText, BLACK);
		gotoxy(53, 12);
		printf("** YOU WIN **");
		gotoxy(45, 15);
		printf("Hit (R) to Restart (Q) to Quit");
		//Sleep(300);
		if (kbhit())
		{
			ch = getch();
			if (ch == 'r' || ch == 'q')
				break;
		}
		removeCursor();
		Sleep(200);
	}

	if (ch == 'r')
		goto START;
}
