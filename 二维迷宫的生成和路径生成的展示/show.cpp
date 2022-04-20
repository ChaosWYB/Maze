#include<iomanip>
#include<graphics.h>
#define N 15
//#define t Sleep(5);
#define t system("pause");

using namespace std;

//����ȫ�ֱ���

int** maze = nullptr;							//�Թ���ά����

int a = 1, b = 1;								//��ɫ����

IMAGE item(160, 20);							//����
IMAGE board(20 * N, 20 * N);							//��ʾ��

//��������

void show();									//��ʾ

void paint();									//�����Թ�
void init_item();								//�Թ�Ԫ�س�ʼ��
void creat_entrance_exit();						//���������

void creat_edge(int**);							//�����Թ���Ե
void travel_maze(int, int);						//���������Թ�
void escape_maze(int, int);						//�����Թ�����·��
void initialize(int**, int);					//�����ʼ��
void draw(int**, int**, int, int);				//�������޸�

bool judge(int**, int, int, int, int);			//�ж��Ƿ���·����

int main()
{
	for (;;)
	{
		show();
		system("pause");
	}
	return 0;
}

void show()//��Ϸ��ʼ��
{
	srand(unsigned(time(NULL)));				//�������ʱ������
	initgraph(20 * N, 20 * N);
	//init_background();							//��ʼ������
	init_item();								//��ʼ��Ԫ��ͼ��

	//������̬��άά���飨ԭ���飩
	maze = new int* [N];
	for (int i = 0; i < N; ++i)
	{
		maze[i] = new int[N];
	}
	initialize(maze, 0);						//��ԭ�����ʼ��
	creat_edge(maze);							//�����Թ��߽�
	paint();
	system("pause");
	travel_maze(N - 2, N - 2);					//�����Թ������ɱ�׼�Թ�
	creat_entrance_exit();						//���ɳ����
	paint();
	//�ڴ����л����Թ�
	system("pause");
	escape_maze(1, 1);

	paint();									//�����Թ�

	for (int i = 0; i < N; ++i)
	{
		delete[] maze[i];
	}
	delete[] maze;
}

void paint()//�����Թ�
{
	int x = 0, y = 0;
	SetWorkingImage(&board);
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			x = (i) * 20;
			y = (j) * 20;
			if (maze[i][j] == 0)//ǽ
			{
				putimage(x, y, 20, 20, &item, 0, 0);
			}
			if (maze[i][j] == 1)//·
			{
				putimage(x, y, 20, 20, &item, 20, 0);
			}
			if (maze[i][j] == 2)//���
			{
				putimage(x, y, 20, 20, &item, 40, 0);
			}
			if (maze[i][j] == 3)//����
			{
				putimage(x, y, 20, 20, &item, 60, 0);
			}
			if (maze[i][j] == 4)//���
			{
				putimage(x, y, 20, 20, &item, 80, 0);
			}
			if (maze[i][j] == 5)//·��
			{
				putimage(x, y, 20, 20, &item, 100, 0);
			}
			if (maze[i][j] == 6)//�߽�
			{
				putimage(x, y, 20, 20, &item, 120, 0);
			}
			if (maze[i][j] == 11 || maze[i][j] == 12 || maze[i][j] == 14)//��ɫ
			{
				putimage(x, y, 20, 20, &item, 140, 0);
			}
		}
	}
	//����ͼ�񵽴���
	SetWorkingImage();
	putimage(0, 0, 20 * N, 20 * N, &board, 0, 0);
}

void init_item()//��ʼ����Ԫ��ͼ��
{
	SetWorkingImage(&item);
	cleardevice();
	//ǽ
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
	//·
	setorigin(20, 0);
	setfillcolor(BLACK);
	solidrectangle(0, 0, 20, 20);
	//���
	setorigin(40, 0);
	settextcolor(GREEN);
	settextstyle(12, 0, _T("����"));
	outtextxy(4, 4, _T("��"));
	//����
	setorigin(60, 0);
	outtextxy(4, 4, _T("��"));
	//���
	setorigin(80, 0);
	setfillcolor(WHITE);
	solidrectangle(0, 0, 20, 20);
	//·��
	setfillcolor(BLACK);
	setorigin(100, 0);
	solidrectangle(0, 0, 20, 20);
	setfillcolor(RED);
	solidcircle(10, 10, 6);
	//�߽�
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
	//��ɫ
	setorigin(140, 0);
	setfillcolor(RED);
	solidrectangle(0, 0, 20, 20);
	/*setlinecolor(RED);
	line(0, 0, 20, 20);
	line(20, 0, 0, 20);*/
	setorigin(0, 0);
}

