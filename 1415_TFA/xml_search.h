#ifndef XML_SEARCH_H_INCLUDED
#define	XML_SEARCH_H_INCLUDED

template <class T>

class TXML_Search
{
	public:

		TXML_Search();

		const char * m_pNameToSearch;
		T * m_tItemFound;
};

template <class T>

TXML_Search<T>::TXML_Search():
m_pNameToSearch(nullptr),
m_tItemFound(nullptr)
{

}

#endif
