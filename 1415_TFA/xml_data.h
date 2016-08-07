#ifndef	XML_DATA_INCLUDED
#define XML_DATA_INCLUDED

class TXML_Data
{
	private:

		char *  m_pName;
		char *  m_pValue;

	public:

		TXML_Data();
		~TXML_Data();

		char * GetNameArray()
		{
			return m_pName;
		}

		char * GetValueArray()
		{
			return m_pValue;
		}


		bool IsValueNumeric();
		int GetValueNumerici();
		float GetValueNumericf();
		const char * GetValueString();

		
		//Parsing methods needed in both element and attribute

		//Return adress of first non blank char after(iParsingSign > 0) or before(iParsingSign < 0) pChar.
		static char * IgnoreBlanks(char * pChar, int iParsingSign);

		//Return adress of first blank occurence after(iParsingSign > 0) or before(iParsingSign < 0) pChar.
		static char * FindBlank(char * pChar, int iParsingSign);

		//Copy string between boundaries pStart and pEnd in pTarget and truncate it if exceeding iMaxSize, while displaying
		//a warning including the desired name pWarningParam.
		static void BoundariesStrncpyMaxed(char * pStart, char * pEnd, char * pTarget, int iMaxSize, const char pWarningParam[]);

		//Return true if the tag begin by the pIgnoreStartString and end by the pIgnoreEndString
		static bool TagToIgnore(const char * pTagOpening, const char * pTagClosing, const char pIgnoreStartString[], const char pIgnoreEndString[]);

		//Find next tag and store the adresses of openning and closing char in pNextTagOpening and pNextTagClosing.
		//If no tag is found, Store end of file in  both pNextTagOpening and pNextTagClosing.
		//If tag is oppened but not closed, store nullptr in both pNextTagOpening and pNextTagClosing and print error.
		//If tag is to be ignored (comments or version info) function call itself again.
		static void FindNextTag(char * pCurrentTagEnd, char *& pNextTagOpening, char *& pNextTagClosing);
};


#endif
