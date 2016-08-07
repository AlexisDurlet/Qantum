#ifndef	DECORATION_ANIMATED_H_INCLUDED
#define DECORATION_ANIMATED_H_INCLUDED

class TDecorationAnimated : public TDecoration
{
	private:

		TAnimation m_tAnimation;

	public:

		void InitializeAnim(const ECutOutIncrement eCutOutIncrement, const int iTimePerFrame, const int iFrameNumber);
		void StartAnim();
		virtual void Update();
};


#endif
