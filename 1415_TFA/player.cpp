#include "flib_vec2.h"
#include "transform.h"
#include "blink_vector.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "kinematic.h"
#include "kinematic_gravity.h"
#include "trigger.h"
#include "trigger_linked.h"
#include "uo_box_physical.h"
#include "time.h"
#include "animation.h"
#include "sprite.h"
#include "decoration.h"
#include "decoration_animated.h"
#include "player.h"
#include "e_player_move_state.h"
#include "e_player_tp_state.h"
#include "e_player_life_state.h"
#include "input.h"
#include "e_input_id.h"
#include "flib.h"
#include "constants.h"
#include "camera.h"
#include "e_orientation.h"
#include "check_point.h"
#include "lerp.h"


TPlayer::TPlayer()
{

}


void TPlayer::AddSpeedByInput()
{
	m_tKinematicGravity.m_tSpeed += m_tSpeedByInput;
}

void TPlayer::RemoveSpeedFromInput()
{
	m_tKinematicGravity.m_tSpeed -= m_tSpeedByInput;
	m_tSpeedByInput = TGfxVec2(0, 0);
}


void TPlayer::SetMoveLeft()
{
	m_bPhysical = true;

	RemoveSpeedFromInput();
	m_tSpeedByInput = -PLAYER_WALK_SPEED;
	AddSpeedByInput();

	m_tAnimationRunning.Start();
	m_tSprite.Flip(EOrientation_Left);

	m_eCurrentMoveState = EPlayerMoveState_MovingLeft;
}

void TPlayer::SetMoveRight()
{
	m_bPhysical = true;

	RemoveSpeedFromInput();
	m_tSpeedByInput = PLAYER_WALK_SPEED;
	AddSpeedByInput();

	m_tAnimationRunning.Start();
	m_tSprite.Flip(EOrientation_Right);

	m_eCurrentMoveState = EPlayerMoveState_MovingRight;
}

void TPlayer::SetMoveIdle()
{
	m_bPhysical = true;

	RemoveSpeedFromInput();

	m_tAnimationIdle.Start();

	m_eCurrentMoveState = EPlayerMoveState_Idle;
}

void TPlayer::SetMoveFalling()
{
	m_bPhysical = true;

	if (m_bPortalActive)
	{
		m_tSprite.SetCutOut(m_tFallingCutOutOptional);
	}
	else
	{
		m_tSprite.SetCutOut(m_tFallingCutOut);
	}

	m_eCurrentMoveState = EPlayerMoveState_Falling;
}


void TPlayer::SetPortalActive()
{
	m_bPortalActive = true;
	m_tPortal.InitializeTransform(m_tWorldPos);
	m_tPortal.StartAnim();

	m_tAnimationIdle.SwitchInitialCutOut();
	m_tAnimationRunning.SwitchInitialCutOut();
}

void TPlayer::SetPortalInactive()
{
	m_bPortalActive = false;

	m_tAnimationIdle.SwitchInitialCutOut();
	m_tAnimationRunning.SwitchInitialCutOut();
}


void TPlayer::SetTPVoid()
{
	m_pExternalInterface = nullptr;

	SetKinematic(TGfxVec2(0, 0), TGfxVec2(0, 0));
	m_tKinematicGravity.m_bGravityOn = false;
	m_tSpeedByInput = TGfxVec2(0, 0);
	ResynchGroundInterface();

	SetMoveIdle();

	m_bPhysical = true;

	m_eCurrentTPState = EPlayerTPState_Void;
}

void TPlayer::SetTPPortalGo()
{
	m_tTimer.Initialize();

	m_tAnimationPortalTp.Reverse();
	m_tAnimationPortalTp.Start();

	SetKinematic(TGfxVec2(0, 0), TGfxVec2(0, 0));
	m_tKinematicGravity.m_bGravityOn = false;

	m_bPhysical = false;

	m_eCurrentTPState = EPlayerTPState_PortalTPGo;
}

