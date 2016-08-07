#ifndef	TRAP_H_INCLUDED
#define TRAP_H_INCLUDED

enum EOrientation : int;

class TTrap : private TPlatform
{
	private:

		TTrigerLinked m_tLaserHitBox;
		TDecoration m_tLaserBody;
		TDecorationAnimated m_tLaserEnd;

	public:

		const TUoBox* GetCastToUoBox()
		{
			return static_cast<TUoBox*>(this);
		}

		void InitializeTrap(const TGfxTexture * pTexture, const TGfxVec2 & tWorldPos, const EOrientation eOrientation, const float fLaserLenght);
		virtual void Update(const TTime & tTime);
		virtual void Render();

};

#endif
