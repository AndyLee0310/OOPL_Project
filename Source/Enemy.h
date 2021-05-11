#define STEP   6
namespace game_framework {
	class Enemy {
	private:
		CAnimation Character_down;	// �}��V�U���ʵe
		CAnimation Character_up;
		CAnimation Character_left;
		CAnimation Character_right;
		int  Animate_State;         // �}�Ⲿ�ʪ��A 1���U 2���W 3���� 4���k
		int  x, y;					// �}�⥪�W���y��
		int  move_step = STEP;      // �}��B��
		int  map_simple[13][15];
		int  map[416][480];
		bool isMoveable(int, int);
		// int health;
	public:
		Enemy();
		int  GetX1();					// �}�⥪�W�� x �y��
		int  GetY1();					// �}�⥪�W�� y �y��
		int  GetX2();					// �}��k�U�� x �y��
		int  GetY2();					// �}��k�U�� y �y��
		int  GetStep();                 // �}��B��
		void Initialize(int, int);// �]�w�}�⬰��l�� �藍�P�}��]�w��l��m
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ���ʸ}��
		void OnShow();					// �N�}��ϧζK��e��
		void LoadMap(int maps[13][15]);
		int  GetPosition(int, int);
	};
}