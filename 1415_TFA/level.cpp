#include "constants.h"
#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "sprite.h"
#include "button.h"
#include "level.h"
#include "flib_vec2.h"
#include "transform.h"
#include "uo_box.h"
#include "uo_box_collider.h"
#include "sprite.h"
#include "platform.h"
#include "kinematic.h"
#include "kinematic_gravity.h"
#include "time.h"
#include "kinematic_cycle.h"
#include "trigger.h"
#include "time.h"
#include "trigger.h"
#include "trigger_linked.h"
#include "uo_box_physical.h"
#include "platform_moving.h"
#include "decoration.h"
#include "animation.h"
#include "decoration_animated.h"
#include "trap.h"
#include "e_orientation.h"
#include "flib.h"
#include "blink_vector.h"
#include "player.h"
#include "check_point.h"
#include "sll_item.h"
#include "xml_data.h"
#include "xml_search.h"
#include "xml_element.h"
#include "sll_root.h"
#include "xml_root.h"
#include "xml_attribute.h"
#include "load_utilities.h"
#include "camera.h"
#include <cstring>

TLevel::TLevel() :
	m_pCollisionZone(nullptr),
	m_pPlatforms(nullptr)
{

}

TLevel::~TLevel()
{
	DeleteArray(m_pPlatforms);
	DeleteArray(m_pPlatformsMoving);
	DeleteArray(m_pTraps);
	DeleteArray(m_pCheckPoints);
	DeleteArrayOfPointer(m_pTextures, m_iTexturesNumber);

	TPlayer::GetInstance().SetCheckPoint(nullptr);
}


