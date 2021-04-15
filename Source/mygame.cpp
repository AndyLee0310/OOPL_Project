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
	logo.LoadBitmap(IDB_LOGO,RGB(255,255,255));

	scr_start.LoadBitmap(IDB_SCREEN_START, RGB(0, 0, 255));
	scr_load.LoadBitmapA(IDB_SCREEN_LOAD, RGB(0, 0, 255));
	scr_preferences.LoadBitmapA(IDB_SCREEN_PREFERENCES, RGB(0, 0, 255));
	scr_about.LoadBitmap(IDB_SCREEN_ABOUT, RGB(0, 0, 255));
	scr_exit.LoadBitmapA(IDB_SCREEN_EXIT, RGB(0, 0, 255));

	Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';
	/*
	if (nChar == KEY_SPACE)
		GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
	
	else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// 關閉遊戲
	*/
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (p.x > scr_start.Left() && p.x < scr_start.Left() + scr_start.Width() &&
		p.y > scr_start.Top() && p.y < scr_start.Top() + scr_start.Height()) {
		GotoGameState(GAME_STAGE_1);		// 切換至GAME_STATE_RUN
	}else if (p.x > scr_load.Left() && p.x < scr_load.Left() + scr_load.Width() &&
			  p.y > scr_load.Top() && p.y < scr_load.Top() + scr_load.Height()) {
		//Load Game
	}else if (p.x > scr_preferences.Left() && p.x < scr_preferences.Left() + scr_preferences.Width() &&
			  p.y > scr_preferences.Top() && p.y < scr_preferences.Top() + scr_preferences.Height()) {
		//Preferences
	}else if (p.x > scr_about.Left() && p.x < scr_about.Left() + scr_about.Width() &&
			  p.y > scr_about.Top() && p.y < scr_about.Top() + scr_about.Height()) {
		//About
	}
	else if (p.x > scr_exit.Left() && p.x < scr_exit.Left() + scr_exit.Width() &&
		p.y > scr_exit.Top() && p.y < scr_exit.Top() + scr_exit.Height()) {
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}
}

void CGameStateInit::OnMove()
{
	GetCursorPos(&p);
	ScreenToClient(AfxGetMainWnd()->m_hWnd, &p);	//把螢幕座標轉換為視窗座標，並讀取出來
	//TRACE("mouse:: x: %d, y: %d\n", p.x, p.y);
	//TRACE("image:: x: %d-%d, y: %d-%d\n", scr_start.Left(), scr_start.Left() + scr_start.Width(), scr_start.Top(), scr_start.Top() + scr_start.Height());
}

