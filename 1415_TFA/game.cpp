#include "time.h"
#include "constants.h"
#include "constants.h"
#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "sprite.h"
#include "button.h"
#include "level.h"
#include "flib_vec2.h"
#include "transform.h"
#include "menu.h"
#include "flib.h"
#include "game.h"
#include  <cstring>

void TGame::Initialize()
{
	m_eCurrentState = EGameState_Menu;
	m_tMenu.Load("menu_main.tmx");
	m_tGlobalTime.Initialize();
}

void TGame::Update()
{
	m_tGlobalTime.Update();

	char * pFile;

	switch (m_eCurrentState)
	{
		case EGameState_Menu :

			m_tMenu.Update();

			switch (m_tMenu.GetCurrentButtonAction())
			{
				case EButtonAction_LoadMenu:

					pFile = new char[MAX_XML_DATA_SIZE + 1];
					strncpy(pFile ,m_tMenu.GetCurrentFileToLoad(), MAX_XML_DATA_SIZE);
					m_tMenu.~TMenu();
					m_tMenu.Load(pFile);
					delete[] pFile;
					pFile = nullptr;

					break;

				case EButtonAction_LoadLevel:

					m_tLevel.Load(m_tMenu.GetCurrentFileToLoad());
					m_tMenu.~TMenu();
					m_eCurrentState = EGameState_Game;

					break;

				case EButtonAction_Quit:

					GfxExitRequest();

					break;

				case EButtonAction_Void:

					break;
			}

			break;

		case EGameState_Game:

			m_tLevel.Update(m_tGlobalTime);

			if (m_tLevel.GetExitToMenu())
			{
				m_tLevel.~TLevel();
				m_tMenu.Load("menu_main.tmx");
				m_eCurrentState = EGameState_Menu;
			}

			break;
	}
}

void TGame::Render()
{
	switch (m_eCurrentState)
	{
		case EGameState_Menu:

			m_tMenu.Render();

			break;

		case EGameState_Game:

			m_tLevel.Render();

			break;
	}
}
