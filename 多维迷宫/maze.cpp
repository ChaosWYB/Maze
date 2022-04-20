/*
迷宫元素数字对应表
0：墙		1：路		2：入口
3：出口		4：			5：路线
6：边界		11、12、14：人
*/

#include<iostream>
#include<iomanip>
#include<Windows.h>
#include<graphics.h>

using namespace std;

//声明全局变量

int** maze = nullptr;							//迷宫二维数组

int N = 0;										//迷宫阶数
int a = 1, b = 1;								//角色坐标
int map_top = 0, map_bottom = 13;				//显示屏上下边坐标
int map_left = 0, map_right = 17;				//显示屏左右边坐标

bool quit = false;								//判断退出
bool running_maze = true;						//移动迷宫开关

unsigned long long t1 = 0, t2 = 0;				//用于时间对比

IMAGE item(160, 20);							//画板
IMAGE board(360, 280);							//显示屏

//声明函数

void begin();									//游戏初始化
void play();									//开始游戏
void exit();									//游戏结束，退出游戏

void check();									//检查迷宫阶数
void paint();									//绘制迷宫
void init_item();								//迷宫元素初始化
void init_background();							//迷宫背景初始化
void creat_entrance_exit();						//创建出入口

void do_cmd(int);								//执行用户指令
void creat_edge(int**);							//创建迷宫边缘
void travel_maze(int, int);						//遍历生成迷宫
void escape_maze(int, int);						//遍历迷宫生成路径
void initialize(int**, int);					//数组初始化
void draw(int**, int**, int, int);				//对数组修改

bool end(int, int);								//判断结束游戏
bool judge(int**, int, int, int, int);			//判断是否有路可走

int get_cmd();									//获取用户指令

int main()
{
	//游戏初始化
	begin();
	//开始游戏
	play();
	//游戏结束，退出游戏
	exit();
	return 0;
}

void begin()//游戏初始化
{
	srand(unsigned(time(NULL)));				//加入随机时间种子
	cout << "请输入迷宫阶数(大于20)" << endl;
	cin >> N;
	while (N < 20)
	{
		cout << "输入错误！！！" << endl;
		cin >> N;
	}
	check();

	//创建动态二维维数组（原数组）
	maze = new int* [N];
	for (int i = 0; i < N; ++i)
	{
		maze[i] = new int[N];
	}
	initialize(maze, 0);						//对原数组初始化
	creat_edge(maze);							//创造迷宫边界
	travel_maze(N - 2, N - 2);					//遍历迷宫，生成标准迷宫
	creat_entrance_exit();						//生成出入口
	maze[1][1] = 12;

	//在窗口中绘制迷宫
	initgraph(640, 480);
	init_background();							//初始化背景
	init_item();								//初始化元素图像
	paint();									//绘制迷宫
}

void play()//开始游戏
{
	//进入游戏，判断用户指令
	int c = -1;
	t1 = clock();
	while (c = get_cmd())
	{
		do_cmd(c);								//执行指令
		Sleep(80);
		//判断是否走出迷宫
		if (end(a, b))
		{
			cout << "恭喜走出迷宫！！！" << endl;
			break;
		}
		//移动迷宫
		t2 = clock();
		int delta_t = 10000 + rand() % 10000;
		if (a % 2 == 1 && b % 2 == 1 && running_maze && (t2 - t1 > delta_t))
		{
			initialize(maze, 0);
			creat_edge(maze);
			travel_maze(a, b);
			creat_entrance_exit();
			if (a == 1 && b == 1)
			{
				maze[a][b] = 12;
			}
			else
				maze[a][b] = 11;
			paint();
			t1 = clock();
		}
	}
}

void exit()
{
	//游戏结束
	cout << "感谢您试玩本游戏！" << endl;
	//释放数组
	for (int i = 0; i < N; ++i)
	{
		delete[] maze[i];
	}
	maze = nullptr;
}

void check()//检查输入阶数是否为奇数，并增加二阶作为墙壁
{
	if (N % 2 == 0)
		N += 3;
	else
		N += 2;
}

void paint()//绘制迷宫
{
	int x = 0, y = 0;
	SetWorkingImage(&board);
	for (int j = map_top; j < map_bottom; ++j)
	{
		for (int i = map_left; i < map_right; ++i)
		{
			x = (i - map_left) * 20;
			y = (j - map_top) * 20;
			if (maze[i][j] == 0)//墙
			{
				putimage(x, y, 20, 20, &item, 0, 0);
			}
			if (maze[i][j] == 1)//路
			{
				putimage(x, y, 20, 20, &item, 20, 0);
			}
			if (maze[i][j] == 2)//入口
			{
				putimage(x, y, 20, 20, &item, 40, 0);
			}
			if (maze[i][j] == 3)//出口
			{
				putimage(x, y, 20, 20, &item, 60, 0);
			}
			if (maze[i][j] == 4)//标记
			{
				putimage(x, y, 20, 20, &item, 80, 0);
			}
			if (maze[i][j] == 5)//路线
			{
				putimage(x, y, 20, 20, &item, 100, 0);
			}
			if (maze[i][j] == 6)//边界
			{
				putimage(x, y, 20, 20, &item, 120, 0);
			}
			if (maze[i][j] == 11 || maze[i][j] == 12 || maze[i][j] == 14)//角色
			{
				putimage(x, y, 20, 20, &item, 140, 0);
			}
		}
	}
	//更新图像到窗口
	SetWorkingImage();
	putimage(150, 110, 340, 260, &board, 0, 0);
}

