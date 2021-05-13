#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <cstdlib> /* 亂數相關函數 */
#include "Enemy.h"

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
	}
	void Enemy::LoadBitmap() {
		Character_down.SetDelayCount(5);
		Character_down.AddBitmap(IDB_Test, RGB(255,255,255));
		//Character_down.AddBitmap(IDB_Test2, RGB(255, 255, 255));
	}
	void Enemy::OnMove() {
		time++;
		if ((x - 128) % 32 == 0 && (y - 32) % 32 == 0) {
			descision = GetPath();
		}
		if (descision == 1) { y -= move_step; Character_down.OnMove(); }
		else if (descision == 2) { y += move_step; Character_down.OnMove(); }
		else if (descision == 3) { x -= move_step; Character_down.OnMove(); }
		else if (descision == 4) { x += move_step; Character_down.OnMove(); }
	}
	void Enemy::OnShow() {
		Character_down.SetTopLeft(x, y);
		if (descision == 1)Character_down.OnShow();
		else if (descision == 2)Character_down.OnShow();
		else if (descision == 3)Character_down.OnShow();
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
		int upRange, downRange, leftRange, rightRange;        // 各方向可移動布數
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
		int Rand = rand() % total;
		TRACE("%d %d\n", time, rand);
		TRACE("%d %d %d %d\n", upRange, downRange, leftRange, rightRange);
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
}