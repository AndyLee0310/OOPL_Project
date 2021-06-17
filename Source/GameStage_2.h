#ifndef _STAGE2_
#define _STAGE2_
namespace game_framework {
	class GameStage_2 : public CGameState {
	public:
		GameStage_2(CGame* g);
		~GameStage_2();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();                                  // 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);               // 鍵盤動作
		void OnKeyUp(UINT, UINT, UINT);
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
		void setBomb(int);
		void mapChange(int, int, int);                            // 地圖變動&通知character
		void BombState();
		void setBombRange(int, int, int, int);                                    // 爆炸時設置範圍
		void GetCoins();				//偵測碰撞金幣
		void CoinState();				//金幣動畫
		void HealthState();
		void HeartChange();
	private:
		int bg[13][15];              //0地板 1石塊 2粉色石 4未爆彈 5爆炸中
		int coins_pos[9][2];         //硬幣位置
		CMovingBitmap block_0;
		CMovingBitmap block_1;
		Obstacle*     block_2;
		int           block_2_pos[88][2],block2_num;
		CMovingBitmap panel;
		CMovingBitmap border;
		Character     character_1;   //Range undone
		CMovingBitmap character_2;   //類別之後改
		Enemy* AI;
		int coins_num;	             //金幣總數
		int sc;		                 //紀錄吃到幾個金幣
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