void TLevel::Load(const char * pLevelFileName)
{
	TXML_Root * tXML_Root = nullptr;
	tXML_Root = tXML_Root->ParseXML(pLevelFileName);
	//tXML_Root->DebugPrint();
	
	LoadTextures(tXML_Root, m_pTextures, m_iTexturesNumber);
	int * pTextureIndex;
	
	//Loading platforms
	TXML_Element * pPlatforms = ExtractAndCreateObjects(tXML_Root, m_pPlatforms, m_iPlatformsNumber, "platforms");
	TGfxVec2 * pPlatformsCoordinates = ExtractVec2(pPlatforms, m_iPlatformsNumber, "x", "y", 1.0f, false);
	TGfxVec2 * pPlatformsRadius = ExtractVec2(pPlatforms, m_iPlatformsNumber, "width", "height", 1 / 2.0f, false);
	pTextureIndex = GetTextureIndex(pPlatforms, m_iPlatformsNumber);
	
	for (int i = 0; i < m_iPlatformsNumber; i++)
	{
		CorrectCoordinates(pPlatformsCoordinates[i], pPlatformsRadius[i]);
		m_pPlatforms[i].Initialize(m_pTextures[pTextureIndex[i]], pPlatformsCoordinates[i], pPlatformsRadius[i]);
	}
	delete[] pPlatformsCoordinates;
	delete[] pPlatformsRadius;
	delete[] pTextureIndex;
	
	
	//Loading moving platforms
	TXML_Element * pPlatformsMoving = ExtractAndCreateObjects(tXML_Root, m_pPlatformsMoving, m_iPlatformsMovingNumber, "platforms_moving");
	TGfxVec2 * pPlatformsMovingCoordinates = ExtractVec2(pPlatformsMoving, m_iPlatformsMovingNumber, "x", "y", 1.0f, false);
	TGfxVec2 * pPlatformsMovingRadius = ExtractVec2(pPlatformsMoving, m_iPlatformsMovingNumber, "width", "height", 1 / 2.0f, false);
	TGfxVec2 * pPlatformsMovingDestinationCoordinates = ExtractVec2(pPlatformsMoving, m_iPlatformsMovingNumber, "dest_x", "dest_y", TILE_SIZE * SCALE, true);
	int * pPlatformsMovingStopTime = ExtractInt(pPlatformsMoving, m_iPlatformsMovingNumber, "stop_time", true);
	float * pPlatformsMovingSpeed = ExtractFloat(pPlatformsMoving, m_iPlatformsMovingNumber, "speed", 1.0f, true);
	pTextureIndex = GetTextureIndex(pPlatformsMoving, m_iPlatformsMovingNumber);
	
	for (int i = 0; i < m_iPlatformsMovingNumber; i++)
	{
		CorrectCoordinates(pPlatformsMovingCoordinates[i], pPlatformsMovingRadius[i]);
		CorrectCoordinates(pPlatformsMovingDestinationCoordinates[i], pPlatformsMovingRadius[i]);
		m_pPlatformsMoving[i].Initialize(m_pTextures[pTextureIndex[i]], pPlatformsMovingCoordinates[i], pPlatformsMovingRadius[i]);
		m_pPlatformsMoving[i].InitializeMovement(pPlatformsMovingDestinationCoordinates[i], pPlatformsMovingStopTime[i], pPlatformsMovingSpeed[i]);
	}
	delete[] pPlatformsMovingCoordinates;
	delete[] pPlatformsMovingRadius;
	delete[] pPlatformsMovingDestinationCoordinates;
	delete[] pPlatformsMovingStopTime;
	delete[] pPlatformsMovingSpeed;
	delete[] pTextureIndex;
	
	
	//Loading traps
	TXML_Element * pTraps = ExtractAndCreateObjects(tXML_Root, m_pTraps, m_iTrapsNumber, "traps");
	TGfxVec2 * pTrapsCoordinates = ExtractVec2(pTraps, m_iTrapsNumber, "x", "y", 1.0f, false);
	float * pTrapsLenght = ExtractFloat(pTraps, m_iTrapsNumber, "lenght", TILE_SIZE * SCALE, true);
	int * pOrientation = ExtractInt(pTraps, m_iTrapsNumber, "orientation", true);
	pTextureIndex = GetTextureIndex(pTraps, m_iTrapsNumber);
	
	for (int i = 0; i < m_iTrapsNumber; i++)
	{
		CorrectCoordinates(pTrapsCoordinates[i], TRAP_BASE_RADIUS);
		m_pTraps[i].InitializeTrap(m_pTextures[pTextureIndex[i]], pTrapsCoordinates[i], static_cast<EOrientation>(pOrientation[i]), pTrapsLenght[i]);
	}
	delete[] pTrapsCoordinates;
	delete[] pTrapsLenght;
	delete[] pOrientation;
	delete[] pTextureIndex;
	
	
	//Loading check points
	TXML_Element * pCheckPoints = ExtractAndCreateObjects(tXML_Root, m_pCheckPoints, m_iCheckPointsNumber, "checkpoints");
	TGfxVec2 * pCheckPointsCoordinates = ExtractVec2(pCheckPoints, m_iCheckPointsNumber, "x", "y", 1.0f, false);
	pTextureIndex = GetTextureIndex(pCheckPoints, m_iCheckPointsNumber);
	
	for (int i = 0; i < m_iCheckPointsNumber; i++)
	{
		CorrectCoordinates(pCheckPointsCoordinates[i], CHECKPOINT_RADIUS);
		m_pCheckPoints[i].Initialize(m_pTextures[pTextureIndex[i]], pCheckPointsCoordinates[i]);
	}
	delete[] pCheckPointsCoordinates;
	delete[] pTextureIndex;
	
	m_pCheckPoints[m_iCheckPointsNumber - 1].SetAsEndCheckPoint();

	//Loading decorations
	TXML_Element * pDecorations = ExtractAndCreateObjects(tXML_Root, m_pDecorations, m_iDecorationsNumber, "decorations");
	TGfxVec2 * pDecorationCoordinates = ExtractVec2(pDecorations, m_iDecorationsNumber, "x", "y", 1.0f, false);
	TGfxVec2 * pDecorationCutOutSize = ExtractVec2(pDecorations, m_iDecorationsNumber, "width", "height", 1/SCALE, false);
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
	
	//Loading name of next level File

	TXML_Element * pNextLevel = tXML_Root->FindTiledObjectGroup("next_level");
	const char ** pNextLevelName = ExtractString(pNextLevel, 1, "name", false);
	m_pNextLevelFileName = pNextLevelName[0];

	delete[]pNextLevelName;
	
	//Loading player
	TXML_Element * pPlayer = tXML_Root->FindTiledObjectGroup("player");
	TGfxVec2 * pPlayerCoordinates = ExtractVec2(pPlayer, 1, "x", "y", 1.0f, false);
	pTextureIndex = GetTextureIndex(pPlayer, 1);
	
	CorrectCoordinates(pPlayerCoordinates[0], PLAYER_RADIUS);
	
	TPlayer::GetInstance().Initialize(m_pTextures[pTextureIndex[0]], pPlayerCoordinates[0], TGfxVec2(0, 0), TGfxVec2(0, 0));
	delete[] pPlayerCoordinates;
	delete[] pTextureIndex;
	
	TCamera::GetInstance().SetFocus(TPlayer::GetInstance().GetCastToUoBox());
	
	//Spawn Player
	TPlayer::GetInstance().SetCheckPoint(&m_pCheckPoints[0]);
	TPlayer::GetInstance().FirstSpawn();

	//Loading Exit Button

	m_tExitButton.InitializeButton(TPlayer::GetInstance().GetWorldPos() + TGfxVec2(GfxGetDisplaySizeX() / 2.0f, -GfxGetDisplaySizeY() / 2.0f), TGfxVec2(TILE_SIZE * SCALE, TILE_SIZE * SCALE));
	m_tExitButton.InitializeSprite(m_pTextures[0], BUTTON_EXIT_CUTOUT, BUTTON_EXIT_CUTOUT);

	m_bExitToMenu = false;
	
	//Free XML DOM
	delete tXML_Root;
	
	//Collision zone
	m_iCollisionNumber = m_iPlatformsNumber + m_iPlatformsMovingNumber + m_iTrapsNumber;
	m_pCollisionZone = new TUoBox[m_iCollisionNumber];
	
	for (int i = 0; i < m_iPlatformsNumber; i++)
	{
		m_pCollisionZone[i] = *m_pPlatforms[i].GetCastToUoBox();
	}
	for (int i = 0; i < m_iPlatformsMovingNumber; i++)
	{
		m_pCollisionZone[m_iPlatformsNumber + i] = *m_pPlatformsMoving[i].GetCastToUoBox();
	}
	for (int i = 0; i < m_iTrapsNumber; i++)
	{
		m_pCollisionZone[m_iPlatformsNumber + m_iPlatformsMovingNumber + i] = *m_pTraps[i].GetCastToUoBox();
	}
}

