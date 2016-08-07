#ifndef	WALL_H_INCLUDED
#define WALL_H_INCLUDED

class TTime;

class TPlatform : protected TUoBox
{
	protected:

		TSprite  m_tSprite;

	public:

		virtual ~TPlatform(){};

		const TUoBox* GetCastToUoBox()
		{
			return static_cast<TUoBox*>(this);
		}

		virtual void Initialize(const TGfxTexture * pTexture, const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius);
		virtual void Update(const TTime & tTime);
		virtual void Render();
};

#endif
