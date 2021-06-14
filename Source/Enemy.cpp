

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
		upRange = downRange = leftRange = rightRange = 0;
		DeathAnimateCount = 0;
		isAlive = true;
		BulletHit = false;
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

		Character_death.SetDelayCount(1);
		Character_death.AddBitmap(IDB_EM_DE_1, RGB(255, 255, 255));
		Character_death.AddBitmap(IDB_EM_DE_2, RGB(255, 255, 255));
		b.LoadBitmap();
	}
	void Enemy::OnMove(int ax, int ay, int bx, int by) {
		if (isAlive) {
			time++;

			if (bg[(y - 32) / 32][(x - 128) / 32] == 5) {
				isAlive = false;
				time = -1;
			}

			if ((x - 128) % 32 == 0 && (y - 32) % 32 == 0) {
				descision = GetPath();
				if (!b.getActive())Attack(ax, ay);
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
			if (b.getActive())BulletTouch(ax, ay);
			b.OnMove();
		}
		else if (time == -1) {
			Character_death.OnMove();
		}
	}
	void Enemy::OnShow() {
		if (isAlive) {
			Character_down.SetTopLeft(x, y);
			Character_up.SetTopLeft(x, y);
			Character_left.SetTopLeft(x, y);
			Character_right.SetTopLeft(x, y);
			if (descision == 1)Character_up.OnShow();
			else if (descision == 4)Character_right.OnShow();
			else if (descision == 3)Character_left.OnShow();
			else Character_down.OnShow();
			b.OnShow();
		}
		else if (time == -1) {
			Character_death.SetTopLeft(x, y);
			Character_death.OnShow();
			if (Character_death.IsFinalBitmap()) {
				DeathAnimateCount++;
			}
			if (DeathAnimateCount == 3) {
				time = -2;
			}
		}
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
			if (ny + i >= 12 || bg[ny + i][nx] == 1 || bg[ny + i][nx] == 2 || bg[ny + i][nx] == 4) {
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
			if (nx + i >= 14 || bg[ny][nx + i] == 1 || bg[ny][nx + i] == 2 || bg[ny][nx + i] == 4) {
				rightRange = --i;
				break;
			}
		}

		srand(time);
		int total = upRange + downRange + leftRange + rightRange;
		if (total == 0)return 0;
		int Rand = rand() % total;
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
	void Enemy::Attack(int nx, int ny) {
		nx = nx + 16;                                                   // 腳色中心點
		ny = ny + 16;
		if (nx >= x && nx <= x + 32) {                                  // 上下判斷 先判斷x軸是否相同
			if (ny <= y && ny >= y - upRange * 32) {                    // 用ENEMY的左上做判斷
				b.setPath(x + 16, y, 1);
			}
			else if (ny >= y + 32 && ny <= y + downRange * 32 + 32) {   // 用ENEMY的右下做判斷，不然腳色在最下那格會判斷不到
				b.setPath(x + 16, y + 32, 2);
			}
		}
		else if (ny >= y && ny <= y + 32) {                             // 左右判斷
			if (nx <= x && nx >= x - leftRange * 32) {
				b.setPath(x, y + 16, 3);
			}
			else if (nx >= x + 32 && nx <= x + rightRange * 32 + 32) {  // 用ENEMY的右下做判斷，不然腳色在最右那格會判斷不到
				b.setPath(x + 32, y + 16, 4);
			}
		}
	}
	void Enemy::BulletTouch(int cx, int cy) {
		int bx = b.getX();
		int by = b.getY();
		int dir = b.getDir();
		if (!b.isTouched()) {
			if (bx >= cx && bx <= cx + 32 && by >= cy && by <= cy + 32) {    // 子彈打到玩家
				BulletHit = true;
				if (dir == 1) b.isTouched(bx, cy + 32);
				else if (dir == 2) b.isTouched(bx, cy);
				else if (dir == 3) b.isTouched(cx + 32, by);
				else if (dir == 4) b.isTouched(cx, by);
			}
			else if (bx <= 128 || bx >= 128 + 32 * 15) {
				int nx = 128 + 480 * (dir - 3);                              // 向左(dir = 3)超出邊界在x = 128爆裂 向右(dir = 4)設在x = 128 + 480
				b.isTouched(nx, by);
			}
			else if (by <= 32 || by >= 32 + 32 * 13) {
				int ny = 32 + 416 * (dir - 1);                               // 同上
				b.isTouched(bx, ny);
			}
			else if (bg[(by - 32) / 32][(bx - 128) / 32] == 1 || bg[(by - 32) / 32][(bx - 128) / 32] == 2) {  // 子彈打到牆壁或障礙
				int nx = (bx - 128) / 32;
				int ny = (by - 32) / 32;
				if (dir == 1) b.isTouched(bx, (ny + 1) * 32 + 31);
				else if (dir == 2) b.isTouched(bx, ny * 32 + 32);
				else if (dir == 3) b.isTouched((nx + 1) * 32 + 127, by);
				else if (dir == 4) b.isTouched(nx * 32 + 128, by);
			}
		}
		else {
			BulletHit = false;
		}
	}
	bool Enemy::BulletHitPlayer() {
		return BulletHit;
	}
	bool Enemy::Alive() {
		return isAlive;
	}
}
