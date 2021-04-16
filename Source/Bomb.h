
namespace game_framework {
	class Bomb {
	public:
		Bomb();
		void Initialize(int nx, int ny);
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
		int  getUp();
		int  getDown();
		int  getRight();
		int  getLeft();
		bool getActive();
	protected:

	private:
		CAnimation waiting;
		CAnimation Explosion;
		CAnimation Exp_up;
		CAnimation Exp_down;
		CAnimation Exp_left;
		CAnimation Exp_right;
		bool       active;
		bool       isExp;
		int        x, y;
		int        range_up;
		int        range_down;
		int        range_left;
		int        range_right;
		int        timer;         //Ãz¬µ­Ë¼Æ
	};
}