void TLevel::Update(const TTime & tTime)
{
	for (int i = 0; i < m_iPlatformsNumber; i++)
	{
		m_pPlatforms[i].Update(tTime);
	}
	
	for (int i = 0; i < m_iPlatformsMovingNumber; i++)
	{
		m_pPlatformsMoving[i].Update(tTime);
		m_pCollisionZone[m_iPlatformsNumber + i] = *m_pPlatformsMoving[i].GetCastToUoBox();
	}
	
	for (int i = 0; i < m_iTrapsNumber; i++)
	{
		m_pTraps[i].Update(tTime);
	}
	
	for (int i = 0; i < m_iCheckPointsNumber; i++)
	{
		m_pCheckPoints[i].Update();
	}
	
	for (int i = 0; i < m_iDecorationsNumber; i++)
	{
		m_pDecorations[i].Update();
	}
	
	for (int i = 0; i < m_iDecorationsAnimatedNumber; i++)
	{
		m_pDecorationsAnimated[i].Update();
	}
	
	TPlayer::GetInstance().Update(tTime, m_pCollisionZone, m_iCollisionNumber);

	if (TPlayer::GetInstance().GetEndOfLevelReached())
	{
		char * pFile = new char[MAX_XML_DATA_SIZE + 1];
		strncpy(pFile, m_pNextLevelFileName, MAX_XML_DATA_SIZE);
		this->~TLevel();
		this->Load(pFile);
		delete[] pFile;
		pFile = nullptr;
	}

	m_tExitButton.SetPos(TPlayer::GetInstance().GetWorldPos() + TGfxVec2(GfxGetDisplaySizeX() / 2.0f , -GfxGetDisplaySizeY() / 2.0f));
	if (m_tExitButton.Update())
	{
		m_bExitToMenu = true;
	}

}

void TLevel::Render()
{
	for (int i = 0; i < m_iDecorationsNumber; i++)
	{
		m_pDecorations[i].Render();
	}
	
	for (int i = 0; i < m_iDecorationsAnimatedNumber; i++)
	{
		m_pDecorationsAnimated[i].Render();
	}
	
	for (int i = 0; i < m_iCheckPointsNumber; i++)
	{
		m_pCheckPoints[i].Render();
	}
	
	for (int i = 0; i < m_iPlatformsNumber; i++)
	{
		m_pPlatforms[i].Render();
	}
	
	for (int i = 0; i < m_iPlatformsMovingNumber; i++)
	{
		m_pPlatformsMoving[i].Render();
	}
	
	for (int i = 0; i < m_iTrapsNumber; i++)
	{
		m_pTraps[i].Render();
	}
	
	TPlayer::GetInstance().Render();

	m_tExitButton.Render();
}
