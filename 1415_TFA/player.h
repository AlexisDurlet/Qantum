#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

enum EPlayerMoveState : int;
enum EPlayerTPState : int;
enum EPlayerLifeState : int;
struct TGfxTexture;
class TCheckPoint;

class TPlayer : private TUoBoxPhysical
{
	private:

		EPlayerLifeState m_eLifeState;

		EPlayerMoveState m_eCurrentMoveState;
		TGfxVec2 m_tSpeedByInput;

		EPlayerTPState m_eCurrentTPState;

		TSprite m_tSprite;
		TAnimation m_tAnimationIdle;
		TAnimation m_tAnimationRunning;
		TAnimation m_tAnimationPortalTp;
		TAnimation m_tAnimationBlinkTp;
		TAnimation m_tAnimationDeath;
		TGfxVec2 m_tFallingCutOut;
		TGfxVec2 m_tFallingCutOutOptional;

		TDecorationAnimated m_tPortal;
		bool m_bPortalActive;

		TBlinkVector m_tBlinkVector;

		TCheckPoint * m_pCurrentCheckPoint;
		float m_fDeathTravelingTime;

		TTime m_tTimer;

		bool m_bEndOfLevelReached;

		TPlayer();

		void AddSpeedByInput();
		void RemoveSpeedFromInput();

		void SetMoveLeft();
		void SetMoveRight();
		void SetMoveIdle();
		void SetMoveFalling();

		void SetPortalActive();
		void SetPortalInactive();

		void SetTPVoid();
		void SetTPPortalGo();
		void SetTPPortalArr();
		void SetTPBlinkStart(const TGfxVec2 & tTarget, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		void SetTPBlinkGo();
		void SetTPBlinkArr();

		void SetDead();
		void SetRespawn();
		void SetAlive();

		void InputMove();
		void InputTP(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		void InputReaction(const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);

		void UpdateTp(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		void UpdateAlife(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);

	public:

		static TPlayer & GetInstance()
		{
			static TPlayer * tPlayer = new TPlayer();
			return *tPlayer;
		}

		void SetKinematic(const TGfxVec2 & tSpeed, const TGfxVec2 & tAcceleration)
		{
			m_tKinematicGravity.m_tSpeed = tSpeed;
			m_tKinematicGravity.m_tAcceleration = tAcceleration;
		}

		EPlayerLifeState GetLifeState() const
		{
			return m_eLifeState;
		}

		TGfxVec2 GetWorldPos() const
		{
			return TTransform::GetWorldPos();
		}

		const TUoBox* GetCastToUoBox()
		{
			return static_cast<TUoBox*>(this);
		}

		TUoBoxPhysical& GetCastToUoBoxPhys()
		{
			return static_cast<TUoBoxPhysical&>(*this);
		}

		bool GetEndOfLevelReached() const
		{
			return m_bEndOfLevelReached;
		}

		void SetCheckPoint(TCheckPoint * pCheckPoint)
		{
			m_pCurrentCheckPoint = pCheckPoint;
		}


		void Initialize(const TGfxTexture * pTexture, const TGfxVec2 & tWorldPos, const TGfxVec2 & tSpeed, const TGfxVec2 & tAcceleration);
		void Update(const TTime & tTime, const TUoBox * tBoxesCollided, const int iBoxesCollidedNumber);
		void Render();
		void Kill();
		void FirstSpawn();
		void ExitLevel();

};


#endif
