#ifndef FADE_INCLUDED
#define	FADE_INCLUDED

enum EFadeState
{
	EFadeState_AllOpaque = 0,
	EFadeState_FadingIn,
	EFadeState_AllTranslucent,
	EFadeState_FadingOut
};

class TSprite;
class TTime;

class TFade
{
	private:

		TSprite * m_pSprite;

		EFadeState m_eFadeState;

		float m_fMinAlpha;
		float m_fMaxAlpha;
		float m_fAlphaValue;
		float m_fAlphaSlope;

	public:

		TFade();

		void StartFadeIn(float fDelay);
		void StartFadeOut(float fDelay);

		void SetSprite(TSprite * pSprite)
		{
			m_pSprite = pSprite;
		}

		void SetMinAlpha(float fMinAlpha)
		{
			m_fMinAlpha = fMinAlpha;
		}

		void SetMaxAlpha(float fMaxAlpha)
		{
			m_fMaxAlpha = fMaxAlpha;
		}

		void Update(const TTime & tTime);


};


#endif
