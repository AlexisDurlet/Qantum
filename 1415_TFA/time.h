#ifndef  TIME_H_INCLUDED
#define	 TIME_H_INCLUDED

class TTime
{
	private:

		int m_iTimeBegin;
		int m_iTimeNow;
		int m_iTimePrevious;
		int m_iTimeDelta;
		int m_iTimeFromBegin;
	
	public:
	
		 int GetTimeBegin() const
		{
			return m_iTimeBegin;
		}
	
		 int GetTimeNow() const
		{
			return m_iTimeNow;
		}
	
		 int GetTimePrevious() const
		{
			return m_iTimePrevious;
		}
	
		 int GetTimeDelta() const
		{
			return m_iTimeDelta;
		}
	
		 float GetTimeDeltaInSec() const
		{
			return m_iTimeDelta/1000.0f;
		}
	
		 int GetTimeFromBegin() const
		{
			return m_iTimeFromBegin;
		}
	
		void Initialize();
		void Update();
};


#endif
