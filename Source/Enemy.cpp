

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <cstdlib> /* �üƬ������ */
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
		if (Rand < upRange && upRange != 0) {        //rand��n�㰣�BupRange�S��0 AI���੹�W *�u���V�W�~�|���o�ر��p
			return 1;                   // �V�W
		}
		else if (Rand < upRange + downRange) {
			return 2;                   // �V�U
		}
		else if (Rand < upRange + downRange + leftRange) {
			return 3;                   // �V��
		}
		else return 4;                  // �V�k
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
		nx = nx + 16;                                                   // �}�⤤���I
		ny = ny + 16;
		if (nx >= x && nx <= x + 32) {                                  // �W�U�P�_ ���P�_x�b�O�_�ۦP
			if (ny <= y && ny >= y - upRange * 32) {                    // ��ENEMY�����W���P�_
				b.setPath(x + 16, y, 1);
			}
			else if (ny >= y + 32 && ny <= y + downRange * 32 + 32) {   // ��ENEMY���k�U���P�_�A���M�}��b�̤U����|�P�_����
				b.setPath(x + 16, y + 32, 2);
			}
		}
		else if (ny >= y && ny <= y + 32) {                             // ���k�P�_
			if (nx <= x && nx >= x - leftRange * 32) {
				b.setPath(x, y + 16, 3);
			}
			else if (nx >= x + 32 && nx <= x + rightRange * 32 + 32) {  // ��ENEMY���k�U���P�_�A���M�}��b�̥k����|�P�_����
				b.setPath(x + 32, y + 16, 4);
			}
		}
	}
	void Enemy::BulletTouch(int cx, int cy) {
		int bx = b.getX();
		int by = b.getY();
		int dir = b.getDir();
		if (!b.isTouched()) {
			if (bx >= cx && bx <= cx + 32 && by >= cy && by <= cy + 32) {    // �l�u���쪱�a
				BulletHit = true;
				if (dir == 1) b.isTouched(bx, cy + 32);
				else if (dir == 2) b.isTouched(bx, cy);
				else if (dir == 3) b.isTouched(cx + 32, by);
				else if (dir == 4) b.isTouched(cx, by);
			}
			else if (bx <= 128 || bx >= 128 + 32 * 15) {
				int nx = 128 + 480 * (dir - 3);                              // �V��(dir = 3)�W�X��ɦbx = 128�z�� �V�k(dir = 4)�]�bx = 128 + 480
				b.isTouched(nx, by);
			}
			else if (by <= 32 || by >= 32 + 32 * 13) {
				int ny = 32 + 416 * (dir - 1);                               // �P�W
				b.isTouched(bx, ny);
			}
			else if (bg[(by - 32) / 32][(bx - 128) / 32] == 1 || bg[(by - 32) / 32][(bx - 128) / 32] == 2) {  // �l�u��������λ�ê
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
