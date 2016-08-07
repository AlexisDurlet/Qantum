#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "float.h"
#include "math.h"
#include"clamp.h"

TUoBoxCollider::TUoBoxCollider():
	m_tCollisionVector(FLT_MAX, FLT_MAX)
{

}

void TUoBoxCollider::InitializeUoBox(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius)
{
	TUoBox::InitializeUoBox(tWorldPos, tRadius);
}

bool TUoBoxCollider::Collision(const TUoBox & tBoxCollided)
{
	const TGfxVec2 tBoxCollidedPos = tBoxCollided.GetWorldPos();
	const TGfxVec2 tBoxCollidedRadius = tBoxCollided.GetRadius();
	
	const TGfxVec2 tMinDistance = m_tRadius + tBoxCollidedRadius;
	const TGfxVec2 tColliderToBoxCollidedVector = tBoxCollidedPos - m_tWorldPos;
	bool bOverlapX = fabsf(tColliderToBoxCollidedVector.x) < tMinDistance.x;
	bool bOverlapY = fabsf(tColliderToBoxCollidedVector.y) < tMinDistance.y;
	bool bCollision = bOverlapX && bOverlapY;

	if (bCollision)
	{
		const TGfxVec2 tPenetration = tMinDistance - TGfxVec2(fabsf(tColliderToBoxCollidedVector.x), fabsf(tColliderToBoxCollidedVector.y));
		m_tCollisionVector.x = -(copysignf(1, tColliderToBoxCollidedVector.x) * tPenetration.x);
		m_tCollisionVector.y = -(copysignf(1, tColliderToBoxCollidedVector.y) * tPenetration.y);
	}

	else
	{
		m_tCollisionVector = TGfxVec2(FLT_MAX, FLT_MAX);
	}


	return bCollision;
}



void TUoBoxCollider::CollisionResolveX()
{
	if (m_tCollisionVector != TGfxVec2(FLT_MAX, FLT_MAX))
	{
		m_tWorldPos.x += m_tCollisionVector.x;
	}
}

void TUoBoxCollider::CollisionResolveY()
{
	if (m_tCollisionVector != TGfxVec2(FLT_MAX, FLT_MAX))
	{
		m_tWorldPos.y += m_tCollisionVector.y;
	}
}

void TUoBoxCollider::ForceCollision(const TUoBox & tBoxCollided)
{
	if (Collision(tBoxCollided))
	{
		const TGfxVec2 tBoxCollidedPos = tBoxCollided.GetWorldPos();
		const TGfxVec2 tBoxCollidedRadius = tBoxCollided.GetRadius();

		TGfxVec2 tXEdges = TGfxVec2(tBoxCollidedPos.x + tBoxCollidedRadius.x, tBoxCollidedPos.x - tBoxCollidedRadius.x);
		TGfxVec2 tYEdges = TGfxVec2(tBoxCollidedPos.y + tBoxCollidedRadius.y, tBoxCollidedPos.y - tBoxCollidedRadius.y);

		bool bXCollision = m_tWorldPos.y < tYEdges.x && m_tWorldPos.y > tYEdges.y;
		bool bYCollision = m_tWorldPos.x < tXEdges.x && m_tWorldPos.x > tXEdges.y;
		bool bTotalOverlapCollision = bXCollision && bYCollision;

		if (bTotalOverlapCollision)
		{
			const TGfxVec2 tColliderToBoxCollidedVector = tBoxCollidedPos - m_tWorldPos;

			const float tDistanceFromXEdge = tBoxCollidedRadius.x - fabsf(tColliderToBoxCollidedVector.x);
			const float tDistanceFromYEdge = tBoxCollidedRadius.y - fabsf(tColliderToBoxCollidedVector.y);

			if (tDistanceFromXEdge < tDistanceFromYEdge)
			{
				CollisionResolveX();
			}
			else
			{
				CollisionResolveY();
			}
		}
		else if (bXCollision)
		{
			CollisionResolveX();
		}
		else if (bYCollision)
		{
			CollisionResolveY();
		}
		else
		{
			CollisionResolveY();
		}
	}
}
