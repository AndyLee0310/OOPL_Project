/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include <iostream>

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0);	// 一開始的loading進度為0%
	//
	// 開始載入資料
	//
	logo.LoadBitmap(IDB_LOGO, RGB(255, 255, 255));

	scr_start.LoadBitmap(IDB_SCREEN_START, RGB(0, 0, 255));
	scr_gie.LoadBitmap(IDB_SCREEN_GI, RGB(0, 0, 255));
	scr_preferences.LoadBitmap(IDB_SCREEN_PREFERENCES, RGB(0, 0, 255));
	scr_about.LoadBitmap(IDB_SCREEN_ABOUT, RGB(0, 0, 255));
	scr_exit.LoadBitmapA(IDB_SCREEN_EXIT, RGB(0, 0, 255));

	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
	form_state = 1;
	CAudio::Instance()->Load(AUDIO_MEUM, "sounds\\meum.mp3");
	CAudio::Instance()->Play(AUDIO_MEUM, true);
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (show == 1) {
		if (p.x > close.Left() && p.x < close.Left() + close.Width() &&
			p.y > close.Top() && p.y < close.Top() + close.Height()) {
			show = 0;
		}
	} else {
		if (p.x > scr_start.Left() && p.x < scr_start.Left() + scr_start.Width() &&
			p.y > scr_start.Top() && p.y < scr_start.Top() + scr_start.Height()) {
			CAudio::Instance()->Stop(AUDIO_MEUM);
			GotoGameState(GAME_STAGE_1);		// 切換至第一關
		}
		else if (p.x > scr_gie.Left() && p.x < scr_gie.Left() + scr_gie.Width() &&
			p.y > scr_gie.Top() && p.y < scr_gie.Top() + scr_gie.Height()) {
			show = 1;							// 顯示遊戲說明
		}
		else if (p.x > scr_preferences.Left() && p.x < scr_preferences.Left() + scr_preferences.Width() &&
			p.y > scr_preferences.Top() && p.y < scr_preferences.Top() + scr_preferences.Height()) {
			GotoGameState(GAME_PREFENCES);		// 切換至Prefences畫面
		}
		else if (p.x > scr_about.Left() && p.x < scr_about.Left() + scr_about.Width() &&
			p.y > scr_about.Top() && p.y < scr_about.Top() + scr_about.Height()) {
			GotoGameState(GAME_ABOUT);			// 切換至About畫面
		}
		else if (p.x > scr_exit.Left() && p.x < scr_exit.Left() + scr_exit.Width() &&
			p.y > scr_exit.Top() && p.y < scr_exit.Top() + scr_exit.Height()) {
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
		}
	}
}

void CGameStateInit::OnMove()
{
	GetCursorPos(&p);
	ScreenToClient(AfxGetMainWnd()->m_hWnd, &p);	// 把螢幕座標轉換為視窗座標，並讀取出來
	//TRACE("mouse:: x: %d, y: %d\n", p.x, p.y);
}

void CGameStateInit::OnShow()
{
	// 放背景
	CMovingBitmap bg;
	bg.LoadBitmap(IDB_SCREENBG1, RGB(0,0,0));
	for (int x = 0; x < SIZE_X; x += bg.Width()) {
		for (int y = 0; y < SIZE_Y; y += bg.Height()) {
			bg.SetTopLeft(x,y);
			bg.ShowBitmap();
		}
	}
	//
	// 貼上logo
	//
	logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
	logo.ShowBitmap();
	//Screen_Start
	if (p.x > scr_start.Left() && p.x < scr_start.Left() + scr_start.Width() &&
		p.y > scr_start.Top() && p.y < scr_start.Top() + scr_start.Height()) {
		CMovingBitmap scr_start_red;
		scr_start_red.LoadBitmap(IDB_SCREEN_START_RED, RGB(0, 0, 255));
		scr_start_red.SetTopLeft((SIZE_X - scr_start_red.Width()) / 2, SIZE_Y / 2);
		scr_start_red.ShowBitmap();
	} else {
		scr_start.SetTopLeft((SIZE_X - scr_start.Width()) / 2, SIZE_Y / 2);
		scr_start.ShowBitmap();
	}
	//Screen_Game_Instructions
	if (p.x > scr_gie.Left() && p.x < scr_gie.Left() + scr_gie.Width() &&
		p.y > scr_gie.Top() && p.y < scr_gie.Top() + scr_gie.Height()) {
		CMovingBitmap scr_gie_red;
		scr_gie_red.LoadBitmap(IDB_SCREEN_GI_RED, RGB(0, 0, 255));
		scr_gie_red.SetTopLeft((SIZE_X - scr_gie_red.Width()) / 2, SIZE_Y / 2 + scr_start.Height());
		scr_gie_red.ShowBitmap();
	}
	else {
		scr_gie.SetTopLeft((SIZE_X - scr_gie.Width()) / 2, SIZE_Y / 2 + scr_gie.Height());
		scr_gie.ShowBitmap();
	}
	//Screen_Preferences
	if (p.x > scr_preferences.Left() && p.x < scr_preferences.Left() + scr_preferences.Width() &&
		p.y > scr_preferences.Top() && p.y < scr_preferences.Top() + scr_preferences.Height()) {
		CMovingBitmap scr_preferences_red;
		scr_preferences_red.LoadBitmap(IDB_SCREEN_PREFERENCES_RED, RGB(0, 0, 255));
		scr_preferences_red.SetTopLeft((SIZE_X - scr_preferences_red.Width()) / 2, SIZE_Y / 2 + scr_gie.Height() + scr_preferences.Height());
		scr_preferences_red.ShowBitmap();
	}
	else {
		scr_preferences.SetTopLeft((SIZE_X - scr_preferences.Width()) / 2, SIZE_Y / 2 + scr_gie.Height() + scr_preferences.Height());
		scr_preferences.ShowBitmap();
	}
	//Screen_about
	if (p.x > scr_about.Left() && p.x < scr_about.Left() + scr_about.Width() &&
		p.y > scr_about.Top() && p.y < scr_about.Top() + scr_about.Height()) {
		CMovingBitmap scr_about_red;
		scr_about_red.LoadBitmap(IDB_SCREEN_ABOUT_RED, RGB(0, 0, 255));
		scr_about_red.SetTopLeft((SIZE_X - scr_about_red.Width()) / 2, SIZE_Y / 2 + scr_gie.Height() + scr_preferences.Height() + scr_about.Height());
		scr_about_red.ShowBitmap();
	}
	else {
		scr_about.SetTopLeft((SIZE_X - scr_about.Width()) / 2, SIZE_Y / 2 + scr_gie.Height() + scr_preferences.Height() + scr_about.Height());
		scr_about.ShowBitmap();
	}
	//Screen_Exit
	if (p.x > scr_exit.Left() && p.x < scr_exit.Left() + scr_exit.Width() &&
		p.y > scr_exit.Top() && p.y < scr_exit.Top() + scr_exit.Height()) {
		CMovingBitmap scr_exit_red;
		scr_exit_red.LoadBitmap(IDB_SCREEN_EXIT_RED, RGB(0, 0, 255));
		scr_exit_red.SetTopLeft((SIZE_X - scr_exit_red.Width()) / 2, SIZE_Y / 2 + scr_gie.Height() + scr_preferences.Height() + scr_about.Height() + scr_exit.Height());
		scr_exit_red.ShowBitmap();
	}
	else {
		scr_exit.SetTopLeft((SIZE_X - scr_exit.Width()) / 2, SIZE_Y / 2 + scr_gie.Height() + scr_preferences.Height() + scr_about.Height() + scr_exit.Height());
		scr_exit.ShowBitmap();
	}
	// 顯示遊戲說明
	if (show == 1) {
		gameInstruction.LoadBitmap(IDB_SCR_GAMEINFO);
		close.LoadBitmap(IDB_CLOSE);
		gameInstruction.SetTopLeft((SIZE_X - gameInstruction.Width()) / 2, (SIZE_Y - gameInstruction.Height()) / 2);
		close.SetTopLeft((SIZE_X - gameInstruction.Width()) / 2 + (gameInstruction.Width() - close.Width()), (SIZE_Y - gameInstruction.Height()) / 2);
		gameInstruction.ShowBitmap();
		close.ShowBitmap();
	}
}								

