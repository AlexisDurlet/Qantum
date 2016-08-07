#ifndef MENU_H_INCLUDED
#define	MENU_H_INCLUDED

class TButton;
class TDecoration;
class TDecorationAnimated;
struct TGfxTexture;

enum EButtonAction
{
	EButtonAction_LoadMenu = 0,
	EButtonAction_LoadLevel,
	EButtonAction_Quit,
	EButtonAction_Void,
};

class TMenu
{
	private:

		TTransform m_tFocusPoint;

		EButtonAction m_eCurrentButtonAction;
		EButtonAction * m_pButtonsAction;

		TGfxTexture ** m_pTextures;
		int m_iTexturesNumber;

		TButton * m_pButtons;
		int m_iButtonsNumber;
		const char ** m_pButtonsFileToLoad;
		const char * m_pCurrentFileToLoad;

		TDecoration * m_pDecorations;
		int m_iDecorationsNumber;

		TDecorationAnimated * m_pDecorationsAnimated;
		int m_iDecorationsAnimatedNumber;

	public:

		TMenu();
		~TMenu();

		EButtonAction GetCurrentButtonAction() const 
		{
			return m_eCurrentButtonAction;
		}

		const char * GetCurrentFileToLoad() const
		{
			return m_pCurrentFileToLoad;
		}

		void Load(const char * pMenuFileName);
		void Update();
		void Render();
};


#endif
