#ifndef ENEMY
#define ENEMY
#include "Bullet.h"
namespace game_framework {
	class Enemy {
	private:
		CAnimation Character_down;	// 腳色向下的動畫
		CAnimation Character_up;
		CAnimation Character_left;
		CAnimation Character_right;
		int  Animate_State;         // 腳色移動狀態 1為下 2為上 3為左 4為右
		int  x, y;					// 腳色左上角座標
		int  move_step = 2;         // 腳色步數
		int  upRange, downRange, leftRange, rightRange;        // 各方向可移動布數
		int  descision;             // 1上 2下 3左 4右
		int  time;                  // FOR random
		int  bg[13][15];
		int  GetPath();
		// int health;
		Bullet b;
		bool   BulletHit;           // 子彈打到玩家
	public:
		Enemy();
		int  GetX1();					// 腳色左上角 x 座標
		int  GetY1();					// 腳色左上角 y 座標
		int  GetX2();					// 腳色右下角 x 座標
		int  GetY2();					// 腳色右下角 y 座標
		void Initialize(int, int);      // 設定腳色為初始值 對不同腳色設定初始位置
		void LoadBitmap();				// 載入圖形
		void OnMove(int, int, int, int);// 移動腳色
		void OnShow();					// 將腳色圖形貼到畫面
		void LoadMap(int maps[13][15]);
		int  GetPosition(int, int);
		void Attack(int nx, int ny);
		int  BulletPosX();
		int  BulletPosY();
		void BulletTouch(int, int);
		bool BulletHitPlayer();              // 攻擊判斷 碰到玩家返回true到mygame
	};
}

#endif