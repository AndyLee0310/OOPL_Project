#ifndef _STAGE2_
#define _STAGE2_
namespace game_framework {
	class GameStage_2 : public CGameState {
	public:
		GameStage_2(CGame* g);
		~GameStage_2();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();                                  // �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);               // ��L�ʧ@
		void OnKeyUp(UINT, UINT, UINT);
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
		void setBomb(int);
		void mapChange(int, int, int);                            // �a���ܰ�&�q��character
		void BombState();
		void setBombRange(int, int, int, int);                                    // �z���ɳ]�m�d��
		void GetCoins();				//�����I������
		void CoinState();				//�����ʵe
		void HealthState();
		void HeartChange();
	private:
		int bg[13][15];              //0�a�O 1�۶� 2����� 4���z�u 5�z����
		int coins_pos[9][2];         //�w����m
		CMovingBitmap block_0;
		CMovingBitmap block_1;
		Obstacle*     block_2;
		int           block_2_pos[88][2],block2_num;
		CMovingBitmap panel;
		CMovingBitmap border;
		Character     character_1;   //Range undone
		CMovingBitmap character_2;   //���O�����
		Enemy* AI;
		int coins_num;	             //�����`��
		int sc;		                 //�����Y��X�Ӫ���
		CoinsAnimation* coin_Ani;
		Bomb* Bomb_ch1;

		CMovingBitmap playerhead_1;
		CMovingBitmap playerhead_2;

		double heart_num[8];
		double blood_ori, blood_vol;
		Healths* heart;

		CInteger count_down;
		int* tempTime;
		int timer;
	};
}

#endif