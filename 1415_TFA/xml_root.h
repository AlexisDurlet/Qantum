#ifndef	XML_ROOT_INCLUDED
#define XML_ROOT_INCLUDED

class TXML_Root : private TXML_Element
{
	private:

		static void DbgPrintAttributeData(TXML_Attribute * pAtribute);
		static void DbgPrintElementData(TXML_Element * pElement);

	public:

		static TXML_Root * ParseXML(const char * pFilename);

		TXML_Element * FindFirstElement(const char * pName);
		TXML_Element * FindTiledObjectGroup(const char * pName)
		{
			return TXML_Element::FindTiledObjectGroup(pName);
		}
		TXML_Element * FindFirstChildElement(const char * pName)
		{
			return TXML_Element::FindFirstChildElement(pName);
		}
		void DebugPrint();
};


#endif
