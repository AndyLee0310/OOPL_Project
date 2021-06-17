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
		Obs_break = false;
		timer = 0;
		Obs_break=range_up = range_down = range_left = range_right = 0;
		Explosion.Reset();
		Exp_H.Reset();
		Exp_V.Reset();
	}
	void Bomb::LoadBitmap() {
		waiting.AddBitmap(IDB_BOMB_1, RGB(255, 255, 255));
		waiting.AddBitmap(IDB_BOMB_2, RGB(255, 255, 255));
		Explosion.SetDelayCount(5);
		Explosion.AddBitmap(IDB_expC_4, RGB(255, 255, 255));
		Explosion.AddBitmap(IDB_expC_3, RGB(255, 255, 255));
		Explosion.AddBitmap(IDB_expC_2, RGB(255, 255, 255));
		Explosion.AddBitmap(IDB_expC_1, RGB(255, 255, 255));
		Exp_H.SetDelayCount(5);
		Exp_H.AddBitmap(IDB_expH_4, RGB(255, 255, 255));
		Exp_H.AddBitmap(IDB_expH_3, RGB(255, 255, 255));
		Exp_H.AddBitmap(IDB_expH_2, RGB(255, 255, 255));
		Exp_H.AddBitmap(IDB_expH_1, RGB(255, 255, 255));
		Exp_V.SetDelayCount(5);
		Exp_V.AddBitmap(IDB_expV_4, RGB(255, 255, 255));
		Exp_V.AddBitmap(IDB_expV_3, RGB(255, 255, 255));
		Exp_V.AddBitmap(IDB_expV_2, RGB(255, 255, 255));
		Exp_V.AddBitmap(IDB_expV_1, RGB(255, 255, 255));

	}
	void Bomb::OnMove() {
		if (active && !isExp) {
			waiting.OnMove();
		}
		else if (active && isExp) {
			Explosion.OnMove();
			Exp_H.OnMove();
			Exp_V.OnMove();
		}

	}
	void Bomb::OnShow() {
		if (active && !isExp) {
			timer++;
			waiting.SetTopLeft(x, y);
			waiting.OnShow();
			if (timer == 30 * 2) {            //30FPS * 2¬í
				timer = 0;
				isExp  = true;
			}
		}
		else if (active && isExp && Obs_break) {
			Explosion.SetTopLeft(x, y);
			Explosion.OnShow();
			for (int i = 1; i <= range_left; i++) {
				Exp_H.SetTopLeft(x - 32 * i, y);
				Exp_H.OnShow();
			}

			for (int i = 1; i <= range_right; i++) {
				Exp_H.SetTopLeft(x + 32 * i, y);
				Exp_H.OnShow();
			}

			for (int i = 1; i <= range_up; i++) {
				Exp_V.SetTopLeft(x, y - 32 * i);
				Exp_V.OnShow();
			}

			for (int i = 1; i <= range_down; i++) {
				Exp_V.SetTopLeft(x, y + 32 * i);
				Exp_V.OnShow();
			}

			if (Explosion.IsFinalBitmap()) {
				active = false;
			}
		}
	}
	void Bomb::setActive(bool act) {
		if (act)isExp = 0;
		active = act;
	}
	void Bomb::setUp(int up) {
		range_up = up;
		if (range_up < 0)range_up = 0;
	}
	void Bomb::setDown(int dw) {
		range_down = dw;
		if (range_down < 0)range_down = 0;
	}
	void Bomb::setRight(int rt) {
		range_right = rt;
		if (range_right < 0)range_right = 0;
	}
	void Bomb::setLeft(int le) {
		range_left = le;
		if (range_left < 0)range_left = 0;
	}
	void Bomb::setRange(int up, int dw, int le, int rt) {
		range_up = up;
		range_down = dw;
		range_left = le;
		range_right = rt;
	}

	void Bomb::setObs(bool o) {
		Obs_break = o;
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
	bool Bomb::getObs() {
		return Obs_break;
	}
}