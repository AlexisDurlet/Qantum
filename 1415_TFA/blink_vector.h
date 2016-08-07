#ifndef	BLINK_VECTOR_H_INCLUDED
#define BLINK_VECTOR_H_INCLUDED

struct TGfxSprite;
class TUoBox;

class TBlinkVector : private TTransform
{
	private:

		TGfxSprite * m_pLineSprite;

		TGfxVec2 m_tTarget;
		TGfxVec2 m_tVector;

		TGfxVec2 m_tTPSpeed;
		float m_fTPtime;

		float m_fStepLenght;
		unsigned int m_iDrawingColor;

		bool m_bActive;
		bool m_bValidTarget;
		bool m_bTargetChanged;

		void DrawVector(const TGfxVec2 tOrigin, const TGfxVec2 tDestination);

	public:

		TGfxVec2 GetTarget() const
		{
			return m_tTarget;
		}

		TGfxVec2 GetTPSpeed() const
		{
			return m_tTPSpeed;
		}

		float GetTPTime() const
		{
			return m_fTPtime;
		}

		bool GetTargetValidity() const
		{
			return m_bValidTarget;
		}

		bool GetChangeOfTarget() const
		{
			return m_bTargetChanged;
		}

		void Initialize();
		void Start(const TGfxVec2 & tTarget, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		void Update(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		void Stop();
		void Render();

};

#endif