//////////////////////////////////////////////////////////////////////////////
//game pause
/////////////////////////////////////////////////////////////////////////////

CGamestatePause::CGamestatePause(CGame* g)
	: CGameState(g)
{
}

void CGamestatePause::OnInit()
{
	scr_resume.LoadBitmap(IDB_SCREEN_RESUME, RGB(0, 0, 255));
	scr_gie.LoadBitmapA(IDB_SCREEN_GI, RGB(0, 0, 255));
	scr_preferences.LoadBitmapA(IDB_SCREEN_PREFERENCES, RGB(0, 0, 255));
	scr_quitToMenu.LoadBitmap(IDB_SCREEN_QUIT_TO_MENU, RGB(0, 0, 255));
	scr_exit.LoadBitmapA(IDB_SCREEN_EXIT, RGB(0, 0, 255));
}

void CGamestatePause::OnBeginState()
{
	form_state = 2;
}

void CGamestatePause::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (show == 1) {
		if (p.x > close.Left() && p.x < close.Left() + close.Width() &&
			p.y > close.Top() && p.y < close.Top() + close.Height()) {
			show = 0;
		}
	}
	else {
		if (p.x > scr_resume.Left() && p.x < scr_resume.Left() + scr_resume.Width() &&
			p.y > scr_resume.Top() && p.y < scr_resume.Top() + scr_resume.Height()) {
			//Resume
			game_framework::CGame::Instance()->ContiuneState(game_framework::CGame::Instance()->getState());
			game_framework::CGame::Instance()->SaveState(nullptr);		// clean savestate
		}
		else if (p.x > scr_gie.Left() && p.x < scr_gie.Left() + scr_gie.Width() &&
			p.y > scr_gie.Top() && p.y < scr_gie.Top() + scr_gie.Height()) {
			show = 1;
		}
		else if (p.x > scr_preferences.Left() && p.x < scr_preferences.Left() + scr_preferences.Width() &&
			p.y > scr_preferences.Top() && p.y < scr_preferences.Top() + scr_preferences.Height()) {
			GotoGameState(GAME_PREFENCES);
		}
		else if (p.x > scr_quitToMenu.Left() && p.x < scr_quitToMenu.Left() + scr_quitToMenu.Width() &&
			p.y > scr_quitToMenu.Top() && p.y < scr_quitToMenu.Top() + scr_quitToMenu.Height()) {
			GotoGameState(GAME_STATE_INIT);
		}
		else if (p.x > scr_exit.Left() && p.x < scr_exit.Left() + scr_exit.Width() &&
			p.y > scr_exit.Top() && p.y < scr_exit.Top() + scr_exit.Height()) {
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
		}
	}
}

void CGamestatePause::OnMouseMove(UINT nFlags, CPoint point)
{
	GetCursorPos(&p);
	ScreenToClient(AfxGetMainWnd()->m_hWnd, &p);	// 把螢幕座標轉換為視窗座標，並讀取出來
	//TRACE("mouse:: x: %d, y: %d\n", p.x, p.y);
}

void CGamestatePause::OnShow()
{
	// 放背景
	bg.LoadBitmap(IDB_SCREENBG1, RGB(0, 0, 0));
	for (int x = 0; x < SIZE_X; x += bg.Width()) {
		for (int y = 0; y < SIZE_Y; y += bg.Height()) {
			bg.SetTopLeft(x, y);
			bg.ShowBitmap();
		}
	}
	//Resume
	if (p.x > scr_resume.Left() && p.x < scr_resume.Left() + scr_resume.Width() &&
		p.y > scr_resume.Top() && p.y < scr_resume.Top() + scr_resume.Height()) {
		CMovingBitmap scr_resume_red;
		scr_resume_red.LoadBitmap(IDB_SCREEN_RESUME_RED, RGB(0, 0, 255));
		scr_resume_red.SetTopLeft((SIZE_X - scr_resume_red.Width()) / 2, SIZE_Y * 25 / 100);
		scr_resume_red.ShowBitmap();
	}
	else {
		scr_resume.SetTopLeft((SIZE_X - scr_resume.Width()) / 2, SIZE_Y * 25 / 100);
		scr_resume.ShowBitmap();
	}
	//Game Instruction
	if (p.x > scr_gie.Left() && p.x < scr_gie.Left() + scr_gie.Width() &&
		p.y > scr_gie.Top() && p.y < scr_gie.Top() + scr_gie.Height()) {
		CMovingBitmap scr_gie_red;
		scr_gie_red.LoadBitmap(IDB_SCREEN_GI_RED, RGB(0, 0, 255));
		scr_gie_red.SetTopLeft((SIZE_X - scr_gie_red.Width()) / 2, SIZE_Y * 35 / 100);
		scr_gie_red.ShowBitmap();
	}
	else {
		scr_gie.SetTopLeft((SIZE_X - scr_gie.Width()) / 2, SIZE_Y * 35 / 100);
		scr_gie.ShowBitmap();
	}
	//Preferences
	if (p.x > scr_preferences.Left() && p.x < scr_preferences.Left() + scr_preferences.Width() &&
		p.y > scr_preferences.Top() && p.y < scr_preferences.Top() + scr_preferences.Height()) {
		CMovingBitmap scr_preferences_red;
		scr_preferences_red.LoadBitmap(IDB_SCREEN_PREFERENCES_RED, RGB(0, 0, 255));
		scr_preferences_red.SetTopLeft((SIZE_X - scr_preferences_red.Width()) / 2, SIZE_Y * 45 / 100);
		scr_preferences_red.ShowBitmap();
	}
	else {
		scr_preferences.SetTopLeft((SIZE_X - scr_preferences.Width()) / 2, SIZE_Y * 45 / 100);
		scr_preferences.ShowBitmap();
	}
	//Quit to Menu
	if (p.x > scr_quitToMenu.Left() && p.x < scr_quitToMenu.Left() + scr_quitToMenu.Width() &&
		p.y > scr_quitToMenu.Top() && p.y < scr_quitToMenu.Top() + scr_quitToMenu.Height()) {
		CMovingBitmap scr_quitToMenu_red;
		scr_quitToMenu_red.LoadBitmap(IDB_SCREEN_QUIT_TO_MENU_RED, RGB(0, 0, 255));
		scr_quitToMenu_red.SetTopLeft((SIZE_X - scr_quitToMenu_red.Width()) / 2, SIZE_Y * 55 / 100);
		scr_quitToMenu_red.ShowBitmap();
	}
	else {
		scr_quitToMenu.SetTopLeft((SIZE_X - scr_quitToMenu.Width()) / 2, SIZE_Y * 55 / 100);
		scr_quitToMenu.ShowBitmap();
	}
	//Screen_Exit
	if (p.x > scr_exit.Left() && p.x < scr_exit.Left() + scr_exit.Width() &&
		p.y > scr_exit.Top() && p.y < scr_exit.Top() + scr_exit.Height()) {
		CMovingBitmap scr_exit_red;
		scr_exit_red.LoadBitmap(IDB_SCREEN_EXIT_RED, RGB(0, 0, 255));
		scr_exit_red.SetTopLeft((SIZE_X - scr_exit_red.Width()) / 2, SIZE_Y * 70 / 100);
		scr_exit_red.ShowBitmap();
	}
	else {
		scr_exit.SetTopLeft((SIZE_X - scr_exit.Width()) / 2, SIZE_Y * 70 / 100);
		scr_exit.ShowBitmap();
	}
	// 顯示遊戲說明
	if (show == 1) {
		gameInstruction.LoadBitmap(IDB_SCR_GAMEINFO);
		close.LoadBitmap(IDB_CLOSE);
		gameInstruction.SetTopLeft((SIZE_X - gameInstruction.Width()) / 2, (SIZE_Y - gameInstruction.Height()) / 2);
		close.SetTopLeft((SIZE_X - gameInstruction.Width()) / 2 + (gameInstruction.Width() - close.Width()), (SIZE_Y - gameInstruction.Height()) / 2);
		gameInstruction.ShowBitmap();
		close.ShowBitmap();
	}
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	counter--;
	if (counter < 0)
		GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState()
{
	counter = 30 * 5; // 5 seconds
	over.LoadBitmap(IDB_SCREEN_GAMEOVER, RGB(255, 255, 255));
}

void CGameStateOver::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
	//
	// 開始載入資料
	//
	//
	//
	// 最終進度為100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	over.SetTopLeft((SIZE_X - over.Width()) / 2, SIZE_Y * 25 / 100);
	over.ShowBitmap();
	CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f, * fp;
	f.CreatePointFont(200, "Times New Roman");	// 產生 font f; 160表示16 point的字
	fp = pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0, 0, 0));
	pDC->SetTextColor(RGB(255, 255, 0));
	char str[80];								// Demo 數字對字串的轉換
	sprintf(str, "You got %d points in this game!", score);
	pDC->TextOut(140, SIZE_Y * 40 / 100, str);
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

	CDC* pDC1 = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f1, * fp1;
	f1.CreatePointFont(140, "Times New Roman");	// 產生 font f; 160表示16 point的字
	fp1 = pDC1->SelectObject(&f1);					// 選用 font f
	pDC1->SetBkColor(RGB(0, 0, 0));
	pDC1->SetTextColor(RGB(255, 255, 255));
	char str1[80];								// Demo 數字對字串的轉換
	sprintf(str1, "Wait %d second back to Menu !", counter / 30);
	pDC1->TextOut(220, SIZE_Y * 50 / 100, str1);
	pDC1->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的第一關
