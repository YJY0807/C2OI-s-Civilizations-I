#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

using namespace std;

const int cA = 5;	  //red city
const int cB = 3;	  //blue city
const int ca = 4;	  //red ground
const int cb = 1;	  //blue ground
const int cC = 0;	  //black city
const int msize = 25; //map size
const int mount = 25; //mountain count
const int grey = 25;  //city count
const int pmain = 1;  //城市hp每回合增加量
const int lcnt = 25;  //地面hp增加1所需回合

char cMap[msize][msize]; //大写字母表示城市，小写表示地面，Aa是红，Bb是蓝，C是白，X是障碍
int hp[msize][msize];
int nCount, sx, sy;							//回合累加，和光标的坐标
bool bVis[msize][msize];					//bfs
int dir[4][2] = {1, 0, -1, 0, 0, 1, 0, -1}; //bfs

//决策信息：位置为(x,y)的数移到(x+dx,y+dy)
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

void vInit();					   //生成地图
inline void vMove(int dx, int dy); //移动光标
//以下5个函数，保证A=='A'||A=='B'
inline void vMoveNum(int dx, int dy, char A);
inline void vMoveNum(int x, int y, int dx, int dy, char A);
inline void vDiv(int dx, int dy, char A);
inline void vDecide(char A);
inline dRet bfs(int x, int y, char A);
inline void vPlus();			   //每回合数值增加，计算army和land
inline void vChange(int x, int y); //刷新屏幕上指定坐标的数
inline void gotoxy(int x, int y);  //移动输出的位置
inline void color(int t, int b);   //设置输出颜色，t为文字色，b为背景色
void vMain0();
void vMain1();
void vEnd(); //判断游戏结束

int main()
{
	bool p;
	int fs;
	cout << "玩法 (0: 玩家 VS AI, 1: AI VS AI) : ";
	cin >> p;
	cout << "字体大小: ";
	cin >> fs;
	system("cls");
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = (fs / 3 != 0 ? fs / 3 : fs / 3 + 1);
	cfi.dwFontSize.Y = fs;
	cfi.FontWeight = FW_THIN;
	cfi.FontFamily = FF_DONTCARE;
	wcscpy(cfi.FaceName, L"Raster");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	if (p)
		vMain0();
	else
		vMain1();
	return 0;
}

void vMain0()
{
	int in;
	bool cur = true;
	vInit();
	for (nCount = 0;; nCount++)
	{
		gotoxy(msize, 0);
		color(15, 0);
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
						vMove(-1, 0);
					if (in == 80)
						vMove(1, 0);
					if (in == 75)
						vMove(0, -1);
					if (in == 77)
						vMove(0, 1);
				}
			} while (in != ' ' && in != 'f');
			if (in == 'f')
				cur = false;
		}
		vDecide('A');
		vDecide('B');
		vPlus();
	}
}

void vMain1()
{
	int in;
	vInit();
	for (nCount = 0;; nCount++)
	{
		gotoxy(msize, 0);
		color(15, 0);
		cout << "第" << nCount << "回合" << endl;
		do
		{
			in = getch();
			if (in == 224)
			{
				in = getch();
				if (in == 72)
					vMove(-1, 0);
				if (in == 80)
					vMove(1, 0);
				if (in == 75)
					vMove(0, -1);
				if (in == 77)
					vMove(0, 1);
			}
		} while (in != 'w' && in != 's' && in != 'a' && in != 'd' && in != 'z' && in != ' ');
		if (in == 'w')
			vMoveNum(-1, 0, 'A');
		if (in == 's')
			vMoveNum(1, 0, 'A');
		if (in == 'a')
			vMoveNum(0, -1, 'A');
		if (in == 'd')
			vMoveNum(0, 1, 'A');
		if (in == 'z')
		{
			in = getch();
			if (in == 'w')
				vDiv(-1, 0, 'A');
			if (in == 's')
				vDiv(1, 0, 'A');
			if (in == 'a')
				vDiv(0, -1, 'A');
			if (in == 'd')
				vDiv(0, 1, 'A');
		}
		vDecide('B');
		vPlus();
	}
}

void vInit()
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
	hp[0][0] = pmain;
	vChange(0, 0);
	cMap[msize - 1][msize - 1] = 'B';
	hp[msize - 1][msize - 1] = pmain;
	vChange(msize - 1, msize - 1);
	for (i = 1; i <= mount; i++)
	{
		x = rand() % msize;
		y = rand() % msize;
		if (cMap[x][y] != 0)
			i--;
		else
		{
			cMap[x][y] = 'X';
			gotoxy(x, y * 5);
			color(15, 0);
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
			vChange(x, y);
		}
	}
	sx = sy = 0;
	gotoxy(0, 3);
	color(15, 0);
	cout << "<";
}