void creat_entrance_exit()//��������
{
	maze[1][1] = 2;
	maze[N - 2][N - 2] = 3;
}

void creat_edge(int** maze)//�����ͼ�߽磨maze��Ҫ�����ڵ����飩
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

void travel_maze(int a, int b)//�����Թ�����·��
{
	//������̬��ά�������ڱ��
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
		if (judge(mark, x, y, 2, -1))//����
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 4)
				{
				case 0://��
					if (x + 2 < N - 1 && mark[x + 2][y] == -1)
					{
						++x;
						++mark[x][y];
						++x;
						++mark[x][y];
						go = true;
					}
					break;
				case 1://��
					if (x - 2 > 0 && mark[x - 2][y] == -1)
					{
						--x;
						++mark[x][y];
						--x;
						++mark[x][y];
						go = true;
					}
					break;
				case 2://��
					if (y + 2 < N - 1 && mark[x][y + 2] == -1)
					{
						++y;
						++mark[x][y];
						++y;
						++mark[x][y];
						go = true;
					}
					break;
				case 3://��
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
		else//����
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 4)
				{
				case 0://��
					if (x + 2 < N - 1 && mark[x + 1][y] == 0)
					{
						++mark[x][y];
						++x;
						++mark[x][y];
						++x;
						go = true;
					}
					break;
				case 1://��
					if (x - 2 > 0 && mark[x - 1][y] == 0)
					{
						++mark[x][y];
						--x;
						++mark[x][y];
						--x;
						go = true;
					}
					break;
				case 2://��
					if (y + 2 < N - 1 && mark[x][y + 1] == 0)
					{
						++mark[x][y];
						++y;
						++mark[x][y];
						++y;
						go = true;
					}
					break;
				case 3://��
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
	//�ͷű������
	for (int i = 0; i < N; ++i)
	{
		delete[] mark[i];
	}
	mark = nullptr;
}

void escape_maze(int a, int b)//�õݹ���ݷ�Ѱ���Թ�·��
{
	//���ɶ�̬�������ڱ��
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
		if (judge(escape, x, y, 1, 1))//����
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 4)
				{
				case 0://��
					if (x + 2 < N - 1 && escape[x + 1][y] == 1)
					{
						++x;
						++escape[x][y];
						++x;
						++escape[x][y];
						go = true;
					}
					break;
				case 1://��
					if (x - 2 > 0 && escape[x - 1][y] == 1)
					{
						--x;
						++escape[x][y];
						--x;
						++escape[x][y];
						go = true;
					}
					break;
				case 2://��
					if (y + 2 < N - 1 && escape[x][y + 1] == 1)
					{
						++y;
						++escape[x][y];
						++y;
						++escape[x][y];
						go = true;
					}
					break;
				case 3://��
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
		else//����
		{
			for (;;)
			{
				bool go = false;
				switch (rand() % 4)
				{
				case 0://��
					if (x + 2 < N - 1 && escape[x + 1][y] == 2)
					{
						++escape[x][y];
						++x;
						++escape[x][y];
						++x;
						go = true;
					}
					break;
				case 1://��
					if (x - 2 > 0 && escape[x - 1][y] == 2)
					{
						++escape[x][y];
						--x;
						++escape[x][y];
						--x;
						go = true;
					}
					break;
				case 2://��
					if (y + 2 < N - 1 && escape[x][y + 1] == 2)
					{
						++escape[x][y];
						++y;
						++escape[x][y];
						++y;
						go = true;
					}
					break;
				case 3://��
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

	//�ͷű������
	for (int i = 0; i < N; ++i)
	{
		delete[] escape[i];
	}
	escape = nullptr;
}

void initialize(int** maze, int k)//�����ʼ����maze��Ҫ��ʼ�������飬k:��ʼ�������֣�
{
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < N; ++i)
		{
			maze[i][j] = k;
		}
	}
}

void draw(int** maze, int** mark, int m, int n)//����mark����maze�������λ��markΪm����Ӧλ��mazeΪn��
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

bool judge(int** mark, int x, int y, int m, int n)//�ж��Ƿ���·���ߣ�x��y����������,�ж�������m���Ƿ�Ϊn��
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