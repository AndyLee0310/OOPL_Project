
namespace game_framework {
	class Bomb {
	public:
		Bomb();
		void Initialize();
		void LoadBitmap();
		void OnMove();
		void OnShow();
		void setActive(bool);
		void setUp(int);
		void setDown(int);
		void setRight(int);
		void setLeft(int);
		void setRange(int, int, int, int);
		void setTopleft(int, int);
		void setObs(bool);
		int  getUp();
		int  getDown();
		int  getRight();
		int  getLeft();
		int  getTop_Bomb();
		int  getLeft_Bomb();
		bool getActive();
		bool getExp();
		bool getObs();
	private:
		CAnimation waiting;
		CAnimation Explosion;
		CAnimation Exp_V;
		CAnimation Exp_H;
		bool       active;        // 00等待 10未爆 11爆炸中 01結束
		bool       isExp;
		bool       Obs_break;     // 是否範圍設置
		int        x, y;
		int        range_up;
		int        range_down;
		int        range_left;
		int        range_right;
		int        timer;         //爆炸倒數
	};
}
