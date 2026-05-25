#include "StaticObject.h"

void StaticObject::OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) {
	if (e->nx != 0) {
		mario->SetSpeedX(0);
	}
	if (e->ny != 0) {
		mario->SetSpeedY(0);
	}
}