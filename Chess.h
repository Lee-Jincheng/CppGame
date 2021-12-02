#pragma once

#ifndef _CHESS_H_
#define _CHESS_H_

const int MAXN = 3;

class Chess
{
	public:
		Chess();												// 构造函数
		void GameStart();										// 游戏开始
		int IsOver();											// 判断是否游戏结束
		bool IsOffensive();										// 判断先后手进攻
		int Get(int row, int col);								// 绝杀
		int GetNext();											// 获取下一步落子信息
		int GetPoints(int num);									// 获取场上棋子得分数据
		void UpdatePoints();									// 重置场上得分
		void UpdateP1();										// 重置九格得分
		void UpdateP2();										// 重置连线得分
		bool MoveChess(int location, int kind);					// 落子

		void DrawBoard();										// 绘制棋盘
		void DrawChess(int location, int kind);					// 绘制棋子
		void GameOver();										// 游戏结束
		int UserOperation();									// 获取用户输入
		

	private:
		int iBoard[MAXN][MAXN];									// 记录棋盘各位置信息
		bool bCheck[MAXN][MAXN];								// 记录该位置是否落子
		int iCp[2];												// 记录棋子信息
		int iSum;												// 记录当前棋子数
		bool bPlay;												// 状态码，是否进行游戏
		int Points1[2][4];										// 记录连线得分
		int Points2[3][3];										// 记录九格得分

};

//---------------------------------------------------------*/

Chess::Chess()
{
	for (int i = 0; i < MAXN; i++)								// 初始化棋盘
	{
		for (int j = 0; j < MAXN; j++)
		{
			iBoard[i][j] = 0;
			bCheck[i][j] = 0;
			Points2[i][j] = 0;
			gsl::at(iBoard, 3);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 3; j < 4; j++)
		{
			Points1[i][j] = 0;
		}
	}

	iSum = 0;
	iCp[0] = 1;													// iCp[0]表示计算机,默认为棋子 “O”
	iCp[1] = 2;													// iCp[1]表示用户,默认为棋子“X”
	bPlay = true;
}

/*-----------------------------------------------------------*/

void
Chess::GameStart()
{
	while (bPlay)
	{
		new (this) Chess();
		bool bFlag = IsOffensive();

		DrawBoard();

		if (!bFlag)
		{
			srand((unsigned)time(NULL));

			if (rand() % 2)
			{
				MoveChess(1, iCp[0]);
			}
			else
			{
				MoveChess(5, iCp[0]);
			}
		}

		int temp;
		bool IsReplay = false;

		while (iSum != MAXN * MAXN)
		{
			temp = UserOperation();

			if (temp == 0)
			{
				IsReplay = true;
				break;
			}

			if (!MoveChess(temp, iCp[1]))
			{
				continue;
			}

			if (IsOver() || iSum == MAXN * MAXN)
			{
				break;
			}

			MoveChess(GetNext(), iCp[0]);

			if (IsOver())
			{
				break;
			}
		}

		if (IsReplay)
		{
			continue;
		}

		GameOver();
	}
}

/*-----------------------------------------------------------*/

int
Chess::IsOver()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (Points1[i][j] == 3)
			{
				return 1;
			}
			else if (Points1[i][j] == -3)
			{
				return 2;
			}
		}
	}

	return 0;
}

/*-----------------------------------------------------------*/

int 
Chess::GetPoints(int num)
{
	if (iCp[0] == num)
	{
		return 1;
	}
	else if (iCp[1] == num)
	{
		return -1;
	}

	return 0;
}

/*-----------------------------------------------------------*/

void
Chess::UpdatePoints()
{
	UpdateP1();
	UpdateP2();
}

/*-----------------------------------------------------------*/

