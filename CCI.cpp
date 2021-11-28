#include <bits/stdc++.h>
#include <console.hpp>
#include <conio.h>

using namespace std;

const int cA = 5;	   //Red city
const int cB = 3;	   //Blue city
const int ca = 4;	   //Red ground
const int cb = 1;	   //Blue ground
const int cC = 0;	   //Black city
const int msize = 25;  //Map size
const int mount = 100; //Mountain count
const int grey = 100;  //City count

char cMap[msize][msize]; //Capital letters indicate cities, lowercase letters indicate ground, Aa is red, Bb is blue, C is white, and X is obstacle
int hp[msize][msize];
int nCount, sx, sy;							//Round accumulation, and the coordinates of the cursor
bool bVis[msize][msize];					//bfs
int dir[4][2] = {1, 0, -1, 0, 0, 1, 0, -1}; //bfs
int lA, lB;									//Player level

//Decision information: the number at position (x, y) is moved to (x + dx, y + dy)
struct dRet
{
	int x;
	int y;
	int dx;
	int dy;
};

//bfs
struct node
{
	int x;
	int y;
	int step;
};

void gInit();					   //Generate map
inline void gMove(int dx, int dy); //Move the cursor

//For following 5 functions, make sure A == 'A' or A == 'B'
inline void gMoveNum(int dx, int dy, char A);
inline void gMoveNum(int x, int y, int dx, int dy, char A);
inline void gDiv(int dx, int dy, char A);
inline void gDecide(char A, bool d);
inline dRet gBFS(int x, int y, char A);
inline void gPlus();			   //Increases the army every round, count army and land
inline void gChange(int x, int y); //Refresh the number of specified coordinates on the screen

void gMain0();
void gMain1();
void gEnd(); //Check the game is over

int main()
{
	bool p;
	int fs;
	cout << "玩法 (0: 玩家 VS AI, 1: AI VS AI) : ";
	cin >> p;
	cout << "字体大小: ";
	cin >> fs;
	system("cls");
	cSetFont((fs / 3 != 0 ? fs / 3 : fs / 3 + 1), fs);
	if (p)
		gMain0();
	else
		gMain1();
	return 0;
}

void gMain0()
{
	int in;
	bool cur = true;
	int i;
	cout << "AI A等级(终极困难请输入0): ";
	cin >> lA;
	cout << "AI B等级(终极困难请输入0): ";
	cin >> lB;
	gInit();
	for (nCount = 0;; nCount++)
	{
		cGotoXY(msize, 0);
		cSetColor(15, 0);
		cout << "第" << nCount << "回合" << endl;
		if (cur)
		{
			do
			{
				in = getch();
				if (in == 224)
				{
					in = getch();
					if (in == 72)
						gMove(-1, 0);
					if (in == 80)
						gMove(1, 0);
					if (in == 75)
						gMove(0, -1);
					if (in == 77)
						gMove(0, 1);
				}
			} while (in != ' ' && in != 'f');
			if (in == 'f')
				cur = false;
		}
		if (lA == 0)
		{
			gDecide('A', 1);
		}
		for (i = 1; i <= lA; i++)
		{
			gDecide('A', 0);
		}
		if (lB == 0)
		{
			gDecide('B', 1);
		}
		for (i = 1; i <= lB; i++)
		{
			gDecide('B', 0);
		}
		gPlus();
	}
}

void gMain1()
{
	int in;
	int i;
	cout << "AI等级(终极困难请输入0): ";
	cin >> lB;
	gInit();
	for (nCount = 0;; nCount++)
	{
		cGotoXY(msize, 0);
		cSetColor(15, 0);
		cout << "第" << nCount << "回合" << endl;
		do
		{
			in = getch();
			if (in == 224)
			{
				in = getch();
				if (in == 72)
					gMove(-1, 0);
				if (in == 80)
					gMove(1, 0);
				if (in == 75)
					gMove(0, -1);
				if (in == 77)
					gMove(0, 1);
			}
		} while (in != 'w' && in != 's' && in != 'a' && in != 'd' && in != 'z' && in != ' ');
		if (in == 'w')
			gMoveNum(-1, 0, 'A');
		if (in == 's')
			gMoveNum(1, 0, 'A');
		if (in == 'a')
			gMoveNum(0, -1, 'A');
		if (in == 'd')
			gMoveNum(0, 1, 'A');
		if (in == 'z')
		{
			in = getch();
			if (in == 'w')
				gDiv(-1, 0, 'A');
			if (in == 's')
				gDiv(1, 0, 'A');
			if (in == 'a')
				gDiv(0, -1, 'A');
			if (in == 'd')
				gDiv(0, 1, 'A');
		}
		if (lB == 0)
		{
			gDecide('B', 1);
		}
		for (i = 1; i <= lB; i++)
		{
			gDecide('B', 0);
		}
		gPlus();
	}
}

