#pragma once
#ifndef _2048GAME_H_
#define _2048GAME_H_

#include <iostream>
#include <cstdio>
#include <cstring>
#include <conio.h>
#include <graphics.h>

constexpr int MAXN = 5;
constexpr COLORREF BKC = RGB(250, 248, 239);				// �涨����ɫ��

class Basic
{
	public:
	int score;												// �Ʒ�
	int best = 0;											// ��߷�
	int Table[MAXN][MAXN];									// ����
	int Table_backup[MAXN][MAXN];							// ������
	bool IsMove[MAXN][MAXN];								// �ж��Ƿ��ƶ�

	void Initialize();										// ��ʼ����Ϸ
	void DrawMenu();										// ���ƿ�ʼ����
	void DrawPionts();										// ���Ƶ÷�
	void DrawBest();										// ������ѵ÷�
	void DrawTable();										// ���Ƹ���
	void UserOperaion();									// �û�����
	bool IsOver();											// ��Ϸ����
	bool IsWin();											// �ж��Ƿ��ʤ
}
Game;

//------------------------------------------------------------------*/

void
Basic::Initialize()
{
	setbkcolor(BKC);										// ����������ñ�����ɫ
	setbkmode(TRANSPARENT);									// ��������������õ�ǰ�豸ͼ�������������ʱ�ı���ģʽ��͸����

	score = 0;
	//best = 0;

	memset(Table, 0, sizeof(Table));
	/*�������������ʼλ��*/
	int x1 = rand() % 4 + 1, y1 = rand() % 4 + 1;
	int x2 = rand() % 4 + 1, y2 = rand() % 4 + 1;

	Table[x1][y1] = 2;
	Table[x2][y2] = 2;
}

/*----------------------------------------------------------------------*/

void
Basic::DrawMenu()
{
	//BeginBatchDraw();										// ����������ڿ�ʼ������ͼ
															// ִ�к��κλ�ͼ����������ʱ���������ͼ������
															// ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����
																/*���ƽ�������*/
	cleardevice();											// ʹ�õ�ǰ����ɫ��ջ�ͼ�豸
	settextcolor(RGB(120, 110, 100));
	settextstyle(50, 0, _T("����"));
	outtextxy(10, 10, _T("2048"));
	// �����ַ��� �Լ� �����ַ���б�Ƕ�,�����ϸ,б�壬�»��ߣ�ɾ����
	settextstyle(20, 0, L"΢���ź�", 0, 0, 550, false, false, false);
	outtextxy(10, 65, _T("�ƶ�����������ӵõ�2048���ɻ�ʤ"));

	setfillcolor(RGB(190, 170, 160));

	//EndBatchDraw();											// // ����������ڽ���������ͼ
}

/*----------------------------------------------------------------------*/