void init_item()//初始化各元素图像
{
	SetWorkingImage(&item);
	cleardevice();
	//墙
	setorigin(0, 0);
	setfillcolor(LIGHTGRAY);
	setlinecolor(BLACK);
	solidrectangle(1, 1, 19, 19);
	line(1, 5, 19, 5);
	line(1, 10, 19, 10);
	line(1, 15, 19, 15);
	line(10, 1, 10, 5);
	line(10, 10, 10, 15);
	line(5, 5, 5, 10);
	line(5, 15, 5, 20);
	line(15, 5, 15, 10);
	line(15, 15, 15, 20);
	rectangle(0, 0, 20, 20);
	//路
	setorigin(20, 0);
	setfillcolor(BLACK);
	solidrectangle(0, 0, 20, 20);
	//入口
	setorigin(40, 0);
	settextcolor(GREEN);
	settextstyle(12, 0, _T("宋体"));
	outtextxy(4, 4, _T("入"));
	//出口
	setorigin(60, 0);
	outtextxy(4, 4, _T("出"));
	//标记
	setorigin(80, 0);
	setlinecolor(RED);
	setfillcolor(BLACK);
	solidrectangle(0, 0, 20, 20);
	line(5, 5, 15, 15);
	line(5, 15, 15, 5);
	//路线
	setorigin(100, 0);
	solidrectangle(0, 0, 20, 20);
	setfillcolor(RED);
	solidcircle(10, 10, 6);
	//边界
	setorigin(120, 0);
	setfillcolor(LIGHTGRAY);
	setlinecolor(BLACK);
	solidrectangle(1, 1, 19, 19);
	line(1, 5, 19, 5);
	line(1, 10, 19, 10);
	line(1, 15, 19, 15);
	line(10, 1, 10, 5);
	line(10, 10, 10, 15);
	line(5, 5, 5, 10);
	line(5, 15, 5, 20);
	line(15, 5, 15, 10);
	line(15, 15, 15, 20);
	rectangle(0, 0, 20, 20);
	//角色
	setorigin(140, 0);
	setfillcolor(YELLOW);
	setlinecolor(YELLOW);
	fillellipse(2, 2, 17, 17);
	setlinecolor(BLACK);
	line(7, 7, 7, 8);
	line(12, 7, 12, 8);
	arc(5, 6, 14, 14, 3.34, 6.08);
	setorigin(0, 0);
}

void init_background()//初始化背景
{
	//地图外边框
	for (int i = 0; i < 5; ++i)
	{
		setlinecolor(DARKGRAY);
		rectangle(149 - i, 109 - i, 491 + i, 371 + i);
	}
	//地图外渐变色
	for (int i = 0; i < 100; ++i)
	{
		setlinecolor(RGB((50 - 0.5 * i), (150 - 1.5 * i), (250 - 2.5 * i)));
		rectangle(144 - i, 104 - i, 496 + i, 376 + i);
	}
	//绘制标题
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, _T("楷体"));
	outtextxy(196, 20, _T("永恒的迷宫"));
	//绘制操作指示
	settextstyle(18, 0, _T("宋体"));
	outtextxy(80, 377, _T("控制说明："));
	outtextxy(120, 395, _T("方向键或 A/S/D/W：移动    C: 绘制显示逃脱路线"));
	outtextxy(120, 413, _T("B：清除逃脱路线           N：打开/关闭 移动迷宫"));
	outtextxy(120, 431, _T("ESC：退出程序"));
	//绘制移动迷宫开关
	outtextxy(265, 460, _T("移动迷宫：开"));
}

void creat_entrance_exit()//创造出入口
{
	maze[1][1] = 2;
	maze[N - 2][N - 2] = 3;
}

