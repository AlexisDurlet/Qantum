#ifndef UO_BOX_COLLIDER_H_INCLUDED
#define UO_BOX_COLLIDER_H_INCLUDED

class TUoBoxCollider : public TUoBox
{
	private:

		TGfxVec2 m_tCollisionVector;

	public:

		TUoBoxCollider();

		virtual void InitializeUoBox(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius);

		bool Collision(const TUoBox & tBoxCollided);
		void CollisionResolveX();
		void CollisionResolveY();

		void ForceCollision(const TUoBox & tBoxCollided);
};


#endif
