#ifndef UO_BOX_H_INCLUDED
#define UO_BOX_H_INCLUDED

class TUoBox : public TTransform  //unoriented box
{
	protected:

		TGfxVec2 m_tRadius;

	public:

		TUoBox();
		
		virtual void InitializeUoBox(const TGfxVec2 & tWorldPos, const TGfxVec2 & tRadius)
		{
			InitializeTransform(tWorldPos);
			m_tRadius = tRadius;
		}

		 TGfxVec2 GetRadius() const
		{
			return m_tRadius;
		}
};


#endif
