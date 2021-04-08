#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Character.h"

namespace game_framework {
	Character::Character() {
		Initialize(0, 0);
	}
	void Character::Initialize(const int nx, const int ny) {
		x = nx;
		y = ny;
		isMovingDown = isMovingLeft = isMovingRight = isMovingUp = false;
	}
	int Character::GetX1() {
		return x;
	}
	int Character::GetY1() {
		return y;
	}
	int Character::GetX2() {
		return x + animation.Width();
	}
	int Character::GetY2() {
		return y + animation.Height();
	}
	int Character::GetStep() {
		return move_step;
	}
	void Character::LoadBitmap() {
		animation.SetDelayCount(5);
		animation.AddBitmap(IDB_Test,  RGB(255, 255, 255));   //測試用 待修正
		animation.AddBitmap(IDB_Test1, RGB(255, 255, 255));
	}
	void Character::OnMove() {
		/*
		animation.OnMove();
		if (isMovingLeft)
			x -= move_step;
		if (isMovingRight)
			x += move_step;
		if (isMovingUp)
			y -= move_step;
		if (isMovingDown)
			y += move_step;
		*/
		if (isMovingDown) {
			animation.OnMove();
			int x1 = x - 128;
			int x2 = x + animation.Width() - 128 - 1;
			int y2 = y + animation.Height() - 32 - 1;
			if (GetPosition(x1, y2 + move_step) == 0 && GetPosition(x2, y2 + move_step) == 0) //邊界判定
				y += move_step;
		}
		if (isMovingUp) {
			animation.OnMove();
			int x1 = x - 128;
			int x2 = x + animation.Width() - 128 - 1;
			int y1 = y - 32;
			if (GetPosition(x1, y1 - move_step) == 0 && GetPosition(x2, y1 - move_step) == 0)                //邊界判定
				y -= move_step;
		}
		if (isMovingLeft) {
			animation.OnMove();
			int x1 = x - 128;
			int y1 = y - 32;
			int y2 = y + animation.Height() - 32 - 1;
			if (GetPosition(x1 - move_step, y1) == 0 && GetPosition(x1 - move_step, y2) == 0)
				x -= move_step;
		}
		if (isMovingRight) {
			animation.OnMove();
			int x2 = x + animation.Width() - 128 - 1;
			int y1 = y - 32;
			int y2 = y + animation.Height() - 32 - 1;
			if (GetPosition(x2 + move_step, y1) == 0 && GetPosition(x2 + move_step, y2) == 0)
				x += move_step;
		}
	}
	void Character::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void Character::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void Character::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void Character::SetMovingUp(bool flag)
	{
		isMovingUp = flag;
	}

	void Character::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}

	void Character::OnShow()
	{
		animation.SetTopLeft(x, y);
		animation.OnShow();
	}
	void Character::LoadMap(int maps[13][15]) {
		for (int i = 0; i < 416; i++) {
			for (int j = 0; j < 480; j++) {
				map[i][j] = maps[i / 32][j / 32];
			}
		}
	}
	int Character::GetPosition(int px, int py) {
		if (px < 0 || px > 480 || py < 0 || py > 416)
			return 2;
		return map[py][px];
	}
}