void gInit()
{
	int i, x, y;
	string sSeed;
	unsigned int sd = 20190622;
	cout << "种子: ";
	cin >> sSeed;
	system("cls");
	for (i = 0; i < (int)sSeed.size(); i++)
		sd = sd * 233 + sSeed[i];
	srand(sd);
	cMap[0][0] = 'A';
	hp[0][0] = 1;
	gChange(0, 0);
	cMap[msize - 1][msize - 1] = 'B';
	hp[msize - 1][msize - 1] = 1;
	gChange(msize - 1, msize - 1);
	for (i = 1; i <= mount; i++)
	{
		x = rand() % msize;
		y = rand() % msize;
		if (cMap[x][y] != 0)
			i--;
		else
		{
			cMap[x][y] = 'X';
			cGotoXY(x, y * 5);
			cSetColor(15, 0);
			cout << "  X";
		}
	}
	for (i = 1; i <= grey; i++)
	{
		x = rand() % msize;
		y = rand() % msize;
		if (cMap[x][y] != 0)
			i--;
		else
		{
			cMap[x][y] = 'C';
			hp[x][y] = 40 + rand() % 10;
			gChange(x, y);
		}
	}
	sx = sy = 0;
	cGotoXY(0, 3);
	cSetColor(15, 0);
	cout << "<";
}

inline void gMove(int dx, int dy)
{
	cSetColor(15, 0);
	dx += sx;
	dy += sy;
	if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize))
		return;
	cGotoXY(sx, sy * 5 + 4);
	cout << " ";
	sx = dx;
	sy = dy;
	cGotoXY(sx, sy * 5 + 4);
	cout << "<";
	cGotoXY(msize + 1, 0);
	cout << "拥有者: 玩家" << cMap[dx][dy] << ", 兵力: " << hp[dx][dy] << "            ";
}

inline void gMoveNum(int x, int y, int dx, int dy, char A)
{
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	if (cMap[x][y] != a && cMap[x][y] != A)
		return;
	dx += x;
	dy += y;
	if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize && cMap[dx][dy] != 'X'))
		return;
	char &cd = cMap[dx][dy];
	int &hd = hp[dx][dy], &hs = hp[x][y];
	if (cd != a && cd != A && hd >= hs - 1)
		return;
	if (cd == 0)
	{
		cd = a;
		hd = hs - 1;
	}
	else if (cd == A || cd == a)
	{
		hd += hs - 1;
	}
	else if (cd == B || cd == b)
	{
		if (B == 'B')
			cd--;
		else
			cd++;
		hd = hs - 1 - hd;
	}
	else
	{ //'C'
		cd = A;
		hd = hs - 1 - hd;
	}
	hs = 1;
	gChange(dx, dy);
	gChange(x, y);
}

inline void gMoveNum(int dx, int dy, char A)
{
	gMoveNum(sx, sy, dx, dy, A);
	gMove(dx, dy);
}

inline void gDiv(int dx, int dy, char A)
{
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	if (cMap[sx][sy] != a && cMap[sx][sy] != A)
		return;
	dx += sx;
	dy += sy;
	if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize && cMap[dx][dy] != 'X'))
		return;
	char &cd = cMap[dx][dy];
	int &hd = hp[dx][dy], &hs = hp[sx][sy], tmp = hs / 2;
	if (tmp == 0)
		return;
	if (cd != a && cd != A && hd >= tmp - 1)
		return;
	if (cd == 0)
	{
		cd = a;
		hd = tmp;
	}
	else if (cd == A || cd == a)
	{
		hd += tmp;
	}
	else if (cd == B || cd == b)
	{
		if (B == 'B')
			cd--;
		else
			cd++;
		hd = tmp - hd;
	}
	else
	{ //'C'
		cd = A;
		hd = tmp - hd;
	}
	hs -= tmp;
	gChange(dx, dy);
	gChange(sx, sy);
	gMove(dx - sx, dy - sy);
}

inline void gDecide(char A, bool d)
{
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	int i, j, k, dx, dy;
	dRet tmp;
	vector<dRet> res;
	res.clear();
	for (i = (A == 'A') ? 0 : (msize - 1); (A == 'A') ? (i <= msize - 1) : (i >= 0); (A == 'A') ? (i++) : (i--))
	{
		for (j = (A == 'A') ? 0 : (msize - 1); (A == 'A') ? (j <= msize - 1) : (j >= 0); (A == 'A') ? (j++) : (j--))
		{
			if (cMap[i][j] == B || cMap[i][j] == 'C')
			{
				tmp = gBFS(i, j, A);
				if (tmp.dx + tmp.dy == 0)
				{
					continue;
				}
				res.push_back(tmp);
			}
			if ((cMap[i][j] == A || cMap[i][j] == a) && hp[i][j] > 1)
			{
				for (k = 0; k < min(msize - 10, nCount / 800 + 4); k++)
				{
					dx = dir[k][0] + i;
					dy = dir[k][1] + j;
					if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize))
					{
						continue;
					}
					if ((cMap[dx][dy] == 0 || cMap[dx][dy] == b) && hp[dx][dy] < hp[i][j] - 1)
					{
						res.push_back({dir[k][0], dir[k][1], i, j});
						break;
					}
				}
			}
		}
	}
	if (d)
	{
		for (i = 0; i < (int)res.size(); i++)
		{
			gMoveNum(res[i].x, res[i].y, res[i].dx, res[i].dy, A);
		}
	}
	else
	{
		if (res.empty())
		{
			return;
		}
		random_shuffle(res.begin(), res.end());
		tmp = res[0];
		gMoveNum(tmp.x, tmp.y, tmp.dx, tmp.dy, A);
	}
}

