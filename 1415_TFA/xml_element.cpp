#include "sll_item.h"
#include "xml_data.h"
#include "xml_search.h"
#include "xml_element.h"
#include "sll_root.h"
#include "xml_attribute.h"
#include "constants.h"
#include <cstring>
#include <cctype>
#include "flib.h"

TXML_Element::TXML_Element():
	m_pAttributeList(new TSLL_Root<TXML_Attribute>()),
	m_pElementChildList(new TSLL_Root<TXML_Element>())
{

}


TXML_Element::~TXML_Element()
{
	delete m_pAttributeList;
	delete m_pElementChildList;
	m_pAttributeList = nullptr;
	m_pElementChildList = nullptr;
	m_tXML_Data.~TXML_Data();
}

void TXML_Element::ExtractElementName(char * pChar, char * pElementName)
{
	bool bParsingElementName = true;
	int iIndex = 0;

	while (bParsingElementName)
	{
		if (pChar[iIndex] != '>' && pChar[iIndex] != '/' && !(isspace(pChar[iIndex])))
		{
			iIndex++;
		}
		else
		{
			bParsingElementName = false;
		}
	}

	m_tXML_Data.BoundariesStrncpyMaxed(pChar, &pChar[iIndex], pElementName, MAX_XML_DATA_SIZE, "Element Name");
}

bool TXML_Element::CheckElementValue(char * pTagClosing, char * pNextTagOpening)
{
	if (m_pElementChildList->GetCount() == 0 && m_pAttributeList->GetCount() == 0)
	{
		if (!isspace(pTagClosing[1]) && pTagClosing[1] != '<')
		{
			m_tXML_Data.BoundariesStrncpyMaxed(pTagClosing + 1, pNextTagOpening, m_tXML_Data.GetValueArray(), MAX_XML_DATA_SIZE, "Element Value");
		}
	}

	return false;
}

void TXML_Element::ExtractAttributes(char * pTagOpening, char * pTagClosing)
{
	char * pFirstEqual = pTagOpening;
	bool bNoMoreAttribute = false;

	while (!bNoMoreAttribute)
	{
		//check if there is any attribute left
		pFirstEqual = strchr(pFirstEqual + 1, '=');

		if (pFirstEqual == nullptr || pFirstEqual > pTagClosing)
		{
			bNoMoreAttribute = true;
			break;
		}

		//create new attribute item to store extracted attribute
		TXML_Attribute * pAttribute = new TXML_Attribute();
		pAttribute->ExtractAttributeName(pFirstEqual);
		pAttribute->ExtractAttributeValue(pFirstEqual);

		//append it to the XML element
		m_pAttributeList->Append(pAttribute);
	}
}

void TXML_Element::SearchTiledProperty(TXML_Element * pProperty, TXML_Search<TXML_Element> & tSearchResult)
{
	if (tSearchResult.m_tItemFound == nullptr)
	{
		const char * pPropertyName = pProperty->FindAttribute("name")->m_tXML_Data.GetValueString();

		if (strcmp(pPropertyName, tSearchResult.m_pNameToSearch) == 0)
		{
			tSearchResult.m_tItemFound = pProperty;
		}

		delete[] pPropertyName;
	}

}

template<class T>
void TXML_Element::SearchName(T * pSearched, TXML_Search<T> & tSearchResult)
{
	if (tSearchResult.m_tItemFound == nullptr)
	{
		if (strcmp(pSearched->m_tXML_Data.GetNameArray(), tSearchResult.m_pNameToSearch) == 0)
		{
			tSearchResult.m_tItemFound = pSearched;
		}
	}
}