void TPlayer::SetTPPortalArr()
{
	m_tTimer.Initialize();

	m_tAnimationPortalTp.Reverse();
	m_tAnimationPortalTp.Start();

	m_tWorldPos = m_tPortal.GetWorldPos();

	SetPortalInactive();

	m_bPhysical = false;

	m_eCurrentTPState = EPlayerTPState_PortalTPArr;
}

void TPlayer::SetTPBlinkStart(const TGfxVec2 & tTarget, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	m_tBlinkVector.Start(tTarget, tBoxesCollided, iBoxesCollidedNumber);

	if (m_tBlinkVector.GetTargetValidity())
	{
		m_tTimer.Initialize();

		m_tAnimationBlinkTp.Start();

		SetKinematic(TGfxVec2(0, 0), TGfxVec2(0, 0));
		m_tKinematicGravity.m_bGravityOn = false;

		m_bPhysical = false;

		if (m_tBlinkVector.GetTPSpeed().x < 0)
		{
			m_tSprite.Flip(EOrientation_Left);
		}
		else
		{
			m_tSprite.Flip(EOrientation_Right);
		}

		m_eCurrentTPState = EPlayerTPState_BlinkTPStart;
	}
	else
	{
		m_tTimer.Initialize();

		m_bPhysical = true;

		m_eCurrentTPState = EPlayerTPState_BlinkTPCancel;
	}
}

void TPlayer::SetTPBlinkGo()
{
	m_tRadius = PLAYER_RADIUS_BLINK;

	m_tSprite.SetCutOut(m_tAnimationBlinkTp.GetLastFrameCutOut());

	m_tTimer.Initialize();

	m_tKinematicGravity.m_tSpeed = m_tBlinkVector.GetTPSpeed();

	m_bPhysical = false;

	m_eCurrentTPState = EPlayerTPState_BlinkTPGo;
}

void TPlayer::SetTPBlinkArr()
{
	m_tWorldPos = m_tBlinkVector.GetTarget();

	m_tRadius = PLAYER_RADIUS;

	m_tBlinkVector.Stop();

	m_tTimer.Initialize();

	m_tAnimationBlinkTp.Reverse();
	m_tAnimationBlinkTp.Start();

	SetKinematic(TGfxVec2(0, 0), TGfxVec2(0, 0));
	m_tKinematicGravity.m_bGravityOn = false;

	m_bPhysical = false;

	m_eCurrentTPState = EPlayerTPState_BlinkTPArr;
	
}


void TPlayer::SetDead()
{
	m_tTimer.Initialize();

	m_tSprite.SetCutOut(m_tAnimationDeath.GetLastFrameCutOut());

	m_fDeathTravelingTime = Lerpf(m_tWorldPos, m_pCurrentCheckPoint->GetWorldPos(), CAMERA_DEATH_TRAVELING_SPEED) * 1000;

	SetKinematic(TGfxVec2(0, 0), TGfxVec2(0, 0));
	m_tKinematicGravity.m_bGravityOn = false;
	m_tKinematicGravity.m_bOnGround = true;
	m_tKinematicGravity.m_tSpeed = Lerpv(m_tWorldPos, m_pCurrentCheckPoint->GetWorldPos(), CAMERA_DEATH_TRAVELING_SPEED);

	m_eLifeState = EPlayerLifeState_Dead;
}

void TPlayer::SetRespawn()
{
	m_tTimer.Initialize();

	m_tWorldPos = m_pCurrentCheckPoint->GetWorldPos();

	m_tAnimationPortalTp.Start();
	m_pCurrentCheckPoint->StartRespawn();

	m_tSprite.Flip(EOrientation_Right);

	m_eLifeState = EPlayerLifeState_Respawn;
}

void TPlayer::SetAlive()
{
	if (m_eCurrentTPState == EPlayerTPState_BlinkTPArr)
	{
		m_tAnimationBlinkTp.Reverse();
	}

	if (m_eCurrentTPState == EPlayerTPState_BlinkTPGo)
	{
		m_tRadius = PLAYER_RADIUS;
	}

	m_pCurrentCheckPoint->StopRespawn();
	m_eLifeState = EPlayerLifeState_Alive;
	SetTPVoid();
}


