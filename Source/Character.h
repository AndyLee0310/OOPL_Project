#define STEP   4
namespace game_framework {
	class Character {
	protected:
		CAnimation animation;		// �}�⪺�ʵe
		int x, y;					// �}�⥪�W���y��
		int move_step = STEP;       // �}��B��
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		int  map[416][480];
		// int health;
		// int *bombs;
		// int life;
	public:
		Character();
		int  GetX1();					// �}�⥪�W�� x �y��
		int  GetY1();					// �}�⥪�W�� y �y��
		int  GetX2();					// �}��k�U�� x �y��
		int  GetY2();					// �}��k�U�� y �y��
		int  GetStep();                 // �}��B��
		void Initialize(int nx, int ny);// �]�w�}�⬰��l�� �藍�P�}��]�w��l��m
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ���ʸ}��
		void OnShow();					// �N�}��ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetXY(int nx, int ny);		// �]�w�}�⥪�W���y��
		void LoadMap(int maps[13][15]);
		int  GetPosition(int, int);
	};
}