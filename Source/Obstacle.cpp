#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Obstacle.h"
namespace game_framework {
	void Obstacle::Initialize() {
		block.Reset();
		Active = exp = false;
		x = y = 0;
	}
	void Obstacle::Initialize(int nx, int ny) {
		block.Reset();
		Active = exp = false;
		x = nx;
		y = ny;
	}
	void Obstacle::LoadBitmap() {
		block.SetDelayCount(5);
		block.AddBitmap(IDB_BREAK_0, RGB(255, 255, 255));
		block.AddBitmap(IDB_BREAK_1, RGB(255, 255, 255));
		block.AddBitmap(IDB_BREAK_2, RGB(255, 255, 255));
		block.AddBitmap(IDB_BREAK_3, RGB(255, 255, 255));
	}
	void Obstacle::OnMove() {
		if (Active && exp) {
			block.OnMove();
			if (block.IsFinalBitmap()) {
				exp = false;
			}
		}
	}
	void Obstacle::OnShow() {
		if ((Active && exp) || (!Active && !exp)) {
			block.SetTopLeft(x, y);
			block.OnShow();
		}
	}
	void Obstacle::setActive() {
		Active = true;
		exp = true;
	}
	/*
	void Obstacle::setExp(bool e) {
		exp = e;
	}
	*/
	void Obstacle::setTopLeft(int nx, int ny) {
		x = nx;
		y = ny;
	}
	bool Obstacle::getActive() {
		return Active;
	}
	bool Obstacle::getExp() {
		return exp;
	}
}