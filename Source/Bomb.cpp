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
	void Bomb::Initialize(int nx, int ny) {
		x = nx;
		y = ny;
		active = true;
		isExp =  false;
		timer = 0;
	}
	void Bomb::LoadBitmap() {
		waiting.AddBitmap(IDB_BOMB_1, RGB(255, 255, 255));
		waiting.AddBitmap(IDB_BOMB_2, RGB(255, 255, 255));

	}
	void Bomb::OnMove() {
		waiting.OnMove();
	}
	void Bomb::OnShow() {
		waiting.SetTopLeft(x, y);
		if (timer == 30 * 3 && !isExp) {            //30FPS * 5¬í
			timer = 0;
			isExp = true;
		}
		if (active && !isExp) {
			timer++;
			waiting.OnShow();
		}
		else if (active && isExp)active = false;
	}
	void Bomb::setActive(bool act) {
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
	void Bomb::setTopleft(int nx, int ny) {
		x = nx;
		y = ny;
	}
}