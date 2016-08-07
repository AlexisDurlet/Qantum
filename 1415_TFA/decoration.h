#ifndef	DECORATION_H_INCLUDED
#define DECORATION_H_INCLUDED

struct TGfxTexture;
class TTime;

class TDecoration : public TTransform
{
	public:

		TSprite m_tSprite;
		void InitializeSprite(const TGfxTexture * pTexture, const TGfxVec2 & tCutOut, const TGfxVec2 & tSize, const TGfxVec2 & tScale);
		virtual void Update();
		void Render();
};

#endif
