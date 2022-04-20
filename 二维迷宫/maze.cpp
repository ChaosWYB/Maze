///////////////////////////////////////////////////
// 程序名称：迷宫 游戏
// 编译环境：Visual C++ 6.0 / 2010，EasyX 2013霜降版
// 作　　者：krissi <zhaoh1987@qq.com>
// 最后修改：2011-3-12
//
#include <graphics.h>
#include <time.h>
#include <iostream>



//////////////////////////////////////////////////////
// 定义全局变量
//
BYTE** g_aryMap = NULL;			// 迷宫地图
SIZE	g_szMap;					// 迷宫地图的尺寸
IMAGE	g_imgSight(360, 280);		// 游戏的视野
RECT	g_rtSight;					// 游戏的视野的范围
IMAGE	g_imgItem(180, 20);			// 地图元素
IMAGE	g_imgGPS;					// 迷你地图，用于显示游戏者在地图中的位置
POINT	g_ptGPS;					// 迷你地图的显示位置
SIZE	g_szGPS;					// 迷你地图的尺寸
POINT	g_ptPlayer;					// 游戏者的位置

// 枚举地图元素，兼做元素位置的 x 坐标
enum MAPITEM { MAP_WALL = 0, MAP_PLAYER = 20, MAP_GROUND = 40, MAP_MARKRED = 60, MAP_MARKGREEN = 80, MAP_MARKYELLOW = 100, MAP_ENTRANCE = 120, MAP_EXIT = 140, MAP_OUTSIDE = 160 };
// 枚举用户的控制命令
enum CMD { CMD_QUIT = 1, CMD_UP = 2, CMD_DOWN = 4, CMD_LEFT = 8, CMD_RIGHT = 16, CMD_MARKRED = 32, CMD_MARKGREEN = 64, CMD_MARKYELLOW = 128, CMD_CLEARMARK = 256 };



//////////////////////////////////////////////////////
// 函数声明
//
void	Welcome();							// 绘制游戏界面
void	InitImage();						// 初始化游戏图片
void	InitGame();							// 初始化游戏数据
void	GetMazeSize();						// 提示用户输入迷宫大小
void	MakeMaze(int width, int height);	// 生成迷宫：初始化（注：宽高必须是奇数）
void	TravelMaze(int x, int y);			// 生成迷宫：遍历 (x, y) 四周
MAPITEM	GetMazeItem(int x, int y);			// 获取指定坐标的迷宫元素
void	Paint();							// 绘制视野范围内的迷宫
int		GetCmd();							// 获取用户输入的命令
void	DispatchCmd(int cmd);				// 处理用户输入的命令
void	OnUp();								// 向上移动
void	OnLeft();							// 向左移动
void	OnRight();							// 向右移动
void	OnDown();							// 向下移动
void	OnMark(MAPITEM value);				// 在地图中做标记
bool	CheckWin();							// 检查是否到出口
bool	Quit();								// 询问用户是否退出游戏



//////////////////////////////////////////////////////
// 函数定义
//


// 主程序
void main()
{
	// 初始化
	initgraph(640, 480);			// 创建绘图窗口
	srand((unsigned)time(NULL));	// 设置随机种子

	// 显示主界面
	Welcome();

	// 初始化
	InitImage();
	InitGame();

	// 游戏过程
	int c;
	while (!(((c = GetCmd()) & CMD_QUIT) && Quit()))
	{
		DispatchCmd(c);
		Paint();
		if (CheckWin())
			break;

		// 延时
		Sleep(100);
	}

	// 清理迷宫地图占用的内存
	for (int x = 0; x < g_szMap.cx + 2; x++)
		delete[] g_aryMap[x];
	delete[] g_aryMap;

	// 关闭图形模式
	closegraph();
}


// 绘制游戏界面
void Welcome()
{
	// 绘制渐变色外框
	for (int i = 0; i < 128; i++)
	{
		setlinecolor(RGB(0, 0, (127 - i) << 1));
		rectangle(149 - i, 109 - (i >> 1), 490 + i, 370 + (i >> 1));
	}

	// 设置字体样式
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);

	// 绘制标题
	settextstyle(36, 0, _T("宋体"));
	outtextxy(248, 40, _T("迷　　宫"));

	// 绘制操作说明
	settextstyle(12, 0, _T("宋体"));
	outtextxy(50, 382, _T("控制说明："));
	outtextxy(74, 400, _T("方向键或 A/S/D/W：移动"));
	outtextxy(74, 418, _T("空格、Y、G：在地图上做红、黄、绿色 M 标记"));
	outtextxy(74, 436, _T("C：清除地图上的标记"));
	outtextxy(74, 454, _T("ESC：退出程序"));
}


