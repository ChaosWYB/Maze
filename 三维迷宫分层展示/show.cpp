/*
�Թ�Ԫ�����ֶ�Ӧ��
0��ǽ			1��·
2�����			3������
5��·��			6���߽�
*/

#include<iostream>
#include<iomanip>
#include<Windows.h>

using namespace std;

//����ȫ�ֱ���

int*** maze = nullptr;											//�Թ���ά����

int N = 11;														//�Թ�����
int a = 1, b = 1, c = 1;										//��ɫ����

//��������

void show();													
void creat_entrance_exit();										//���������
void print();

void creat_edge(int***);										//�����Թ���Ե
void travel_maze(int, int, int);								//���������Թ�
void escape_maze(int, int, int);								//�����Թ�����·��
void initialize(int***, int);									//�����ʼ��
void draw(int***, int***, int, int);							//�������޸�

bool judge(int***, int, int, int, int, int);					//�ж��Ƿ���·����

int main()
{
	show();
	return 0;
}

void show()
{
	srand(unsigned(time(NULL)));								//�������ʱ������

	//���춯̬��ά���飨ԭ���飩
	maze = new int** [N];
	for (int i = 0; i < N; ++i)
	{
		maze[i] = new int* [N];
		for (int j = 0; j < N; ++j)
		{
			maze[i][j] = new int[N];
		}
	}
	initialize(maze, 0);										//��ԭ�����ʼ��
	creat_edge(maze);											//�����Թ��߽�
	travel_maze(N - 2, N - 2, N - 2);							//�����Թ������ɱ�׼�Թ�
	creat_entrance_exit();										//���ɳ����
	print();
}

void check()//�����������Ƿ�Ϊ�����������Ӷ�����Ϊǽ��
{
	if (N % 2 == 0)
		N += 3;
	else
		N += 2;
}

void creat_entrance_exit()//��������
{
	maze[1][1][1] = 2;
	maze[N - 2][N - 2][N - 2] = 3;
}

void creat_edge(int*** maze)//�����ͼ�߽磨maze��Ҫ�����ڵ����飩
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

void travel_maze(int a, int b, int c)//�����Թ�����·������(a,b,c)��ʼ�����Թ���
{
	//������̬��ά�������ڱ��
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
		if (judge(mark, x, y, z, 2, -1))//����
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
		else//����
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
	draw(maze, mark, 1, 1);//��λ�����긴�Ƶ�ԭ����
	//�ͷű������
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			delete[] mark[i][j];
		}
	}
	mark = nullptr;
}

void escape_maze(int a, int b, int c)//�õݹ���ݷ�Ѱ���Թ�·����Ѱ�Ҵӳ��ڵ�(a,b,c)��·��
{
	//���ɶ�̬�������ڱ��
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
		if (judge(escape, x, y, z, 1, 1))//����
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
		else//����
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
	//�ͷű������
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			delete[] escape[i][j];
		}
	}
	escape = nullptr;
}

void initialize(int*** maze, int m)//�����ʼ����maze��Ҫ��ʼ�������飬m:��ʼ�������֣�
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

void draw(int*** maze, int*** mark, int m, int n)//����mark����maze�������λ��markΪm����Ӧλ��mazeΪn��
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

bool judge(int*** mark, int x, int y, int z, int m, int n)//�ж��Ƿ���·���ߣ�x��y��z����������,�ж�������m���Ƿ�Ϊn��
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
		cout << "��" << k + 1 << "�㣺" << endl;
		for (int j = 0; j < N; ++j)
		{
			for (int i = 0; i < N; ++i)
			{
				switch (maze[i][j][k])
				{
				case 0:
				case 6:
					cout << "��";
					break;
				case 1:
					cout << "  ";
					break;
				case 2:
					cout << "��";
					break;
				case 3:
					cout << "��";
					break;
				}
			}
			cout << endl;
		}

	}
}