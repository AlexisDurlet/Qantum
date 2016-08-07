#ifndef	SLL_ITEM_H_INCLUDED
#define SLL_ITEM_H_INCLUDED

//DECLARATION
template <class T>

class TSLL_Item
{

	private:

		T * m_pNextItem;

	public:

		TSLL_Item();
		~TSLL_Item();
		
		T * GetNextItem() const;
		void InsertAfter(T * pNewItem);
		bool Remove(T * pToRemoveItem);
		int GetCount();

		bool Iterate(void(*pFunction)(T * pItem));
		template <class T2>
		bool Iterate(void(*pFunction)(T * pItem, T2 & StorageClass), T2 & StorageClass);

};

//DEFINITIONS
template <class T>

TSLL_Item<T>::TSLL_Item() :
m_pNextItem(nullptr)
{

}

template <class T>

TSLL_Item<T>::~TSLL_Item()
{
	if (m_pNextItem != nullptr)
	{
		delete m_pNextItem;
		m_pNextItem = nullptr;
	}
}

template <class T>

T * TSLL_Item<T>::GetNextItem() const
{
	return m_pNextItem;
}

template <class T>

void TSLL_Item<T>::InsertAfter(T * pNewItem)
{
	bool bLastItem = false;
	T * pNextItem = pNewItem;

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

	TSLL_Item<T> * pEndOfNewItem = pNextItem;

	pEndOfNewItem->m_pNextItem = m_pNextItem;
	m_pNextItem = pNewItem;
}

template <class T>

bool TSLL_Item<T>::Remove(T * pItemToRemove)
{
	bool bItemRemoved = false;
	bool bLastItem = false;
	TSLL_Item * pCurrentItem = this;
	TSLL_Item * pNextItem = this;

	while (!bLastItem)
	{
		if (pNextItem->m_pNextItem == nullptr)
		{
			bLastItem = true;
			bItemRemoved = false;
		}
		else
		{
			if (pNextItem == pItemToRemove)
			{
				pCurrentItem->m_pNextItem = pNextItem->m_pNextItem;
				pNextItem->m_pNextItem = nullptr;
				bItemRemoved = true;
				break;
			}
			pCurrentItem = pNextItem;
			pNextItem = pNextItem->m_pNextItem;
		}
	}

	return bItemRemoved;
}

template <class T>

int TSLL_Item<T>::GetCount()
{
	int iItemCount = 0;
	bool bLastItem = false;
	TSLL_Item * pNextItem = this;

	while (!bLastItem)
	{
		if (pNextItem->m_pNextItem == nullptr)
		{
			iItemCount++;
			bLastItem = true;
		}
		else
		{
			iItemCount++;
			pNextItem = pNextItem->m_pNextItem;
		}
	}
	return iItemCount;
}

template <class T>

bool TSLL_Item<T>::Iterate(void(*pFunction)(T * pItem))
{
	T * pNextItem = m_pNextItem;
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

template <class T>
template <class T2>
//general iterate method overloaded with storage class
bool TSLL_Item<T>::Iterate(void(*pFunction)(T * pItem, T2 & StorageClass), T2 & StorageClass)
{
	T * pNextItem = m_pNextItem;
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


#endif
