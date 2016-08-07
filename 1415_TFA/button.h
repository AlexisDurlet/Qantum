#ifndef BUTTON_H_INCLUDED
#define	BUTTON_H_INCLUDED

struct TGfxTexture;

class TButton : private TUoBox
{
	private : 

		TSprite m_tSprite;
		TGfxVec2 m_tPressedCutout;
		bool m_bHasBeenPressed;

	public:

		void SetPos(TGfxVec2 tPos)
		{
			m_tWorldPos = tPos;
		}

		void InitializeButton(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius);
		void InitializeSprite(TGfxTexture * pTexture, const TGfxVec2 & tCutout, const TGfxVec2 & tPressedCutout);
		bool Update();
		void Render();
};


#endif
