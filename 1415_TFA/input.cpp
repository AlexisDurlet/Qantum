#include "flib_vec2.h"
#include "time.h"
#include "input.h"
#include "e_input_id.h"
#include "e_input_state.h"
#include "flib.h"
#include "constants.h"
#include "math.h"

TInput::TInput():
	m_eCurrentInput(EInputID_Void),
	m_eCurrentState(EInputState_Waiting)
{

}

void TInput::Update()
{
	const bool bInputIsJustPressed = GfxInputIsJustPressed(EGfxInputID_MouseLeft);
	const bool bInputIsJustReleased = GfxInputIsJustReleased(EGfxInputID_MouseLeft);

	const TGfxVec2 tCurrentMousePos = TGfxVec2(float(GfxGetCurrentMouseX()), float(GfxGetCurrentMouseY()));

	switch (m_eCurrentState)
	{
		case EInputState_Waiting:

			if (bInputIsJustPressed)
			{
				m_tInitialPressedPos = tCurrentMousePos;
				m_tDeltaPressedPos = TGfxVec2(0, 0);
				m_tTimerPressed.Initialize();
				m_eCurrentState = EInputState_Pressed;
			}
			else m_eCurrentInput = EInputID_Void;

			break;

		case EInputState_Pressed:

			m_tTimerPressed.Update();
			m_tDeltaPressedPos = tCurrentMousePos - m_tInitialPressedPos;

			if (bInputIsJustReleased)
			{
				m_tTimerRealeased.Initialize();
				m_eCurrentState = EInputState_Released;
			}
			else if (m_tTimerPressed.GetTimeFromBegin() > PRESSED_TIME_LIMIT)
			{
				m_bHasBeenHolded = false;
				m_eCurrentState = EInputState_Holded;
			}
			else m_eCurrentInput = EInputID_Void;

			break;

		case EInputState_Released:

			m_tTimerRealeased.Update();

			if (m_tTimerRealeased.GetTimeFromBegin() > DOUBLE_TAP_TIMING)
			{
				m_eCurrentState = EInputState_Waiting;
			}

			else if (bInputIsJustPressed)
			{
				m_eCurrentInput = EInputID_DoubleTap;
				m_eCurrentState = EInputState_Waiting;
			}
			else m_eCurrentInput = EInputID_Void;

			break;

		case EInputState_Holded:

			m_tDeltaPressedPos = tCurrentMousePos - m_tInitialPressedPos;

			if (bInputIsJustReleased)
			{
				m_eCurrentState = EInputState_Waiting;
			}

			else if (m_tDeltaPressedPos.Length() <= SWIPE_MIN_DISTANCE && !m_bHasBeenHolded)
			{
				m_eCurrentInput = EInputID_HoldTap;
				m_bHasBeenHolded = true;
			}

			else if (m_tDeltaPressedPos.Length() >= SWIPE_MIN_DISTANCE 
					&& fabsf(m_tDeltaPressedPos.x) < fabsf(m_tDeltaPressedPos.y) / SWIPE_MAX_AXIS_FACTOR)
			{
				if (m_tDeltaPressedPos.y < 0)
				{
					m_eCurrentInput = EInputID_SwipeUp;
					m_eCurrentState = EInputState_Waiting;
				}

				else if (m_tDeltaPressedPos.y > 0)
				{
					m_eCurrentInput = EInputID_SwipeDown;
					m_eCurrentState = EInputState_Waiting;
				}
			}

			else if (m_tDeltaPressedPos.Length() >= SWIPE_MIN_DISTANCE
					&&fabsf(m_tDeltaPressedPos.y) < fabsf(m_tDeltaPressedPos.x) / SWIPE_MAX_AXIS_FACTOR)
			{
				if (m_tDeltaPressedPos.x < 0)
				{
					m_eCurrentInput = EInputID_SwipeLeft;
					m_eCurrentState = EInputState_Waiting;
				}

				else if (m_tDeltaPressedPos.x > 0)
				{
					m_eCurrentInput = EInputID_SwipeRight;
					m_eCurrentState = EInputState_Waiting;
				}
			}

			else
			{
				m_eCurrentInput = EInputID_Void;
			}

			break;

	}
}