inline dRet gBFS(int x, int y, char A)
{
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	int i, dx, dy;
	dRet ret;
	node now, nxt;
	queue<node> q;
	now.step = hp[x][y] + 2;
	now.x = x;
	now.y = y;
	q.push(now);
	memset(bVis, false, sizeof bVis);
	bVis[x][y] = true;
	while (!q.empty())
	{
		now = q.front();
		q.pop();
		for (i = 0; i < 4; i++)
		{
			dx = dir[i][0] + now.x;
			dy = dir[i][1] + now.y;
			if (dx >= 0 && dx < msize && dy >= 0 && dy < msize && !bVis[dx][dy] && cMap[dx][dy] != 'X')
			{
				bVis[dx][dy] = true;
				if (cMap[dx][dy] == 0)
				{
					nxt.step = now.step + 1;
				}
				if (cMap[dx][dy] == b || cMap[dx][dy] == B || cMap[dx][dy] == 'C')
				{
					nxt.step = now.step + hp[dx][dy] + 2;
				}
				else
				{
					nxt.step = now.step - hp[dx][dy] + 1;
					if (nxt.step < -5 && (cMap[dx][dy] == A || cMap[dx][dy] == a))
					{
						ret.dx = -dir[i][0];
						ret.dy = -dir[i][1];
						ret.x = dx;
						ret.y = dy;
						return ret;
					}
				}
				nxt.x = dx;
				nxt.y = dy;
				q.push(nxt);
			}
		}
	}
	ret.dx = ret.dy = 0;
	return ret;
}

inline void gPlus()
{
	int i, j, aa = 0, ba = 0, al = 0, bl = 0;
	if (nCount % 25 == 0)
	{
		for (i = 0; i < msize; i++)
		{
			for (j = 0; j < msize; j++)
			{
				if (cMap[i][j] == 'a' || cMap[i][j] == 'b')
				{
					++hp[i][j];
					gChange(i, j);
				}
			}
		}
	}
	for (i = 0; i < msize; i++)
	{
		for (j = 0; j < msize; j++)
		{
			if (cMap[i][j] == 'A' || cMap[i][j] == 'B')
			{
				++hp[i][j];
				gChange(i, j);
			}
			if (cMap[i][j] == 'A' || cMap[i][j] == 'a')
				al++, aa += hp[i][j];
			if (cMap[i][j] == 'B' || cMap[i][j] == 'b')
				bl++, ba += hp[i][j];
		}
	}
	cSetColor(15, 0);
	cGotoXY(msize + 2, 0);
	cout << "红方兵力: " << aa << "   " << endl;
	cout << "红方土地: " << al << "   " << endl;
	cout << "蓝方兵力: " << ba << "   " << endl;
	cout << "蓝方土地: " << bl << "   " << endl;
	gEnd();
}

void gEnd()
{

	if (cMap[0][0] == 'B')
	{
		cout << "蓝方获胜" << endl;
		while (1)
			;
	}
	if (cMap[msize - 1][msize - 1] == 'A')
	{
		cout << "红方获胜" << endl;
		while (1)
			;
	}
}

inline void gChange(int x, int y)
{
	int c;
	char cm = cMap[x][y];
	if (cm == 'A')
		c = cA;
	if (cm == 'a')
		c = ca;
	if (cm == 'B')
		c = cB;
	if (cm == 'b')
		c = cb;
	if (cm == 'C')
		c = cC;
	cGotoXY(x, y * 5);
	cSetColor(15, c);
	int val = hp[x][y];
	if (val < 10)
		cout << "   " << val;
	else if (val < 100)
		cout << "  " << val;
	else if (val < 1000)
		cout << " " << val;
	else if (val < 10000)
		cout << "  " << val / 1000 << "K";
	else if (val < 100000)
		cout << " " << val / 1000 << "K";
	else if (val < 1000000)
		cout << val / 1000 << "K";
	else if (val < 10000000)
		cout << "  " << val / 1000000 << "M";
	else if (val < 100000000)
		cout << " " << val / 1000000 << "M";
	else if (val < 1000000000)
		cout << val / 1000000 << "M";
	else
		cout << "INF";
}