#include "flib_vec2.h"
#include "transform.h"
#include "menu.h"
#include "flib.h"
#include "uo_box.h"
#include "sprite.h"
#include "decoration.h"
#include "time.h"
#include "animation.h"
#include "decoration_animated.h"
#include "button.h"
#include "sll_item.h"
#include "xml_data.h"
#include "xml_search.h"
#include "xml_element.h"
#include "sll_root.h"
#include "xml_root.h"
#include "xml_attribute.h"
#include "load_utilities.h"
#include "constants.h"
#include "camera.h"

TMenu::TMenu():
m_pButtonsAction(nullptr),
m_pTextures(nullptr),
m_pButtons(nullptr),
m_pButtonsFileToLoad(nullptr),
m_pDecorations(nullptr),
m_pDecorationsAnimated(nullptr)
{

}

TMenu::~TMenu()
{
	DeleteArrayOfPointer(m_pTextures, m_iTexturesNumber);
	DeleteArray(m_pButtonsAction);
	DeleteArray(m_pButtons);
	DeleteArrayOfPointer(m_pButtonsFileToLoad, m_iButtonsNumber);
	m_pCurrentFileToLoad = nullptr;
	DeleteArray(m_pDecorations);
	DeleteArray(m_pDecorationsAnimated);
}

