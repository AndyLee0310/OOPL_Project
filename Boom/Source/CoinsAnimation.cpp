#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CoinsAnimation.h"

namespace game_framework {

	void CoinsAnimation::Initialize() {
		glod.Reset();
		Active = exp = false;
		x = y = 0;
	}
	void CoinsAnimation::Initialize(int nx, int ny) {
		glod.Reset();
		Active = exp = false;
		x = nx;
		y = ny;
	}
	void CoinsAnimation::LoadBitmap() {
		glod.SetDelayCount(5);
		glod.AddBitmap(IDB_COIN_0, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_1, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_2, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_3, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_4, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_5, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_6, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_7, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_8, RGB(255, 255, 255));
		glod.AddBitmap(IDB_COIN_9, RGB(255, 255, 255));
	}
	void CoinsAnimation::OnMove() {
		if (Active && exp) {
			glod.OnMove();
			if (glod.IsFinalBitmap()) {
				exp = false;
				//Active = false;
			}
		}
	}
	void CoinsAnimation::OnShow() {
		if ((Active && exp) || (!Active && !exp)) {
			glod.SetTopLeft(x, y);
			glod.OnShow();
		}
	}
	void CoinsAnimation::setActive() {
		Active = true;
		exp = true;
	}
	/*
	void CoinsAnimation::setExp(bool e) {
		exp = e;
	}
	*/
	void CoinsAnimation::setTopLeft(int nx, int ny) {
		x = nx;
		y = ny;
	}
	bool CoinsAnimation::getActive() {
		return Active;
	}
	bool CoinsAnimation::getExp() {
		return exp;
	}
}