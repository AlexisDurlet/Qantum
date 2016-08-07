#ifndef	SLL_ROOT_H_INCLUDED
#define SLL_ROOT_H_INCLUDED


////DECLARATION
template <class T>

class TSLL_Root
{
	private:

		T * m_pRoot;

	public:

		TSLL_Root();
		~TSLL_Root();

		T * FindLastItem();
		void Append(T * pNewItem);
		bool Remove(T * pToRemoveItem);
		int GetCount();

		bool Iterate(void (*pFunction)(T * pItem));

		template <class T2>
		bool Iterate(void(*pFunction)(T * pItem, T2 & StorageClass), T2 & StorageClass);
};



/////DEFINITIONS
template <class T>

TSLL_Root<T>::TSLL_Root() :
m_pRoot(nullptr)
{

}

template <class T>

TSLL_Root<T>::~TSLL_Root()
{
	if (m_pRoot != nullptr)
	{
		delete m_pRoot;
		m_pRoot = nullptr;
	}
}

template <class T>

T * TSLL_Root<T>::FindLastItem()
{
	if (m_pRoot == nullptr)
	{
		return nullptr;
	}
	else
	{
		bool bLastItem = false;
		T * pNextItem = m_pRoot;

		while (!bLastItem)
		{
			if (pNextItem->GetNextItem() == nullptr)
			{
				bLastItem = true;
			}
			else
			{
				pNextItem = pNextItem->GetNextItem();
			}
		}
		return pNextItem;
	}
}

template <class T>

void TSLL_Root<T>::Append(T * pNewItem)
{
	if (m_pRoot == nullptr)
	{
		m_pRoot = pNewItem;
	}
	else
	{
		T * pEndListItem = FindLastItem();
		pEndListItem->InsertAfter(pNewItem);
	}
}

template <class T>

bool TSLL_Root<T>::Remove(T * pToRemoveItem)
{
	if (m_pRoot == nullptr)
	{
		return false;
	}
	else if (m_pRoot == pToRemoveItem)
	{
		m_pRoot = m_pRoot->GetNextItem();
		return true;
	}
	else
	{
		m_pRoot->Remove(pToRemoveItem);
		return true;
	}

}

template <class T>

int TSLL_Root<T>::GetCount()
{
	if (m_pRoot == nullptr)
	{
		return 0;
	}
	else
	{
		return m_pRoot->GetCount();
	}
}

template <class T>
//general iterate method
bool TSLL_Root<T>::Iterate(void(*pFunction)(T * pItem))
{
	T * pNextItem = m_pRoot;
	if (pNextItem != nullptr)
	{
		while (pNextItem != nullptr)
		{
			pFunction(pNextItem);
			pNextItem = pNextItem->GetNextItem();
		}

		return true;
	}

	return false;
}

template <>
//specialized iterate method for TXML_Element (iterate on childs)
inline bool TSLL_Root<TXML_Element>::Iterate(void(*pFunction)(TXML_Element * pItem))
{
	TXML_Element * pNextItem = m_pRoot;
	if (pNextItem != nullptr)
	{
		while (pNextItem != nullptr)
		{
			pFunction(pNextItem);
			pNextItem->m_pElementChildList->Iterate(pFunction);
			pNextItem = pNextItem->GetNextItem();
		}

		return true;
	}

	return false;
}

template <class T>
template <class T2>
//general iterate method overloaded with storage class
bool TSLL_Root<T>::Iterate(void(*pFunction)(T * pItem, T2 & StorageClass), T2 & StorageClass)
{
	T * pNextItem = m_pRoot;
	if (pNextItem != nullptr)
	{
		while (pNextItem != nullptr)
		{
			pFunction(pNextItem, StorageClass);
			pNextItem = pNextItem->GetNextItem();
		}

		return true;
	}

	return false;
}

//Specialized iterate method for TXML_Element, overloaded with storage class
template <>
template <class T2>
bool TSLL_Root<TXML_Element>::Iterate(void(*pFunction)(TXML_Element * pItem, T2 & StorageClass), T2 & StorageClass)
{
	TXML_Element * pNextItem = m_pRoot;
	if (pNextItem != nullptr)
	{
		while (pNextItem != nullptr)
		{
			pFunction(pNextItem, StorageClass);
			pNextItem->m_pElementChildList->Iterate(pFunction, StorageClass);
			pNextItem = pNextItem->GetNextItem();
		}

		return true;
	}

	return false;
}

#endif