/////////////////////////////////////////////////////////////////////////////
GameStage_1::GameStage_1(CGame* g) : CGameState(g)
{
	Bomb_ch1 = new Bomb [7];
	block_2 = new Obstacle[42];
	coin_Ani = new CoinsAnimation[5];
	heart = new Healths[8];
	AI = new Enemy[2];
}
GameStage_1::~GameStage_1() {
	delete [] Bomb_ch1;
	delete[] block_2;
	delete[] coin_Ani;
	delete[] heart;
	delete[] AI;
}
void GameStage_1::OnBeginState() {
	for (int i = 0; i < 7; i++) {
		Bomb_ch1[i].Initialize();
	}
	int bg_reset[13][15] = {           // 0地板 1石塊 2粉色石
			{0,0,0,0,2,0,2,0,0,0,0,2,0,0,0},
			{0,1,2,1,0,1,2,1,0,1,2,1,0,1,0},
			{0,0,0,0,2,0,2,0,2,0,0,0,0,0,2},
			{0,1,2,1,0,1,0,1,0,1,2,1,0,1,0},
			{2,0,0,0,0,0,0,0,2,0,0,0,2,0,2},
			{0,1,0,1,2,1,0,1,0,1,2,1,0,1,0},
			{0,0,0,2,0,0,2,0,0,0,2,0,0,0,0},
			{2,1,0,1,2,1,0,1,0,1,2,1,0,1,0},
			{2,0,0,2,0,0,0,2,2,0,0,0,0,0,2},
			{0,1,2,1,0,1,0,1,2,1,2,1,0,1,0},
			{0,0,0,0,2,0,2,0,0,0,0,0,2,2,0},
			{2,1,2,1,0,1,0,1,2,1,0,1,0,1,0},
			{0,0,2,0,0,2,0,0,0,0,2,0,0,0,0}
	};
	int obstacle_reset[42][2] = {
		{0, 4},{0, 6},{0, 11},{1, 2},{1, 6},{1, 10},{2, 4},{2, 6},{2, 8},{2, 14},{3, 2},{3,10},
		{4,0},{4,8},{4,12},{4,14},{5,4},{5,10},{6,3},{6,6},{6,10},{7,0},{7,4},{7,10},{8,0},{8,3},
		{8,7},{8,8},{8,14},{9,2},{9,8},{9,10},{10,4},{10,6},{10,12},{10,13},{11,0},{11,2},{11,8},
		{12,2},{12,5},{12,10}
	};
	int coins_reset[5][2] = {
		{2,9},{4,1},{6,8},{8,13},{9,4}
	};
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 15; j++) {
			bg[i][j] = bg_reset[i][j];
		}
	}
	for (int i = 0; i < 42; i++) {
		block_2_pos[i][0] = obstacle_reset[i][0];
		block_2_pos[i][1] = obstacle_reset[i][1];
		
		block_2[i].Initialize(block_2_pos[i][1] * 32 + 128, block_2_pos[i][0] * 32 + 32);
	}
	for (int i = 0; i < 5; i++) {
		coins_pos[i][0] = coins_reset[i][0];
		coins_pos[i][1] = coins_reset[i][1];

		coin_Ani[i].Initialize(coins_pos[i][1] * 32 + 128, coins_pos[i][0] * 32 + 32);
	}

	life = 3;
	int health_reset[8] = { 2, 2, 2, 2, 2, 2, 2, 2};
	for (int i = 0; i < 8; i++) {
		heart_num[i] = health_reset[i];
	}
	blood_ori = blood_vol = 16;		//預設血量總值為8

	//腳色數值重置
	character_1.Initialize(128, 32);
	character_1.LoadMap(bg);

	coins_num = 5;		//該關卡共有5個金幣
	Enemy1_num = 1;		// 敵人1的數量
	Enemy2_num = 1;		// 敵人2的數量
	AI[0].Initialize(6 * 32 + 128, 4 * 32 + 32);
	AI[1].Initialize(12 * 32 + 128,  8 * 32 + 32);
	for (int i = 0; i < 2; i++) {
		AI[i].LoadMap(bg);
	}
	score = 0;
	timer = 0;
	CAudio::Instance()->Play(AUDIO_BGM1, true);
}
void GameStage_1::OnInit() {
	timer = 250;
	level.LoadBitmap(IDB_LEVEL_1);
	block_0.LoadBitmap(IDB_Bg_1, RGB(255, 255, 255));
	block_1.LoadBitmap(IDB_Blocks, RGB(255, 255, 255));
	for (int i = 0; i < 42; i++) {
		block_2[i].LoadBitmap();
	}
	border.LoadBitmap(IDB_BORDER_0, RGB(255, 255, 255));
	panel.LoadBitmap(IDB_Panel, RGB(255, 255, 255));
	character_1.LoadBitmap();
	for (int i = 0; i < 7; i++) {
		Bomb_ch1[i].LoadBitmap();
	}


	for (int i = 0; i < 5; i++) {
		coin_Ani[i].LoadBitmap();
	}

	for (int i = 0; i < 2; i++) {
		AI[i].LoadBitmap();
	}

	playerhead_1.LoadBitmap(IDB_PLAYERHEAD1, RGB(255, 255, 255));
	playerhead_2.LoadBitmap(IDB_PLAYERHEAD2, RGB(255, 255, 255));
	for (int i = 0; i < 8; i++) {
		heart[i].LoadBitmap();
	}

	count_down.SetInteger(60);
	/*
	for (int i = 0; i < 7; i++) {
		CAudio::Instance()->Load(AUDIO_BOMB + i, "sounds\\POWER.wav");
		TRACE("%d\n", AUDIO_BOMB + i);
	}
	*/
	CAudio::Instance()->Load(AUDIO_BOMB, "sounds\\POWER.wav");
	CAudio::Instance()->Load(AUDIO_BGM1, "sounds\\stage1BGM.wav");
	CAudio::Instance()->Load(AUDIO_OOF, "sounds\\player1_hurt.mp3");
}
void GameStage_1::OnMove() {
	timer++;
	int second = timer / 30;
	// int min = second / 60;
	second %= 60;
	//TRACE("second %d\n", second);
	//TRACE("min %d\n", min);

	if (!(timer % 30))
		count_down.Add(-1);

	bool nextState = false;               // 下一關 為0就進下一關
	for (int i = 0; i < 2; i++) {
		nextState = nextState | AI[i].Alive();
	}
	if (!nextState) {
		int HealthData[2] = { 6, 0 };
		//game->saveData(HealthData, 2);
		CAudio::Instance()->Stop(AUDIO_BGM1);
		game->saveData(heart_num, 8);
		GotoGameState(GAME_STAGE_2);
	}

	for (int i = 0; i < 42; i++) {
		block_2[i].OnMove();
		if (block_2[i].getActive() && !block_2[i].getExp()) {
			mapChange(block_2_pos[i][1], block_2_pos[i][0], 0);
			block_2[i].setActive(false);
			block_2[i].setExp(true);

		}
	}
	BombState();
	character_1.OnMove();

	for (int i = 0; i < 2; i++) {
		AI[i].OnMove(character_1.GetX1(), character_1.GetY1(), 0, 0);
	}
	GetCoins();

	HealthState();
	if (blood_vol > 0) {
		character_1.SetDead(false);
	}
	if (blood_vol == 0 && life != 1) {
		character_1.SetDead(true);
		life--;
		int health_reset[8] = { 2, 2, 2, 2, 2, 2, 2, 2};
		for (int i = 0; i < 8; i++) {
			heart_num[i] = health_reset[i];
		}
		blood_ori = blood_vol = 16;		// 預設血量總值為16
	}
	else if (blood_vol == 0 && life == 1) {
		life--;
		GotoGameState(GAME_STATE_OVER);
	}
	// 判斷敵人被殺死後給予得分
	if (!(AI[0].Alive()) && Enemy1_num > 0) {
		score += 100;
		Enemy1_num--;
	}
	if (!(AI[1].Alive()) && Enemy2_num > 0) {
		score += 100;
		Enemy2_num--;
	}
}
void GameStage_1::OnShow() {                   // 越後放的顯示會越上層
	panel.SetTopLeft(0, 0);
	panel.ShowBitmap();
	border.SetTopLeft(96, 0);
	border.ShowBitmap();
	level.SetTopLeft(609, 0);
	level.ShowBitmap();
	for (int i = 0; i < 13; i++) {             //方塊顯示    j是X軸 i是Y軸
		for (int j = 0; j < 15; j++) {
			if (bg[i][j] == 1) {
				block_1.SetTopLeft(128 + 32 * j, 32 * (i + 1));
				block_1.ShowBitmap();
			}
			else {
				block_0.SetTopLeft(128 + 32 * j, 32 * (i + 1));
				block_0.ShowBitmap();
			}
		}
	}

	for (int i = 0; i < 42; i++) {
		block_2[i].OnShow();
	}

	for (int i = 0; i < coins_num; i++) {
		coin_Ani[i].setTopLeft(128 + coins_pos[i][1] * 32, 32 * (coins_pos[i][0] + 1));
		coin_Ani[i].OnShow();
	}

	count_down.SetTopLeft(panel.Width() * 25 / 100, panel.Height() * 48 / 100);
	count_down.LoadBitmap();
	count_down.ShowBitmap();
	character_1.OnShow();
	for (int i = 0; i < 7; i++) {
		Bomb_ch1[i].OnShow();
	}
	for (int i = 0; i < 2; i++) {
		AI[i].OnShow();
	}

	playerhead_1.SetTopLeft((panel.Width() * 16 / 100), panel.Height() * 13 / 100);
	playerhead_1.ShowBitmap();
	CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f, * fp;
	f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
	fp = pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(0, 0, 255));
	pDC->SetTextColor(RGB(255, 255, 255));
	char str[80];								// Demo 數字對字串的轉換
	sprintf(str, "X %d", life);
	pDC->TextOut((panel.Width() * 57 / 100), panel.Height() * 13 / 100, str);
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

	CDC* pDC1 = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f1, * fp1;
	f1.CreatePointFont(140, "Times New Roman");	// 產生 font f; 160表示16 point的字
	fp1 = pDC1->SelectObject(&f1);					// 選用 font f
	pDC1->SetBkMode(TRANSPARENT);
	pDC1->SetBkColor(RGB(0, 0, 255));
	pDC1->SetTextColor(RGB(255, 255, 255));
	pDC1->TextOut((panel.Width() * 16 / 100), panel.Height() * 375 / 1000, "SCORE");
	char str1[80];								// Demo 數字對字串的轉換
	sprintf(str1, "%06d", score);
	pDC1->TextOut((panel.Width() * 20 / 100), panel.Height() * 41 / 100, str1);
	pDC1->SelectObject(fp1);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

	playerhead_2.SetTopLeft((panel.Width() * 16 / 100), panel.Height() * 56 / 100);
	playerhead_2.ShowBitmap();
	CDC* pDC2 = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f2, * fp2;
	f2.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
	fp2 = pDC2->SelectObject(&f2);					// 選用 font f
	pDC2->SetBkMode(TRANSPARENT);
	pDC2->SetBkColor(RGB(0, 0, 255));
	pDC2->SetTextColor(RGB(255, 255, 255));
	char str2[80];								// Demo 數字對字串的轉換
	sprintf(str2, "X %d", 0);
	pDC2->TextOut((panel.Width() * 57 / 100), panel.Height() * 56 / 100, str2);
	pDC2->SelectObject(fp2);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	CMovingBitmap playerover;
	playerover.LoadBitmap(IDB_PLAYER_GAMEOVER, RGB(255, 255, 0));
	playerover.SetTopLeft(panel.Width() * 10 / 100, panel.Height() * 61 / 100);
	playerover.ShowBitmap();
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			if (i == 0 && j < 4) {
				heart[j].setTopLeft((panel.Width() * 15 / 100) + 17 * j, panel.Height() * 19 / 100);
				heart[j].OnShow();
			}
			else if (i == 1 && j > 3 &&j < 8) {
				heart[j].setTopLeft((panel.Width() * 15 / 100) + 17 * (j - 4), panel.Height() * 225 / 1000);
				heart[j].OnShow();
			}
		}
	}
}

