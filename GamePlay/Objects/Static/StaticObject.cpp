#include "StaticObject.h"

void StaticObject::OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) {
	if (e->nx != 0) {
		mario->SetSpeedX(0);  // Dừng tốc độ X khi chạm tường
	}
	if (e->ny != 0) {
		mario->SetSpeedY(0);  // Dừng tốc độ Y khi chạm đất/trần
	}
}