void TMenu::Load(const char * pMenuFileName)
{
	TXML_Root * tXML_Root = nullptr;
	tXML_Root = tXML_Root->ParseXML(pMenuFileName);
	//tXML_Root->DebugPrint();

	LoadTextures(tXML_Root, m_pTextures, m_iTexturesNumber);
	int * pTextureIndex;

	//Loading initial focus point
	TXML_Element * pFocus = tXML_Root->FindFirstChildElement("map")->FindTiledObjectGroup("focus");
	TGfxVec2 tFocusPos;
	tFocusPos.x = pFocus->FindFirstChildElement("object")->FindAttribute("x")->m_tXML_Data.GetValueNumericf();
	tFocusPos.y = pFocus->FindFirstChildElement("object")->FindAttribute("y")->m_tXML_Data.GetValueNumericf();
	CorrectCoordinates(tFocusPos, TGfxVec2(GfxGetDisplaySizeX() / 2.0f, GfxGetDisplaySizeY() / 2.0f ));
	m_tFocusPoint.InitializeTransform(tFocusPos);

	TCamera::GetInstance().SetFocus(&m_tFocusPoint);

	//Loading buttons
	TXML_Element * pButtons = ExtractAndCreateObjects(tXML_Root, m_pButtons, m_iButtonsNumber, "buttons");
	TGfxVec2 * pButtonsCoordinates = ExtractVec2(pButtons, m_iButtonsNumber, "x", "y", 1.0f, false);
	TGfxVec2 * pButtonsRadius = ExtractVec2(pButtons, m_iButtonsNumber, "width", "height", 1 / 2.0f, false);
	TGfxVec2 * pButtonsCutout = ExtractVec2(pButtons, m_iButtonsNumber, "cutout_x", "cutout_y", float(TILE_SIZE), true);
	TGfxVec2 * pButtonsPressedCutout = ExtractVec2(pButtons, m_iButtonsNumber, "pressed_cutout_x", "pressed_cutout_y", float(TILE_SIZE), true);
	int * pButtonsAction = ExtractInt(pButtons, m_iButtonsNumber, "action", true);
	m_pButtonsFileToLoad = ExtractString(pButtons, m_iButtonsNumber, "file", true);
	pTextureIndex = GetTextureIndex(pButtons, m_iButtonsNumber);

	m_pButtonsAction = new EButtonAction[m_iButtonsNumber];

	for (int i = 0; i < m_iButtonsNumber; i++)
	{
		CorrectCoordinates(pButtonsCoordinates[i], pButtonsRadius[i]);
		m_pButtons[i].InitializeButton(pButtonsCoordinates[i], pButtonsRadius[i]);
		m_pButtons[i].InitializeSprite(m_pTextures[pTextureIndex[i]], pButtonsCutout[i], pButtonsPressedCutout[i]);
		m_pButtonsAction[i] = static_cast<EButtonAction>(pButtonsAction[i]);
	}
	delete[] pButtonsCoordinates;
	delete[] pButtonsRadius;
	delete[] pButtonsCutout;
	delete[] pButtonsPressedCutout;
	delete[] pButtonsAction;
	delete[] pTextureIndex;

	//Loading decorations
	TXML_Element * pDecorations = ExtractAndCreateObjects(tXML_Root, m_pDecorations, m_iDecorationsNumber, "decorations");
	TGfxVec2 * pDecorationCoordinates = ExtractVec2(pDecorations, m_iDecorationsNumber, "x", "y", 1.0f, false);
	TGfxVec2 * pDecorationCutOutSize = ExtractVec2(pDecorations, m_iDecorationsNumber, "width", "height", 1 / SCALE, false);
	TGfxVec2 * pDecorationCutOut = ExtractVec2(pDecorations, m_iDecorationsNumber, "cutout_x", "cutout_y", float(TILE_SIZE), true);
	pTextureIndex = GetTextureIndex(pDecorations, m_iDecorationsNumber);
	for (int i = 0; i < m_iDecorationsNumber; i++)
	{
		CorrectCoordinates(pDecorationCoordinates[i], (pDecorationCutOutSize[i] / 2.0f) * SCALE);
		m_pDecorations[i].InitializeTransform(pDecorationCoordinates[i]);
		m_pDecorations[i].InitializeSprite(m_pTextures[pTextureIndex[i]], pDecorationCutOut[i], pDecorationCutOutSize[i], TGfxVec2(SCALE, SCALE));
	}
	delete[] pDecorationCoordinates;
	delete[] pDecorationCutOutSize;
	delete[] pDecorationCutOut;
	delete[] pTextureIndex;

	//Loading animated decorations

	TXML_Element * pDecorationsAnimated = ExtractAndCreateObjects(tXML_Root, m_pDecorationsAnimated, m_iDecorationsAnimatedNumber, "decorations_animated");
	TGfxVec2 * pDecorationsAnimatedCoordinates = ExtractVec2(pDecorationsAnimated, m_iDecorationsAnimatedNumber, "x", "y", 1.0f, false);
	TGfxVec2 * pDecorationsAnimatedCutOutSize = ExtractVec2(pDecorationsAnimated, m_iDecorationsAnimatedNumber, "width", "height", 1 / SCALE, false);
	TGfxVec2 * pDecorationsAnimatedCutOut = ExtractVec2(pDecorationsAnimated, m_iDecorationsAnimatedNumber, "cutout_x", "cutout_y", float(TILE_SIZE), true);
	int * pDecorationsAnimatedTimePerFrame = ExtractInt(pDecorationsAnimated, m_iDecorationsAnimatedNumber, "frame_time", true);
	int * pDecorationsAnimatedFrameNumber = ExtractInt(pDecorationsAnimated, m_iDecorationsAnimatedNumber, "frame_nb", true);
	int * pDecorationsAnimatedIncrement = ExtractInt(pDecorationsAnimated, m_iDecorationsAnimatedNumber, "increment", true);
	pTextureIndex = GetTextureIndex(pDecorationsAnimated, m_iDecorationsAnimatedNumber);

	for (int i = 0; i < m_iDecorationsAnimatedNumber; i++)
	{
		CorrectCoordinates(pDecorationsAnimatedCoordinates[i], (pDecorationsAnimatedCutOutSize[i] / 2.0f) * SCALE);
		m_pDecorationsAnimated[i].InitializeTransform(pDecorationsAnimatedCoordinates[i]);
		m_pDecorationsAnimated[i].InitializeSprite(m_pTextures[pTextureIndex[i]], pDecorationsAnimatedCutOut[i], pDecorationsAnimatedCutOutSize[i], TGfxVec2(SCALE, SCALE));
		m_pDecorationsAnimated[i].InitializeAnim(static_cast<ECutOutIncrement>(pDecorationsAnimatedIncrement[i]), pDecorationsAnimatedTimePerFrame[i], pDecorationsAnimatedFrameNumber[i]);
		m_pDecorationsAnimated[i].StartAnim();
	}
	delete[] pDecorationsAnimatedCoordinates;
	delete[] pDecorationsAnimatedCutOutSize;
	delete[] pDecorationsAnimatedCutOut;
	delete[] pDecorationsAnimatedTimePerFrame;
	delete[] pDecorationsAnimatedFrameNumber;
	delete[] pDecorationsAnimatedIncrement;
	delete[] pTextureIndex;

	//Free XML DOM
	delete tXML_Root;
}

void TMenu::Update()
{
	m_eCurrentButtonAction = EButtonAction_Void;

	for (int i = 0; i < m_iButtonsNumber; i++)
	{
		if (m_pButtons[i].Update())
		{
			m_eCurrentButtonAction = m_pButtonsAction[i];
			m_pCurrentFileToLoad = m_pButtonsFileToLoad[i];
		}
	}

	for (int i = 0; i < m_iDecorationsNumber; i++)
	{
		m_pDecorations[i].Update();
	}

	for (int i = 0; i < m_iDecorationsAnimatedNumber; i++)
	{
		m_pDecorationsAnimated[i].Update();
	}
}

void TMenu::Render()
{
	for (int i = 0; i < m_iDecorationsNumber; i++)
	{
		m_pDecorations[i].Render();
	}

	for (int i = 0; i < m_iDecorationsAnimatedNumber; i++)
	{
		m_pDecorationsAnimated[i].Render();
	}

	for (int i = 0; i < m_iButtonsNumber; i++)
	{
		m_pButtons[i].Render();
	}

}
