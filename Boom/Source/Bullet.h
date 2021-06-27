namespace game_framework {
	class Bullet {
	private:
		CAnimation bullet;
		int  x, y;                       
		int  direction;                 // 1�W 2�U 3�� 4�k
		int  speed = 8;
		bool active;
		bool touched;
	public:
		Bullet();
		void setPath(int, int, int);
		void setActive(bool);
		bool getActive();               // 20210516 - None used at this time
		int  getX();                    // �����I
		int  getY();
		int  getDir();
		void isTouched(int, int);
		bool isTouched();
		void Initialize(int, int);      // �]�w�}�⬰��l�� �藍�P�}��]�w��l��m
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ���ʸ}��
		void OnShow();					// �N�}��ϧζK��e��
	};
}