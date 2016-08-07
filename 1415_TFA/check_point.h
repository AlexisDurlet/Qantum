#ifndef	CHECK_POINT_H_INCLUDED
#define CHECK_POINT_H_INCLUDED

struct TGfxTexture;

class TCheckPoint : private TTrigger
{
	private:

		TAnimation m_tAnimation;
		TSprite m_tSprite;
		bool m_bActive;
		bool m_bRespawning;
		bool m_bEndCheckPoint;

	public:

		TGfxVec2 GetWorldPos()
		{
			return TTransform::GetWorldPos();
		}

		void Initialize(TGfxTexture * pTexture, const TGfxVec2 & tWorldPos);
		void Update();
		void Render();
		void StartRespawn();
		void StopRespawn();
		void SetAsEndCheckPoint()
		{
			m_bEndCheckPoint = true;
		}
};


#endif
