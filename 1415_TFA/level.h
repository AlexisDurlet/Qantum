#ifndef	ZONE_H_INCLUDED
#define ZONE_H_INCLUDED

class TUoBox;
class TPlatform;
class TPlatformMoving;
class TTrap;
class TCheckPoint;
class TDecoration;
class TDecorationAnimated;
struct TGfxTexture;
class TTime;
class TXML_Element;
class TXML_Root;

class TLevel
{
	private:

		TButton m_tExitButton;

		TGfxTexture ** m_pTextures;
		int m_iTexturesNumber;

		TUoBox * m_pCollisionZone;
		int m_iCollisionNumber;

		TPlatform * m_pPlatforms;
		int m_iPlatformsNumber;

		TTrap * m_pTraps;
		int m_iTrapsNumber;

		TPlatformMoving * m_pPlatformsMoving;
		int m_iPlatformsMovingNumber;

		TCheckPoint * m_pCheckPoints;
		int m_iCheckPointsNumber;

		TDecoration * m_pDecorations;
		int m_iDecorationsNumber;

		TDecorationAnimated * m_pDecorationsAnimated;
		int m_iDecorationsAnimatedNumber;

		const char * m_pNextLevelFileName;

		bool m_bExitToMenu;

	public: 

		TLevel();
		~TLevel();

		 const TUoBox * GetCollisionZone() const
		{
			return m_pCollisionZone;
		}

		 int GetCollisionNumber() const
		{
			return m_iCollisionNumber;
		}

		 bool GetExitToMenu()const
		 {
			 return m_bExitToMenu;
		 }

		void Load(const char * pLevelFileName);
		void Update(const TTime & tTime);
		void Render();
};


#endif
