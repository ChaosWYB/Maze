/*
�Թ�Ԫ�����ֶ�Ӧ��
0��ǽ		1��·		2�����
3������		4��			5��·��
6���߽�		11��12��14����
*/

#include<iostream>
#include<iomanip>
#include<Windows.h>
#include<graphics.h>

using namespace std;

//����ȫ�ֱ���

int** maze = nullptr;							//�Թ���ά����

int N = 0;										//�Թ�����
int a = 1, b = 1;								//��ɫ����
int map_top = 0, map_bottom = 13;				//��ʾ�����±�����
int map_left = 0, map_right = 17;				//��ʾ�����ұ�����

bool quit = false;								//�ж��˳�
bool running_maze = true;						//�ƶ��Թ�����

unsigned long long t1 = 0, t2 = 0;				//����ʱ��Ա�

IMAGE item(160, 20);							//����
IMAGE board(360, 280);							//��ʾ��

//��������

void begin();									//��Ϸ��ʼ��
void play();									//��ʼ��Ϸ
void exit();									//��Ϸ�������˳���Ϸ

void check();									//����Թ�����
void paint();									//�����Թ�
void init_item();								//�Թ�Ԫ�س�ʼ��
void init_background();							//�Թ�������ʼ��
void creat_entrance_exit();						//���������

void do_cmd(int);								//ִ���û�ָ��
void creat_edge(int**);							//�����Թ���Ե
void travel_maze(int, int);						//���������Թ�
void escape_maze(int, int);						//�����Թ�����·��
void initialize(int**, int);					//�����ʼ��
void draw(int**, int**, int, int);				//�������޸�

bool end(int, int);								//�жϽ�����Ϸ
bool judge(int**, int, int, int, int);			//�ж��Ƿ���·����

int get_cmd();									//��ȡ�û�ָ��

int main()
{
	//��Ϸ��ʼ��
	begin();
	//��ʼ��Ϸ
	play();
	//��Ϸ�������˳���Ϸ
	exit();
	return 0;
}

void begin()//��Ϸ��ʼ��
{
	srand(unsigned(time(NULL)));				//�������ʱ������
	cout << "�������Թ�����(����20)" << endl;
	cin >> N;
	while (N < 20)
	{
		cout << "������󣡣���" << endl;
		cin >> N;
	}
	check();

	//������̬��άά���飨ԭ���飩
	maze = new int* [N];
	for (int i = 0; i < N; ++i)
	{
		maze[i] = new int[N];
	}
	initialize(maze, 0);						//��ԭ�����ʼ��
	creat_edge(maze);							//�����Թ��߽�
	travel_maze(N - 2, N - 2);					//�����Թ������ɱ�׼�Թ�
	creat_entrance_exit();						//���ɳ����
	maze[1][1] = 12;

	//�ڴ����л����Թ�
	initgraph(640, 480);
	init_background();							//��ʼ������
	init_item();								//��ʼ��Ԫ��ͼ��
	paint();									//�����Թ�
}

