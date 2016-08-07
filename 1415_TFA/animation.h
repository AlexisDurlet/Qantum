#ifndef	ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

class TSprite;

enum ECutOutIncrement
{
	ECutOutIncrement_X_Positive = 0,
	ECutOutIncrement_X_Negative,
	ECutOutIncrement_Y_Positive,
	ECutOutIncrement_Y_Negative,
};

class TAnimation
{
	private:

		TSprite * m_pSprite;

		TGfxVec2 m_tInitialCutout;
		TGfxVec2 m_tCutOutIncrement;

		TGfxVec2 m_tOptionalInitialCutOut;

		TTime m_tTimer;

		int m_iTimePerFrame;
		int m_iFrameNumber;
		int m_iCurrentFrame;

		bool m_bStarted;

	public:

		TAnimation();

		int GetFrameNumber() const
		{
			return m_iFrameNumber;
		}

		int GetCurrentFrame() const
		{
			return m_iCurrentFrame;
		}

		void Initialize(TSprite * pSprite, const TGfxVec2 & tInitialCutout, const ECutOutIncrement eCutOutIncrement, const int iTimePerFrame, const int iFrameNumber);
		void InitializeOptional(const TGfxVec2 & tOptionalInitialCutOut);
		int GetCycleDuration();
		void Start();
		void Update();
		void SwitchInitialCutOut();
		void Reverse();
		TGfxVec2 GetLastFrameCutOut() const;

};


#endif
