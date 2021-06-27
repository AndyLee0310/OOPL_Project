/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/


#ifndef _MYGAME_
#define _MYGAME_
#include "Character.h"
#include "Bomb.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "CoinsAnimation.h"
#include "Healths.h"
#include "GameStage_2.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////


	static int form_state = 0;			// 1為起始畫面  2為暫停畫面	3為Preference
	static int form_ori = 0;			// 讀取上一個離開的畫面是哪一個
	static int show = 1;				// 是否顯示遊戲說明對話框(show:1 unshow:0)
	static int FS_state = 0;
	static int SF_state = 0;
	static int Vsync_state = 1;

	

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnMove();
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap logo;								// csie的logo
		CMovingBitmap scr_start;
		CMovingBitmap scr_gie;
		CMovingBitmap scr_preferences;
		CMovingBitmap scr_about;
		CMovingBitmap scr_exit;
		CMovingBitmap gameInstruction;
		CMovingBitmap close;

		POINT p;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的第一關執行物件
	/////////////////////////////////////////////////////////////////////////////

	class GameStage_1 : public CGameState {
	public:
		GameStage_1(CGame* g);
		~GameStage_1();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();                                  // 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);               // 鍵盤動作
		void OnKeyUp(UINT, UINT, UINT);
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void setBomb(int);
		void mapChange(int,int,int);                            // 地圖變動&通知character
		void BombState();
		void setBombRange(int,int,int,int);                                    // 爆炸時設置範圍
		void GetCoins();				// 偵測碰撞金幣&動畫
		void HealthState();
	private:
		CMovingBitmap level;
		int bg[13][15];              //0地板 1石塊 2粉色石 4未爆彈 5爆炸中
		int coins_pos[5][2];         //硬幣位置
		CMovingBitmap block_0;
		CMovingBitmap block_1;
		Obstacle*     block_2;
		int           block_2_pos[42][2];
		CMovingBitmap panel;
		CMovingBitmap border;
		Character     character_1;   //Range undone
		CMovingBitmap character_2;   //類別之後改
		int Enemy1_num;			// 敵人1的數量
		int Enemy2_num;			// 敵人2的數量
		Enemy         *AI;
		int coins_num;	             //金幣總數
		CoinsAnimation* coin_Ani;
		Bomb*         Bomb_ch1;

		CMovingBitmap playerhead_1;
		CMovingBitmap playerhead_2;

		int life;
		int heart_num[8];
		int blood_ori, blood_vol;
		Healths* heart;
		bool taking_Damage;
		int k = 0;
		CInteger count_down;
		int timer;
		int score;
	};

	/////////////////////////////////////////////////////////////////////////////
	//遊戲過程中的暫停畫面
	/////////////////////////////////////////////////////////////////////////////

	class CGamestatePause : public CGameState {
	public:
		CGamestatePause(CGame* g);
		void OnInit();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);
	protected:
		void OnShow();
	private:
		CMovingBitmap bg;
		CMovingBitmap scr_resume;
		CMovingBitmap scr_gie;
		CMovingBitmap scr_preferences;
		CMovingBitmap scr_quitToMenu;
		CMovingBitmap scr_exit;
		CMovingBitmap gameInstruction;
		CMovingBitmap close;
		POINT p;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap over;
		int counter;									// 倒數之計數器
		int score;
	};
	////////////////////////////////////////////////////////////////////////////
	//Prefences畫面
	////////////////////////////////////////////////////////////////////////////
	class GamePrefences : public CGameState {
	public:
		GamePrefences(CGame* g);
		void OnBeginState();
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);
	protected:
		void OnShow();
	private:
		CMovingBitmap bg;
		CMovingBitmap scr;
		CMovingBitmap scr_ok;
		CMovingBitmap scr_cancel;
		CMovingBitmap scr_FX_down;		// FX音量減小
		CMovingBitmap scr_FX_up;		// FX音量放大
		CMovingBitmap scr_FS_yes;		// Fullscreen YES
		CMovingBitmap scr_FS_no;		// Fullscreen NO
		CMovingBitmap scr_FR;			// Fllscr. Res.
		CMovingBitmap scr_SF_yes;		// Show FPS YES
		CMovingBitmap scr_SF_no;		// Show FPS NO
		CMovingBitmap scr_Vsync_yes;	// Vsync YES
		CMovingBitmap scr_Vsync_no;		// Vsync NO
		int FS_ori_state;				// Fullscreen 原狀態
		int SF_ori_state;				// Show FPS 原狀態
		int Vsync_ori_state;			// Vsync 原狀態
		POINT p;
	};

	////////////////////////////////////////////////////////////////////////////
	//About畫面
	////////////////////////////////////////////////////////////////////////////
	class GameAbout : public CGameState {
	public:
		GameAbout(CGame* g);
		void OnBeginState();
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);
	protected:
		void OnShow();
	private:
		CMovingBitmap bg;
		CMovingBitmap aboutForm;
		CMovingBitmap scr_back;
		POINT p;
	};
}

#endif