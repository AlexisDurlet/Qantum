#ifndef TRIGGER_H_INCLUDED
#define	TRIGGER_H_INCLUDED

class TTime;

class TTrigger : protected TUoBoxCollider
{
	public:
	
		void Initialize(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius);
		virtual bool CheckTrigger(const TUoBox & tTarget);
};


#endif
