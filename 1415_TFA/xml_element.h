#ifndef	 TXML_ELEMENT_H_INCLUDED
#define  TXML_ELEMENT_H_INCLUDED

class TXML_Attribute;

template <class T>
class TSLL_Root;

class TXML_Element : public TSLL_Item<TXML_Element>
{
	private:

		//Extract an element name begining at pChar and store it in pElementName
		//Truncate it if exceeding max size and print warning
		void ExtractElementName(char * pChar, char * pElementName);		

		//Extract both attribute name and value for all attributes of a tag and append them to the attribute list.
		void ExtractAttributes(char * pTagOpening, char * pTagClosing);

		//Check if element has a value
		//If yes store it in TXML_Data
		//Truncate it if exceeding max size and print warning
		bool CheckElementValue(char * pTagClosing, char * pNextTagOpening);




		static void SearchTiledProperty(TXML_Element * pProperty, TXML_Search<TXML_Element> & tSearchResult);

	protected:

		template<class T>
		static void SearchName(T * pSearched, TXML_Search<T> & tSearchResult);

	public:

		TXML_Data m_tXML_Data;

		TSLL_Root<TXML_Attribute> * m_pAttributeList;
		TSLL_Root<TXML_Element> * m_pElementChildList;

		TXML_Element();
		~TXML_Element();

		char * ScanElement(char * pChar, bool & bError);
		TXML_Element * FindFirstChildElement(const char * pName);
		TXML_Element * FindNextSiblingElement(const char * pName);
		TXML_Element * FindTiledObjectGroup(const char * pName);
		TXML_Attribute * FindAttribute(const char * pName);
		TXML_Element * FindTiledObjectProperty(const char * pName);
};

#endif
