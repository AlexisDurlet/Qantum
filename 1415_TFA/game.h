#ifndef	GAME_H_INCLUDED
#define GAME_H_INCLUDED

class TLevel;
class TMenu;

enum EGameState
{
	EGameState_Menu = 0,
	EGameState_Game,
};

class TGame
{
	private:

		TTime m_tGlobalTime;

		TLevel m_tLevel;
		TMenu  m_tMenu;

		EGameState m_eCurrentState;

	public:

		void Initialize();
		void Update();
		void Render();

	
};


#endif