char * TXML_Element::ScanElement(char * pChar, bool & bError)
{
	//find tag to scan
	char * pTagOpening = nullptr;
	char * pTagClosing = nullptr;
	m_tXML_Data.FindNextTag(pChar, pTagOpening, pTagClosing);

	if (pTagOpening == nullptr || pTagOpening[0] == '\0')
	{
		if (pTagOpening == nullptr)
		{
			bError = true;
		}

		return strchr(pChar, '\0');
	}

	pChar = pTagOpening + 1;


	//STEP 1 CURRENT TAG

	//error if closing tag
	if (pChar[0] == '/')
	{
		GfxDbgPrintf("ERROR : FOUND A LONE CLOSING TAG");
		bError = true;
		return strchr(pChar, '\0');
	}

	//extract element name
	ExtractElementName(pChar, m_tXML_Data.GetNameArray());

	//extract attributes
	ExtractAttributes(pTagOpening, pTagClosing);

	//auto closing tag -> end of element -> return pointer to after tag
	if (pTagClosing[-1] == '/')
	{
		return pTagClosing + 1;
	}

	//STEP 2 FIND NEXT TAG

	char * pNextTagOpening = nullptr;
	char * pNextTagClosing = nullptr;

	m_tXML_Data.FindNextTag(pTagClosing + 1, pNextTagOpening, pNextTagClosing);
	if (pNextTagOpening == nullptr || pNextTagOpening[0] == '\0')
	{
		if (pTagOpening == nullptr)
		{
			bError = true;
		}

		return strchr(pChar, '\0');
	}

	//STEP 3 SCAN NEXT TAG

	//child elements
	bool bChildElement;
	char * pCharChildElement = pTagClosing + 1;

	do
	{
		bChildElement = pNextTagOpening[1] != '/';

		if (bChildElement)
		{
			TXML_Element * tChildElement = new TXML_Element();
			pCharChildElement = tChildElement->ScanElement(pCharChildElement, bError);
			m_pElementChildList->Append(tChildElement);

			m_tXML_Data.FindNextTag(pCharChildElement, pNextTagOpening, pNextTagClosing);
			if (pNextTagOpening == nullptr || pNextTagClosing[0] == '\0')
			{
				if (pTagOpening == nullptr)
				{
					bError = true;
				}

				return strchr(pChar, '\0');
			}
		}

	} while (bChildElement);

	//closing tag
	if (pNextTagOpening[1] == '/')
	{
		char * pClosingTagElementName = new char[MAX_XML_DATA_SIZE + 1];
		for (int i = 0; i < MAX_XML_DATA_SIZE + 1; i++)
		{
			pClosingTagElementName[i] = '\0';
		}

		ExtractElementName(pNextTagOpening + 2, pClosingTagElementName);

		bool bSameElement = strcmp(m_tXML_Data.GetNameArray(), pClosingTagElementName) == 0;

		delete[] pClosingTagElementName;

		if (bSameElement)
		{
			CheckElementValue(pTagClosing, pNextTagOpening);
			return pNextTagClosing + 1;
		}
		else
		{
			GfxDbgPrintf("ERROR : CLOSING TAG DO NOT MATCH ELEMENT");
			return strchr(pChar, '\0');
		}
	}

	return strchr(pChar, '\0');
}

TXML_Element * TXML_Element::FindFirstChildElement(const char * pName)
{
	TXML_Search<TXML_Element> tSearchResult;
	tSearchResult.m_pNameToSearch = pName;
	m_pElementChildList->Iterate(SearchName<TXML_Element>, tSearchResult);
	
	return tSearchResult.m_tItemFound;
}

TXML_Element * TXML_Element::FindNextSiblingElement(const char * pName)
{
	TXML_Search<TXML_Element> tSearchResult;
	tSearchResult.m_pNameToSearch = pName;
	this->Iterate(SearchName<TXML_Element>, tSearchResult);

	return tSearchResult.m_tItemFound;
}

TXML_Element * TXML_Element::FindTiledObjectGroup(const char * pName)
{
	TXML_Element * pObjectGroup = this->FindFirstChildElement("objectgroup");
	TXML_Attribute * pObjectGroupNameAttribute = pObjectGroup->FindAttribute("name");
	const char * pObjectGroupName = pObjectGroupNameAttribute->m_tXML_Data.GetValueString();

	if (strcmp(pName, pObjectGroupName) == 0)
	{
		return pObjectGroup;
	}
	else
	{
		while (pObjectGroup != nullptr)
		{
			pObjectGroup = pObjectGroup->FindNextSiblingElement("objectgroup");
			if (pObjectGroup != nullptr)
			{
				pObjectGroupNameAttribute = pObjectGroup->FindAttribute("name");
				pObjectGroupName = pObjectGroupNameAttribute->m_tXML_Data.GetValueString();
				if (strcmp(pName, pObjectGroupName) == 0)
				{
					return pObjectGroup;
				}
			}
		}
	}

	return nullptr;
}

TXML_Attribute * TXML_Element::FindAttribute(const char * pName)
{
	TXML_Search<TXML_Attribute> tSearchResult;
	tSearchResult.m_pNameToSearch = pName;
	m_pAttributeList->Iterate(SearchName<TXML_Attribute>, tSearchResult);

	return tSearchResult.m_tItemFound;
}

TXML_Element * TXML_Element::FindTiledObjectProperty(const char * pName)
{
	TXML_Element * pProperties = this->FindFirstChildElement("properties");
	TXML_Search<TXML_Element> tSearchResult;
	tSearchResult.m_pNameToSearch = pName;

	pProperties->m_pElementChildList->Iterate(SearchTiledProperty, tSearchResult);

	return tSearchResult.m_tItemFound;
}
