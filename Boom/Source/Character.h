#define STEP   4
namespace game_framework {
	class Character {
	
	public:
		Character();
		int  GetX1();					// �}�⥪�W�� x �y��
		int  GetY1();					// �}�⥪�W�� y �y��
		int  GetX2();					// �}��k�U�� x �y��
		int  GetY2();					// �}��k�U�� y �y��
		int  GetStep();                 // �}��B��
		int  GetRange();                // �z���Z��
		bool GetDead();					// �}��O�_���`
		void Initialize(int nx, int ny);// �]�w�}�⬰��l�� �藍�P�}��]�w��l��m
		void LoadBitmap();				// ���J�ϧ�
		void OnMove();					// ���ʸ}��
		void OnShow();					// �N�}��ϧζK��e��
		void SetMovingDown(bool flag);	// �]�w�O�_���b���U����
		void SetMovingLeft(bool flag);	// �]�w�O�_���b��������
		void SetMovingRight(bool flag); // �]�w�O�_���b���k����
		void SetMovingUp(bool flag);	// �]�w�O�_���b���W����
		void SetDead(bool flag);
		void SetXY(int nx, int ny);		// �]�w�}�⥪�W���y��
		void SetRange(int);             // �]�w�z���Z��
		void LoadMap(int maps[13][15]);
		int  GetPosition(int, int);
	private:
		CAnimation Character_down;	// �}��V�U���ʵe
		CAnimation Character_up;
		CAnimation Character_left;
		CAnimation Character_right;
		CAnimation Character_dead;	// �}�⦺�`
		int  Animate_State;         // �}�Ⲿ�ʪ��A 1���U 2���W 3���� 4���k
		int  x, y;					// �}�⥪�W���y��
		int  move_step = STEP;      // �}��B��
		int  Explosion_range;       // �̤j�z���Z��
		//int  Bomb_count;            // �i�ά��u��
		bool isMovingDown;			// �O�_���b���U����
		bool isMovingLeft;			// �O�_���b��������
		bool isMovingRight;			// �O�_���b���k����
		bool isMovingUp;			// �O�_���b���W����
		bool isDead;				// �O�_���`
		int  map_simple[13][15];
		int  map[416][480];
		bool isMoveable(int, int);
		// int health;
		// int life;
	};
}

/*
	���@:���u�b�u�ɶ������ƭn�D��m�|�����v�b�@�Ӧ�m��m��ӥH�W �����O�]���b�a�ϧ�s���e�S�n�D�]�m
*/
