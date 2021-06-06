#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <cstdlib> /* 亂數相關函數 */
#include "Enemy.h"
#include "Bullet.h"

namespace game_framework {
	Enemy::Enemy() {
		Initialize(0, 0);
	}
	void Enemy::Initialize(int nx, int ny) {
		x = nx;
		y = ny;
		time = 0;
		Animate_State = 1;
		descision = 0;
		upRange = downRange = leftRange = rightRange = 0;
	}
	void Enemy::LoadBitmap() {

		Character_down.SetDelayCount(5);
		Character_down.AddBitmap(IDB_EM_DW_1, RGB(255, 255, 255));
		Character_down.AddBitmap(IDB_EM_DW_2, RGB(255, 255, 255));
		Character_down.AddBitmap(IDB_EM_DW_3, RGB(255, 255, 255));
		Character_down.AddBitmap(IDB_EM_DW_4, RGB(255, 255, 255));

		Character_up.SetDelayCount(5);
		Character_up.AddBitmap(IDB_EM_UP_1, RGB(255, 255, 255));
		Character_up.AddBitmap(IDB_EM_UP_2, RGB(255, 255, 255));
		Character_up.AddBitmap(IDB_EM_UP_3, RGB(255, 255, 255));
		Character_up.AddBitmap(IDB_EM_UP_4, RGB(255, 255, 255));

		Character_left.SetDelayCount(5);
		Character_left.AddBitmap(IDB_EM_LE_1, RGB(255, 255, 255));
		Character_left.AddBitmap(IDB_EM_LE_2, RGB(255, 255, 255));
		Character_left.AddBitmap(IDB_EM_LE_3, RGB(255, 255, 255));
		Character_left.AddBitmap(IDB_EM_LE_4, RGB(255, 255, 255));

		Character_right.SetDelayCount(5);
		Character_right.AddBitmap(IDB_EM_RE_1, RGB(255, 255, 255));
		Character_right.AddBitmap(IDB_EM_RE_2, RGB(255, 255, 255));
		Character_right.AddBitmap(IDB_EM_RE_3, RGB(255, 255, 255));
		Character_right.AddBitmap(IDB_EM_RE_4, RGB(255, 255, 255));

	}
	void Enemy::OnMove() {
		time++;
		if ((x - 128) % 32 == 0 && (y - 32) % 32 == 0) {
			descision = GetPath();
			//get Ax & Bx
			Attack();
		}
		if (descision == 1) { 
			y -= move_step;
			Character_up.OnMove();

		}
		else if (descision == 2) { 
			y += move_step;
			Character_down.OnMove();

		}
		else if (descision == 3) { 
			x -= move_step;
			Character_left.OnMove();
		}
		else if (descision == 4) { 
			x += move_step;
			Character_right.OnMove();
		}
		b.OnMove();
		BulletTouch();
	}
	void Enemy::OnShow() {
		Character_down.SetTopLeft(x, y);
		Character_up.SetTopLeft(x, y);
		Character_left.SetTopLeft(x, y);
		Character_right.SetTopLeft(x, y);
		if (descision == 1)Character_up.OnShow();
		else if (descision == 4)Character_right.OnShow();
		else if (descision == 3)Character_left.OnShow();
		else Character_down.OnShow();
	}
	void Enemy::LoadMap(int maps[13][15]) {
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 15; j++) {
				bg[i][j] = maps[i][j];
			}
		}
	}
	int  Enemy::GetPosition(int nx, int ny) {
		return bg[ny][nx];
	}
	int  Enemy::GetPath() {
		int nx = (x - 128) / 32;
		int ny = (y - 32) / 32;
		for (int i = 1;; i++) {
			if (ny - i < 0 || bg[ny - i][nx] == 1 || bg[ny - i][nx] == 2 || bg[ny - i][nx] == 4) {
				upRange = --i;
				break;
			}
		}
		for (int i = 1;; i++) {
			if (ny + i > 12 || bg[ny + i][nx] == 1 || bg[ny + i][nx] == 2 || bg[ny + i][nx] == 4) {
				downRange = --i;
				break;
			}
		}
		for (int i = 1;; i++) {
			if (nx - i < 0 || bg[ny][nx - i] == 1 || bg[ny][nx - i] == 2 || bg[ny][nx - i] == 4) {
				leftRange = --i;
				break;
			}
		}
		for (int i = 1;; i++) {
			if (nx + i > 14 || bg[ny][nx + i] == 1 || bg[ny][nx + i] == 2 || bg[ny][nx + i] == 4) {
				rightRange = --i;
				break;
			}
		}

		srand(time);
		int total = upRange + downRange + leftRange + rightRange;
		if (total == 0)return 0;
		int Rand = rand() % total;
		//TRACE("%d %d %d %d %d %d\n", x, y, upRange, downRange, leftRange, rightRange);
		TRACE("%d \n",rand());
		if (Rand < upRange && upRange != 0) {        //rand剛好整除且upRange又為0 AI不能往上 *只有向上才會有這種情況
			return 1;                   // 向上
		}
		else if (Rand < upRange + downRange) {
			return 2;                   // 向下
		}
		else if (Rand < upRange + downRange + leftRange) {
			return 3;                   // 向左
		}
		else return 4;                  // 向右
	}
	int Enemy::GetX1() {
		return x;
	}
	int Enemy::GetY1() {
		return y;
	}
	int Enemy::GetX2() {
		return x + Character_down.Width();
	}
	int Enemy::GetY2() {
		return y + Character_down.Width();
	}
	void Enemy::Attack() {
		if (Ax >= x && Ax <= x + 32) {            // 上下判斷
			if (Ay <= y && Ay >= y - upRange * 32) {
				b.setPath(x + 16,y,1);
			}
			else if (Ay >= y && Ay <= y + downRange * 32) {
				b.setPath(x + 16, y + 32, 2);
			}
		}
		else if (Ay >= y && Ay <= y + 32) {      // 左右判斷
			if (Ax <= x && Ax >= x - leftRange * 32) {
				b.setPath(x, y + 16,3);
			}
			else if (Ax >= x && Ax <= x + rightRange * 32) {
				b.setPath(x + 32, y + 16,4);
			}
		}
		else if (Bx >= x && Bx <= x + 32) {       // 上下判斷
			if (By <= y && By >= y - upRange * 32) {
				b.setPath(x + 16, y, 1);
			}
			else if (By >= y && By <= y + downRange * 32) {
				b.setPath(x + 16, y + 32, 2);
			}
		}
		else if (By >= y && By <= y + 32) {      // 左右判斷
			if (Bx <= x && Bx >= x - leftRange * 32) {
				b.setPath(x, y + 16, 3);
			}
			else if (Bx >= x && Bx <= x + rightRange * 32) {
				b.setPath(x + 32, y + 16, 4);
			}
		}
	}
	void Enemy::BulletTouch() {

	}
}