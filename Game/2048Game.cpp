// Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//*********************************************************
//File name    :    2048GAME
//Author       :    Ivan Lee
//Date         :    20211201
//version      :    1.0
//*********************************************************

#include <cstdio>
#include <graphics.h>

#include "2048Game.h"

int main(void)
{
    bool bFlag = true;

    initgraph(400, 480);

    srand((unsigned)time(NULL));

    Basic Game;

    while (bFlag)
    {
        Game.Initialize();

        BeginBatchDraw();
        Game.DrawMenu();
        Game.DrawPionts();
        Game.DrawBest();
        Game.DrawTable();
        EndBatchDraw();

        int End = 0;

        while (1)
        {
            Game.UserOperaion();

            BeginBatchDraw();
            Game.DrawMenu();
            Game.DrawPionts();
            Game.DrawBest();
            Game.DrawTable();
            EndBatchDraw();

            if (Game.IsWin())
            {
                End = 1;

                break;
            }

            if (Game.IsOver())
            {
                End = 2;

                break;
            }
        }

        int choice = 0;

        if (End == 1)
        {
            choice = MessageBox(0, _T("胜利!\n再来一局?"), _T("继续"), MB_OKCANCEL);
        }
        else if (End == 2)
        {
            choice = MessageBox(0, _T("失败!\n再来一局?"), _T("继续"), MB_OKCANCEL);
        }

        if (choice == IDCANCEL)
        {
            bFlag = false;
        }
    }

    closegraph();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