void CGameStateInit::OnShow()
{
	//放背景
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
	//
	// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
	//

	//Screen_Start
	if (p.x > scr_start.Left() && p.x < scr_start.Left() + scr_start.Width() &&
		p.y > scr_start.Top() && p.y < scr_start.Top() + scr_start.Height()) {
		CMovingBitmap scr_about_red;
		scr_about_red.LoadBitmap(IDB_SCREEN_START_RED, RGB(0, 0, 255));
		scr_about_red.SetTopLeft((SIZE_X - scr_about_red.Width()) / 2, SIZE_Y / 2);
		scr_about_red.ShowBitmap();
	} else {
		scr_start.SetTopLeft((SIZE_X - scr_start.Width()) / 2, SIZE_Y / 2);
		scr_start.ShowBitmap();
	}
	

	//Screen_Load
	if (p.x > scr_load.Left() && p.x < scr_load.Left() + scr_load.Width() &&
		p.y > scr_load.Top() && p.y < scr_load.Top() + scr_load.Height()) {
		CMovingBitmap scr_load_red;
		scr_load_red.LoadBitmap(IDB_SCREEN_LOAD_RED, RGB(0, 0, 255));
		scr_load_red.SetTopLeft((SIZE_X - scr_load_red.Width()) / 2, SIZE_Y / 2 + scr_start.Height());
		scr_load_red.ShowBitmap();
	}
	else {
		scr_load.SetTopLeft((SIZE_X - scr_load.Width()) / 2, SIZE_Y / 2 + scr_load.Height());
		scr_load.ShowBitmap();
	}

	//Screen_Preferences
	if (p.x > scr_preferences.Left() && p.x < scr_preferences.Left() + scr_preferences.Width() &&
		p.y > scr_preferences.Top() && p.y < scr_preferences.Top() + scr_preferences.Height()) {
		CMovingBitmap scr_preferences_red;
		scr_preferences_red.LoadBitmap(IDB_SCREEN_PREFERENCES_RED, RGB(0, 0, 255));
		scr_preferences_red.SetTopLeft((SIZE_X - scr_preferences_red.Width()) / 2, SIZE_Y / 2 + scr_load.Height() + scr_preferences.Height());
		scr_preferences_red.ShowBitmap();
	}
	else {
		scr_preferences.SetTopLeft((SIZE_X - scr_preferences.Width()) / 2, SIZE_Y / 2 + scr_load.Height() + scr_preferences.Height());
		scr_preferences.ShowBitmap();
	}
	
	//Screen_about
	if (p.x > scr_about.Left() && p.x < scr_about.Left() + scr_about.Width() &&
		p.y > scr_about.Top() && p.y < scr_about.Top() + scr_about.Height()) {
		CMovingBitmap scr_about_red;
		scr_about_red.LoadBitmap(IDB_SCREEN_ABOUT_RED, RGB(0, 0, 255));
		scr_about_red.SetTopLeft((SIZE_X - scr_about_red.Width()) / 2, SIZE_Y / 2 + scr_load.Height() + scr_preferences.Height() + scr_about.Height());
		scr_about_red.ShowBitmap();
	}
	else {
		scr_about.SetTopLeft((SIZE_X - scr_about.Width()) / 2, SIZE_Y / 2 + scr_load.Height() + scr_preferences.Height() + scr_about.Height());
		scr_about.ShowBitmap();
	}

	//Screen_Exit
	if (p.x > scr_exit.Left() && p.x < scr_exit.Left() + scr_exit.Width() &&
		p.y > scr_exit.Top() && p.y < scr_exit.Top() + scr_exit.Height()) {
		CMovingBitmap scr_exit_red;
		scr_exit_red.LoadBitmap(IDB_SCREEN_EXIT_RED, RGB(0, 0, 255));
		scr_exit_red.SetTopLeft((SIZE_X - scr_exit_red.Width()) / 2, SIZE_Y / 2 + scr_load.Height() + scr_preferences.Height() + scr_about.Height() + scr_exit.Height());
		scr_exit_red.ShowBitmap();
	}
	else {
		scr_exit.SetTopLeft((SIZE_X - scr_exit.Width()) / 2, SIZE_Y / 2 + scr_load.Height() + scr_preferences.Height() + scr_about.Height() + scr_exit.Height());
		scr_exit.ShowBitmap();
	}


	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	//string message = string("%d x %d", width, height);
	
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,255));
	pDC->SetTextColor(RGB(255,255,0));
	//pDC->TextOut(120,220,"Please click mouse or press SPACE to begin.");
	//pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");
	//if (ENABLE_GAME_PAUSE)
	//	pDC->TextOut(5,425,"Press Ctrl-Q to pause the Game.");
	pDC->TextOut(5,455, "test");
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	
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
	scr_saveGame.LoadBitmapA(IDB_SCREEN_SAVE_GAME, RGB(0, 0, 255));
	scr_preferences.LoadBitmapA(IDB_SCREEN_PREFERENCES, RGB(0, 0, 255));
	scr_quitToMenu.LoadBitmap(IDB_SCREEN_QUIT_TO_MENU, RGB(0, 0, 255));
	scr_exit.LoadBitmapA(IDB_SCREEN_EXIT, RGB(0, 0, 255));


	//int time_temp = game_framework::GameStage_1::gettime();
}

void CGamestatePause::OnBeginState()
{
}