void play()//��ʼ��Ϸ
{
	//������Ϸ���ж��û�ָ��
	int c = -1;
	t1 = clock();
	while (c = get_cmd())
	{
		do_cmd(c);								//ִ��ָ��
		Sleep(80);
		//�ж��Ƿ��߳��Թ�
		if (end(a, b))
		{
			cout << "��ϲ�߳��Թ�������" << endl;
			break;
		}
		//�ƶ��Թ�
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
	//��Ϸ����
	cout << "��л�����汾��Ϸ��" << endl;
	//�ͷ�����
	for (int i = 0; i < N; ++i)
	{
		delete[] maze[i];
	}
	maze = nullptr;
}

void check()//�����������Ƿ�Ϊ�����������Ӷ�����Ϊǽ��
{
	if (N % 2 == 0)
		N += 3;
	else
		N += 2;
}

void paint()//�����Թ�
{
	int x = 0, y = 0;
	SetWorkingImage(&board);
	for (int j = map_top; j < map_bottom; ++j)
	{
		for (int i = map_left; i < map_right; ++i)
		{
			x = (i - map_left) * 20;
			y = (j - map_top) * 20;
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
	putimage(150, 110, 340, 260, &board, 0, 0);
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
	setlinecolor(RED);
	setfillcolor(BLACK);
	solidrectangle(0, 0, 20, 20);
	line(5, 5, 15, 15);
	line(5, 15, 15, 5);
	//·��
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
	setfillcolor(YELLOW);
	setlinecolor(YELLOW);
	fillellipse(2, 2, 17, 17);
	setlinecolor(BLACK);
	line(7, 7, 7, 8);
	line(12, 7, 12, 8);
	arc(5, 6, 14, 14, 3.34, 6.08);
	setorigin(0, 0);
}

void init_background()//��ʼ������
{
	//��ͼ��߿�
	for (int i = 0; i < 5; ++i)
	{
		setlinecolor(DARKGRAY);
		rectangle(149 - i, 109 - i, 491 + i, 371 + i);
	}
	//��ͼ�⽥��ɫ
	for (int i = 0; i < 100; ++i)
	{
		setlinecolor(RGB((50 - 0.5 * i), (150 - 1.5 * i), (250 - 2.5 * i)));
		rectangle(144 - i, 104 - i, 496 + i, 376 + i);
	}
	//���Ʊ���
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, _T("����"));
	outtextxy(196, 20, _T("������Թ�"));
	//���Ʋ���ָʾ
	settextstyle(18, 0, _T("����"));
	outtextxy(80, 377, _T("����˵����"));
	outtextxy(120, 395, _T("������� A/S/D/W���ƶ�    C: ������ʾ����·��"));
	outtextxy(120, 413, _T("B���������·��           N����/�ر� �ƶ��Թ�"));
	outtextxy(120, 431, _T("ESC���˳�����"));
	//�����ƶ��Թ�����
	outtextxy(265, 460, _T("�ƶ��Թ�����"));
}

void creat_entrance_exit()//��������
{
	maze[1][1] = 2;
	maze[N - 2][N - 2] = 3;
}

void do_cmd(int cmd)//ִ���û�ָ�cmd��ָ����룩
{
	if (cmd == 1)//�����ƶ�
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
	else if (cmd == 2)//�����ƶ�
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
	else if (cmd == 3)//�����ƶ�
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
	else if (cmd == 4)//�����ƶ�
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
	else if (cmd == 5)//��������·��
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
	else if (cmd == 6)//�������·��
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
	else if (cmd == 7)//��/�ر��ƶ��Թ�
	{
		SetWorkingImage();
		setfillcolor(BLACK);
		solidrectangle(265, 460, 373, 478);
		if (running_maze == true)
		{
			running_maze = false;
			settextstyle(18, 0, _T("����"));
			outtextxy(265, 460, _T("�ƶ��Թ�����"));
		}
		else
		{
			running_maze = true;
			settextstyle(18, 0, _T("����"));
			outtextxy(265, 460, _T("�ƶ��Թ�����"));
		}
	}
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
		if (x == a && y == b)
			break;
	}
	draw(maze, mark, 1, 1);//��λ�����긴�Ƶ�ԭ����
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
	escape[N - 2][N - 2] = 1;
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
		if (x == a && y == b)
			break;
	}
	draw(maze, escape, 2, 5);
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

bool end(int x, int y)//�ж��Ƿ��߳��Թ���x��yΪ�������꣩
{
	if ((x == N - 3 && y == N - 2) || (x == N - 2 && y == N - 3))
		return true;
	return false;
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

int get_cmd()//��ȡ�û�ָ��
{
	int c = -1;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)//esc��
		c = 0;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)//�����
		c = 1;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)//�ҷ����
		c = 2;
	if (GetAsyncKeyState(VK_UP) & 0x8000)//�Ϸ����
		c = 3;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)//�·����
		c = 4;
	if (GetAsyncKeyState('A') & 0x8000)//A��
		c = 1;
	if (GetAsyncKeyState('D') & 0x8000)//D��
		c = 2;
	if (GetAsyncKeyState('W') & 0x8000)//W��
		c = 3;
	if (GetAsyncKeyState('S') & 0x8000)//S��
		c = 4;
	if (GetAsyncKeyState('C') & 0x8000)//C��
		c = 5;
	if (GetAsyncKeyState('B') & 0x8000)//B��
		c = 6;
	if (GetAsyncKeyState('N') & 0x8000)//N��
		c = 7;
	return c;
}