// 初始化游戏图片
void InitImage()
{
	// 预绘制游戏图片到 IMAGE 缓存（可以修改为加载图片以获得更好效果）
	SetWorkingImage(&g_imgItem);
	cleardevice();
	// 绘制 PLAYER
	setorigin(MAP_PLAYER, 0);
	setfillcolor(YELLOW);
	setlinecolor(YELLOW);
	fillellipse(2, 2, 17, 17);
	setlinecolor(BLACK);
	line(7, 7, 7, 8);
	line(12, 7, 12, 8);
	arc(5, 6, 14, 14, 3.34, 6.08);
	// 绘制墙壁
	setorigin(MAP_WALL, 0);
	settextcolor(BROWN);
	//setfillstyle((BYTE*)"\x20\x20\x20\xff\x04\x04\x04\xff");
	setfillcolor(BROWN);
	setlinecolor(BLACK);
	solidrectangle(1, 1, 18, 18);
	rectangle(0, 0, 19, 19);
	// 绘制红色标记
	setorigin(MAP_MARKRED, 0);
	setlinecolor(RED);
	moveto(5, 15);
	linerel(0, -10);	linerel(5, 5);	linerel(5, -5);		linerel(0, 10);
	//// 绘制绿色标记
	//setorigin(MAP_MARKGREEN, 0);
	//setlinecolor(GREEN);
	//moveto(5, 15);
	//linerel(0, -10);	linerel(5, 5);	linerel(5, -5);		linerel(0, 10);
	//// 绘制黄色标记
	//setorigin(MAP_MARKYELLOW, 0);
	//setlinecolor(YELLOW);
	//moveto(5, 15);
	//linerel(0, -10);	linerel(5, 5);	linerel(5, -5);		linerel(0, 10);
	// 绘制入口
	setorigin(MAP_ENTRANCE, 0);
	setlinecolor(GREEN);
	settextstyle(12, 0, _T("宋体"));
	outtextxy(4, 4, _T("入"));
	// 绘制出口
	setorigin(MAP_EXIT, 0);
	outtextxy(4, 4, _T("出"));
	// 绘制迷宫外面的空地
	setorigin(MAP_OUTSIDE, 0);
	settextcolor(GREEN);
	setfillstyle((BYTE*)"\x50\x55\x22\x20\x05\x55\x22\x02");
	solidrectangle(0, 0, 19, 19);
	// 恢复坐标系
	setorigin(0, 0);

	//// 显示作者
	//SetWorkingImage();
	//settextcolor(BLUE);
	//TCHAR author[] = _T("Powered by zhaoh1987@qq.com");
	//outtextxy(471, 4, author);
	//settextcolor(LIGHTBLUE);
	//outtextxy(470, 3, author);
}


// 初始化游戏数据
void InitGame()
{
	// 提示用户输入迷宫大小
	GetMazeSize();

	// 初始化参数
	if (g_aryMap != NULL)
	{	// 清理迷宫地图占用的内存
		for (int x = 0; x < g_szMap.cx + 2; x++)
			delete[] g_aryMap[x];
		delete[] g_aryMap;
	}
	MakeMaze(g_szMap.cx, g_szMap.cy);		// 创建迷宫
	g_ptPlayer.x = 2;					// 设置游戏者的位置
	g_ptPlayer.y = 2;
	g_rtSight.left = 0;					// 设置视野范围
	g_rtSight.top = 0;
	g_rtSight.right = 17;
	g_rtSight.bottom = 13;

	//// 设置 GPS 显示区
	//setfillcolor(BLUE);
	//solidrectangle(522, 368, 637, 471);
	//if (g_szMap.cx > g_szMap.cy) { g_szGPS.cx = 100; g_szGPS.cy = (int)(100.0 * g_szMap.cy / g_szMap.cx + 0.5); }
	//else { g_szGPS.cy = 100; g_szGPS.cx = (int)(100.0 * g_szMap.cx / g_szMap.cy + 0.5); }
	//Resize(&g_imgGPS, g_szGPS.cx, g_szGPS.cy);
	//g_ptGPS.x = 530 + 50 - g_szGPS.cx / 2;
	//g_ptGPS.y = 370 + 50 - g_szGPS.cy / 2;
	//// 画迷你地图外框
	//setlinecolor(RED);
	//rectangle(g_ptGPS.x - 1, g_ptGPS.y - 1, g_ptGPS.x + g_szGPS.cx, g_ptGPS.y + g_szGPS.cy);
	//// 画迷你地图入口和出口
	//setlinecolor(YELLOW);
	//moveto(g_ptGPS.x - 8, g_ptGPS.y + g_szGPS.cy / g_szMap.cy);
	//linerel(7, 0);	linerel(-3, -3);	moverel(3, 3);	linerel(-3, 3);
	//moveto(g_ptGPS.x + g_szGPS.cx, g_ptGPS.y + g_szGPS.cy - g_szGPS.cy / g_szMap.cy);
	//linerel(7, 0);	linerel(-3, -3);	moverel(3, 3);	linerel(-3, 3);

	// 绘制游戏区
	Paint();
}


