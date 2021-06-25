#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Character.h"
#include "Bomb.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "CoinsAnimation.h"
#include "Healths.h"
#include <iostream>
#include "GameStage_2.h"

namespace game_framework {
	GameStage_2::GameStage_2(CGame* g) : CGameState(g)
	{
		block2_num = 88;
		coins_num = 9;
		sc = 0;
		Bomb_ch1 = new Bomb[7];
		block_2 = new Obstacle[88];
		coin_Ani = new CoinsAnimation[9];
		heart = new Healths[8];
		AI = new Enemy[4];
	}
	GameStage_2::~GameStage_2() {
		delete[] Bomb_ch1;
		delete[] block_2;
		delete[] coin_Ani;
		delete[] heart;
		delete[] AI;
	}
	void GameStage_2::OnBeginState() {
		for (int i = 0; i < 7; i++) {
			Bomb_ch1[i].Initialize();
		}
		int obstacle_reset[88][2] = {
			{0,3},{0,5},{0,6},{0,8},{0,9},{0,12},{0,13},{0,14},
			{1,2},{1,4},{1,6},{1,8},{1,10},{1,12},{1,14},
			{2,2},{2,3},{2,4},{2,5},{2,9},{2,10},{2,11},{2,12},{2,14},
			{3,0},{3,6},{3,8},
			{4,0},{4,2},{4,3},{4,4},{4,10},{4,11},{4,12},{4,13},{4,14},
			{5,0},{5,2},{5,6},{5,8},{5,14},
			{6,0},{6,1},{6,2},{6,3},{6,4},{6,11},{6,12},{6,14},
			{7,6},{7,8},{7,12},
			{8,0},{8,1},{8,2},{8,4},{8,10},{8,11},{8,12},{8,14},
			{9,2},{9,6},{9,8},{9,12},{9,14},
			{10,0},{10,2},{10,3},{10,4},{10,5},{10,9},{10,10},{10,11},
			{11,0},{11,4},{11,6},{11,8},{11,10},{11,12},
			{12,0},{12,1},{12,3},{12,5},{12,6},{12,8},{12,9},{12,10},{12,12}
		};
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 15; j++) {
				if (i % 2 == 1 && j % 2 == 1)bg[i][j] = 1;
				else bg[i][j] = 0;
			}
		}
		for (int i = 0; i < block2_num; i++) {
			block_2_pos[i][0] = obstacle_reset[i][0];
			block_2_pos[i][1] = obstacle_reset[i][1];
			bg[block_2_pos[i][0]][block_2_pos[i][1]] = 2;
			block_2[i].Initialize(block_2_pos[i][1] * 32 + 128, block_2_pos[i][0] * 32 + 32);
		}
		coins_num = 9;
		int coins_reset[9][2] = {
			{0,7},{3,4},{3,10},{6,5},{6,7},{6,9},{9,4},{9,10},{12,7}
		};
		for (int i = 0; i < coins_num; i++) {
			coins_pos[i][0] = coins_reset[i][0];
			coins_pos[i][1] = coins_reset[i][1];

			coin_Ani[i].Initialize(coins_pos[i][1] * 32 + 128, coins_pos[i][0] * 32 + 32);
		}

		int health_reset[8] = { 2, 2, 2, 2, 2, 2, 2, 2 };
		for (int i = 0; i < 8; i++) {
			heart_num[i] = health_reset[i];
		}
		blood_ori = blood_vol = 16;		//預設血量總值為8

		//腳色數值重置
		character_1.Initialize(128, 32);
		character_1.LoadMap(bg);

		AI[0].Initialize(7 * 32 + 128, 2 * 32 + 32);
		AI[1].Initialize(7 * 32 + 128, 4 * 32 + 32);
		AI[2].Initialize(7 * 32 + 128, 8 * 32 + 32);
		AI[3].Initialize(7 * 32 + 128, 10 * 32 + 32);
		for (int i = 0; i < 4; i++) {
			AI[i].LoadMap(bg);
		}
		timer = 0;
		CAudio::Instance()->Play(AUDIO_BGM2, true);
	}
	void GameStage_2::OnInit() {
		timer = 250;
		level.LoadBitmap(IDB_LEVEL_2);
		block_0.LoadBitmap(IDB_Bg_1, RGB(255, 255, 255));
		block_1.LoadBitmap(IDB_Blocks, RGB(255, 255, 255));
		for (int i = 0; i < block2_num; i++) {
			block_2[i].LoadBitmap();
		}
		border.LoadBitmap(IDB_BORDER_0, RGB(255, 255, 255));
		panel.LoadBitmap(IDB_Panel, RGB(255, 255, 255));
		character_1.LoadBitmap();
		for (int i = 0; i < 7; i++) {
			Bomb_ch1[i].LoadBitmap();
		}


		for (int i = 0; i < coins_num; i++) {
			coin_Ani[i].LoadBitmap();
		}

		for (int i = 0; i < 4; i++) {
			AI[i].LoadBitmap();
		}

		playerhead_1.LoadBitmap(IDB_PLAYERHEAD1, RGB(255, 255, 255));
		playerhead_2.LoadBitmap(IDB_PLAYERHEAD2, RGB(255, 255, 255));
		for (int i = 0; i < 8; i++) {
			heart[i].LoadBitmap();
		}

		// 因撰寫關卡內容須測試，故先將時間倒數註解
		count_down.SetInteger(60);
		CAudio::Instance()->Load(AUDIO_BGM2, "sounds\\stage2BGM.mp3");
	}
	void GameStage_2::OnMove() {
		timer++;
		int second = timer / 30;
		int min = second / 60;
		second %= 60;
		//TRACE("second %d\n", second);
		//TRACE("min %d\n", min);

		if (!(timer % 30))
			count_down.Add(-1);

		bool nextState = false;               //下一關 為0就進下一關
		for (int i = 0; i < 4; i++) {
			nextState = nextState | AI[i].Alive();
		}
		if (!nextState) {
			int HealthData[2] = { 6, 0 };
			//game->saveData(HealthData, 2);
			CAudio::Instance()->Stop(AUDIO_BGM2);
			//GotoGameState(GAME_STAGE_2);
		}

		for (int i = 0; i < block2_num; i++) {
			block_2[i].OnMove();
			if (block_2[i].getActive() && !block_2[i].getExp()) {
				mapChange(block_2_pos[i][1], block_2_pos[i][0], 0);
				block_2[i].setActive(false);
				block_2[i].setExp(true);

			}
		}
		BombState();
		character_1.OnMove();

		for (int i = 0; i < 4; i++) {
			AI[i].OnMove(character_1.GetX1(), character_1.GetY1(), 0, 0);
		}
		GetCoins();

		HealthState();
		if (blood_vol == 0) {
		}
	}
	void GameStage_2::OnShow() {                   //越後放的顯示會越上層
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

		for (int i = 0; i < block2_num; i++) {
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
		for (int i = 0; i < 4; i++) {
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
	sprintf(str, "*%d", 3);
	pDC->TextOut((panel.Width() * 59 / 100), panel.Height() * 13 / 100, str);
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
	sprintf(str1, "%06d", 0);
	pDC1->TextOut((panel.Width() * 20 / 100), panel.Height() * 41 / 100, str1);
	pDC1->SelectObject(fp1);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

		playerhead_2.SetTopLeft((panel.Width() * 16 / 100), panel.Height() * 56 / 100);
		playerhead_2.ShowBitmap();
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 8; j++) {
				if (i == 0 && j < 4) {
					heart[j].setTopLeft((panel.Width() * 15 / 100) + 17 * j, panel.Height() * 19 / 100);
					heart[j].OnShow();
				}
				else if (i == 1 && j > 3 && j < 8) {
					heart[j].setTopLeft((panel.Width() * 15 / 100) + 17 * (j - 4), panel.Height() * 225 / 1000);
					heart[j].OnShow();
				}
			}
		}

	}

	void GameStage_2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;  // keyboard左箭頭
		const char KEY_UP = 0x26;    // keyboard上箭頭
		const char KEY_RIGHT = 0x27; // keyboard右箭頭
		const char KEY_DOWN = 0x28;  // keyboard下箭頭
		const char KEY_ESC = 0x1B;
		const char KEY_P = 0x50;
		const char KEY_SPACE = 0x20;


		if (nChar == KEY_ESC || nChar == KEY_P) {
			game_framework::CGame::Instance()->OnFilePause();
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
	}

	void GameStage_2::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	void GameStage_2::setBomb(int id) {
		if (id == 1) {                                                  //[y][x]
			int x = (character_1.GetX1() + character_1.GetX2()) / 2;    //腳色中心點
			int y = (character_1.GetY1() + character_1.GetY2()) / 2;    //腳色中心點
			x = (x - 128) / 32;                                         //轉換成13*15地圖模式
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

	void GameStage_2::mapChange(int x, int y, int value) {
		bg[y][x] = value;
		character_1.LoadMap(bg);
		for (int i = 0; i < 4; i++) {
			AI[i].LoadMap(bg);
		}
	}
	void GameStage_2::BombState() {
		for (int i = 0; i < 7; i++) {
			Bomb_ch1[i].OnMove();
			if (Bomb_ch1[i].getActive() && !Bomb_ch1[i].getExp()) {
				int nx = (Bomb_ch1[i].getTop_Bomb() - 128) / 32;
				int ny = (Bomb_ch1[i].getLeft_Bomb() - 32) / 32;
				if (bg[ny][nx] != 4)mapChange(nx, ny, 4);            // 
			}
			if (Bomb_ch1[i].getActive() && Bomb_ch1[i].getExp()) {  //爆炸中的炸彈位置重設成可行走
				int nx = (Bomb_ch1[i].getTop_Bomb() - 128) / 32;
				int ny = (Bomb_ch1[i].getLeft_Bomb() - 32) / 32;
				mapChange(nx, ny, 5);
				if (!Bomb_ch1[i].getObs())setBombRange(1, i, nx, ny);
				if (Bomb_ch1[i].getAud())CAudio::Instance()->Play(AUDIO_BOMB, false);
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
	void GameStage_2::setBombRange(int id, int i, int x, int y) {
		if (id == 1) {
			int j;
			int range = character_1.GetRange();
			for (j = 1; j <= range; j++) {
				if (y - j < 0 || bg[y - j][x] == 1) {
					break;
				}
				else if (bg[y - j][x] == 2) {
					for (int k = 0; k < block2_num; k++) {
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
					for (int k = 0; k < block2_num; k++) {
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
					for (int k = 0; k < block2_num; k++) {
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
					for (int k = 0; k < block2_num; k++) {
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
	void GameStage_2::GetCoins() {
		//找出腳色所在位置(x,y)
		int x = (character_1.GetX1() + character_1.GetX2()) / 2;    // 腳色中心點
		int y = (character_1.GetY1() + character_1.GetY2()) / 2;    // 腳色中心點
		x = (x - 128) / 32;                                         // 轉換成13*15地圖模式
		y = (y - 32) / 32;
		for (int i = 0; i < coins_num; i++) {
			coin_Ani[i].OnMove();
			if (x == coins_pos[i][1] && y == coins_pos[i][0] && !coin_Ani[i].getActive() && !coin_Ani[i].getExp()) {
				coin_Ani[i].setActive();
				/*吃掉的硬幣+1*/
				sc++;
				TRACE("you get %d coins\n", sc);
			}
		}

	}

	void GameStage_2::HealthState() {
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
	if (!character_1.GetDead()) {
		for (int i = 0; i < 2; i++) {
			int x1 = (AI[i].GetX1() + AI[i].GetX2()) / 2;    // 敵人中心點
			int y1 = (AI[i].GetY1() + AI[i].GetY2()) / 2;    // 敵人中心點
			x1 = (x1 - 128) / 32;							 // 轉換成13*15地圖模式
			y1 = (y1 - 32) / 32;

			if (x == x1 && y == y1 && AI[i].Alive()) {
				blood_vol = blood_vol - 1;
			}
			if (AI[i].BulletHitPlayer() && AI[i].Alive()) {
				blood_vol = blood_vol - 1;
			}
		}
		// player碰到爆炸火花會扣血(不知道為什麼血量明明正常扣減，但是圖片卻沒更正)
		if (taking_Damage) {
			// wait two seconds
			k++;
			if (k >= 60) {
				taking_Damage = false;
				k = 0;
			}
		} else {
			if (bg[y][x] == 5) {
				blood_vol = blood_vol - 7;
				taking_Damage = true;
			}
		}
	}
	double value = std::fmod(blood_ori, blood_vol);
	for (int i = 7; i >= 0; i--) {
		if (heart_num[i] != 0) {
			if (heart_num[i] - value < 0) {
				value -= heart_num[i];
				heart_num[i] = 0;
				blood_ori = blood_vol;
				value = 0;
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
}