void TPlayer::InputMove()
{
	TInput & tInput = tInput.GetInstance();
	const EInputID eInput = tInput.GetInput();

	switch (m_eCurrentMoveState)
	{
		case EPlayerMoveState_Idle:

			m_tAnimationIdle.Update();

			switch (eInput)
			{
				case EInputID_SwipeLeft:

					SetMoveLeft();

					break;

				case EInputID_SwipeRight:

					SetMoveRight();

					break;

				default:

					break;
			}

			break;

		case EPlayerMoveState_MovingLeft:

			m_tAnimationRunning.Update();

			switch (eInput)
			{
				case EInputID_SwipeRight:

					SetMoveRight();

					break;

				case EInputID_HoldTap:

					SetMoveIdle();

					break;

				default:

					break;
			}

			break;

		case EPlayerMoveState_MovingRight:

			m_tAnimationRunning.Update();

			switch (eInput)
			{
				case EInputID_SwipeLeft:

					SetMoveLeft();

					break;

				case EInputID_HoldTap:

					SetMoveIdle();

					break;

				default:

					break;
			}

			break;

		case EPlayerMoveState_Falling:

			if (m_tKinematicGravity.m_bOnGround)
			{
				SetMoveIdle();
			}

			break;

		default:

			break;
	}
}

void TPlayer::InputTP(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	TInput & tInput = tInput.GetInstance();
	const EInputID eInput = tInput.GetInput();

	const TGfxVec2 tMouseScreenPos = TGfxVec2(float(GfxGetCurrentMouseX()), float(GfxGetCurrentMouseY()));
	const TGfxVec2 tMouseWorldPos = TCamera::GetInstance().ConvertScreenPosToWorldPos(tMouseScreenPos);

	switch (eInput)
	{
		case EInputID_SwipeUp:

			if (m_bPortalActive && m_eCurrentTPState == EPlayerTPState_Void)
			{
				SetPortalInactive();
			}

			break;

		case EInputID_SwipeDown:

			if (m_bPortalActive && m_eCurrentTPState == EPlayerTPState_Void)
			{
				SetTPPortalGo();
			}
			else if (!m_bPortalActive && m_eCurrentTPState == EPlayerTPState_Void)
			{
				SetPortalActive();
			}

			break;

		case EInputID_DoubleTap:

			SetTPBlinkStart(tMouseWorldPos, tBoxesCollided, iBoxesCollidedNumber);

			break;

		default:

			break;
	}
}

void TPlayer::InputReaction(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	if (m_eCurrentTPState == EPlayerTPState_Void || m_eCurrentTPState == EPlayerTPState_BlinkTPCancel)
	{
		InputMove();
	}

	if (m_eCurrentMoveState != EPlayerMoveState_Falling && m_eCurrentTPState == EPlayerTPState_Void)
	{
		InputTP(tBoxesCollided, iBoxesCollidedNumber);
	}
}