void GameStage_1::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25;  // keyboard左箭頭
	const char KEY_UP = 0x26;    // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN = 0x28;  // keyboard下箭頭
	const char KEY_ESC = 0x1B;
	const char KEY_P = 0x50;
	const char KEY_SPACE = 0x20;
	const char KET_Y = 0x59;

	
	if (nChar == KEY_ESC || nChar == KEY_P) {
		//game_framework::CGame::Instance()->OnFilePause();
		CAudio::Instance()->Pause();
		game_framework::CGame::Instance()->SaveState(this);
		GotoGameState(GAME_STATE_PAUSE);
	}

	if (nChar == KEY_LEFT) {
		character_1.SetMovingLeft(true);
	}
	if (nChar == KEY_RIGHT) {
		character_1.SetMovingRight(true);
	}
	if (nChar == KEY_UP) {
		character_1.SetMovingUp(true);
	}
	if (nChar == KEY_DOWN) {
		character_1.SetMovingDown(true);
	}
	if (nChar == KEY_SPACE) {
		setBomb(1);
	}
	if (nChar == KET_Y) {
		CAudio::Instance()->Stop(AUDIO_BGM1);
		game->saveData(heart_num, 8);
		GotoGameState(GAME_STAGE_2);
	}
}

void GameStage_1::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25;		// keyboard左箭頭
	const char KEY_UP = 0x26;		// keyboard上箭頭
	const char KEY_RIGHT = 0x27;	// keyboard右箭頭
	const char KEY_DOWN = 0x28;		// keyboard下箭頭
	if (nChar == KEY_LEFT)
		character_1.SetMovingLeft(false);
	if (nChar == KEY_RIGHT)
		character_1.SetMovingRight(false);
	if (nChar == KEY_UP)
		character_1.SetMovingUp(false);
	if (nChar == KEY_DOWN)
		character_1.SetMovingDown(false);
}

