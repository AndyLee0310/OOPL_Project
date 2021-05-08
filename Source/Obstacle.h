namespace game_framework {
	class Obstacle {
	public:
		void Initialize();
		void Initialize(int, int);
		void LoadBitmap();
		void OnMove();
		void OnShow();
		void setActive();
		//void setExp(bool);
		void setTopLeft(int, int);
		bool getActive();
		bool getExp();
	private:
		CAnimation block;
		bool Active, exp;     //00等待 11爆裂動畫 10結束
		int x, y;
	};
}