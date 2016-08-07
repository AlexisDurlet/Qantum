#include "constants.h"
#include"sll_item.h"
#include "xml_data.h"
#include "xml_attribute.h"
#include <cstring>

void TXML_Attribute::ExtractAttributeName(char * pEqual)
{
	char * pAttributeNameEnd = m_tXML_Data.IgnoreBlanks(pEqual, -1);
	char * pAttributeNameStart = m_tXML_Data.FindBlank(pAttributeNameEnd, -1) + 1;

	m_tXML_Data.BoundariesStrncpyMaxed(pAttributeNameStart, pAttributeNameEnd + 1, m_tXML_Data.GetNameArray(), MAX_XML_DATA_SIZE, "Attribute name");
}

void TXML_Attribute::ExtractAttributeValue(char * pEqual)
{
	char * pAttributeValueStart = m_tXML_Data.IgnoreBlanks(pEqual, 1);
	char * pAttributeValueEnd;

	if (pAttributeValueStart[0] == '"' )
	{
		pAttributeValueEnd = strchr(pAttributeValueStart + 1, '"');
	}
	else if (pAttributeValueStart[0] == '\'')
	{
		pAttributeValueEnd = strchr(pAttributeValueStart + 1, '\'');
	}
	else
	{
		pAttributeValueEnd = m_tXML_Data.FindBlank(pAttributeValueStart, 1);
	}

	char * pTagClosing = strchr(pEqual, '>');
	if (pAttributeValueEnd > pTagClosing)
	{
		pAttributeValueEnd = pTagClosing - 1;
	}

	m_tXML_Data.BoundariesStrncpyMaxed(pAttributeValueStart, pAttributeValueEnd + 1, m_tXML_Data.GetValueArray(), MAX_XML_DATA_SIZE, "Attribute value");
}