void do_cmd(int cmd)//执行用户指令（cmd：指令代码）
{
	if (cmd == 1)//向左移动
	{
		if (maze[a - 1][b] != 0 && maze[a - 1][b] != 6)
		{
			maze[a][b] -= 10;
			--a;
			if (maze[a][b] == 5)
				maze[a][b] = 11;
			else
				maze[a][b] += 10;
			if (a - map_left < 5 && map_left > 0)
			{
				--map_left;
				--map_right;
			}
			paint();
		}
	}
	else if (cmd == 2)//向右移动
	{
		if (maze[a + 1][b] != 0 && maze[a + 1][b] != 6)
		{
			maze[a][b] -= 10;
			++a;
			if (maze[a][b] == 5)
				maze[a][b] = 11;
			else
				maze[a][b] += 10;
			if (map_right - a < 5 && map_right < N)
			{
				++map_left;
				++map_right;
			}
			paint();
		}
	}
	else if (cmd == 3)//向上移动
	{
		if (maze[a][b - 1] != 0 && maze[a][b - 1] != 6)
		{
			maze[a][b] -= 10;
			--b;
			if (maze[a][b] == 5)
				maze[a][b] = 11;
			else
				maze[a][b] += 10;
			if (b - map_top < 4 && map_top > 0)
			{
				--map_top;
				--map_bottom;
			}
			paint();
		}
	}
	else if (cmd == 4)//向下移动
	{
		if (maze[a][b + 1] != 0 && maze[a][b + 1] != 6)
		{
			maze[a][b] -= 10;
			++b;
			if (maze[a][b] == 5)
				maze[a][b] = 11;
			else
				maze[a][b] += 10;
			if (map_bottom - b < 4 && map_bottom < N)
			{
				++map_top;
				++map_bottom;
			}
			paint();
		}
	}
	else if (cmd == 5)//绘制逃脱路线
	{
		if (a % 2 == 1 && b % 2 == 1)
		{
			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					if (maze[i][j] == 5)
						maze[i][j] = 1;
				}
			}
			escape_maze(a, b);
			paint();
		}
	}
	else if (cmd == 6)//清除逃脱路线
	{
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				if (maze[i][j] == 5)
					maze[i][j] = 1;
			}
		}
		paint();
	}
	else if (cmd == 7)//打开/关闭移动迷宫
	{
		SetWorkingImage();
		setfillcolor(BLACK);
		solidrectangle(265, 460, 373, 478);
		if (running_maze == true)
		{
			running_maze = false;
			settextstyle(18, 0, _T("宋体"));
			outtextxy(265, 460, _T("移动迷宫：关"));
		}
		else
		{
			running_maze = true;
			settextstyle(18, 0, _T("宋体"));
			outtextxy(265, 460, _T("移动迷宫：开"));
		}
	}
}

void creat_edge(int** maze)//创造地图边界（maze：要作用于的数组）
{
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			if (i == 0 || j == 0 || i == N - 1 || j == N - 1)
				maze[i][j] = 6;
		}
	}
}

void travel_maze(int a, int b)//遍历迷宫生成路径
{
	//创建动态二维数组用于标记
	int** mark = new int* [N];
	for (int i = 0; i < N; ++i)
	{
		mark[i] = new int[N];
	}
	initialize(mark, -1);
	mark[a][b] = 1;
	creat_edge(mark);
	int x = a, y = b;
	for (;;)
	{
		if (judge(mark, x, y, 2, -1))//遍历
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 4)
				{
				case 0://右
					if (x + 2 < N - 1 && mark[x + 2][y] == -1)
					{
						++x;
						++mark[x][y];
						++x;
						++mark[x][y];
						go = true;
					}
					break;
				case 1://左
					if (x - 2 > 0 && mark[x - 2][y] == -1)
					{
						--x;
						++mark[x][y];
						--x;
						++mark[x][y];
						go = true;
					}
					break;
				case 2://下
					if (y + 2 < N - 1 && mark[x][y + 2] == -1)
					{
						++y;
						++mark[x][y];
						++y;
						++mark[x][y];
						go = true;
					}
					break;
				case 3://上
					if (y - 2 > 0 && mark[x][y - 2] == -1)
					{
						--y;
						++mark[x][y];
						--y;
						++mark[x][y];
						go = true;
					}
					break;
				}
				if (go)
					break;
			}
		}
		else//回溯
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 4)
				{
				case 0://右
					if (x + 2 < N - 1 && mark[x + 1][y] == 0)
					{
						++mark[x][y];
						++x;
						++mark[x][y];
						++x;
						go = true;
					}
					break;
				case 1://左
					if (x - 2 > 0 && mark[x - 1][y] == 0)
					{
						++mark[x][y];
						--x;
						++mark[x][y];
						--x;
						go = true;
					}
					break;
				case 2://上
					if (y + 2 < N - 1 && mark[x][y + 1] == 0)
					{
						++mark[x][y];
						++y;
						++mark[x][y];
						++y;
						go = true;
					}
					break;
				case 3://下
					if (y - 2 > 0 && mark[x][y - 1] == 0)
					{
						++mark[x][y];
						--y;
						++mark[x][y];
						--y;
						go = true;
					}
					break;
				}
				if (go)
					break;
			}
		}
		if (x == a && y == b)
			break;
	}
	draw(maze, mark, 1, 1);//把位置坐标复制到原数组
	//释放标记数组
	for (int i = 0; i < N; ++i)
	{
		delete[] mark[i];
	}
	mark = nullptr;
}

