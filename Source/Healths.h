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
		bool Active, exp;     //00���� 11���� 10����
		int x, y;
		int descision;		 //0�S�� 1�b�� 2����
	};
}