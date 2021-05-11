#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Enemy.h"

namespace game_framework {
	Enemy::Enemy() {
		Initialize(0, 0);
	}
	void Enemy::Initialize(int nx, int ny) {
		x = nx;
		y = ny;
		Animate_State = 1;
	}
	void Enemy::LoadBitmap() {

	}
}