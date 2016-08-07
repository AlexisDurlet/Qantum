#ifndef	CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

class TTransform;

class TCamera
{
	private:

		TCamera();

		const TTransform * m_pFocus;

	public:

		static TCamera & GetInstance()
		{
			static TCamera * tCamera = new TCamera();
			return * tCamera;
		}

		void SetFocus(const TTransform * pTransformFocus)
		{
			m_pFocus = pTransformFocus;
		}

		TGfxVec2 ConvertWorldPosToScreenPos(const TGfxVec2 & tWorldPos) const;
		TGfxVec2 ConvertScreenPosToWorldPos(const TGfxVec2 & tScreenPos) const;

		TGfxVec2 GetScreenPos(const TTransform & tTransform) const;

};


#endif
