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
		Animate_State = 1;
		isMovingDown = isMovingLeft = isMovingRight = isMovingUp = false;
	}
	int Character::GetX1() {
		return x;
	}
	int Character::GetY1() {
		return y;
	}
	int Character::GetX2() {
		return x + Character_down.Width();
	}
	int Character::GetY2() {
		return y + Character_down.Height();
	}
	int Character::GetStep() {
		return move_step;
	}
	void Character::LoadBitmap() {
		Character_down.SetDelayCount(5);
		Character_down.AddBitmap(IDB_PLAYER1_DW_1, RGB(255, 255, 255));   
		Character_down.AddBitmap(IDB_PLAYER1_DW_2, RGB(255, 255, 255));
		Character_up.SetDelayCount(5);
		Character_up.AddBitmap(IDB_PLAYER1_UP_1, RGB(255, 255, 255));
		Character_up.AddBitmap(IDB_PLAYER1_UP_2, RGB(255, 255, 255));
		Character_left.SetDelayCount(5);
		Character_left.AddBitmap(IDB_PLAYER1_LE_1, RGB(255, 255, 255));
		Character_left.AddBitmap(IDB_PLAYER1_LE_2, RGB(255, 255, 255));
		Character_right.SetDelayCount(5);
		Character_right.AddBitmap(IDB_PLAYER1_RE_1, RGB(255, 255, 255));
		Character_right.AddBitmap(IDB_PLAYER1_RE_2, RGB(255, 255, 255));
	}
	void Character::OnMove() {
		if (isMovingDown) {
			Animate_State = 1;
			Character_down.OnMove();
			int x1 = x - 128;
			int x2 = x + Character_down.Width() - 128 - 1;
			int y2 = y + Character_down.Height() - 32 - 1;
			if (GetPosition(x1, y2 + move_step) == 0 && GetPosition(x2, y2 + move_step) == 0) //邊界判定
				y += move_step;
		}
		if (isMovingUp) {
			Animate_State = 2;
			Character_up.OnMove();
			int x1 = x - 128;
			int x2 = x + Character_up.Width() - 128 - 1;
			int y1 = y - 32;
			if (GetPosition(x1, y1 - move_step) == 0 && GetPosition(x2, y1 - move_step) == 0)                //邊界判定
				y -= move_step;
		}
		if (isMovingLeft) {
			Animate_State = 3;
			Character_left.OnMove();
			int x1 = x - 128;
			int y1 = y - 32;
			int y2 = y + Character_left.Height() - 32 - 1;
			if (GetPosition(x1 - move_step, y1) == 0 && GetPosition(x1 - move_step, y2) == 0)
				x -= move_step;
		}
		if (isMovingRight) {
			Animate_State = 4;
			Character_right.OnMove();
			int x2 = x + Character_right.Width() - 128 - 1;
			int y1 = y - 32;
			int y2 = y + Character_right.Height() - 32 - 1;
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
		Character_down.SetTopLeft(x, y);
		Character_up.SetTopLeft(x, y);
		Character_left.SetTopLeft(x, y);
		Character_right.SetTopLeft(x, y);
		if (Animate_State == 1)     Character_down.OnShow();
		else if (Animate_State == 2)Character_up.OnShow();
		else if (Animate_State == 3)Character_left.OnShow();
		else if (Animate_State == 4)Character_right.OnShow();
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