inline void vMove(int dx, int dy)
{
	color(15, 0);
	dx += sx;
	dy += sy;
	if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize))
		return;
	gotoxy(sx, sy * 5 + 4);
	cout << " ";
	sx = dx;
	sy = dy;
	gotoxy(sx, sy * 5 + 4);
	cout << "<";
	gotoxy(msize + 1, 0);
	cout << "拥有者: 玩家" << cMap[dx][dy] << ", 兵力: " << hp[dx][dy] << "            ";
}

inline void vMoveNum(int x, int y, int dx, int dy, char A)
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
	vChange(dx, dy);
	vChange(x, y);
}

inline void vMoveNum(int dx, int dy, char A)
{
	vMoveNum(sx, sy, dx, dy, A);
	vMove(dx, dy);
}

inline void vDiv(int dx, int dy, char A)
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
	vChange(dx, dy);
	vChange(sx, sy);
	vMove(dx - sx, dy - sy);
}

inline void vDecide(char A)
{
	char a, B, b;
	a = A - 'A' + 'a';
	B = (A == 'A') ? 'B' : 'A';
	b = B - 'A' + 'a';
	int i, j, k, dx, dy;
	dRet tmp, res = {0, 0, 0, 0};
	bool bd = 0, bm = 0;
	for (i = (A == 'A') ? 0 : (msize - 1); (A == 'A') ? (i <= msize - 1) : (i >= 0); (A == 'A') ? (i++) : (i--))
	{
		for (j = (A == 'A') ? 0 : (msize - 1); (A == 'A') ? (j <= msize - 1) : (j >= 0); (A == 'A') ? (j++) : (j--))
		{
			if (!bd && (cMap[i][j] == B || cMap[i][j] == 'C'))
			{
				tmp = bfs(i, j, A);
				if (tmp.dx + tmp.dy == 0)
					continue;
				bd = true;
				res = tmp;
			}
			if (!bd && !bm && (cMap[i][j] == A || cMap[i][j] == a) && hp[i][j] > 1)
			{
				for (k = 0; k < min(msize - 10, nCount / 800 + 4); k++)
				{
					dx = dir[k][0] + i;
					dy = dir[k][1] + j;
					if (!(dx >= 0 && dx < msize && dy >= 0 && dy < msize))
						continue;
					if ((cMap[dx][dy] == 0 || cMap[dx][dy] == b) && hp[dx][dy] < hp[i][j] - 1)
					{
						bm = true;
						res.dx = dir[k][0];
						res.dy = dir[k][1];
						res.x = i;
						res.y = j;
						break;
					}
				}
			}
			if (bd)
				break;
		}
		if (bd)
			break;
	}
	if (!bd && !bm)
		return;
	vMoveNum(res.x, res.y, res.dx, res.dy, A);
}

inline dRet bfs(int x, int y, char A)
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
					nxt.step = now.step + 1;
				if (cMap[dx][dy] == b || cMap[dx][dy] == B || cMap[dx][dy] == 'C')
					nxt.step = now.step + hp[dx][dy] + 2;
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

inline void vPlus()
{
	int i, j, aa = 0, ba = 0, al = 0, bl = 0;
	if (nCount % lcnt == 0)
	{
		for (i = 0; i < msize; i++)
		{
			for (j = 0; j < msize; j++)
			{
				if (cMap[i][j] == 'a' || cMap[i][j] == 'b')
				{
					hp[i][j]++;
					vChange(i, j);
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
				hp[i][j] += pmain;
				vChange(i, j);
			}
			if (cMap[i][j] == 'A' || cMap[i][j] == 'a')
				al++, aa += hp[i][j];
			if (cMap[i][j] == 'B' || cMap[i][j] == 'b')
				bl++, ba += hp[i][j];
		}
	}
	color(15, 0);
	gotoxy(msize + 2, 0);
	cout << "红方兵力: " << aa << "   " << endl;
	cout << "红方土地: " << al << "   " << endl;
	cout << "蓝方兵力: " << ba << "   " << endl;
	cout << "蓝方土地: " << bl << "   " << endl;
	vEnd();
}

void vEnd()
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

inline void vChange(int x, int y)
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
	gotoxy(x, y * 5);
	color(15, c);
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

inline void gotoxy(int x, int y)
{
	COORD c;
	c.X = y;
	c.Y = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

inline void color(int t, int b)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), t + b * 16);
}