void escape_maze(int a, int b)//用递归回溯法寻找迷宫路径
{
	//生成动态数组用于标记
	int** escape = new int* [N];
	for (int i = 0; i < N; ++i)
	{
		escape[i] = new int[N];
	}
	initialize(escape, -1);
	escape[N - 2][N - 2] = 1;
	creat_edge(escape);
	int x = N - 2, y = N - 2;
	draw(escape, maze, 1, 1);
	for (;;)
	{
		if (judge(escape, x, y, 1, 1))//遍历
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 4)
				{
				case 0://右
					if (x + 2 < N - 1 && escape[x + 1][y] == 1)
					{
						++x;
						++escape[x][y];
						++x;
						++escape[x][y];
						go = true;
					}
					break;
				case 1://左
					if (x - 2 > 0 && escape[x - 1][y] == 1)
					{
						--x;
						++escape[x][y];
						--x;
						++escape[x][y];
						go = true;
					}
					break;
				case 2://下
					if (y + 2 < N - 1 && escape[x][y + 1] == 1)
					{
						++y;
						++escape[x][y];
						++y;
						++escape[x][y];
						go = true;
					}
					break;
				case 3://上
					if (y - 2 > 0 && escape[x][y - 1] == 1)
					{
						--y;
						++escape[x][y];
						--y;
						++escape[x][y];
						go = true;
					}
					break;
				}
				if (go)
					break;
			}
		}
		else//回溯
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 4)
				{
				case 0://右
					if (x + 2 < N - 1 && escape[x + 1][y] == 2)
					{
						++escape[x][y];
						++x;
						++escape[x][y];
						++x;
						go = true;
					}
					break;
				case 1://左
					if (x - 2 > 0 && escape[x - 1][y] == 2)
					{
						++escape[x][y];
						--x;
						++escape[x][y];
						--x;
						go = true;
					}
					break;
				case 2://上
					if (y + 2 < N - 1 && escape[x][y + 1] == 2)
					{
						++escape[x][y];
						++y;
						++escape[x][y];
						++y;
						go = true;
					}
					break;
				case 3://下
					if (y - 2 > 0 && escape[x][y - 1] == 2)
					{
						++escape[x][y];
						--y;
						++escape[x][y];
						--y;
						go = true;
					}
					break;
				}
				if (go)
					break;
			}
		}
		if (x == a && y == b)
			break;
	}
	draw(maze, escape, 2, 5);
	//释放标记数组
	for (int i = 0; i < N; ++i)
	{
		delete[] escape[i];
	}
	escape = nullptr;
}

void initialize(int** maze, int k)//数组初始化（maze：要初始化的数组，k:初始化的数字）
{
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			maze[i][j] = k;
		}
	}
}

void draw(int** maze, int** mark, int m, int n)//利用mark绘制maze（如果该位置mark为m，相应位置maze为n）
{
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			if (mark[i][j] == m)
				maze[i][j] = n;
		}
	}
}

bool end(int x, int y)//判断是否走出迷宫（x，y为人物坐标）
{
	if ((x == N - 3 && y == N - 2) || (x == N - 2 && y == N - 3))
		return true;
	return false;
}

bool judge(int** mark, int x, int y, int m, int n)//判断是否有路可走（x，y是所在坐标,判断往下走m格是否为n）
{
	if (x - m > 0 && mark[x - m][y] == n)
		return true;
	if (x + m < N - 1 && mark[x + m][y] == n)
		return true;
	if (y - m > 0 && mark[x][y - m] == n)
		return true;
	if (y + m < N - 1 && mark[x][y + m] == n)
		return true;
	return false;
}

int get_cmd()//获取用户指令
{
	int c = -1;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)//esc键
		c = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)//左方向键
		c = 1;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)//右方向键
		c = 2;
	if (GetAsyncKeyState(VK_UP) & 0x8000)//上方向键
		c = 3;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)//下方向键
		c = 4;
	if (GetAsyncKeyState('A') & 0x8000)//A键
		c = 1;
	if (GetAsyncKeyState('D') & 0x8000)//D键
		c = 2;
	if (GetAsyncKeyState('W') & 0x8000)//W键
		c = 3;
	if (GetAsyncKeyState('S') & 0x8000)//S键
		c = 4;
	if (GetAsyncKeyState('C') & 0x8000)//C键
		c = 5;
	if (GetAsyncKeyState('B') & 0x8000)//B键
		c = 6;
	if (GetAsyncKeyState('N') & 0x8000)//N键
		c = 7;
	return c;
}