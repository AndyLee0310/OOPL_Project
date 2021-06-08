#ifndef ENEMY
#define ENEMY
#include "Bullet.h"
namespace game_framework {
	class Enemy {
	private:
		CAnimation Character_down;	// �}��V�U���ʵe
		CAnimation Character_up;
		CAnimation Character_left;
		CAnimation Character_right;
		int  Animate_State;         // �}�Ⲿ�ʪ��A 1���U 2���W 3���� 4���k
		int  x, y;					// �}�⥪�W���y��
		int  move_step = 2;         // �}��B��
		int  upRange, downRange, leftRange, rightRange;        // �U��V�i���ʥ���
		int  descision;             // 1�W 2�U 3�� 4�k
		int  time;                  // FOR random
		int  bg[13][15];
		int  GetPath();
		// int health;
		Bullet b;
		bool   BulletHit;           // �l�u���쪱�a
	public:
		Enemy();
		int  GetX1();					// �}�⥪�W�� x �y��
		int  GetY1();					// �}�⥪�W�� y �y��
		int  GetX2();					// �}��k�U�� x �y��
		int  GetY2();					// �}��k�U�� y �y��
		void Initialize(int, int);      // �]�w�}�⬰��l�� �藍�P�}��]�w��l��m
		void LoadBitmap();				// ���J�ϧ�
		void OnMove(int, int, int, int);// ���ʸ}��
		void OnShow();					// �N�}��ϧζK��e��
		void LoadMap(int maps[13][15]);
		int  GetPosition(int, int);
		void Attack(int nx, int ny);
		int  BulletPosX();
		int  BulletPosY();
		void BulletTouch(int, int);
		bool BulletHitPlayer();              // �����P�_ �I�쪱�a��^true��mygame
	};
}

#endif