void CGamestatePause::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (p.x > scr_resume.Left() && p.x < scr_resume.Left() + scr_resume.Width() &&
		p.y > scr_resume.Top() && p.y < scr_resume.Top() + scr_resume.Height()) {
		//Resume

		GotoGameState(GAME_STAGE_1);
	}
	else if (p.x > scr_saveGame.Left() && p.x < scr_saveGame.Left() + scr_saveGame.Width() &&
		p.y > scr_saveGame.Top() && p.y < scr_saveGame.Top() + scr_saveGame.Height()) {
		//Save Game
	}
	else if (p.x > scr_preferences.Left() && p.x < scr_preferences.Left() + scr_preferences.Width() &&
		p.y > scr_preferences.Top() && p.y < scr_preferences.Top() + scr_preferences.Height()) {
		//Preferences
		//CWindows::ResizeClient();
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

void CGamestatePause::OnMouseMove(UINT nFlags, CPoint point)
{
	GetCursorPos(&p);
	ScreenToClient(AfxGetMainWnd()->m_hWnd, &p);	//把螢幕座標轉換為視窗座標，並讀取出來
	//TRACE("mouse:: x: %d, y: %d\n", p.x, p.y);
	//TRACE("image:: x: %d-%d, y: %d-%d\n", scr_start.Left(), scr_start.Left() + scr_start.Width(), scr_start.Top(), scr_start.Top() + scr_start.Height());
}
/*
void CGamestatePause::setTemp(int t)
{
	temp_time = t;
}
*/
void CGamestatePause::OnShow()
{
	//放背景
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


	//Save Game
	if (p.x > scr_saveGame.Left() && p.x < scr_saveGame.Left() + scr_saveGame.Width() &&
		p.y > scr_saveGame.Top() && p.y < scr_saveGame.Top() + scr_saveGame.Height()) {
		CMovingBitmap scr_saveGame_red;
		scr_saveGame_red.LoadBitmap(IDB_SCREEN_SAVE_GAME_RED, RGB(0, 0, 255));
		scr_saveGame_red.SetTopLeft((SIZE_X - scr_saveGame_red.Width()) / 2, SIZE_Y * 35 / 100);
		scr_saveGame_red.ShowBitmap();
	}
	else {
		scr_saveGame.SetTopLeft((SIZE_X - scr_saveGame.Width()) / 2, SIZE_Y * 35 / 100);
		scr_saveGame.ShowBitmap();
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
	Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 最終進度為100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo 數字對字串的轉換
	sprintf(str, "Game Over ! (%d)", counter / 30);
	pDC->TextOut(240,210,str);
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 第一關
/////////////////////////////////////////////////////////////////////////////
GameStage_1::GameStage_1(CGame* g) : CGameState(g)
{
	
}
GameStage_1::~GameStage_1() {

}
void GameStage_1::OnBeginState() {
	//腳色數值重置
	character_1.Initialize(128,32);

	int bg_reset[13][15] = {           //0地板 1石塊 2粉色石
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
	int coins_reset[5][2] = {
		{2,9},{4,1},{6,8},{8,13},{9,4}
	};
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 15; j++) {
			bg[i][j] = bg_reset[i][j];
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			coins_pos[i][j] = coins_reset[i][j];
		}
	}
	timer = 0;
}
void GameStage_1::OnInit() {
	//腳色bitmap load
	block_0.LoadBitmap(IDB_Bg_1, RGB(255, 255, 255));
	block_1.LoadBitmap(IDB_Blocks, RGB(255, 255, 255));
	block_2.LoadBitmap(IDB_BREAK_0, RGB(255, 255, 255));
	border.LoadBitmap(IDB_BORDER_0, RGB(255, 255, 255));
	coins.LoadBitmap(IDB_COIN_0, RGB(255, 255, 255));
	panel.LoadBitmap(IDB_Panel, RGB(255, 255, 255));
	character_1.LoadBitmap();

	if (tempTime == NULL) {
		count_down.SetInteger(60);
	}
	else {
		count_down.SetInteger(*tempTime);
		tempTime = NULL;
	}
}
void GameStage_1::OnMove() {
	timer++;
	int second = timer / 30;
	int min = second / 60;
	second %= 60;

	TRACE("second %d\n", second);
	TRACE("min %d\n", min);

	if (min == 2) {
		//下一關的關卡
	}
	if (!(timer % 30))
		count_down.Add(-1);

	character_1.OnMove();
}
void GameStage_1::OnShow() {                   //越後放的顯示會越上層
	panel.SetTopLeft(0, 0);
	panel.ShowBitmap();
	border.SetTopLeft(96, 0);
	border.ShowBitmap();
	for (int i = 0; i < 13; i++) {             //方塊顯示    j是X軸 i是Y軸
		for (int j = 0; j < 15; j++) {
			switch (bg[i][j]) {
			case 1:
				block_1.SetTopLeft(128 + 32 * j, 32 * (i + 1));
				block_1.ShowBitmap();
				break;
			case 2:
				block_0.SetTopLeft(128 + 32 * j, 32 * (i + 1));
				block_0.ShowBitmap();
				block_2.SetTopLeft(128 + 32 * j, 32 * (i + 1));
				block_2.ShowBitmap();
				break;
			default:
				block_0.SetTopLeft(128 + 32 * j, 32 * (i + 1));
				block_0.ShowBitmap();
				break;
			}
			std::cout << 3+i << std::endl;
		}
	}
	for (int i = 0; i < 5; i++) {
		coins.SetTopLeft(128 + coins_pos[i][1] * 32, 32* (coins_pos[i][0] + 1));
		coins.ShowBitmap();
	}

	count_down.SetTopLeft(panel.Width() * 25 / 100, panel.Height() * 48 / 100);
	count_down.LoadBitmapA();
	count_down.ShowBitmap();

	character_1.OnShow();
}

void GameStage_1::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25; // keyboard左箭頭
	const char KEY_UP = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN = 0x28; // keyboard下箭頭
	const char KEY_ESC = 0x1B;
	const char KEY_P = 0x50;
	//int temp = count_down.GetInteger();

	if (nChar == KEY_ESC || nChar == KEY_P) {
		game_framework::CGame::Instance()->OnFilePause();
		//*tempTime = count_down.GetInteger();
		this -> count_down.GetInteger();
		GotoGameState(GAME_STATE_PAUSE);
	}

	if (nChar == KEY_LEFT) {
		character_1.LoadMap(bg);
		character_1.SetMovingLeft(true);
	}
	if (nChar == KEY_RIGHT) {
		character_1.LoadMap(bg);
		character_1.SetMovingRight(true);
	}
	if (nChar == KEY_UP) {
		character_1.LoadMap(bg);
		character_1.SetMovingUp(true);
	}
	if (nChar == KEY_DOWN) {
		character_1.LoadMap(bg);
		character_1.SetMovingDown(true);
	}
}

void GameStage_1::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25; // keyboard左箭頭
	const char KEY_UP = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN = 0x28; // keyboard下箭頭
	if (nChar == KEY_LEFT)
		character_1.SetMovingLeft(false);
	if (nChar == KEY_RIGHT)
		character_1.SetMovingRight(false);
	if (nChar == KEY_UP)
		character_1.SetMovingUp(false);
	if (nChar == KEY_DOWN)
		character_1.SetMovingDown(false);
}
/*
int GameStage_1::gettime()
{
	return count_down.GetInteger();
}
*/

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////
/*
CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g), NUMBALLS(28)
{
	ball = new CBall [NUMBALLS];
}

CGameStateRun::~CGameStateRun()
{
	delete [] ball;
}

void CGameStateRun::OnBeginState()
{
	const int BALL_GAP = 90;
	const int BALL_XY_OFFSET = 45;
	const int BALL_PER_ROW = 7;
	const int HITS_LEFT = 10;
	const int HITS_LEFT_X = 590;
	const int HITS_LEFT_Y = 0;
	const int BACKGROUND_X = 60;
	const int ANIMATION_SPEED = 15;
	for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
		int x_pos = i % BALL_PER_ROW;
		int y_pos = i / BALL_PER_ROW;
		ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		ball[i].SetDelay(x_pos);
		ball[i].SetIsAlive(true);
	}
	eraser.Initialize();
	//background.SetTopLeft(BACKGROUND_X,0);				// 設定背景的起始座標
	help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
	hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
	hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// 指定剩下撞擊數的座標
	CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
	CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
	CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	//
	// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	//
	
	// 移動背景圖的座標
	//
	if (background.Top() > SIZE_Y)
		background.SetTopLeft(60 ,-background.Height());
	background.SetTopLeft(background.Left(),background.Top()+1);
	

	//
	// 移動球
	//
	int i;
	for (i=0; i < NUMBALLS; i++)
		ball[i].OnMove();
	//
	// 移動擦子
	//
	eraser.OnMove();
	//
	// 判斷擦子是否碰到球
	//
	for (i=0; i < NUMBALLS; i++)
		if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
			ball[i].SetIsAlive(false);
			CAudio::Instance()->Play(AUDIO_DING);
			hits_left.Add(-1);
			//
			// 若剩餘碰撞次數為0，則跳到Game Over狀態
			//
			if (hits_left.GetInteger() <= 0) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
				GotoGameState(GAME_STATE_OVER);
			}
		}
	//
	// 移動彈跳的球
	//
	bball.OnMove();
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
	//
	// 開始載入資料
	//
	int i;
	for (i = 0; i < NUMBALLS; i++)	
		ball[i].LoadBitmap();								// 載入第i個球的圖形
	eraser.LoadBitmap();
	background.LoadBitmap(IDB_LOGO);					// 載入背景的圖形
	//
	// 完成部分Loading動作，提高進度
	//
	ShowInitProgress(50);
	Sleep(300); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 繼續載入其他資料
	//
	help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// 載入說明的圖形
	corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
	//corner.ShowBitmap(background);							// 將corner貼到background
	bball.LoadBitmap();										// 載入圖形
	hits_left.LoadBitmap();									
	CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
	CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
	CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
	//
	// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
	//
}


void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_ESC	 = 0x1B;
	const char KEY_P     = 0x50;

	if (nChar == KEY_ESC || nChar == KEY_P) {
		game_framework::CGame::Instance()->OnFilePause();
		
	}

	if (nChar == KEY_LEFT)
		eraser.SetMovingLeft(true);
	if (nChar == KEY_RIGHT)
		eraser.SetMovingRight(true);
	if (nChar == KEY_UP)
		eraser.SetMovingUp(true);
	if (nChar == KEY_DOWN)
		eraser.SetMovingDown(true);
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	if (nChar == KEY_LEFT)
		eraser.SetMovingLeft(false);
	if (nChar == KEY_RIGHT)
		eraser.SetMovingRight(false);
	if (nChar == KEY_UP)
		eraser.SetMovingUp(false);
	if (nChar == KEY_DOWN)
		eraser.SetMovingDown(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	eraser.SetMovingLeft(true);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	eraser.SetMovingLeft(false);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	eraser.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	eraser.SetMovingRight(false);
}

void CGameStateRun::OnShow()
{
	//
	//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
	//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
	//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
	//
	//
	//  貼上背景圖、撞擊數、球、擦子、彈跳的球
	//
	background.ShowBitmap();			// 貼上背景圖
	help.ShowBitmap();					// 貼上說明圖
	hits_left.ShowBitmap();
	for (int i=0; i < NUMBALLS; i++)
		ball[i].OnShow();				// 貼上第i號球
	bball.OnShow();						// 貼上彈跳的球
	eraser.OnShow();					// 貼上擦子
	//
	//  貼上左上及右下角落的圖
	//
	corner.SetTopLeft(0,0);
	corner.ShowBitmap();
	corner.SetTopLeft(SIZE_X-corner.Width(), SIZE_Y-corner.Height());
	corner.ShowBitmap();

}
*/
}