void GameStage_1::setBomb(int id) {
	if (id == 1) {                                                  // [y][x]
		int x = (character_1.GetX1() + character_1.GetX2()) / 2;    // 腳色中心點
		int y = (character_1.GetY1() + character_1.GetY2()) / 2;    // 腳色中心點
		x = (x - 128) / 32;                                         // 轉換成13*15地圖模式
		y = (y - 32) / 32;
		if (bg[y][x] == 0) {
			for (int i = 0; i < 7; i++) {
				if (!Bomb_ch1[i].getActive()) {
					Bomb_ch1[i].setTopleft(x * 32 + 128, (y + 1) * 32);
					Bomb_ch1[i].setActive(true);
					mapChange(x, y, 4);
					break;
				}
			}
		}
		else {

		}
	}
	else if (id == 2) {
		// player2's operating
	}
}

void GameStage_1::mapChange(int x, int y, int value) {
	bg[y][x] = value;
	character_1.LoadMap(bg);
	for (int i = 0; i < 2; i++) {
		AI[i].LoadMap(bg);
	}
	
}
void GameStage_1::BombState() {
	for (int i = 0; i < 7; i++) {
		Bomb_ch1[i].OnMove();
		if (Bomb_ch1[i].getActive() && !Bomb_ch1[i].getExp()) {
			int nx = (Bomb_ch1[i].getTop_Bomb() - 128) / 32;
			int ny = (Bomb_ch1[i].getLeft_Bomb() - 32) / 32;
			if(bg[ny][nx] != 4)mapChange(nx, ny, 4);            // 
		}
		if (Bomb_ch1[i].getActive() && Bomb_ch1[i].getExp()) {  // 爆炸中的炸彈位置重設成可行走
			int nx = (Bomb_ch1[i].getTop_Bomb() - 128) / 32;
			int ny = (Bomb_ch1[i].getLeft_Bomb() - 32) / 32;
			mapChange(nx, ny, 5);
			if(!Bomb_ch1[i].getObs())setBombRange(1, i, nx, ny);
			if(Bomb_ch1[i].getAud())CAudio::Instance()->Play(AUDIO_BOMB, false);
			for (int j = 1; j <= Bomb_ch1[i].getUp(); j++)mapChange(nx, ny - j, 5);
			for (int j = 1; j <= Bomb_ch1[i].getDown(); j++)mapChange(nx, ny + j, 5);
			for (int j = 1; j <= Bomb_ch1[i].getRight(); j++)mapChange(nx + j, ny, 5);
			for (int j = 1; j <= Bomb_ch1[i].getLeft(); j++)mapChange(nx - j, ny, 5);
			
		}
		if (!Bomb_ch1[i].getActive() && Bomb_ch1[i].getExp()) {
			int nx = (Bomb_ch1[i].getTop_Bomb() - 128) / 32;
			int ny = (Bomb_ch1[i].getLeft_Bomb() - 32) / 32;
			mapChange(nx, ny, 0);
			for (int j = 1; j <= Bomb_ch1[i].getUp(); j++)mapChange(nx, ny - j, 0);
			for (int j = 1; j <= Bomb_ch1[i].getDown(); j++)mapChange(nx, ny + j, 0);
			for (int j = 1; j <= Bomb_ch1[i].getRight(); j++)mapChange(nx + j, ny, 0);
			for (int j = 1; j <= Bomb_ch1[i].getLeft(); j++)mapChange(nx - j, ny, 0);
			
			Bomb_ch1[i].Initialize();
		}
	}
}
void GameStage_1::setBombRange(int id, int i,int x,int y) {
	if (id == 1) {
		int j;
		int range = character_1.GetRange();
		for (j = 1; j <= range; j++) {
			if (y - j < 0 || bg[y - j][x] == 1) {
				break;
			}
			else if (bg[y - j][x] == 2) {
				for (int k = 0; k < 42; k++) {
					if (block_2_pos[k][0] == y - j && block_2_pos[k][1] == x) {
						block_2[k].setActive();
						score += 10;
						break;
					}
				}
				break;
			}
		}
		Bomb_ch1[i].setUp(--j);
		for (j = 1; j <= range; j++) {
			if (y + j > 12 || bg[y + j][x] == 1) {
				break;
			}
			else if (bg[y + j][x] == 2) {
				for (int k = 0; k < 42; k++) {
					if (block_2_pos[k][0] == y + j && block_2_pos[k][1] == x) {
						block_2[k].setActive();
						score += 10;
						break;
					}
				}
				break;
			}
		}
		Bomb_ch1[i].setDown(--j);
		for (j = 1; j <= range; j++) {
			if (x + j > 14 || bg[y][x + j] == 1) {
				break;
			}
			else if (bg[y][x + j] == 2) {
				for (int k = 0; k < 42; k++) {
					if (block_2_pos[k][0] == y && block_2_pos[k][1] == x + j) {
						block_2[k].setActive();
						score += 10;
						break;
					}
				}
				break;
			}
		}
		Bomb_ch1[i].setRight(--j);
		for (j = 1; j <= range; j++) {
			if (x - j < 0 || bg[y][x - j] == 1) {
				break;
			}
			else if (bg[y][x - j] == 2) {
				for (int k = 0; k < 42; k++) {
					if (block_2_pos[k][0] == y && block_2_pos[k][1] == x - j) {
						block_2[k].setActive();
						score += 10;
						break;
					}
				}
				break;
			}
		}
		Bomb_ch1[i].setLeft(--j);
		Bomb_ch1[i].setObs(true);
	}
}
void GameStage_1::GetCoins() {
	//找出腳色所在位置(x,y)
	int x = (character_1.GetX1() + character_1.GetX2()) / 2;    //腳色中心點
	int y = (character_1.GetY1() + character_1.GetY2()) / 2;    //腳色中心點
	x = (x - 128) / 32;                                         //轉換成13*15地圖模式
	y = (y - 32) / 32;
	for (int i = 0; i < coins_num; i++) {
		coin_Ani[i].OnMove();
		if (x == coins_pos[i][1] && y == coins_pos[i][0] && !coin_Ani[i].getActive() && !coin_Ani[i].getExp()) {
			coin_Ani[i].setActive();
			score += 150;
		}
	}
}

