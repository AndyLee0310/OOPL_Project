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
#include "CEraser.h"
#include "CBall.h"
#include "CBouncingBall.h"
#include "Character.h"
#include "Bomb.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "CoinsAnimation.h"
#include "Healths.h"


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	static int form_state = 0;			//1為起始畫面  2為暫停畫面

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyDown(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnMove();
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap logo;								// csie的logo

		CMovingBitmap scr_start;
		CMovingBitmap scr_load;
		CMovingBitmap scr_preferences;
		CMovingBitmap scr_about;
		CMovingBitmap scr_exit;

		POINT p;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////
	/*
	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作

	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		const int		NUMBALLS;	// 球的總數
		CMovingBitmap	background;	// 背景圖
		CMovingBitmap	help;		// 說明圖
		CBall			*ball;		// 球的陣列
		CMovingBitmap	corner;		// 角落圖
		CEraser			eraser;		// 拍子
		CInteger		hits_left;	// 剩下的撞擊數
		CBouncingBall   bball;		// 反覆彈跳的球
		
	};
	*/

	//static int form_flag;
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
		void GetCoins();				//偵測碰撞金幣
		void CoinState();				//金幣動畫
		void HealthState();
		void HeartChange();
	private:
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
		Enemy         *AI;
		int coins_num;	             //金幣總數
		int sc;		                 //紀錄吃到幾個金幣
		CoinsAnimation* coin_Ani;
		Bomb*         Bomb_ch1;

		CMovingBitmap playerhead_1;
		CMovingBitmap playerhead_2;

		double heart_num[8];
		double blood_ori, blood_vol;
		Healths* heart;

		CInteger count_down;
		int* tempTime;
		int timer;
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
		//void setTemp(int t);
	protected:
		void OnShow();
	private:
		CMovingBitmap bg;
		CMovingBitmap scr_resume;
		CMovingBitmap scr_saveGame;
		CMovingBitmap scr_preferences;
		CMovingBitmap scr_quitToMenu;
		CMovingBitmap scr_exit;
		POINT p;

		//static int temp_time;
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
		int counter;	// 倒數之計數器
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

		CMovingBitmap scr_FS_yes;
		CMovingBitmap scr_FS_no;
		int FS_state;

		CMovingBitmap scr_FR;

		CMovingBitmap scr_SF_yes;
		CMovingBitmap scr_SF_no;
		int SF_state;

		CMovingBitmap scr_Vsync_yes;
		CMovingBitmap scr_Vsync_no;
		int Vsync_state;

		POINT p;

		int form_ori;		//讀取剛剛是從哪一個畫面進到此畫面
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
		CMovingBitmap scr_back;

		POINT p;
	};
}

#endif