void TPlayer::UpdateTp(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{

	switch (m_eCurrentTPState)
	{
		case EPlayerTPState_Void:

			break;

		case EPlayerTPState_PortalTPGo:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_tAnimationPortalTp.GetCycleDuration() - TIME_TOLERANCE)
			{
				m_tAnimationPortalTp.Update();
			}
			else
			{
				SetTPPortalArr();
			}

			break;

		case EPlayerTPState_PortalTPArr:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_tAnimationPortalTp.GetCycleDuration() - TIME_TOLERANCE)
			{
				m_tAnimationPortalTp.Update();
			}
			else
			{
				SetTPVoid();
			}

			break;
		
		case EPlayerTPState_BlinkTPStart:

			m_tTimer.Update();

			m_tBlinkVector.Update(tBoxesCollided, iBoxesCollidedNumber);
			
			if (m_tTimer.GetTimeFromBegin() < m_tAnimationBlinkTp.GetCycleDuration() - TIME_TOLERANCE)
			{
				m_tAnimationBlinkTp.Update();
			}
			else
			{
				SetTPBlinkGo();
			}


			break;

		case EPlayerTPState_BlinkTPCancel:

			m_tTimer.Update();

			m_tBlinkVector.Update(tBoxesCollided, iBoxesCollidedNumber);

			if (m_tTimer.GetTimeFromBegin() > m_tAnimationBlinkTp.GetCycleDuration())
			{
				m_tBlinkVector.Stop();

				m_eCurrentTPState = EPlayerTPState_Void;
			}

			break;

		case EPlayerTPState_BlinkTPGo:

			m_tTimer.Update();

			m_tKinematicGravity.m_tSpeed = m_tKinematicGravity.m_tSpeed.Normalize() * BLINK_SPEED / BLINK_SPEED_SUBDIVISION;

			for (int i = 0; i < BLINK_SPEED_SUBDIVISION; i++)
			{
				m_tBlinkVector.Update(tBoxesCollided, iBoxesCollidedNumber);

				if (m_tBlinkVector.GetChangeOfTarget() && m_tBlinkVector.GetTPTime() > 0)
				{
					m_tTimer.Initialize();
				}
			
				if (m_tTimer.GetTimeFromBegin() < m_tBlinkVector.GetTPTime())
				{
					m_tKinematicGravity.MoveX(tTime);
					m_tKinematicGravity.MoveY(tTime);
				}
				else
				{
					SetTPBlinkArr();

					break;
				}
			}

			break;

		case EPlayerTPState_BlinkTPArr:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_tAnimationBlinkTp.GetCycleDuration() - TIME_TOLERANCE)
			{
				m_tAnimationBlinkTp.Update();
			}
			else
			{
				for (int i = 0; i < iBoxesCollidedNumber; i++)
				{
					ForceCollision(tBoxesCollided[i]);
				}

				m_tAnimationBlinkTp.Reverse();

				SetTPVoid();
			}

			break;
	}
}

void TPlayer::UpdateAlife(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	InputReaction(tBoxesCollided, iBoxesCollidedNumber);

	if (m_eCurrentTPState != EPlayerTPState_BlinkTPGo && m_eCurrentTPState != EPlayerTPState_PortalTPGo)
	{
		TUoBoxPhysical::Update(tTime, tBoxesCollided, iBoxesCollidedNumber);
		
		if (m_bCollisionX)
		{
			m_tSpeedByInput = TGfxVec2(0, 0);
			SetMoveIdle();
		}
	}

	if (!m_tKinematicGravity.m_bOnGround && m_eCurrentMoveState != EPlayerMoveState_Falling)
	{
		SetMoveFalling();
	}

	UpdateTp(tTime, tBoxesCollided, iBoxesCollidedNumber);

	m_tSprite.Update();

	if (m_bPortalActive)
	{
		m_tPortal.Update();
	}
}


