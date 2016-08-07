#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

struct TGfxTexture;
struct TGfxSprite;
enum EOrientation : int;

class TSprite
{
	private:

		TGfxSprite * m_pSprite;

		TTransform * m_pTransform;
		TGfxVec2 m_tInitialCutOut;
		TGfxVec2 m_tSize;
		TGfxVec2 m_tScale;

	public:

		TSprite();
		~TSprite();

		TGfxVec2 GetInitialCutOut() const
		{
			return m_tInitialCutOut;
		}

		 TGfxVec2 GetSize() const 
		{
			return m_tSize;
		}
		
		void Initialize(TTransform * pTransform, const TGfxTexture * pTexture, const TGfxVec2 & tCutOut, const TGfxVec2 & tSize, const TGfxVec2 & tScale);
		void SetCutOut(const TGfxVec2 & tCutOut);
		void SetScale(const TGfxVec2 & tScale);
		void SetFilter(bool bFilter);
		void SetColor(unsigned int iColor);
		void ResetCutout();
		void Flip(const EOrientation eOrientation);
		void Rotate(const EOrientation eOrientation);
		void Update();
		void Render();
		void Destroy();

};


#endif
