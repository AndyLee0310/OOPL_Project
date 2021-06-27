#define STEP   4
namespace game_framework {
	class Character {
	
	public:
		Character();
		int  GetX1();					// 腳色左上角 x 座標
		int  GetY1();					// 腳色左上角 y 座標
		int  GetX2();					// 腳色右下角 x 座標
		int  GetY2();					// 腳色右下角 y 座標
		int  GetStep();                 // 腳色步數
		int  GetRange();                // 爆炸距離
		bool GetDead();					// 腳色是否死亡
		void Initialize(int nx, int ny);// 設定腳色為初始值 對不同腳色設定初始位置
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動腳色
		void OnShow();					// 將腳色圖形貼到畫面
		void SetMovingDown(bool flag);	// 設定是否正在往下移動
		void SetMovingLeft(bool flag);	// 設定是否正在往左移動
		void SetMovingRight(bool flag); // 設定是否正在往右移動
		void SetMovingUp(bool flag);	// 設定是否正在往上移動
		void SetDead(bool flag);
		void SetXY(int nx, int ny);		// 設定腳色左上角座標
		void SetRange(int);             // 設定爆炸距離
		void LoadMap(int maps[13][15]);
		int  GetPosition(int, int);
	private:
		CAnimation Character_down;	// 腳色向下的動畫
		CAnimation Character_up;
		CAnimation Character_left;
		CAnimation Character_right;
		CAnimation Character_dead;	// 腳色死亡
		int  Animate_State;         // 腳色移動狀態 1為下 2為上 3為左 4為右
		int  x, y;					// 腳色左上角座標
		int  move_step = STEP;      // 腳色步數
		int  Explosion_range;       // 最大爆炸距離
		//int  Bomb_count;            // 可用炸彈數
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		bool isDead;				// 是否死亡
		int  map_simple[13][15];
		int  map[416][480];
		bool isMoveable(int, int);
		// int health;
		// int life;
	};
}

/*
	註一:炸彈在短時間內重複要求放置會有機率在一個位置放置兩個以上 推測是因為在地圖更新之前又要求設置
*/
