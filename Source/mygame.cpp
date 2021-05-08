/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
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
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
// 
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
	//
	// �}�l���J���
	//
	logo.LoadBitmap(IDB_LOGO,RGB(255,255,255));

	scr_start.LoadBitmap(IDB_SCREEN_START, RGB(0, 0, 255));
	scr_load.LoadBitmapA(IDB_SCREEN_LOAD, RGB(0, 0, 255));
	scr_preferences.LoadBitmapA(IDB_SCREEN_PREFERENCES, RGB(0, 0, 255));
	scr_about.LoadBitmap(IDB_SCREEN_ABOUT, RGB(0, 0, 255));
	scr_exit.LoadBitmapA(IDB_SCREEN_EXIT, RGB(0, 0, 255));

	Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
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
		GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
	
	else if (nChar == KEY_ESC)								// Demo �����C������k
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// �����C��
	*/
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (p.x > scr_start.Left() && p.x < scr_start.Left() + scr_start.Width() &&
		p.y > scr_start.Top() && p.y < scr_start.Top() + scr_start.Height()) {
		GotoGameState(GAME_STAGE_1);		// ������GAME_STATE_RUN
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
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
	}
}

void CGameStateInit::OnMove()
{
	GetCursorPos(&p);
	ScreenToClient(AfxGetMainWnd()->m_hWnd, &p);	//��ù��y���ഫ�������y�СA��Ū���X��
	//TRACE("mouse:: x: %d, y: %d\n", p.x, p.y);
	//TRACE("image:: x: %d-%d, y: %d-%d\n", scr_start.Left(), scr_start.Left() + scr_start.Width(), scr_start.Top(), scr_start.Top() + scr_start.Height());
}

void CGameStateInit::OnShow()
{
	//��I��
	CMovingBitmap bg;
	bg.LoadBitmap(IDB_SCREENBG1, RGB(0,0,0));
	for (int x = 0; x < SIZE_X; x += bg.Width()) {
		for (int y = 0; y < SIZE_Y; y += bg.Height()) {
			bg.SetTopLeft(x,y);
			bg.ShowBitmap();
		}
	}
	//
	// �K�Wlogo
	//
	logo.SetTopLeft((SIZE_X - logo.Width())/2, SIZE_Y/8);
	logo.ShowBitmap();
	//
	// Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
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
	
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
	fp=pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(RGB(0,0,255));
	pDC->SetTextColor(RGB(255,255,0));
	//pDC->TextOut(120,220,"Please click mouse or press SPACE to begin.");
	//pDC->TextOut(5,395,"Press Ctrl-F to switch in between window mode and full screen mode.");
	//if (ENABLE_GAME_PAUSE)
	//	pDC->TextOut(5,425,"Press Ctrl-Q to pause the Game.");
	pDC->TextOut(5,455, "test");
	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	
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
		PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
	}
}

void CGamestatePause::OnMouseMove(UINT nFlags, CPoint point)
{
	GetCursorPos(&p);
	ScreenToClient(AfxGetMainWnd()->m_hWnd, &p);	//��ù��y���ഫ�������y�СA��Ū���X��
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
	//��I��
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
// �o��class���C�����������A(Game Over)
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
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
	//
	// �}�l���J���
	//
	Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// �̲׶i�׬�100%
	//
	ShowInitProgress(100);
}

void CGameStateOver::OnShow()
{
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// ���� font f; 160���16 point���r
	fp=pDC->SelectObject(&f);					// ��� font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
	sprintf(str, "Game Over ! (%d)", counter / 30);
	pDC->TextOut(240,210,str);
	pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}