void
Chess::UpdateP1()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Points1[i][j] = 0;
		}
	}

	for (int i = 0; i < MAXN; i++)
	{
		for (int j = 0; j < MAXN; j++)
		{
			Points1[0][i] += GetPoints(iBoard[j][i]);
			Points1[1][i] += GetPoints(iBoard[i][j]);
		}
	}

	Points1[0][MAXN] = GetPoints(iBoard[0][0]) + GetPoints(iBoard[1][1]) + GetPoints(iBoard[2][2]);
	Points1[1][MAXN] = GetPoints(iBoard[2][0]) + GetPoints(iBoard[1][1]) + GetPoints(iBoard[0][2]);
}

/*-----------------------------------------------------------*/

void
Chess::UpdateP2()
{
	Points2[0][0] = Points1[0][0] + Points1[1][0] + Points1[0][3];
	Points2[0][1] = Points1[0][1] + Points1[1][0];
	Points2[0][2] = Points1[0][2] + Points1[1][0] + Points1[1][3];
	Points2[1][0] = Points1[0][0] + Points1[1][1];
	Points2[1][1] = Points1[0][1] + Points1[1][1] + Points1[0][3] + Points1[1][3];
	Points2[1][2] = Points1[0][2] + Points1[1][1];
	Points2[2][0] = Points1[0][0] + Points1[1][2] + Points1[1][3];
	Points2[2][1] = Points1[0][1] + Points1[1][2];
	Points2[2][2] = Points1[0][2] + Points1[1][2] + Points1[0][3];
}

/*-----------------------------------------------------------*/

int
Chess::Get(int row, int col)
{
	int i = 0;													// 找未落子的位置

	if (row == 0 && col < MAXN)
	{
		for (i = 0; i < MAXN; i++)
		{
			if (!bCheck[i][col])
			{
				return i * 3 + col + 1;
			}
		}
	}

	if (row == 1 && col < MAXN)
	{
		for (i = 0; i < MAXN; i++)
		{
			if (!bCheck[col][i])
			{
				return col * 3 + i + 1;
			}
		}
	}

	if (row == 0 && col == MAXN)								// 主对角线
	{
		for (i = 0; i < MAXN; i++)
		{
			if (!bCheck[i][i])
			{
				return i * 3 + i + 1;
			}
		}
	}

	if (row == 1 && col == MAXN)								// 次对角线
	{
		if (!bCheck[0][2])
		{
			return 3;
		}
		else if (!bCheck[1][1])
		{
			return 5;
		}
		else if (!bCheck[2][0])
		{
			return 7;
		}
	}

	return 0;
}

/*-----------------------------------------------------------*/

