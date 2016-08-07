#ifndef E_PLAYER_STATE_H_INCLUDED
#define	E_PLAYER_STATE_H_INCLUDED

enum EPlayerMoveState : int
{
	EPlayerMoveState_Idle = 0,
	EPlayerMoveState_MovingLeft,
	EPlayerMoveState_MovingRight,
	EPlayerMoveState_Falling,
};



#endif
