#include<iomanip>
#include<graphics.h>
#define N 15
//#define t Sleep(5);
#define t system("pause");

using namespace std;

//声明全局变量

int** maze = nullptr;							//迷宫二维数组

int a = 1, b = 1;								//角色坐标

IMAGE item(160, 20);							//画板
IMAGE board(20 * N, 20 * N);							//显示屏

//声明函数

void show();									//演示

void paint();									//绘制迷宫
void init_item();								//迷宫元素初始化
void creat_entrance_exit();						//创建出入口

void creat_edge(int**);							//创建迷宫边缘
void travel_maze(int, int);						//遍历生成迷宫
void escape_maze(int, int);						//遍历迷宫生成路径
void initialize(int**, int);					//数组初始化
void draw(int**, int**, int, int);				//对数组修改

bool judge(int**, int, int, int, int);			//判断是否有路可走

int main()
{
	for (;;)
	{
		show();
		system("pause");
	}
	return 0;
}

void show()//游戏初始化
{
	srand(unsigned(time(NULL)));				//加入随机时间种子
	initgraph(20 * N, 20 * N);
	//init_background();							//初始化背景
	init_item();								//初始化元素图像

	//创建动态二维维数组（原数组）
	maze = new int* [N];
	for (int i = 0; i < N; ++i)
	{
		maze[i] = new int[N];
	}
	initialize(maze, 0);						//对原数组初始化
	creat_edge(maze);							//创造迷宫边界
	paint();
	system("pause");
	travel_maze(N - 2, N - 2);					//遍历迷宫，生成标准迷宫
	creat_entrance_exit();						//生成出入口
	paint();
	//在窗口中绘制迷宫
	system("pause");
	escape_maze(1, 1);

	paint();									//绘制迷宫

	for (int i = 0; i < N; ++i)
	{
		delete[] maze[i];
	}
	delete[] maze;
}

void paint()//绘制迷宫
{
	int x = 0, y = 0;
	SetWorkingImage(&board);
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			x = (i) * 20;
			y = (j) * 20;
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
	putimage(0, 0, 20 * N, 20 * N, &board, 0, 0);
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
	setfillcolor(WHITE);
	solidrectangle(0, 0, 20, 20);
	//路线
	setfillcolor(BLACK);
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
	setfillcolor(RED);
	solidrectangle(0, 0, 20, 20);
	/*setlinecolor(RED);
	line(0, 0, 20, 20);
	line(20, 0, 0, 20);*/
	setorigin(0, 0);
}

void creat_entrance_exit()//创造出入口
{
	maze[1][1] = 2;
	maze[N - 2][N - 2] = 3;
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
		draw(maze, mark, 1, 1);
		draw(maze, mark, 0, 4);
		paint();
		//t
			if (x == a && y == b)
				break;
	}
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
	escape[N - 2][N - 2] = 4;
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
		draw(maze, escape, 1, 1);
		draw(maze, escape, 2, 5);
		draw(maze, escape, 3, 11);
		draw(maze, escape, 4, 3);
		paint();
		t
			if (x == a && y == b)
				break;
	}

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