int
Chess::GetNext()
{
	bool bFlagOfUser = false;									// 用户是否存在两连子
	bool bFlagOfCptr = false;									// 电脑是否存在两连子
	int UserP[2] = { 0 };										// 用户两连子位置
	int CptrP[2] = { 0 };										// 电脑两连子位置

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (Points1[i][j] == 2)								// 用户存在两连子
			{
				bFlagOfUser = true;
																// 保存其行列位置
				UserP[0] = i;
				UserP[1] = j;
			}
			else if (Points1[i][j] == -2)						// 电脑存在两连子
			{
				bFlagOfCptr = true;
																// 保存其行列位置
				CptrP[0] = i;									
				CptrP[1] = j;
			}
		}
	}
																// 考虑是否即将胜利或失败
	if (bFlagOfUser)
	{
		return Get(UserP[0], UserP[1]);
	}

	if (bFlagOfCptr)
	{
		return Get(CptrP[0], CptrP[1]);
	}
																// 考虑棋盘边棋子优先级大于中心
	if (!bCheck[0][1] && Points2[0][1] == 0 && !bCheck[1][0] && Points2[1][0] == 1)
	{
		return 4;
	}

	if (!bCheck[0][1] && Points2[0][1] == 1 && !bCheck[1][0] && Points2[1][0] == 0)
	{
		return 2;
	}
																// 若中心棋子未被占用，则程序优先落子
	if (!bCheck[1][1])
	{
		return 5;
	}

	int index = 0;
	int book[MAXN * MAXN] = { 0 };

	for (int i = 0; i < MAXN; i++)
	{
		for (int j = 0; j < MAXN; j++)
		{
			if (!bCheck[i][j])
			{
				book[index ++] = Points2[i][j];

				if (Points2[i][j] == 2)
				{
					return i * 3 + j + 1;
				}
			}
		}
	}

	for (int i = 0; i < index - 1; i++)
	{
		for (int j = 0; j < index - i - 1; j++)
		{
			if (book[j] > book[j + 1])
			{
				int temp = book[j];
				book[j] = book[j + 1];
				book[j + 1] = temp;
			}
		}
	}

	int mid = 0;

	if (index % 2 == 0)
	{
		mid = book[index / 2 - 1];
	}
	else
	{
		mid = book[index / 2];
	}

	index = 0;
																// 在遍历，寻找 mid 位置
	for (int i = 0; i < MAXN; i++)
	{
		for (int j = 0; j < MAXN; j++)
		{
			if (!bCheck[i][j] && Points2[i][j] == mid)
			{
				book[index++] = i * 3 + j + 1;					// 保存得分位置，并计数 +1
			}
		}
	}

	for (int i = index - 1; i >= 0; i--)
	{
		if (book[i] == 1 || book[i] == 3 || book[i] == 7 || book[i] == 9)
		{
			return book[i];
		}
	}

	for (int i = 0; i < index; i++)
	{
		if (book[i] == 2 || book[i] == 4 || book[i] == 6 || book[i] == 8)
		{
			return book[i];
		}
	}

	return 0;
}

/*-----------------------------------------------------------*/

bool
Chess::MoveChess(int location, int kind)
{
	if (location == 0)
	{
		HWND hwnd = GetHWnd();									// 获得标准句柄

		MessageBox(hwnd, _T("程序异常，程序退出"), _T("Error"), MB_OK | MB_ICONHAND);
		exit(0);
	}
																// 求取行列坐标
	int row = (location - 1) / 3;
	int col = (location - 1) % 3;

	if (bCheck[row][col])
	{
		return false;
	}

	bCheck[row][col] = true;									// 修改位置
	iBoard[row][col] = kind;									// 修改棋子信息

	UpdatePoints();												// 重置得分
	iSum++;														// 棋子增加
	
	DrawChess(location, kind);									// 绘制棋子

	return true;
}

/*-----------------------------------------------------------*/

bool
Chess::IsOffensive()
{																// 设置背景
	setfillstyle(BS_SOLID);
	setfillcolor(RGB(255, 205, 150));							// 设置填充颜色
	solidrectangle(0, 0, 640, 480);								// 使用当前填充样式画无边框的填充矩形

																// 设置字体
	setbkmode(TRANSPARENT);										// 文字输出时背景透明
	settextcolor(RGB(200, 20, 20));								// 设置字体颜色
	settextstyle(30, 0, _T("宋体"));								// 设置字体大小以及样式

	outtextxy(235, 155, _T("请选择攻或守"));						// 在指定位置输出字符
	settextcolor(RGB(20, 20, 100));
	outtextxy(245, 265, _T("进攻"));
	settextcolor(RGB(20, 100, 20));
	outtextxy(335, 265, _T("防守"));								// 鼠标操作
																
	ExMessage msg;

	while (1)
	{
		// 这个函数用于获取一个消息。如果当前消息队列中没有，就一直等待
		msg = getmessage(EM_MOUSE | EM_KEY);					// 读取鼠标或键盘消息

		if (WM_LBUTTONDOWN == msg.message)
		{														// 先手判断区域
			if (msg.x > 245 && msg.x < 305 &&
				msg.y > 265 && msg.y < 295)
			{
				return true;
			}
			// 后手判断区域
			if (msg.x > 335 && msg.x < 395 &&
				msg.y > 265 && msg.y < 295)
			{
				return false;
			}

			Sleep(10);
		}
	}
}

