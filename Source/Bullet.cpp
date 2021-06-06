#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Bullet.h"
namespace game_framework {
	Bullet::Bullet() {
		Initialize(0, 0);
	}
	void Bullet::Initialize(int nx, int ny) {
		x = nx;
		y = ny;
		direction = 0;
		active = touched =false;
	}
	void Bullet::LoadBitmap() {
		bullet.SetDelayCount(5);
		bullet.AddBitmap(IDB_BULLET_1, RGB(255, 255, 255));
	}
	void Bullet::OnMove() {
		if (!touched && direction == 1) {
			y -= speed;
		}
		else if (!touched && direction == 2) {
			y += speed;
		}
		else if (!touched && direction == 3) {
			x -= speed;
		}
		else if (!touched && direction == 4) {
			x += speed;
		}
		else if (touched) {
			bullet.OnMove();
			if (bullet.IsFinalBitmap()) {
				active = false;
			}
		}
	}
	void Bullet::OnShow() {
		if (active) {
			bullet.SetTopLeft(x - 3, y - 3);            //中心點轉換成左上座標
			bullet.OnShow();
		}
	}
	void Bullet::setPath(int nx, int ny, int dir) {
		x = nx;
		y = ny;
		direction = dir;
		active = true;
	}
	void Bullet::setActive(bool a) {
		active = a;
	}
	bool Bullet::getActive() {
		return active;
	}
	int  Bullet::getX() {
		return x + 3;
	}
	int  Bullet::getY() {
		return y + 3;
	}
	void Bullet::isTouched(int nx, int ny) {
		x = nx;
		y = ny;
		touched = true;
	}
}