namespace game_framework {
	class Healths {
	public:
		void Initialize();
		void Initialize(int, int);
		void LoadBitmap();
		void OnMove();
		void OnShow();
		void SetDescision(int);
		void setActive();
		//void setExp(bool);
		void setTopLeft(int, int);
		bool getActive();
		bool getExp();
	private:
		CAnimation heart2;
		CAnimation heart1;
		CAnimation heart0;
		bool Active, exp;     //00等待 11旋轉 10結束
		int x, y;
		int descision;		 //0沒血 1半血 2滿血
	};
}