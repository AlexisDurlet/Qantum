#ifndef LOAD_UTILITIES_H_INCLUDED
#define	LOAD_UTILITIES_H_INCLUDED

struct TGfxVec2;

template<typename T>

TXML_Element * ExtractAndCreateObjects(TXML_Root * tXML_Root, T *& pObjects, int & iObjectsNumber, const char * pObjectsName);

template<typename T>

void DeleteArray(T *& p);

template<typename T>

void DeleteArrayOfPointer(T **& p, const int n);

TGfxVec2 * ExtractVec2(TXML_Element * pObjectGroup, const int iObjectsNumber, const char * pXName, const char * pYName, const float fFactor, const bool bProperty);
int * ExtractInt(TXML_Element * pObjectGroup, const int iObjectsNumber, const char * pName, const bool bProperty);
float * ExtractFloat(TXML_Element * pObjectGroup, const int iObjectsNumber, const char * pName, const float fFactor, const bool bProperty);
const char ** ExtractString(TXML_Element * pObjectGroup, const int iObjectsNumber, const char * pName, const bool bProperty);

void LoadTextures(TXML_Root * tXML_Root, TGfxTexture **& pTextures, int & iTextureNumber);
int * GetTextureIndex(TXML_Element * pObjectGroup, const int iObjectsNumber);

void CorrectCoordinates(TGfxVec2 & tCoordinates, const TGfxVec2 & tRadius);


//Templates definitions

template<typename T>

TXML_Element * ExtractAndCreateObjects(TXML_Root * tXML_Root, T *& pObjects, int & iObjectsNumber, const char * pObjectsName)
{
	TXML_Element * pObejectGroup = tXML_Root->FindFirstChildElement("map");
	pObejectGroup = pObejectGroup->FindTiledObjectGroup(pObjectsName);
	if (pObejectGroup != nullptr)
	{
		iObjectsNumber = pObejectGroup->m_pElementChildList->GetCount();
	}
	else
	{
		iObjectsNumber = 0;
	}

	pObjects = new T[iObjectsNumber];

	return pObejectGroup;
}

template<typename T>

void DeleteArray(T *& p)
{
	if (p != nullptr)
	{
		delete[] p;
		p = nullptr;
	}
}

template<typename T>

void DeleteArrayOfPointer(T **& p, const int n)
{
	if (p != nullptr)
	{
		for (int i = 0; i < n; i++)
		{
			if (p[i] != nullptr)
			{
				delete p[i];
				p[i] = nullptr;
			}
		}
	}
	DeleteArray(p);
}

template<>

inline void DeleteArrayOfPointer(TGfxTexture **& p, const int n)
{
	if (p != nullptr)
	{
		for (int i = 0; i < n; i++)
		{
			if (p[i] != nullptr)
			{
				GfxTextureDestroy(p[i]);
				p[i] = nullptr;
			}
		}
	}
	DeleteArray(p);
}

#endif 