// 提示用户输入迷宫大小
void GetMazeSize()
{
	g_szMap.cx = g_szMap.cy = 0;

	// 获取用户输入的宽高
	TCHAR s[4];
	while (g_szMap.cx < 20 || g_szMap.cx > 200)
	{
		InputBox(s, 4, _T("请输入迷宫的宽度\n范围：20～200"), _T("输入"), _T("25"));
		g_szMap.cx = _ttoi(s);
	}
	while (g_szMap.cy < 20 || g_szMap.cx > 200)
	{
		InputBox(s, 4, _T("请输入迷宫的高度\n范围：20～200"), _T("输入"), _T("25"));
		g_szMap.cy = _ttoi(s);
	}

	// 确保宽高为奇数
	if (g_szMap.cx % 2 != 1) g_szMap.cx++;
	if (g_szMap.cy % 2 != 1) g_szMap.cy++;
}


// 生成迷宫：初始化（注：宽高必须是奇数）
void MakeMaze(int width, int height)
{
	if (width % 2 != 1 || height % 2 != 1)
		return;

	int x, y;

	// 定义迷宫尺寸，并分配迷宫内存
	g_aryMap = new BYTE * [width + 2];
	for (x = 0; x < width + 2; x++)
	{
		g_aryMap[x] = new BYTE[height + 2];
		memset(g_aryMap[x], MAP_WALL, height + 2);
	}

	// 定义边界
	for (x = 0; x <= width + 1; x++)
		g_aryMap[x][0] = g_aryMap[x][height + 1] = MAP_GROUND;

	for (y = 1; y <= height; y++)
		g_aryMap[0][y] = g_aryMap[width + 1][y] = MAP_GROUND;

	// 定义入口和出口
	g_aryMap[1][2] = MAP_ENTRANCE;
	g_aryMap[width][height - 1] = MAP_EXIT;

	// 从任意点开始遍历生成迷宫
	TravelMaze(((rand() % (width - 1)) & 0xfffe) + 2, ((rand() % (height - 1)) & 0xfffe) + 2);

	// 将边界标记为迷宫外
	for (x = 0; x <= width + 1; x++)
		g_aryMap[x][0] = g_aryMap[x][height + 1] = MAP_OUTSIDE;

	for (y = 1; y <= height; y++)
		g_aryMap[0][y] = g_aryMap[width + 1][y] = MAP_OUTSIDE;
}


// 生成迷宫：遍历 (x, y) 四周
void TravelMaze(int x, int y)
{
	// 定义遍历方向
	int d[4][2] = { 0, 1, 1, 0, 0, -1, -1, 0 };

	// 将遍历方向乱序
	int n, t, i;
	for (i = 0; i < 4; i++)
	{
		n = rand() % 4;
		t = d[i][0], d[i][0] = d[n][0], d[n][0] = t;
		t = d[i][1], d[i][1] = d[n][1], d[n][1] = t;
	}

	// 尝试周围四个方向
	g_aryMap[x][y] = MAP_GROUND;
	for (i = 0; i < 4; i++)
		if (g_aryMap[x + 2 * d[i][0]][y + 2 * d[i][1]] == MAP_WALL)
		{
			g_aryMap[x + d[i][0]][y + d[i][1]] = MAP_GROUND;
			TravelMaze(x + d[i][0] * 2, y + d[i][1] * 2);		// 递归
		}
}


// 获取指定坐标的迷宫元素
MAPITEM GetMazeItem(int x, int y)
{
	return (MAPITEM)g_aryMap[x][y];
}


// 绘制视野范围内的迷宫
void Paint()
{
	int x1, y1;

	// 绘制视野内的迷宫
	SetWorkingImage(&g_imgSight);
	for (int x = g_rtSight.left; x <= g_rtSight.right; x++)
		for (int y = g_rtSight.top; y <= g_rtSight.bottom; y++)
		{
			x1 = (x - g_rtSight.left) * 20;
			y1 = (y - g_rtSight.top) * 20;
			putimage(x1, y1, 20, 20, &g_imgItem, GetMazeItem(x, y), 0);
		}

	// 绘制游戏者
	x1 = (g_ptPlayer.x - g_rtSight.left) * 20;
	y1 = (g_ptPlayer.y - g_rtSight.top) * 20;
	putimage(x1, y1, 20, 20, &g_imgItem, MAP_PLAYER, 0);

	//// 绘制迷你地图
	//SetWorkingImage(&g_imgGPS);
	//cleardevice();
	//int tx = (int)((g_ptPlayer.x - 1) * g_szGPS.cx / (double)(g_szMap.cx - 1) + 0.5);
	//int ty = (int)((g_ptPlayer.y - 1) * g_szGPS.cy / (double)(g_szMap.cy - 1) + 0.5);
	//setlinecolor(YELLOW);
	//circle(tx, ty, 1);

	// 更新到绘图窗口
	SetWorkingImage();
	putimage(150, 110, 340, 260, &g_imgSight, 10, 10);
	//putimage(g_ptGPS.x, g_ptGPS.y, &g_imgGPS);
}


