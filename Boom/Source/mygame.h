/*
 * mygame.h: ���ɮ��x�C��������class��interface
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


	static int form_state = 0;			// 1���_�l�e��  2���Ȱ��e��	3��Preference
	static int form_ori = 0;			// Ū���W�@�����}���e���O���@��
	static int show = 1;				// �O�_��ܹC��������ܮ�(show:1 unshow:0)
	static int FS_state = 0;
	static int SF_state = 0;
	static int Vsync_state = 1;

	

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	protected:
		void OnMove();
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap logo;								// csie��logo
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
	// �o��class���C�����Ĥ@�����檫��
	/////////////////////////////////////////////////////////////////////////////

	class GameStage_1 : public CGameState {
	public:
		GameStage_1(CGame* g);
		~GameStage_1();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();                                  // �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);               // ��L�ʧ@
		void OnKeyUp(UINT, UINT, UINT);
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
		void setBomb(int);
		void mapChange(int,int,int);                            // �a���ܰ�&�q��character
		void BombState();
		void setBombRange(int,int,int,int);                                    // �z���ɳ]�m�d��
		void GetCoins();				// �����I������&�ʵe
		void HealthState();
	private:
		CMovingBitmap level;
		int bg[13][15];              //0�a�O 1�۶� 2����� 4���z�u 5�z����
		int coins_pos[5][2];         //�w����m
		CMovingBitmap block_0;
		CMovingBitmap block_1;
		Obstacle*     block_2;
		int           block_2_pos[42][2];
		CMovingBitmap panel;
		CMovingBitmap border;
		Character     character_1;   //Range undone
		CMovingBitmap character_2;   //���O�����
		int Enemy1_num;			// �ĤH1���ƶq
		int Enemy2_num;			// �ĤH2���ƶq
		Enemy         *AI;
		int coins_num;	             //�����`��
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
	//�C���L�{�����Ȱ��e��
	/////////////////////////////////////////////////////////////////////////////

	class CGamestatePause : public CGameState {
	public:
		CGamestatePause(CGame* g);
		void OnInit();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
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
	// �o��class���C�����������A(Game Over)
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		CMovingBitmap over;
		int counter;									// �˼Ƥ��p�ƾ�
		int score;
	};
	////////////////////////////////////////////////////////////////////////////
	//Prefences�e��
	////////////////////////////////////////////////////////////////////////////
	class GamePrefences : public CGameState {
	public:
		GamePrefences(CGame* g);
		void OnBeginState();
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);
	protected:
		void OnShow();
	private:
		CMovingBitmap bg;
		CMovingBitmap scr;
		CMovingBitmap scr_ok;
		CMovingBitmap scr_cancel;
		CMovingBitmap scr_FX_down;		// FX���q��p
		CMovingBitmap scr_FX_up;		// FX���q��j
		CMovingBitmap scr_FS_yes;		// Fullscreen YES
		CMovingBitmap scr_FS_no;		// Fullscreen NO
		CMovingBitmap scr_FR;			// Fllscr. Res.
		CMovingBitmap scr_SF_yes;		// Show FPS YES
		CMovingBitmap scr_SF_no;		// Show FPS NO
		CMovingBitmap scr_Vsync_yes;	// Vsync YES
		CMovingBitmap scr_Vsync_no;		// Vsync NO
		int FS_ori_state;				// Fullscreen �쪬�A
		int SF_ori_state;				// Show FPS �쪬�A
		int Vsync_ori_state;			// Vsync �쪬�A
		POINT p;
	};

	////////////////////////////////////////////////////////////////////////////
	//About�e��
	////////////////////////////////////////////////////////////////////////////
	class GameAbout : public CGameState {
	public:
		GameAbout(CGame* g);
		void OnBeginState();
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
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