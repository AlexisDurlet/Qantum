#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "kinematic.h"
#include "trigger.h"
#include "time.h"

void TTrigger::Initialize(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius)
{
	InitializeUoBox(tWorldPos, tRadius);
}

bool TTrigger::CheckTrigger(const TUoBox & tTarget)
{
	return Collision(tTarget);
}


