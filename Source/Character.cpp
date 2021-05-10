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
		isPlaceable = true;
		isMovingDown = isMovingLeft = isMovingRight = isMovingUp = false;
		Explosion_range = 5;
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
		return y + Character_down.Width();
	}
	int Character::GetStep() {
		return move_step;
	}
	int Character::GetRange() {
		return Explosion_range;
	}
	bool Character::GetPlaceable() {
		return isPlaceable;
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
			if (isMoveable(x1, y2 + move_step) && isMoveable(x2, y2 + move_step)) { //邊界判定
				y += move_step;
			}
			else if (isMoveable(x1, y2 + move_step - 2) && isMoveable(x2, y2 + move_step - 2)) {
				y += 2;
			}
			else if (GetPosition(x1, y2) == 4 && GetPosition(x1, y2 + move_step) == 4 && y2 / 32 == (y2 + move_step) / 32) {         //在剛設下的32*32炸彈空間內可以正常移動 但離開後不能重新回來
				y += move_step;
			}
			else if (GetPosition(x1, y2) == 4 && GetPosition(x1, y2 + move_step - 2) == 4 && y2 / 32 == (y2 + move_step - 2) / 32) {
				y += 2;
			}
				
		}
		if (isMovingUp) {
			Animate_State = 2;
			Character_up.OnMove();
			int x1 = x - 128;
			int x2 = x + Character_up.Width() - 128 - 1;
			int y1 = y - 32;
			if (isMoveable(x1, y1 - move_step) && isMoveable(x2, y1 - move_step))
				y -= move_step;
			else if (isMoveable(x1, y1 - move_step + 2) && isMoveable(x2, y1 - move_step + 2))                //邊界判定
				y -= 2;
			else if (GetPosition(x1, y1) == 4 && GetPosition(x1, y1 - move_step) == 4 && y1 / 32 == (y1 - move_step) / 32)
				y -= move_step;
			else if (GetPosition(x1, y1) == 4 && GetPosition(x1, y1 - move_step + 2) == 4 && y1 / 32 == (y1 - move_step + 2) / 32)
				y -= 2;
			
		}
		if (isMovingLeft) {
			Animate_State = 3;
			Character_left.OnMove();
			int x1 = x - 128;
			int y1 = y - 32;
			int y2 = y + Character_left.Height() - 32 - 1;
			if (isMoveable(x1 - move_step, y1) && isMoveable(x1 - move_step, y2))
				x -= move_step;
			else if (isMoveable(x1 - move_step + 2, y1) && isMoveable(x1 - move_step + 2, y2))
				x -= 2;
			else if (GetPosition(x1, y1) == 4 && GetPosition(x1 - move_step, y1) == 4 && x1 / 32 == (x1 - move_step) / 32)
				x -= move_step;
			else if (GetPosition(x1, y1) == 4 && GetPosition(x1 - move_step + 2, y1) == 4 && x1 / 32 == (x1 - move_step + 2) / 32)
				x -= 2;
			
		}
		if (isMovingRight) {
			Animate_State = 4;
			Character_right.OnMove();
			int x2 = x + Character_right.Width() - 128 - 1;
			int y1 = y - 32;
			int y2 = y + Character_right.Height() - 32 - 1;
			if (isMoveable(x2 + move_step, y1) && isMoveable(x2 + move_step, y2))
				x += move_step;
			else if (isMoveable(x2 + move_step - 2, y1) && isMoveable(x2 + move_step - 2, y2))
				x += 2;
			else if (GetPosition(x2, y1) == 4 && GetPosition(x2 + move_step, y1) == 4 && x2 / 32 == (x2 + move_step) / 32)
				x += move_step;
			else if (GetPosition(x2, y1) == 4 && GetPosition(x2 + move_step - 2, y1) == 4 && x2 / 32 == (x2 + move_step - 2) / 32)
				x += 2;
			
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

	void Character::SetRange(int i) {
		Explosion_range = i;
	}
	void Character::setPlaceable(bool p) {
		isPlaceable = p;
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
		for (int i = 0; i < 13; i++) {
			for (int j = 0; j < 15; j++) {
				map_simple[i][j] = maps[i][j];
			}
		}
	}
	int Character::GetPosition(int px, int py) {
		if (px < 0 || px > 480 || py < 0 || py > 416)
			return 2;
		return map[py][px];
	}

	bool Character::isMoveable(int x, int y) {
		if (x < 0 || y < 0 || x > 480 || y > 416)return false;
		if (map[y][x] == 1)return false;
		if (map[y][x] == 2)return false;
		if (map[y][x] == 4)return false;
		return true;
	}
}