/*-----------------------------------------------------------*/

void
Chess::DrawBoard()
{																// 设置背景同上参数
	setfillstyle(BS_SOLID);
	setfillcolor(RGB(255, 205, 150));
	solidrectangle(0, 0, 640, 480);
	
	settextcolor(RGB(200, 50, 150));
	outtextxy(500, 35, _T("重新开始"));							// 设置选项

	setlinecolor(RGB(0, 0, 0));									// 设置格线颜色
	setlinestyle(PS_SOLID, 5);									// 设置线条为实线,像素为 5 

	line(275, 115, 275, 365);									// 设置线条起始终止位置
	line(360, 115, 360, 365);
	line(195, 195, 445, 195);
	line(195, 280, 445, 280);

}

/*-----------------------------------------------------------*/

void
Chess::DrawChess(int location, int kind)
{
	int row = (location - 1) / 3;
	int col = (location - 1) % 3;

	int offset1 = row * 5;
	int offset2 = col * 5;

	setlinestyle(PS_SOLID, 3);

	if (kind == 1)												// 画⚪
	{
		setlinecolor(RGB(255, 255, 255));
		circle(234 + col * 80 + offset2, 154 + row * 80 + offset1, 30);
	}
	else if (kind == 2)											// 画 X
	{
		setlinecolor((RGB(0, 0, 0)));

		line(210 + col * 80 + offset2, 130 + row * 80 + offset1,
			 260 + col * 80 + offset2, 180 + row * 80 + offset1);
		line(260 + col * 80 + offset2, 130 + row * 80 + offset1,
			 210 + col * 80 + offset2, 180 + row * 80 + offset1);

	}
}

/*-----------------------------------------------------------*/

int
Chess::UserOperation()
{
	ExMessage msg;

	while (1)
	{
		msg = getmessage(EM_MOUSE || EM_KEY);

		if (WM_LBUTTONDOWN == msg.message)
		{
			for (int i = 0; i < MAXN; i++)
			{
				for (int j = 0; j < MAXN; j++)
				{
					if (msg.x > i * 85 + 195 && msg.x < (i + 1) * 80 + i * 5 + 195 &&
						msg.y > j * 85 + 115 && msg.y < (j + 1) * 80 + j * 5 + 115)
					{
						return j * 3 + i + 1;
					}
				}
			}

			if (msg.x > 500 && msg.x < 620 && msg.y > 35 && msg.y < 65)
			{
				return 0;
			}
		}

		Sleep(10);
	}
}

/*-----------------------------------------------------------*/

void
Chess::GameOver()
{
	int iFlag = IsOver();

	settextcolor(RGB(200, 0, 0));

	if (iFlag == 0)
	{
		outtextxy(285, 15, _T("平局"));
	}
	else if (iFlag == 1)
	{
		outtextxy(285, 15, _T("失败"));
	}
	else if (iFlag == 2)
	{
		outtextxy(285, 15, _T("胜利"));
	}

	outtextxy(245, 65, _T("继续"));
	outtextxy(335, 65, _T("退出"));

	ExMessage msg;

	while (1)
	{
		msg = getmessage(EM_MOUSE || EM_KEY);

		if (WM_LBUTTONDOWN == msg.message)						// 读取鼠标左键操作
		{
			if (msg.x > 245 && msg.x < 305 && msg.y > 65 && msg.y < 95)
			{
				return;											// 游戏继续
			}

			if (msg.x > 335 && msg.x < 395 && msg.y > 65 && msg.y < 95)
			{
				return;											// 游戏退出
			}

			if (msg.x > 500 && msg.x < 620 && msg.y > 35 && msg.y < 65)
			{
				return;											// 重新开始
			}
		}
	}
}

#endif // _CHESS_H_