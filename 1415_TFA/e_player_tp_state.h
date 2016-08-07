#ifndef	 E_PLAYER_TP_STATE_H_INCLUDED
#define  E_PLAYER_TP_STATE_H_INCLUDED

enum EPlayerTPState : int
{
	EPlayerTPState_Void = 0,
	EPlayerTPState_PortalTPGo,
	EPlayerTPState_PortalTPArr,
	EPlayerTPState_BlinkTPStart,
	EPlayerTPState_BlinkTPCancel,
	EPlayerTPState_BlinkTPGo,
	EPlayerTPState_BlinkTPArr,
};

#endif
