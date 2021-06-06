namespace game_framework {
	class Bullet {
	private:
		CAnimation bullet;
		int  x, y;                       
		int  direction;                 // 1上 2下 3左 4右
		int  speed = 8;
		bool active;
		bool touched;
	public:
		Bullet();
		void setPath(int, int, int);
		void setActive(bool);
		bool getActive();               // 20210516 - None used at this time
		int  getX();                    // 中心點
		int  getY();
		void isTouched();               // 0未接觸 1玩家 2牆
		void Initialize(int, int);      // 設定腳色為初始值 對不同腳色設定初始位置
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動腳色
		void OnShow();					// 將腳色圖形貼到畫面
	};
}
