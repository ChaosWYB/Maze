/*
迷宫元素数字对应表
0：墙			1：路
2：入口			3：出口
5：路线			6：边界
*/

#include<iostream>
#include<iomanip>
#include<Windows.h>

using namespace std;

//声明全局变量

int*** maze = nullptr;											//迷宫三维数组

int N = 11;														//迷宫阶数
int a = 1, b = 1, c = 1;										//角色坐标

//声明函数

void show();													
void creat_entrance_exit();										//创建出入口
void print();

void creat_edge(int***);										//创建迷宫边缘
void travel_maze(int, int, int);								//遍历生成迷宫
void escape_maze(int, int, int);								//遍历迷宫生成路径
void initialize(int***, int);									//数组初始化
void draw(int***, int***, int, int);							//对数组修改

bool judge(int***, int, int, int, int, int);					//判断是否有路可走

int main()
{
	show();
	return 0;
}

void show()
{
	srand(unsigned(time(NULL)));								//加入随机时间种子

	//创造动态三维数组（原数组）
	maze = new int** [N];
	for (int i = 0; i < N; ++i)
	{
		maze[i] = new int* [N];
		for (int j = 0; j < N; ++j)
		{
			maze[i][j] = new int[N];
		}
	}
	initialize(maze, 0);										//对原数组初始化
	creat_edge(maze);											//创造迷宫边界
	travel_maze(N - 2, N - 2, N - 2);							//遍历迷宫，生成标准迷宫
	creat_entrance_exit();										//生成出入口
	print();
}

void check()//检查输入阶数是否为奇数，并增加二阶作为墙壁
{
	if (N % 2 == 0)
		N += 3;
	else
		N += 2;
}

void creat_entrance_exit()//创造出入口
{
	maze[1][1][1] = 2;
	maze[N - 2][N - 2][N - 2] = 3;
}

void creat_edge(int*** maze)//创造地图边界（maze：要作用于的数组）
{
	for (int k = 0; k < N; ++k)
	{
		for (int j = 0; j < N; ++j)
		{
			for (int i = 0; i < N; ++i)
			{
				if (i == 0 || j == 0 || k == 0 || i == N - 1 || j == N - 1 || k == N - 1)
					maze[i][j][k] = 6;
			}
		}
	}
}

void travel_maze(int a, int b, int c)//遍历迷宫生成路径（从(a,b,c)开始遍历迷宫）
{
	//创建动态三维数组用于标记
	int*** mark = new int** [N];
	for (int i = 0; i < N; ++i)
	{
		mark[i] = new int* [N];
		for (int j = 0; j < N; ++j)
		{
			mark[i][j] = new int[N];
		}
	}
	initialize(mark, -1);
	mark[a][b][c] = 1;
	creat_edge(mark);
	int x = a, y = b, z = c;
	for (;;)
	{
		if (judge(mark, x, y, z, 2, -1))//遍历
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 6)
				{
				case 0:
					if (x + 2 < N - 1 && mark[x + 2][y][z] == -1)
					{
						++x;
						++mark[x][y][z];
						++x;
						++mark[x][y][z];
						go = true;
					}
					break;
				case 1:
					if (x - 2 > 0 && mark[x - 2][y][z] == -1)
					{
						--x;
						++mark[x][y][z];
						--x;
						++mark[x][y][z];
						go = true;
					}
					break;
				case 2:
					if (y + 2 < N - 1 && mark[x][y + 2][z] == -1)
					{
						++y;
						++mark[x][y][z];
						++y;
						++mark[x][y][z];
						go = true;
					}
					break;
				case 3:
					if (y - 2 > 0 && mark[x][y - 2][z] == -1)
					{
						--y;
						++mark[x][y][z];
						--y;
						++mark[x][y][z];
						go = true;
					}
					break;
				case 4:
					if (z + 2 < N - 1 && mark[x][y][z + 2] == -1)
					{
						++z;
						++mark[x][y][z];
						++z;
						++mark[x][y][z];
						go = true;
					}
					break;
				case 5:
					if (z - 2 > 0 && mark[x][y][z - 2] == -1)
					{
						--z;
						++mark[x][y][z];
						--z;
						++mark[x][y][z];
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
				switch (rand() % 6)
				{
				case 0:
					if (x + 2 < N - 1 && mark[x + 1][y][z] == 0)
					{
						++mark[x][y][z];
						++x;
						++mark[x][y][z];
						++x;
						go = true;
					}
					break;
				case 1:
					if (x - 2 > 0 && mark[x - 1][y][z] == 0)
					{
						++mark[x][y][z];
						--x;
						++mark[x][y][z];
						--x;
						go = true;
					}
					break;
				case 2:
					if (y + 2 < N - 1 && mark[x][y + 1][z] == 0)
					{
						++mark[x][y][z];
						++y;
						++mark[x][y][z];
						++y;
						go = true;
					}
					break;
				case 3:
					if (y - 2 > 0 && mark[x][y - 1][z] == 0)
					{
						++mark[x][y][z];
						--y;
						++mark[x][y][z];
						--y;
						go = true;
					}
					break;
				case 4:
					if (z + 2 < N - 1 && mark[x][y][z + 1] == 0)
					{
						++mark[x][y][z];
						++z;
						++mark[x][y][z];
						++z;
						go = true;
					}
					break;
				case 5:
					if (z - 2 > 0 && mark[x][y][z - 1] == 0)
					{
						++mark[x][y][z];
						--z;
						++mark[x][y][z];
						--z;
						go = true;
					}
					break;
				}
				if (go)
					break;
			}
		}
		if (x == a && y == b && z == c)
			break;
	}
	draw(maze, mark, 1, 1);//把位置坐标复制到原数组
	//释放标记数组
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			delete[] mark[i][j];
		}
	}
	mark = nullptr;
}