/////////////////////////////////////////////////////////////////////////////
// �Ĥ@��
/////////////////////////////////////////////////////////////////////////////
GameStage_1::GameStage_1(CGame* g) : CGameState(g)
{
	Bomb_ch1 = new Bomb [7];
	block_2 = new Obstacle[42];

}
GameStage_1::~GameStage_1() {
	delete [] Bomb_ch1;
	delete[] block_2;
}
void GameStage_1::OnBeginState() {
	for (int i = 0; i < 7; i++) {
		Bomb_ch1[i].Initialize();
	}
	int bg_reset[13][15] = {           //0�a�O 1�۶� 2�����
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
		for (int j = 0; j < 2; j++) {
			coins_pos[i][j] = coins_reset[i][j];
		}
	}
	//�}��ƭȭ��m
	character_1.Initialize(128, 32);
	character_1.LoadMap(bg);
	timer = 0;
}
void GameStage_1::OnInit() {
	timer = 250;
	block_0.LoadBitmap(IDB_Bg_1, RGB(255, 255, 255));
	block_1.LoadBitmap(IDB_Blocks, RGB(255, 255, 255));
	for (int i = 0; i < 42; i++) {
		block_2[i].LoadBitmap();
	}
	border.LoadBitmap(IDB_BORDER_0, RGB(255, 255, 255));
	coins.LoadBitmap(IDB_COIN_0, RGB(255, 255, 255));
	panel.LoadBitmap(IDB_Panel, RGB(255, 255, 255));
	character_1.LoadBitmap();
	for (int i = 0; i < 7; i++) {
		Bomb_ch1[i].LoadBitmap();
	}
}
void GameStage_1::OnMove() {
	timer++;
	int second = timer / 30;
	int min = second / 60;
	second %= 60;

	//TRACE("second %d\n", second);
	//TRACE("min %d\n", min);

	if (min == 2) {
		//�U�@�������d
	}
	if (!(timer % 30))
		count_down.Add(-1);

	for (int i = 0; i < 42; i++) {
		block_2[i].OnMove();
		if (block_2[i].getActive() && !block_2[i].getExp()) {
			mapChange(block_2_pos[i][1], block_2_pos[i][0], 0);
		}
	}
	character_1.OnMove();
	BombState();
}
void GameStage_1::OnShow() {                   //�V�����ܷ|�V�W�h
	panel.SetTopLeft(0, 0);
	panel.ShowBitmap();
	border.SetTopLeft(96, 0);
	border.ShowBitmap();
	for (int i = 0; i < 13; i++) {             //������    j�OX�b i�OY�b
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

	for (int i = 0; i < 5; i++) {
		coins.SetTopLeft(128 + coins_pos[i][1] * 32, 32* (coins_pos[i][0] + 1));
		coins.ShowBitmap();
	}

	count_down.SetTopLeft(panel.Width() * 25 / 100, panel.Height() * 48 / 100);
	count_down.LoadBitmap();
	count_down.ShowBitmap();
	character_1.OnShow();
	for (int i = 0; i < 7; i++) {
		Bomb_ch1[i].OnShow();
	}
}

void GameStage_1::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25;  // keyboard���b�Y
	const char KEY_UP = 0x26;    // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN = 0x28;  // keyboard�U�b�Y
	const char KEY_ESC = 0x1B;
	const char KEY_P = 0x50;
	const char KEY_SPACE = 0x20;

	
	if (nChar == KEY_ESC || nChar == KEY_P) {
		game_framework::CGame::Instance()->OnFilePause();
		//*tempTime = count_down.GetInteger();
		this -> count_down.GetInteger();
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
}

void GameStage_1::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT = 0x25; // keyboard���b�Y
	const char KEY_UP = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN = 0x28; // keyboard�U�b�Y
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
	if (id == 1) {                                                  //[y][x]
		int x = (character_1.GetX1() + character_1.GetX2()) / 2;    //�}�⤤���I
		int y = (character_1.GetY1() + character_1.GetY2()) / 2;    //�}�⤤���I
		x = (x - 128) / 32;                                         //�ഫ��13*15�a�ϼҦ�
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
			TRACE("X,Y = %d %d set to %d\n", x, y, bg[y][x]);
		}
		else {
			TRACE("fail\n");
		}
	}
	else if (id == 2) {

	}
}