void TPlayer::Initialize(const TGfxTexture * pTexture, const TGfxVec2 & tWorldPos, const TGfxVec2 & tSpeed, const TGfxVec2 & tAcceleration)
{
	InitializeUoBoxPhysical(tWorldPos, PLAYER_RADIUS, tSpeed, tAcceleration);
	m_tKinematicGravity.SetTransform(this);

	m_eLifeState = EPlayerLifeState_Alive;

	m_eCurrentMoveState = EPlayerMoveState_Idle;
	m_tSpeedByInput = TGfxVec2(0, 0);

	m_eCurrentTPState = EPlayerTPState_Void;

	m_tSprite.Initialize(this, pTexture, PLAYER_INITIAL_CUTOUT, PLAYER_INITIAL_CUTOUT_SIZE, PLAYER_SCALE);

	m_tAnimationIdle.Initialize(&m_tSprite, IDLE_ANIM_INITIAL_CUTOUT, ECutOutIncrement_X_Positive, IDLE_ANIM_TIME_PER_FRAME, IDLE_ANIM_FRAME_NUMBER);
	m_tAnimationIdle.InitializeOptional(IDLE_ANIM_OPTIONAL_CUTOUT);

	m_tAnimationRunning.Initialize(&m_tSprite, RUNNING_ANIM_INITIAL_CUTOUT, ECutOutIncrement_X_Positive, RUNNING_ANIM_TIME_PER_FRAME, RUNNING_ANIM_FRAME_NUMBER);
	m_tAnimationRunning.InitializeOptional(RUNNING_ANIM_OPTIONAL_CUTOUT);

	m_tAnimationPortalTp.Initialize(&m_tSprite, PORTALTP_ANIM_INITIAL_CUTOUT, ECutOutIncrement_X_Positive, PORTALTP_ANIM_TIME_PER_FRAME, PORTALTP_ANIM_FRAME_NUMBER);

	m_tAnimationBlinkTp.Initialize(&m_tSprite, BLINKTP_ANIM_INITIAL_CUTOUT, ECutOutIncrement_X_Positive, BLINKTP_ANIM_TIME_PER_FRAME, BLINKTP_ANIM_FRAME_NUMBER);

	m_tAnimationDeath.Initialize(&m_tSprite, DEATH_ANIM_INITIAL_CUTOUT, ECutOutIncrement_X_Positive, DEATH_ANIM_TIME_PER_FRAME, DEATH_ANIM_FRAME_NUMBER);

	m_tFallingCutOut = FALLING_CUTOUT;
	m_tFallingCutOutOptional = FALLING_OPTIONAL_CUTOUT;

	m_tPortal.InitializeTransform(m_tWorldPos);
	m_tPortal.InitializeSprite(pTexture, PORTAL_INITIAL_CUTOUT, PORTAL_INITIAL_CUTOUT_SIZE, PORTAL_SCALE);
	m_tPortal.InitializeAnim(ECutOutIncrement_X_Positive, PORTAL_ANIM_TIME_PER_FRAME, PORTAL_ANIM_FRAME_NUMBER);

	TCamera::GetInstance().SetFocus(this);

	m_tBlinkVector.Initialize();

	m_pCurrentCheckPoint = nullptr;

	SetMoveIdle();
	m_bPhysical = true;
	m_bPortalActive = false;
	m_bEndOfLevelReached = false;
}

void TPlayer::Update(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber)
{
	switch (m_eLifeState)
	{
		case EPlayerLifeState_Alive:

			UpdateAlife(tTime, tBoxesCollided, iBoxesCollidedNumber);

			break;

		case EPlayerLifeState_Dying:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_tAnimationDeath.GetCycleDuration())
			{
				m_tAnimationDeath.Update();
			}
			else
			{
				SetDead();
			}

			break;

		case EPlayerLifeState_Dead:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_fDeathTravelingTime)
			{
				m_tKinematicGravity.MoveX(tTime);
				m_tKinematicGravity.MoveY(tTime);
			}
			else
			{
				SetRespawn();
			}

			break;

		case EPlayerLifeState_Respawn:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_tAnimationPortalTp.GetCycleDuration())
			{
				m_tAnimationPortalTp.Update();
			}
			else
			{
				SetAlive();
			}

			break;

		case EPlayerLifeState_ExitLevel:

			m_tTimer.Update();

			if (m_tTimer.GetTimeFromBegin() < m_tAnimationPortalTp.GetCycleDuration())
			{
				m_tAnimationPortalTp.Update();
			}
			else
			{
				m_pCurrentCheckPoint->StopRespawn();
				m_bEndOfLevelReached = true;
			}

			break;

	}
}

void TPlayer::Render()
{
	m_tBlinkVector.Render();

	m_tSprite.Render();

	m_tInterfaceWithGround.Render();

	if (m_bPortalActive)
	{
		m_tPortal.Render();
	}
}

void TPlayer::Kill()
{
	m_tTimer.Initialize();

	m_tBlinkVector.Stop();

	m_tAnimationDeath.Start();

	if (m_bPortalActive)
	{
		SetPortalInactive();
	}

	m_eLifeState = EPlayerLifeState_Dying;
}

void TPlayer::FirstSpawn()
{
	m_tSprite.Update();
	SetDead();
}

void TPlayer::ExitLevel()
{
	m_eLifeState = EPlayerLifeState_ExitLevel;
	m_tWorldPos = m_pCurrentCheckPoint->GetWorldPos();
	m_tTimer.Initialize();
	m_tAnimationPortalTp.Reverse();
	m_tAnimationPortalTp.Start();
	m_pCurrentCheckPoint->StartRespawn();
}