void escape_maze(int a, int b, int c)//用递归回溯法寻找迷宫路径（寻找从出口到(a,b,c)的路）
{
	//生成动态数组用于标记
	int*** escape = new int** [N];
	for (int i = 0; i < N; ++i)
	{
		escape[i] = new int* [N];
		for (int j = 0; j < N; ++j)
		{
			escape[i][j] = new int[N];
		}
	}
	initialize(escape, -1);
	escape[N - 2][N - 2][N - 2] = 1;
	creat_edge(escape);
	int x = N - 2, y = N - 2, z = N - 2;
	draw(escape, maze, 1, 1);
	for (;;)
	{
		if (judge(escape, x, y, z, 1, 1))//遍历
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 6)
				{
				case 0:
					if (x + 2 < N - 1 && escape[x + 1][y][z] == 1)
					{
						++x;
						++escape[x][y][z];
						++x;
						++escape[x][y][z];
						go = true;
					}
					break;
				case 1:
					if (x - 2 > 0 && escape[x - 1][y][z] == 1)
					{
						--x;
						++escape[x][y][z];
						--x;
						++escape[x][y][z];
						go = true;
					}
					break;
				case 2:
					if (y + 2 < N - 1 && escape[x][y + 1][z] == 1)
					{
						++y;
						++escape[x][y][z];
						++y;
						++escape[x][y][z];
						go = true;
					}
					break;
				case 3:
					if (y - 2 > 0 && escape[x][y - 1][z] == 1)
					{
						--y;
						++escape[x][y][z];
						--y;
						++escape[x][y][z];
						go = true;
					}
					break;
				case 4:
					if (z + 2 < N - 1 && escape[x][y][z + 1] == 1)
					{
						++z;
						++escape[x][y][z];
						++z;
						++escape[x][y][z];
						go = true;
					}
					break;
				case 5:
					if (z - 2 > 0 && escape[x][y][z - 1] == 1)
					{
						--z;
						++escape[x][y][z];
						--z;
						++escape[x][y][z];
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
				switch (rand() % 6)
				{
				case 0:
					if (x + 2 < N - 1 && escape[x + 1][y][z] == 2)
					{
						++escape[x][y][z];
						++x;
						++escape[x][y][z];
						++x;
						go = true;
					}
					break;
				case 1:
					if (x - 2 > 0 && escape[x - 1][y][z] == 2)
					{
						++escape[x][y][z];
						--x;
						++escape[x][y][z];
						--x;
						go = true;
					}
					break;
				case 2:
					if (y + 2 < N - 1 && escape[x][y + 1][z] == 2)
					{
						++escape[x][y][z];
						++y;
						++escape[x][y][z];
						++y;
						go = true;
					}
					break;
				case 3:
					if (y - 2 > 0 && escape[x][y - 1][z] == 2)
					{
						++escape[x][y][z];
						--y;
						++escape[x][y][z];
						--y;
						go = true;
					}
					break;
				case 4:
					if (z + 2 < N - 1 && escape[x][y][z + 1] == 2)
					{
						++escape[x][y][z];
						++z;
						++escape[x][y][z];
						++z;
						go = true;
					}
					break;
				case 5:
					if (z - 2 > 0 && escape[x][y][z - 1] == 2)
					{
						++escape[x][y][z];
						--z;
						++escape[x][y][z];
						--z;
						go = true;
					}
					break;
				}
				if (go)
					break;
			}
		}
		if (x == a && y == b && z == c)
			break;
	}
	draw(maze, escape, 2, 5);
	//释放标记数组
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			delete[] escape[i][j];
		}
	}
	escape = nullptr;
}

void initialize(int*** maze, int m)//数组初始化（maze：要初始化的数组，m:初始化的数字）
{
	for (int k = 0; k < N; ++k)
	{
		for (int j = 0; j < N; ++j)
		{
			for (int i = 0; i < N; ++i)
			{
				maze[i][j][k] = m;
			}
		}
	}
}

void draw(int*** maze, int*** mark, int m, int n)//利用mark绘制maze（如果该位置mark为m，相应位置maze为n）
{
	for (int k = 0; k < N; ++k)
	{
		for (int j = 0; j < N; ++j)
		{
			for (int i = 0; i < N; ++i)
			{
				if (mark[i][j][k] == m)
					maze[i][j][k] = n;
			}
		}
	}
}

bool judge(int*** mark, int x, int y, int z, int m, int n)//判断是否有路可走（x，y，z是所在坐标,判断往下走m格是否为n）
{
	if (x - m > 0 && mark[x - m][y][z] == n)
		return true;
	if (x + m < N - 1 && mark[x + m][y][z] == n)
		return true;
	if (y - m > 0 && mark[x][y - m][z] == n)
		return true;
	if (y + m < N - 1 && mark[x][y + m][z] == n)
		return true;
	if (z - m > 0 && mark[x][y][z - m] == n)
		return true;
	if (z + m < N - 1 && mark[x][y][z + m] == n)
		return true;
	return false;
}

void print()
{
	for (int k = 0; k < N; ++k)
	{
		cout << "第" << k + 1 << "层：" << endl;
		for (int j = 0; j < N; ++j)
		{
			for (int i = 0; i < N; ++i)
			{
				switch (maze[i][j][k])
				{
				case 0:
				case 6:
					cout << "□";
					break;
				case 1:
					cout << "  ";
					break;
				case 2:
					cout << "入";
					break;
				case 3:
					cout << "出";
					break;
				}
			}
			cout << endl;
		}

	}
}