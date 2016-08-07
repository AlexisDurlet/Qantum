#include "sll_item.h"
#include "constants.h"
#include "xml_data.h"
#include "xml_search.h"
#include "xml_element.h"
#include "sll_root.h"
#include "xml_root.h"
#include "xml_attribute.h"
#include "flib.h"
#include <cstring>

TXML_Root * TXML_Root::ParseXML(const char * pFilename)
{
	TXML_Root * pXML_Root = new TXML_Root;

	TGfxFile * pFile;
	
	pFile = GfxFileOpenRead(pFilename);
	
	const int iFileSize = GfxFileSize(pFile) + 1;
	void * pBuffer = GfxMemAlloc(iFileSize);
	char * pChar = static_cast<char*>(pBuffer);
	pChar[iFileSize - 1] = '\0';
	
	for (int i = 0; i < iFileSize - 1; i++)
	{
		pChar[i] = GfxFileReadChar(pFile);
	}
	
	GfxFileClose(pFile);
	
	bool bError = false;
	
	while (pChar[0] != '\0')
	{
		TXML_Element * tXMLElement = new TXML_Element;
		pChar = tXMLElement->ScanElement(pChar, bError);
		pXML_Root->m_pElementChildList->Append(tXMLElement);
	}

	GfxMemFree(pBuffer);
	
	if (bError)
	{
		delete pXML_Root;
		pXML_Root = nullptr;
	}
	
	return pXML_Root;
}

void TXML_Root::DbgPrintAttributeData(TXML_Attribute * pAtribute)
{
	GfxDbgPrintf(" ");
	GfxDbgPrintf(pAtribute->m_tXML_Data.GetNameArray());
	GfxDbgPrintf(" = ");
	GfxDbgPrintf(pAtribute->m_tXML_Data.GetValueArray());
}

void TXML_Root::DbgPrintElementData(TXML_Element * pElement)
{
	if (pElement->m_tXML_Data.GetNameArray()[0] != '\0')
	{
		GfxDbgPrintf(pElement->m_tXML_Data.GetNameArray());
		pElement->m_pAttributeList->Iterate(DbgPrintAttributeData);

		if (pElement->m_tXML_Data.GetValueArray()[0] != '\0')
		{
			GfxDbgPrintf(" = ");
			GfxDbgPrintf(pElement->m_tXML_Data.GetValueArray());
		}

		GfxDbgPrintf("\n");
	}

}

TXML_Element * TXML_Root::FindFirstElement(const char * pName)
{
	TXML_Search<TXML_Element> tSearchResult;
	tSearchResult.m_pNameToSearch = pName;
	m_pElementChildList->Iterate(SearchName, tSearchResult);

	return tSearchResult.m_tItemFound;
}

void TXML_Root::DebugPrint()
{
	m_pElementChildList->Iterate(DbgPrintElementData);
}
