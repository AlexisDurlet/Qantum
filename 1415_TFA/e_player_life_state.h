#ifndef	E_PLAYER_LIFE_STATE
#define E_PLAYER_LIFE_STATE

enum EPlayerLifeState : int
{
	EPlayerLifeState_Alive = 0,
	EPlayerLifeState_Dying,
	EPlayerLifeState_Dead,
	EPlayerLifeState_Respawn,
	EPlayerLifeState_ExitLevel,
};

#endif