void
Basic::DrawPionts()
{
	/*���Ƶ�ǰ����*/
	solidroundrect(200, 15, 290, 60, 5, 5);					// ����������ڻ��ޱ߿�����Բ�Ǿ���
	settextcolor(RGB(230, 220, 220));

	settextstyle(15, 0, L"΢���ź�", 0, 0, 600, false, false, false);
	outtextxy(230, 20, _T("�÷�"));

	wchar_t wScore[10] = L"";
	swprintf_s(wScore, L"%d", score);						// ����������ڲ�ͬ����ƴ��
	settextcolor(WHITE);
	settextstyle(25, 0, L"΢���ź�", 0, 0, 600, false, false, false);

	RECT r = { 200, 30, 290, 60 };							// �������������ָ����������ָ����ʽ����ַ���
	drawtext(wScore, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

/*----------------------------------------------------------------------*/

void
Basic::DrawBest()
{																/*������ѵ÷�*/
	solidroundrect(295, 15, 385, 60, 5, 5);
	settextcolor(RGB(230, 220, 210));
	settextstyle(15, 0, L"΢���ź�", 0, 0, 600, false, false, false);
	outtextxy(330, 20, _T("��ѵ÷�"));

	wchar_t wBest[10] = L"";
	swprintf_s(wBest, L"%d", best);
	settextcolor(WHITE);
	settextstyle(25, 0, L"΢���ź�", 0, 0, 600, false, false, false);

	RECT r = { 295, 30, 385, 60 };
	drawtext(wBest, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

/*----------------------------------------------------------------------*/

void
Basic::DrawTable()
{																/*���Ƹ���*/
	solidroundrect(10, 90, 390, 470, 5, 5);
	settextstyle(23, 0, L"΢���ź�");
	settextcolor(WHITE);

	for (int i = 1; i < MAXN; i++)
	{
		for (int j = 1; j < MAXN; j++)
		{
			if (Table[i][j])
			{
				setfillcolor(RGB((unsigned int)(BKC - 3 * (Table[i][j] ^ 29)) % 256,
								 (unsigned int)(BKC - 11 * (Table[i][j] ^ 23)) % 256,
								 (unsigned int)(BKC + 7 * (Table[i][j] ^ 37)) % 256));

				solidroundrect(94 * j - 80, 94 * i, 94 * j + 10, 94 * i + 90, 5, 5);

				wchar_t wNum[10] = L"";
				swprintf_s(wNum, L"%d", Table[i][j]);

				RECT r = { 94 * j - 80, 94 * i, 94 * j + 10, 94 * i + 90 };
				drawtext(wNum, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}
}

/*----------------------------------------------------------------------*/

bool
Basic::IsOver()
{																/*�ж��Ƿ���Ϸ����*/
	for (int i = 1; i < MAXN; i++)
	{
		for (int j = 1; j < MAXN; j++)
		{
			if (!Table[i][j] || Table[i][j] == Table[i + 1][j] ||
				Table[i][j] == Table[i - 1][j] || Table[i][j] == Table[i][j + 1] ||
				Table[i][j] == Table[i][j - 1])
			{
				return false;
			}
		}
	}

	return true;
}

/*----------------------------------------------------------------------*/

bool
Basic::IsWin()
{																/*�ж��Ƿ��ʤ*/
	for (int i = 1; i < MAXN; i++)
	{
		for (int j = 1; j < MAXN; j++)
		{
			if (Table[i][j] == 2048)
			{
				return true;
			}
		}
	}

	return false;
}

/*----------------------------------------------------------------------*/

void
Basic::UserOperaion()
{																/*�û�����*/
	memcpy(Table_backup, Table, sizeof(Table));
	memset(IsMove, false, sizeof(IsMove));

	char key = _getch();

	if (key == -32)
	{
		key = -_getch();
	}

	switch (key)
	{
	case 'w':
	case 'W':
	case -72:
		for (int j = 1; j < MAXN; j++)
		{
			for (int i = 2; i < MAXN; i++)
			{
				if (!Table[i][j])
				{
					continue;
				}

				int k = i;

				while (!Table[k - 1][j] && k > 1)
				{
					Table[k - 1][j] = Table[k][j];
					Table[k][j] = 0;

					k--;
				}

				if (Table[k - 1][j] == Table[k][j] && !IsMove[k - 1][j])
				{
					Table[k - 1][j] = Table[k][j] * 2;
					Table[k][j] = 0;

					IsMove[k - 1][j] = true;

					score += Table[k - 1][j];
				}
			}
		}

		break;
	case 's':
	case 'S':
	case -80:
		for (int j = 1; j < MAXN; j++)
		{
			for (int i = 3; i >= 1; i--)
			{
				if (!Table[i][j])
				{
					continue;
				}

				int k = i;

				while (!Table[k + 1][j] && k <= 3)
				{
					Table[k + 1][j] = Table[k][j];
					Table[k][j] = 0;

					k++;
				}

				if (Table[k + 1][j] == Table[k][j] && !IsMove[k + 1][j])
				{
					Table[k + 1][j] = Table[k][j] * 2;
					Table[k][j] = 0;

					IsMove[k + 1][j] = true;

					score += Table[k + 1][j];
				}
			}
		}

		break;
	case 'd':
	case 'D':
	case -77:
		for (int i = 1; i < MAXN; i++)
		{
			for (int j = 3; j >= 1; j--)
			{
				if (!Table[i][j])
				{
					continue;
				}

				int k = j;

				while (!Table[i][k + 1] && k <= 3)
				{
					Table[i][k + 1] = Table[i][k];
					Table[i][k] = 0;

					k++;
				}

				if (Table[i][k + 1] == Table[i][k] && !IsMove[i][k + 1])
				{
					Table[i][k + 1] = Table[i][k] * 2;
					Table[i][k] = 0;

					IsMove[i][k + 1] = true;

					score += Table[i][k + 1];
				}
			}
		}

		break;
	case 'a':
	case 'A':
	case -75:
		for (int i = 1; i < MAXN; i++)
		{
			for (int j = 2; j < MAXN; j++)
			{
				if (!Table[i][j])
				{
					continue;
				}

				int k = j;

				while (!Table[i][k - 1] && k > 1)
				{
					Table[i][k - 1] = Table[i][k];
					Table[i][k] = 0;

					k--;
				}

				if (Table[i][k - 1] == Table[i][k] && !IsMove[i][k - 1])
				{
					Table[i][k - 1] = Table[i][k] * 2;
					Table[i][k] = 0;

					IsMove[i][k - 1] = true;

					score += Table[i][k - 1];
				}
			}
		}

		break;
	}

	bool bFlag = false;

	for (int i = 1; i < MAXN; i++)
	{
		for (int j = 1; j < MAXN; j++)
		{
			if (Table[i][j] != Table_backup[i][j])
			{
				bFlag = true;
				break;
			}
		}
	}

	if (!bFlag)
	{
		return;
	}

	int x = 0, y = 0;

	do
	{
		x = rand() % 4 + 1;
		y = rand() % 4 + 1;
	} while (Table[x][y]);

	int probbty = rand() % 6;

	if (probbty == 5)
	{
		Table[x][y] = 4;
	}
	else
	{
		Table[x][y] = 2;
	}

	best = max(best, score);
}

/*----------------------------------------------------------------------*/


#endif // _2048GAME_H_
