#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

class TTransform 
{
	friend class TKinematic;

	protected:

		TGfxVec2 m_tWorldPos;

	public:

		TTransform();
		
		void InitializeTransform(const TGfxVec2 & tWorldPos);

		 TGfxVec2 GetWorldPos() const
		{
			return m_tWorldPos;
		}
};


#endif
