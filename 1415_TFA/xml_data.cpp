#include "xml_data.h"
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "math.h"
#include "flib.h"
#include "constants.h"

TXML_Data::TXML_Data():
m_pName(new char[MAX_XML_DATA_SIZE + 1]),
m_pValue(new char[MAX_XML_DATA_SIZE + 1])
{
	for (int i = 0; i < MAX_XML_DATA_SIZE + 1 ; i++)
	{
		m_pName[i] = '\0';
		m_pValue[i] = '\0';
	}
}

TXML_Data::~TXML_Data()
{
	delete[] m_pName;
	delete[] m_pValue;

	m_pName = nullptr;
	m_pValue = nullptr;
}

bool TXML_Data::IsValueNumeric()
{
	if (m_pValue[0] == '"' || m_pValue[0] == '\'')
	{
		return false;
	}

	return true;
}

int TXML_Data::GetValueNumerici()
{
	if (IsValueNumeric())
	{
		return atoi(m_pValue);
	}
	else
	{
		return atoi(&m_pValue[1]);
	}

}

float TXML_Data::GetValueNumericf()
{
	if (IsValueNumeric())
	{
		return strtof(m_pValue, nullptr);
	}
	else
	{
		return strtof(&m_pValue[1], nullptr);
	}
}

const char * TXML_Data::GetValueString()
{
	char * pString = new char[MAX_XML_DATA_SIZE + 1];
	for (int i = 0; i < MAX_XML_DATA_SIZE + 1; i++)
	{
		pString[i] = '\0';
	}


	int iSize;
	char * pStart;
	char * pEnd;

	if (m_pValue[0] == '"')
	{
		pStart = m_pValue + 1;
		pEnd = strchr(m_pValue + 1, '"') - 1;

		if (pEnd == nullptr)
		{
			pEnd = strchr(m_pValue + 1, '\0');
		}
	}
	else if (m_pValue[0] == '\'')
	{
		pStart = m_pValue + 1;
		pEnd = strchr(m_pValue + 1, '\'') - 1;

		if (pEnd == nullptr)
		{
			pEnd = strchr(m_pValue + 1, '\0');
		}
	}
	else
	{
		pStart = m_pValue;
		pEnd = strchr(m_pValue + 1, '\0');
	}

	iSize = (pEnd - pStart) + 1;

	strncpy(pString, pStart, iSize);

	return pString;
}

char * TXML_Data::IgnoreBlanks(char * pChar, int iParsingSign)
{
	iParsingSign = static_cast<int>(copysignf(1.0f, static_cast<float>(iParsingSign)));

	char * pNextChar = pChar + iParsingSign;

	for (int i = 0;; i += iParsingSign)
	{
		if (!isspace(pNextChar[i]))
		{
			return &pNextChar[i];
		}
	}
}

char * TXML_Data::FindBlank(char * pChar, int iParsingSign)
{
	iParsingSign = static_cast<int>(copysignf(1.0f, static_cast<float>(iParsingSign)));

	char * pNextChar = pChar + iParsingSign;

	for (int i = 0;; i += iParsingSign)
	{
		if (isspace(pNextChar[i]))
		{
			return &pNextChar[i];
		}
	}
}

void TXML_Data::BoundariesStrncpyMaxed(char * pStart, char * pEnd, char * pTarget, int iMaxSize, const char pWarningParam[])
{
	int iSize = pEnd - pStart;

	if (iSize > iMaxSize)
	{
		GfxDbgPrintf("WARNING : ");
		GfxDbgPrintf(pWarningParam);
		GfxDbgPrintf(" exceeds max size, truncation will occur \n");
		iSize = iMaxSize;
	}

	strncpy(pTarget, pStart, iSize);
}

bool TXML_Data::TagToIgnore(const char * pTagOpening, const char * pTagClosing, const char pIgnoreStartString[], const char pIgnoreEndString[])
{
	const int iStartCharsNumber = strlen(pIgnoreStartString);
	const int iEndCharsNumber = strlen(pIgnoreEndString);

	const char * pTagLastChars = pTagClosing - iEndCharsNumber;

	char * pCheckTagStart = new char[iStartCharsNumber + 1];
	char * pCheckTagEnd = new char[iEndCharsNumber + 1];

	strncpy(pCheckTagStart, pTagOpening + 1, iStartCharsNumber);
	pCheckTagStart[iStartCharsNumber] = '\0';

	strncpy(pCheckTagEnd, pTagLastChars, iEndCharsNumber);
	pCheckTagEnd[iEndCharsNumber] = '\0';

	const bool bIgnore = strcmp(pCheckTagStart, pIgnoreStartString) == 0 && strcmp(pCheckTagEnd, pIgnoreEndString) == 0;

	if (bIgnore)
	{
		delete[] pCheckTagStart;
		delete[] pCheckTagEnd;
		return true;
	}

	delete[] pCheckTagStart;
	delete[] pCheckTagEnd;
	return false;
}

void TXML_Data::FindNextTag(char * pCurrentTagEnd, char *& pNextTagOpening, char *& pNextTagClosing)
{
	bool bValidTag = true;

	//SEARCH START OF NEXT TAG

	char * pChar = strchr(pCurrentTagEnd, '<');

	if (pChar == nullptr) //end of file found
	{
		pNextTagOpening = strchr(pCurrentTagEnd, '\0');
		pNextTagClosing = pNextTagOpening;

		bValidTag = false;
	}
	else
	{
		pNextTagOpening = pChar;

		//SEARCH END OF NEXT TAG

		pChar = strchr(pNextTagOpening, '>');

		if (pChar == nullptr) //error end of file found before closing
		{
			pNextTagOpening = nullptr;
			pNextTagClosing = nullptr;
			GfxDbgPrintf("ERROR : TAG NOT CLOSED");

			bValidTag = false;
		}
		else
		{
			pNextTagClosing = pChar;

			pChar = strchr(pNextTagOpening + 1, '<');
			if (pChar != nullptr && pChar < pNextTagClosing) //error new tag oppened before closing
			{
				pNextTagOpening = nullptr;
				pNextTagClosing = nullptr;
				GfxDbgPrintf("ERROR : TAG NOT CLOSED");

				bValidTag = false;
			}
		}
	}

	//CHECK IF TAG IS TO BE IGNORED AND IF YES FIND NEXT ONE

	if (bValidTag
		&& (TagToIgnore(pNextTagOpening, pNextTagClosing, "?xml", "?")
			|| TagToIgnore(pNextTagOpening, pNextTagClosing, "!--", "--")))
	{
		char * pTempNextTagClosing = pNextTagClosing;
		FindNextTag(pTempNextTagClosing, pNextTagOpening, pNextTagClosing);
	}
}

