#ifndef	TRIGGER_LINKED_H_INCLUDED
#define TRIGGER_LINKED_H_INCLUDED

class TTime;
struct TGfxSprite;

enum EBoxSide
{
	EBoxSide_Centered = 0,
	EBoxSide_Left,
	EBoxSide_Right,
	EBoxSide_Up,
	EBoxSide_Down,
};

class TTrigerLinked : private TTrigger
{
	private:

		TGfxSprite * m_pLineSprite;

		const TUoBox * m_pLinkTarget;
		EBoxSide m_eBoxSide;

		void DrawBox();
		void Synchronise();

	public:

		TTrigerLinked();

		TGfxVec2 GetWorldPos()
		{
			return TTransform::GetWorldPos();
		}

		void SetRadius(TGfxVec2 tRadius);
	
		void Link(const TUoBox * pLinkTarget, EBoxSide eLinkSide, const TGfxVec2 & tTriggerRadius);
		void Update();
		void Render();
		

		virtual bool CheckTrigger(const TUoBox & tTarget)
		{
			return TTrigger::CheckTrigger(tTarget);
		}
};



#endif
