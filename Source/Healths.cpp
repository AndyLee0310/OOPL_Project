#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Healths.h"

namespace game_framework {
	void Healths::Initialize() {
		Initialize(0, 0);
	}
	void Healths::Initialize(int nx, int ny) {
		Active = exp = false;
		x = nx;
		y = ny;
		descision = 3;
	}
	void Healths::LoadBitmap() {
		heart2.SetDelayCount(5);
		heart2.AddBitmap(IDB_HEALTH_2, RGB(255, 255, 255));

		heart1.SetDelayCount(5);
		heart1.AddBitmap(IDB_HEALTH_1, RGB(255, 255, 255));

		heart0.SetDelayCount(5);
		heart0.AddBitmap(IDB_HEALTH_0, RGB(255, 255, 255));
	}
	void Healths::OnMove() {
		heart2.OnMove();
		heart1.OnMove();
		heart0.OnMove();
	}
	void Healths::OnShow() {
		heart2.SetTopLeft(x, y);
		heart1.SetTopLeft(x, y);
		heart0.SetTopLeft(x, y);
		if (descision == 2) heart2.OnShow();
		else if (descision == 1) heart1.OnShow();
		else if (descision == 0) heart0.OnShow();
	}
	void Healths::SetDescision(int des) {
		descision = des;
	}
	void Healths::setTopLeft(int nx, int ny) {
		x = nx;
		y = ny;
	}
}