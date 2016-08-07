#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "kinematic.h"
#include "trigger.h"
#include "trigger_linked.h"
#include "sprite.h"
#include "decoration.h"
#include "time.h"
#include "animation.h"
#include "decoration_animated.h"
#include "platform.h"
#include "trap.h"
#include "constants.h"
#include "e_orientation.h"
#include "flib.h"
#include "kinematic.h"
#include "kinematic_gravity.h"
#include "uo_box_physical.h"
#include "blink_vector.h"
#include "player.h"
#include "e_player_life_state.h"

void TTrap::InitializeTrap(const TGfxTexture * pTexture, const TGfxVec2 & tWorldPos, const EOrientation eOrientation, const float fLaserLenght)
{
	//Base
	InitializeUoBox(tWorldPos, TRAP_BASE_RADIUS);
	m_tSprite.Initialize(this, pTexture, TRAP_BASE_CUTOUT, TRAP_BASE_CUTOUT_SIZE, PLAYER_SCALE);

	//Laser hitbox and laser end
	switch (eOrientation)
	{
		case EOrientation_Left:

			m_tSprite.Rotate(EOrientation_Left);

			m_tLaserHitBox.Link(this, EBoxSide_Left, TGfxVec2(fLaserLenght / 2.0f, TRAP_LASER_WIDTH));

			m_tLaserEnd.InitializeTransform(m_tLaserHitBox.GetWorldPos() + TGfxVec2(-((fLaserLenght / 2.0f) - 16), 0));

			break;

		case EOrientation_Right:

			m_tSprite.Rotate(EOrientation_Right);

			m_tLaserHitBox.Link(this, EBoxSide_Right, TGfxVec2(fLaserLenght / 2.0f, TRAP_LASER_WIDTH / 2.0f));

			m_tLaserEnd.InitializeTransform(m_tLaserHitBox.GetWorldPos() + TGfxVec2((fLaserLenght / 2.0f) - 16, 0));

			break;

		case EOrientation_Up:

			m_tSprite.Flip(EOrientation_Up);

			m_tLaserHitBox.Link(this, EBoxSide_Up, TGfxVec2(TRAP_LASER_WIDTH / 2.0f, fLaserLenght / 2.0f));

			m_tLaserEnd.InitializeTransform(m_tLaserHitBox.GetWorldPos() + TGfxVec2(0, (fLaserLenght / 2.0f) - 16));

			break;

		case EOrientation_Down:

			m_tLaserHitBox.Link(this, EBoxSide_Down, TGfxVec2(TRAP_LASER_WIDTH / 2.0f, fLaserLenght / 2.0f));

			m_tLaserEnd.InitializeTransform(m_tLaserHitBox.GetWorldPos() + TGfxVec2(0, -(fLaserLenght / 2.0f) + 16));

			break;
	}

	m_tLaserEnd.InitializeSprite(pTexture, TRAP_LASER_END_INITIAL_CUTOUT, TRAP_LASER_END_CUTOUT_SIZE, PLAYER_SCALE);
	m_tLaserEnd.m_tSprite.Rotate(eOrientation);
	m_tLaserEnd.InitializeAnim(ECutOutIncrement_Y_Positive, TRAP_LASER_END_TIME_PER_FRAME, TRAP_LASER_END_FRAME_NUMBER);

	//laser 
	m_tLaserBody.InitializeTransform(m_tLaserHitBox.GetWorldPos());
	m_tLaserBody.InitializeSprite(pTexture, TRAP_LASER_CUTOUT, TGfxVec2(TRAP_LASER_CUTOUT_SIZE_X, fLaserLenght * 2), PLAYER_SCALE);
	m_tLaserBody.m_tSprite.Rotate(eOrientation);

	m_tLaserEnd.StartAnim();
}

void TTrap::Update(const TTime & tTime)
{
	TPlatform::Update(tTime);
	m_tLaserHitBox.Update();
	m_tLaserBody.Update();
	m_tLaserEnd.Update();

	if (m_tLaserHitBox.CheckTrigger(*TPlayer::GetInstance().GetCastToUoBox())
		&& TPlayer::GetInstance().GetLifeState() == EPlayerLifeState_Alive)
	{
		TPlayer::GetInstance().Kill();
	}
}

void TTrap::Render()
{
	TPlatform::Render();
	m_tLaserHitBox.Render();
	m_tLaserBody.Render();
	m_tLaserEnd.Render();
}

