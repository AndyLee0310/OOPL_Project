#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Bomb.h"

namespace game_framework {
	Bomb::Bomb() {
		x = 0;
		y = 0;
		active = false;
		isExp  = false;
	}
	void Bomb::Initialize() {
		x = 0;
		y = 0;
		active = false;
		isExp  = false;
		timer = 0;
		range_up = range_down = range_left = range_right = 0;
		Explosion.Reset();
	}
	void Bomb::LoadBitmap() {
		waiting.AddBitmap(IDB_BOMB_1, RGB(255, 255, 255));
		waiting.AddBitmap(IDB_BOMB_2, RGB(255, 255, 255));
		Explosion.SetDelayCount(5);
		Explosion.AddBitmap(IDB_expC_4, RGB(255, 255, 255));
		Explosion.AddBitmap(IDB_expC_3, RGB(255, 255, 255));
		Explosion.AddBitmap(IDB_expC_2, RGB(255, 255, 255));
		Explosion.AddBitmap(IDB_expC_1, RGB(255, 255, 255));

	}
	void Bomb::OnMove() {
		if (active && !isExp) {
			waiting.OnMove();
		}
		else if (active && isExp) {
			Explosion.OnMove();
		}

	}
	void Bomb::OnShow() {
		if (active && !isExp) {
			timer++;
			waiting.SetTopLeft(x, y);
			waiting.OnShow();
			if (timer == 30 * 3) {            //30FPS * 5¬í
				timer = 0;
				isExp  = true;
			}
		}
		else if (active && isExp) {
			Explosion.SetTopLeft(x, y);
			Explosion.OnShow();
			if (Explosion.IsFinalBitmap()) {
				active = false;
			}
		}
		//else if (active && isExp)active = false;
	}
	void Bomb::setActive(bool act) {
		if (act)isExp = 0;
		active = act;
	}
	void Bomb::setUp(int up) {
		range_up = up;
	}
	void Bomb::setDown(int dw) {
		range_down = dw;
	}
	void Bomb::setRight(int rt) {
		range_right = rt;
	}
	void Bomb::setLeft(int le) {
		range_left = le;
	}
	void Bomb::setRange(int up, int dw, int le, int rt) {
		range_up = up;
		range_down = dw;
		range_left = le;
		range_right = rt;
	}
	int  Bomb::getUp() {
		return range_up;
	}
	int  Bomb::getDown() {
		return range_down;
	}
	int  Bomb::getRight() {
		return range_right;
	}
	int  Bomb::getLeft() {
		return range_left;
	}
	bool  Bomb::getActive() {
		return active;
	}
	bool  Bomb::getExp() {
		return isExp;
	}
	int  Bomb::getTop_Bomb() {
		return x;
	}
	int  Bomb::getLeft_Bomb() {
		return y;
	}
	void Bomb::setTopleft(int nx, int ny) {
		x = nx;
		y = ny;
	}
}