// 获取用户输入的命令
int GetCmd()
{
	int c = 0;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)		c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		c |= CMD_DOWN;
	if (GetAsyncKeyState('A') & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState('D') & 0x8000)		c |= CMD_RIGHT;
	if (GetAsyncKeyState('W') & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState('S') & 0x8000)		c |= CMD_DOWN;
	if (GetAsyncKeyState(' ') & 0x8000)		c |= CMD_MARKRED;
	if (GetAsyncKeyState('G') & 0x8000)		c |= CMD_MARKGREEN;
	if (GetAsyncKeyState('Y') & 0x8000)		c |= CMD_MARKYELLOW;
	if (GetAsyncKeyState('C') & 0x8000)		c |= CMD_CLEARMARK;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)		c |= CMD_QUIT;

	return c;
}


// 处理用户输入的命令
void DispatchCmd(int cmd)
{
	if (cmd & CMD_UP)			OnUp();
	if (cmd & CMD_DOWN)			OnDown();
	if (cmd & CMD_LEFT)			OnLeft();
	if (cmd & CMD_RIGHT)		OnRight();
	if (cmd & CMD_MARKRED)		OnMark(MAP_MARKRED);
	if (cmd & CMD_MARKGREEN)	OnMark(MAP_MARKGREEN);
	if (cmd & CMD_MARKYELLOW)	OnMark(MAP_MARKYELLOW);
	if (cmd & CMD_CLEARMARK)	OnMark(MAP_GROUND);
}


// 向上移动
void OnUp()
{
	if (g_ptPlayer.y > 1 && GetMazeItem(g_ptPlayer.x, g_ptPlayer.y - 1) != MAP_WALL)
	{
		g_ptPlayer.y--;

		if (g_ptPlayer.y - g_rtSight.top < 4 && g_rtSight.top > 0)
		{
			g_rtSight.top--;
			g_rtSight.bottom--;
		}
	}
}


// 向左移动
void OnLeft()
{
	if (g_ptPlayer.x > 1 && GetMazeItem(g_ptPlayer.x - 1, g_ptPlayer.y) != MAP_WALL && GetMazeItem(g_ptPlayer.x - 1, g_ptPlayer.y) != MAP_ENTRANCE)
	{
		g_ptPlayer.x--;

		if (g_ptPlayer.x - g_rtSight.left < 5 && g_rtSight.left > 0)
		{
			g_rtSight.left--;
			g_rtSight.right--;
		}
	}
}


// 向右移动
void OnRight()
{
	if (g_ptPlayer.x < g_szMap.cx && GetMazeItem(g_ptPlayer.x + 1, g_ptPlayer.y) != MAP_WALL)
	{
		g_ptPlayer.x++;

		if (g_rtSight.right - g_ptPlayer.x < 5 && g_rtSight.right <= g_szMap.cx)
		{
			g_rtSight.left++;
			g_rtSight.right++;
		}
	}
}


// 向下移动
void OnDown()
{
	if (g_ptPlayer.y < g_szMap.cy && GetMazeItem(g_ptPlayer.x, g_ptPlayer.y + 1) != MAP_WALL)
	{
		g_ptPlayer.y++;

		if (g_rtSight.bottom - g_ptPlayer.y < 4 && g_rtSight.bottom <= g_szMap.cy)
		{
			g_rtSight.top++;
			g_rtSight.bottom++;
		}
	}
}


// 在地图中做标记
void OnMark(MAPITEM value)
{
	g_aryMap[g_ptPlayer.x][g_ptPlayer.y] = value;
}


// 检查是否到出口
bool CheckWin()
{
	if (g_ptPlayer.x == g_szMap.cx && g_ptPlayer.y == g_szMap.cy - 1)
	{
		HWND hwnd = GetHWnd();
		if (MessageBox(hwnd, _T("恭喜你走出来了！\n您想再来一局吗？"), _T("恭喜"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			InitGame();
			return false;
		}
		else
			return true;
	}

	return false;
}


// 询问用户是否退出游戏
bool Quit()
{
	HWND hwnd = GetHWnd();
	return (MessageBox(hwnd, _T("您确定要退出游戏吗？"), _T("询问"), MB_OKCANCEL | MB_ICONQUESTION) == IDOK);
}