#ifndef INPUT_H_INCLUDED
#define	INPUT_H_INCLUDED

enum EInputID : int;
enum EInputState : int;

class TInput
{
	private:

		TInput();

		EInputID m_eCurrentInput;
		EInputState m_eCurrentState;

		TGfxVec2 m_tInitialPressedPos;
		TGfxVec2 m_tDeltaPressedPos;

		TTime m_tTimerPressed;
		TTime m_tTimerRealeased;

		bool m_bHasBeenHolded;

	public:

		 static TInput & GetInstance()
		{
			static TInput * tInput = new TInput();
			return *tInput;
		}

		 EInputID GetInput() const
		{
			return m_eCurrentInput;
		}

		void Update();

};


#endif