void GameStage_1::mapChange(int x, int y, int value) {
	bg[y][x] = value;
	character_1.LoadMap(bg);
}
void GameStage_1::BombState() {
	for (int i = 0; i < 7; i++) {
		Bomb_ch1[i].OnMove();
		if (Bomb_ch1[i].getActive() && Bomb_ch1[i].getExp()) {  //�z���������u��m���]���i�樫
			int nx = (Bomb_ch1[i].getTop_Bomb() - 128) / 32;
			int ny = (Bomb_ch1[i].getLeft_Bomb() - 32) / 32;
			mapChange(nx, ny, 5);
			if(!Bomb_ch1[i].getObs())setBombRange(1, i, nx, ny);
			for (int i = 1; i <= Bomb_ch1[i].getUp(); i++)mapChange(nx, ny - i, 5);
			for (int i = 1; i <= Bomb_ch1[i].getDown(); i++)mapChange(nx, ny + i, 5);
			for (int i = 1; i <= Bomb_ch1[i].getRight(); i++)mapChange(nx + i, ny, 5);
			for (int i = 1; i <= Bomb_ch1[i].getLeft(); i++)mapChange(nx - i, ny, 5);
			
		}
		if (!Bomb_ch1[i].getActive() && Bomb_ch1[i].getExp()) {
			int nx = (Bomb_ch1[i].getTop_Bomb() - 128) / 32;
			int ny = (Bomb_ch1[i].getLeft_Bomb() - 32) / 32;
			mapChange(nx, ny, 0);
			for (int i = 1; i <= Bomb_ch1[i].getUp(); i++)mapChange(nx, ny - i, 0);
			for (int i = 1; i <= Bomb_ch1[i].getDown(); i++)mapChange(nx, ny + i, 0);
			for (int i = 1; i <= Bomb_ch1[i].getRight(); i++)mapChange(nx + i, ny, 0);
			for (int i = 1; i <= Bomb_ch1[i].getLeft(); i++)mapChange(nx - i, ny, 0);
			
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
/*
int GameStage_1::gettime()
{
	return count_down.GetInteger();
}
*/

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
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
	for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
		int x_pos = i % BALL_PER_ROW;
		int y_pos = i / BALL_PER_ROW;
		ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
		ball[i].SetDelay(x_pos);
		ball[i].SetIsAlive(true);
	}
	eraser.Initialize();
	//background.SetTopLeft(BACKGROUND_X,0);				// �]�w�I�����_�l�y��
	help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
	hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
	hits_left.SetTopLeft(HITS_LEFT_X,HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
	CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
	CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
	CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
}

void CGameStateRun::OnMove()							// ���ʹC������
{
	//
	// �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
	//
	// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
	//
	
	// ���ʭI���Ϫ��y��
	//
	if (background.Top() > SIZE_Y)
		background.SetTopLeft(60 ,-background.Height());
	background.SetTopLeft(background.Left(),background.Top()+1);
	

	//
	// ���ʲy
	//
	int i;
	for (i=0; i < NUMBALLS; i++)
		ball[i].OnMove();
	//
	// �������l
	//
	eraser.OnMove();
	//
	// �P�_���l�O�_�I��y
	//
	for (i=0; i < NUMBALLS; i++)
		if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
			ball[i].SetIsAlive(false);
			CAudio::Instance()->Play(AUDIO_DING);
			hits_left.Add(-1);
			//
			// �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
			//
			if (hits_left.GetInteger() <= 0) {
				CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
				CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
				GotoGameState(GAME_STATE_OVER);
			}
		}
	//
	// ���ʼu�����y
	//
	bball.OnMove();
}

void CGameStateRun::OnInit()  								// �C������Ȥιϧγ]�w
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
	//
	// �}�l���J���
	//
	int i;
	for (i = 0; i < NUMBALLS; i++)	
		ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�
	eraser.LoadBitmap();
	background.LoadBitmap(IDB_LOGO);					// ���J�I�����ϧ�
	//
	// ��������Loading�ʧ@�A�����i��
	//
	ShowInitProgress(50);
	Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// �~����J��L���
	//
	help.LoadBitmap(IDB_HELP,RGB(255,255,255));				// ���J�������ϧ�
	corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
	//corner.ShowBitmap(background);							// �Ncorner�K��background
	bball.LoadBitmap();										// ���J�ϧ�
	hits_left.LoadBitmap();									
	CAudio::Instance()->Load(AUDIO_DING,  "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
	CAudio::Instance()->Load(AUDIO_LAKE,  "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
	CAudio::Instance()->Load(AUDIO_NTUT,  "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
	//
	// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
	//
}


void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
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
	const char KEY_LEFT  = 0x25; // keyboard���b�Y
	const char KEY_UP    = 0x26; // keyboard�W�b�Y
	const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
	const char KEY_DOWN  = 0x28; // keyboard�U�b�Y
	if (nChar == KEY_LEFT)
		eraser.SetMovingLeft(false);
	if (nChar == KEY_RIGHT)
		eraser.SetMovingRight(false);
	if (nChar == KEY_UP)
		eraser.SetMovingUp(false);
	if (nChar == KEY_DOWN)
		eraser.SetMovingDown(false);
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
	eraser.SetMovingLeft(true);
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	eraser.SetMovingLeft(false);
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	// �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // �B�z�ƹ����ʧ@
{
	eraser.SetMovingRight(true);
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
{
	eraser.SetMovingRight(false);
}

void CGameStateRun::OnShow()
{
	//
	//  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
	//        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
	//        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
	//
	//
	//  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
	//
	background.ShowBitmap();			// �K�W�I����
	help.ShowBitmap();					// �K�W������
	hits_left.ShowBitmap();
	for (int i=0; i < NUMBALLS; i++)
		ball[i].OnShow();				// �K�W��i���y
	bball.OnShow();						// �K�W�u�����y
	eraser.OnShow();					// �K�W���l
	//
	//  �K�W���W�Υk�U��������
	//
	corner.SetTopLeft(0,0);
	corner.ShowBitmap();
	corner.SetTopLeft(SIZE_X-corner.Width(), SIZE_Y-corner.Height());
	corner.ShowBitmap();

}
*/
}