void GameStage_1::HealthState() {
	for (int i = 0; i < 8; i++) {
		heart[i].OnMove();

		if (heart_num[i] == 2) {
			heart[i].SetDescision(2);
		}
		else if (heart_num[i] == 1) {
			heart[i].SetDescision(1);
		}
		else if (heart_num[i] == 0) {
			heart[i].SetDescision(0);
		}
	}

	int x = (character_1.GetX1() + character_1.GetX2()) / 2;    // 腳色中心點
	int y = (character_1.GetY1() + character_1.GetY2()) / 2;    // 腳色中心點
	x = (x - 128) / 32;                                         // 轉換成13*15地圖模式
	y = (y - 32) / 32;
	// TRACE(" %d\n", character_1.GetDead());
	if (!character_1.GetDead()) {
		for (int i = 0; i < 2; i++) {
			int x1 = (AI[i].GetX1() + AI[i].GetX2()) / 2;    // 敵人中心點
			int y1 = (AI[i].GetY1() + AI[i].GetY2()) / 2;    // 敵人中心點
			x1 = (x1 - 128) / 32;							 // 轉換成13*15地圖模式
			y1 = (y1 - 32) / 32;

			if (x == x1 && y == y1 && AI[i].Alive()) {
				CAudio::Instance()->Play(AUDIO_OOF, false);
				blood_vol = blood_vol - 1;
			}
			if (AI[i].BulletHitPlayer() && AI[i].Alive()) {
				CAudio::Instance()->Play(AUDIO_OOF, false);
				blood_vol = blood_vol - 1;
			}
		}
		if (taking_Damage) {
			// wait two seconds
			k++;
			if (k >= 60) {
				taking_Damage = false;
				k = 0;
			}
		} else {
			if (bg[y][x] == 5) {
				CAudio::Instance()->Play(AUDIO_OOF, false);
				blood_vol = blood_vol - 7;
				taking_Damage = true;
			}
		}
	}
	TRACE("血量剩餘 %d\n", blood_vol);
	double value = std::fmod(blood_ori, blood_vol);
	for (int i = 7; i >= 0; i--) {
		if (heart_num[i] != 0) {
			if (heart_num[i] - value < 0) {
				value -= heart_num[i];
				heart_num[i] = 0;
				blood_ori = blood_vol;
			}
			else if (heart_num[i] - value == 1) {
				heart_num[i] = 1;
				blood_ori = blood_vol;
				value = 0;
			}
			else if (heart_num[i] - value == 0) {
				heart_num[i] = 0;
				blood_ori = blood_vol;
				value = 0;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//Prefences
/////////////////////////////////////////////////////////////////////////////

GamePrefences::GamePrefences(CGame* g) : CGameState(g)
{
}

void GamePrefences::OnInit()
{
	scr.LoadBitmap(IDB_SCREEN_PREFENCES, RGB(0, 0, 255));
	scr_ok.LoadBitmap(IDB_SCREEN_OK, RGB(0, 0, 255));
	scr_cancel.LoadBitmap(IDB_SCREEN_CANCEL, RGB(0, 0, 255));

	scr_FX_down.LoadBitmap(IDB_SCREEN_MINUX, RGB(0, 0, 255));
	scr_FX_up.LoadBitmap(IDB_SCREEN_PLUS, RGB(0, 0, 255));

	scr_FS_yes.LoadBitmap(IDB_SCREEN_YES, RGB(0, 0, 255));
	scr_FS_no.LoadBitmap(IDB_SCREEN_NO, RGB(0, 0, 255));

	scr_FR.LoadBitmap(IDB_SCREEN_MINUX, RGB(0, 0, 255));

	scr_SF_yes.LoadBitmap(IDB_SCREEN_YES, RGB(0, 0, 255));
	scr_SF_no.LoadBitmap(IDB_SCREEN_NO, RGB(0, 0, 255));

	scr_Vsync_yes.LoadBitmap(IDB_SCREEN_YES, RGB(0, 0, 255));
	scr_Vsync_no.LoadBitmap(IDB_SCREEN_NO, RGB(0, 0, 255));
}

void GamePrefences::OnBeginState()
{
	form_ori = form_state;
	form_state = 3;

	FS_ori_state = FS_state;
	SF_ori_state = SF_state;
	Vsync_ori_state = Vsync_state;
}

void GamePrefences::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (p.x > scr_ok.Left() && p.x < scr_ok.Left() + scr_ok.Width() &&
		p.y > scr_ok.Top() && p.y < scr_ok.Top() + scr_ok.Height()) {
		//OK
		if (form_ori == 1) {
			GotoGameState(GAME_STATE_INIT);
		}
		else if (form_ori == 2) {
			GotoGameState(GAME_STATE_PAUSE);
		}
	}
	else if (p.x > scr_cancel.Left() && p.x < scr_cancel.Left() + scr_cancel.Width() &&
		p.y > scr_cancel.Top() && p.y < scr_cancel.Top() + scr_cancel.Height()) {
		//Cancel
		FS_state = FS_ori_state;
		SF_state = SF_ori_state;
		Vsync_state = Vsync_ori_state;
		if (form_ori == 1) {
			GotoGameState(GAME_STATE_INIT);
		}
		else if (form_ori == 2) {
			GotoGameState(GAME_STATE_PAUSE);
		}
	}

	if (p.x > scr_FS_no.Left() && p.x < scr_FS_no.Left() + scr_FS_no.Width() &&
		p.y > scr_FS_no.Top() && p.y < scr_FS_no.Top() + scr_FS_no.Height() && FS_state == 0) {
		FS_state = 1;
	} else if (p.x > scr_FS_yes.Left() && p.x < scr_FS_yes.Left() + scr_FS_yes.Width() &&
		p.y > scr_FS_yes.Top() && p.y < scr_FS_yes.Top() + scr_FS_yes.Height() && FS_state == 1) {
		FS_state = 0;
	}

	if (p.x > scr_SF_no.Left() && p.x < scr_SF_no.Left() + scr_SF_no.Width() &&
		p.y > scr_SF_no.Top() && p.y < scr_SF_no.Top() + scr_SF_no.Height() && SF_state == 0) {
		SF_state = 1;
	}
	else if (p.x > scr_SF_yes.Left() && p.x < scr_SF_yes.Left() + scr_SF_yes.Width() &&
		p.y > scr_SF_yes.Top() && p.y < scr_SF_yes.Top() + scr_SF_yes.Height() && SF_state == 1) {
		SF_state = 0;
	}

	if (p.x > scr_Vsync_no.Left() && p.x < scr_Vsync_no.Left() + scr_Vsync_no.Width() &&
		p.y > scr_Vsync_no.Top() && p.y < scr_Vsync_no.Top() + scr_Vsync_no.Height() && Vsync_state == 0) {
		Vsync_state = 1;
	}
	else if (p.x > scr_Vsync_yes.Left() && p.x < scr_Vsync_yes.Left() + scr_Vsync_yes.Width() &&
		p.y > scr_Vsync_yes.Top() && p.y < scr_Vsync_yes.Top() + scr_Vsync_yes.Height() && Vsync_state == 1) {
		Vsync_state = 0;
	}

}

void GamePrefences::OnMouseMove(UINT nFlags, CPoint point)
{
	GetCursorPos(&p);
	ScreenToClient(AfxGetMainWnd()->m_hWnd, &p);	// 把螢幕座標轉換為視窗座標，並讀取出來
	//TRACE("mouse:: x: %d, y: %d\n", p.x, p.y);
}

void GamePrefences::OnShow()
{
	//??I??
	bg.LoadBitmap(IDB_SCREENBG1, RGB(0, 0, 0));
	for (int x = 0; x < SIZE_X; x += bg.Width()) {
		for (int y = 0; y < SIZE_Y; y += bg.Height()) {
			bg.SetTopLeft(x, y);
			bg.ShowBitmap();
		}
	}
	scr.SetTopLeft(SIZE_X * 4 / 1000, SIZE_Y * 4 / 1000);
	scr.ShowBitmap();

	//OK
	if (p.x > scr_ok.Left() && p.x < scr_ok.Left() + scr_ok.Width() &&
		p.y > scr_ok.Top() && p.y < scr_ok.Top() + scr_ok.Height()) {
		CMovingBitmap scr_ok_red;
		scr_ok_red.LoadBitmap(IDB_SCREEN_OK_RED, RGB(0, 0, 255));
		scr_ok_red.SetTopLeft((SIZE_X - scr_ok_red.Width()) * 34 / 100, SIZE_Y * 85 / 100);
		scr_ok_red.ShowBitmap();
	}
	else {
		scr_ok.SetTopLeft((SIZE_X - scr_ok.Width()) * 34 / 100, SIZE_Y * 85 / 100);
		scr_ok.ShowBitmap();
	}
	//Cancel
	if (p.x > scr_cancel.Left() && p.x < scr_cancel.Left() + scr_cancel.Width() &&
		p.y > scr_cancel.Top() && p.y < scr_cancel.Top() + scr_cancel.Height()) {
		CMovingBitmap scr_cancel_red;
		scr_cancel_red.LoadBitmap(IDB_SCREEN_CANCEL_RED, RGB(0, 0, 255));
		scr_cancel_red.SetTopLeft((SIZE_X - scr_cancel_red.Width()) * 68 / 100, SIZE_Y * 85 / 100);
		scr_cancel_red.ShowBitmap();
	}
	else {
		scr_cancel.SetTopLeft((SIZE_X - scr_cancel.Width()) * 68 / 100, SIZE_Y * 85 / 100);
		scr_cancel.ShowBitmap();
	}
	//FX_down
	if (p.x > scr_FX_down.Left() && p.x < scr_FX_down.Left() + scr_FX_down.Width() &&
		p.y > scr_FX_down.Top() && p.y < scr_FX_down.Top() + scr_FX_down.Height()) {
		CMovingBitmap scr_FX_down_red;
		scr_FX_down_red.LoadBitmap(IDB_SCREEN_MINUX_RED, RGB(0, 0, 255));
		scr_FX_down_red.SetTopLeft((SIZE_X - scr_FX_down_red.Width()) * 36 / 100, SIZE_Y * 11 / 100);
		scr_FX_down_red.ShowBitmap(1.6);
	}
	else {
		scr_FX_down.SetTopLeft((SIZE_X - scr_FX_down.Width()) * 36 / 100, SIZE_Y * 11 / 100);
		scr_FX_down.ShowBitmap(1.6);
	}
	//FX_up
	if (p.x > scr_FX_up.Left() && p.x < scr_FX_up.Left() + scr_FX_up.Width() &&
		p.y > scr_FX_up.Top() && p.y < scr_FX_up.Top() + scr_FX_up.Height()) {
		CMovingBitmap scr_FX_up_red;
		scr_FX_up_red.LoadBitmap(IDB_SCREEN_PLUS_RED, RGB(0, 0, 255));
		scr_FX_up_red.SetTopLeft((SIZE_X - scr_FX_up_red.Width()) * 71 / 100, SIZE_Y * 115 / 1000);
		scr_FX_up_red.ShowBitmap(1.6);
	}
	else {
		scr_FX_up.SetTopLeft((SIZE_X - scr_FX_up.Width()) * 71 / 100, SIZE_Y * 115 / 1000);
		scr_FX_up.ShowBitmap();
	}
	//Fullscreen
	if (FS_state == 1) {
		//scr_FS_yes
		if (p.x > scr_FS_yes.Left() && p.x < scr_FS_yes.Left() + scr_FS_yes.Width() &&
			p.y > scr_FS_yes.Top() && p.y < scr_FS_yes.Top() + scr_FS_yes.Height()) {
			CMovingBitmap scr_FS_yes_red;
			scr_FS_yes_red.LoadBitmap(IDB_SCREEN_YES_RED, RGB(0, 0, 255));
			scr_FS_yes_red.SetTopLeft((SIZE_X - scr_FS_yes_red.Width()) * 39 / 100, SIZE_Y * 22 / 100);
			scr_FS_yes_red.ShowBitmap();
		}
		else {
			scr_FS_yes.SetTopLeft((SIZE_X - scr_FS_yes.Width()) * 39 / 100, SIZE_Y * 22 / 100);
			scr_FS_yes.ShowBitmap();
		}
	}
	else if (FS_state == 0) {
		//scr_FS_no
		if (p.x > scr_FS_no.Left() && p.x < scr_FS_no.Left() + scr_FS_no.Width() &&
			p.y > scr_FS_no.Top() && p.y < scr_FS_no.Top() + scr_FS_no.Height()) {
			CMovingBitmap scr_FS_no_red;
			scr_FS_no_red.LoadBitmap(IDB_SCREEN_NO_RED, RGB(0, 0, 255));
			scr_FS_no_red.SetTopLeft((SIZE_X - scr_FS_no_red.Width()) * 38 / 100, SIZE_Y * 22 / 100);
			scr_FS_no_red.ShowBitmap();
		}
		else {
			scr_FS_no.SetTopLeft((SIZE_X - scr_FS_no.Width()) * 38 / 100, SIZE_Y * 22 / 100);
			scr_FS_no.ShowBitmap();
		}
	}

	//scr_FR: Fllscr.Res
	if (p.x > scr_FR.Left() && p.x < scr_FR.Left() + scr_FR.Width() &&
		p.y > scr_FR.Top() && p.y < scr_FR.Top() + scr_FR.Height()) {
		CMovingBitmap scr_FR_red;
		scr_FR_red.LoadBitmap(IDB_SCREEN_MINUX_RED, RGB(0, 0, 255));
		scr_FR_red.SetTopLeft((SIZE_X - scr_FR_red.Width()) * 38 / 100, SIZE_Y * 31 / 100);
		scr_FR_red.ShowBitmap();
	}
	else {
		scr_FR.SetTopLeft((SIZE_X - scr_FR.Width()) * 38 / 100, SIZE_Y * 31 / 100);
		scr_FR.ShowBitmap();
	}

	//Show FPS
	if (SF_state == 1) {
		//scr_SF_yes
		if (p.x > scr_SF_yes.Left() && p.x < scr_SF_yes.Left() + scr_SF_yes.Width() &&
			p.y > scr_SF_yes.Top() && p.y < scr_SF_yes.Top() + scr_SF_yes.Height()) {
			CMovingBitmap scr_SF_yes_red;
			scr_SF_yes_red.LoadBitmap(IDB_SCREEN_YES_RED, RGB(0, 0, 255));
			scr_SF_yes_red.SetTopLeft((SIZE_X - scr_SF_yes_red.Width()) * 39 / 100, SIZE_Y * 35 / 100);
			scr_SF_yes_red.ShowBitmap();
		}
		else {
			scr_SF_yes.SetTopLeft((SIZE_X - scr_SF_yes.Width()) * 39 / 100, SIZE_Y * 35 / 100);
			scr_SF_yes.ShowBitmap();
		}
	}
	else if (SF_state == 0) {
		//scr_SF_no
		if (p.x > scr_SF_no.Left() && p.x < scr_SF_no.Left() + scr_SF_no.Width() &&
			p.y > scr_SF_no.Top() && p.y < scr_SF_no.Top() + scr_SF_no.Height()) {
			CMovingBitmap scr_SF_no_red;
			scr_SF_no_red.LoadBitmap(IDB_SCREEN_NO_RED, RGB(0, 0, 255));
			scr_SF_no_red.SetTopLeft((SIZE_X - scr_SF_no_red.Width()) * 38 / 100, SIZE_Y * 35 / 100);
			scr_SF_no_red.ShowBitmap();
		}
		else {
			scr_SF_no.SetTopLeft((SIZE_X - scr_SF_no.Width()) * 38 / 100, SIZE_Y * 35 / 100);
			scr_SF_no.ShowBitmap();
		}
	}

	//Vsync
	if (Vsync_state == 1) {
		//scr_Vsync_yes
		if (p.x > scr_Vsync_yes.Left() && p.x < scr_Vsync_yes.Left() + scr_Vsync_yes.Width() &&
			p.y > scr_Vsync_yes.Top() && p.y < scr_Vsync_yes.Top() + scr_Vsync_yes.Height()) {
			CMovingBitmap scr_Vsync_yes_red;
			scr_Vsync_yes_red.LoadBitmap(IDB_SCREEN_YES_RED, RGB(0, 0, 255));
			scr_Vsync_yes_red.SetTopLeft((SIZE_X - scr_Vsync_yes_red.Width()) * 39 / 100, SIZE_Y * 43 / 100);
			scr_Vsync_yes_red.ShowBitmap();
		}
		else {
			scr_Vsync_yes.SetTopLeft((SIZE_X - scr_Vsync_yes.Width()) * 39 / 100, SIZE_Y * 43 / 100);
			scr_Vsync_yes.ShowBitmap();
		}
	}
	else if (Vsync_state == 0) {
		//scr_Vsync_no
		if (p.x > scr_Vsync_no.Left() && p.x < scr_Vsync_no.Left() + scr_Vsync_no.Width() &&
			p.y > scr_Vsync_no.Top() && p.y < scr_Vsync_no.Top() + scr_Vsync_no.Height()) {
			CMovingBitmap scr_Vsync_no_red;
			scr_Vsync_no_red.LoadBitmap(IDB_SCREEN_NO_RED, RGB(0, 0, 255));
			scr_Vsync_no_red.SetTopLeft((SIZE_X - scr_Vsync_no_red.Width()) * 38 / 100, SIZE_Y * 43 / 100);
			scr_Vsync_no_red.ShowBitmap();
		}
		else {
			scr_Vsync_no.SetTopLeft((SIZE_X - scr_Vsync_no.Width()) * 38 / 100, SIZE_Y * 43 / 100);
			scr_Vsync_no.ShowBitmap();
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
//About
/////////////////////////////////////////////////////////////////////////////

GameAbout::GameAbout(CGame* g) : CGameState(g)
{
}

void GameAbout::OnInit()
{
	aboutForm.LoadBitmap(IDB_SCR_ABOUT, RGB(0, 0, 255));
	scr_back.LoadBitmap(IDB_SCREEN_BACK, RGB(0, 0, 255));
}

void GameAbout::OnBeginState()
{
}

void GameAbout::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (p.x > scr_back.Left() && p.x < scr_back.Left() + scr_back.Width() &&
		p.y > scr_back.Top() && p.y < scr_back.Top() + scr_back.Height()) {
		GotoGameState(GAME_STATE_INIT);
	}
}

void GameAbout::OnMouseMove(UINT nFlags, CPoint point)
{
	GetCursorPos(&p);
	ScreenToClient(AfxGetMainWnd()->m_hWnd, &p);	// 把螢幕座標轉換為視窗座標，並讀取出來
	//TRACE("mouse:: x: %d, y: %d\n", p.x, p.y);
}

void GameAbout::OnShow()
{
	bg.LoadBitmap(IDB_SCREENBG1, RGB(0, 0, 0));
	for (int x = 0; x < SIZE_X; x += bg.Width()) {
		for (int y = 0; y < SIZE_Y; y += bg.Height()) {
			bg.SetTopLeft(x, y);
			bg.ShowBitmap();
		}
	}
	aboutForm.SetTopLeft(0, 0);
	aboutForm.ShowBitmap();
	//Back
	if (p.x > scr_back.Left() && p.x < scr_back.Left() + scr_back.Width() &&
		p.y > scr_back.Top() && p.y < scr_back.Top() + scr_back.Height()) {
		CMovingBitmap scr_back_red;
		scr_back_red.LoadBitmap(IDB_SCREEN_BACK_RED, RGB(0, 0, 255));
		scr_back_red.SetTopLeft((SIZE_X - scr_back_red.Width()) / 2, SIZE_Y * 88 / 100);
		scr_back_red.ShowBitmap();
	}
	else {
		scr_back.SetTopLeft((SIZE_X - scr_back.Width()) / 2, SIZE_Y * 88 / 100);
		scr_back.ShowBitmap();
	}
}
}