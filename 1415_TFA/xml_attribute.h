#ifndef	 TXML_ATTRIBUTE_H_INCLUDED
#define  TXML_ATTRIBUTE_H_INCLUDED

class TXML_Attribute : public TSLL_Item<TXML_Attribute>
{
	public:

		TXML_Data m_tXML_Data;

		//Extract attribute name corresponding to an '=' of a tag and store it in tXML_Data.
		//Truncate it if exceeding max size and print warning
		void ExtractAttributeName(char * pEqual);

		//Extract attribute Value corresponding to an '=' of a tag and store it in tXML_Data.
		//Truncate it if exceeding max size and print warning
		void ExtractAttributeValue(char * pEqual);
};

#endif
