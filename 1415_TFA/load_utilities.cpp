#include "flib.h"
#include "sll_item.h"
#include "constants.h"
#include "xml_data.h"
#include "xml_search.h"
#include "xml_element.h"
#include "sll_root.h"
#include "xml_root.h"
#include "xml_attribute.h"
#include "load_utilities.h"
#include "flib_vec2.h"


TGfxVec2 * ExtractVec2(TXML_Element * pObjectGroup, const int iObjectsNumber, const char * pXName, const char * pYName, const float fFactor, const bool bProperty)
{
	TGfxVec2 * pVec2 = new TGfxVec2[iObjectsNumber];

	if (pObjectGroup != nullptr)
	{
		TXML_Element * pObject = pObjectGroup->FindFirstChildElement("object");

		for (int i = 0; i < iObjectsNumber; i++)
		{
			float x;
			float y;

			if (bProperty)
			{
				x = pObject->FindTiledObjectProperty(pXName)->FindAttribute("value")->m_tXML_Data.GetValueNumericf() * fFactor;
				y = pObject->FindTiledObjectProperty(pYName)->FindAttribute("value")->m_tXML_Data.GetValueNumericf() * fFactor;
			}
			else
			{
				x = pObject->FindAttribute(pXName)->m_tXML_Data.GetValueNumericf() * fFactor;
				y = pObject->FindAttribute(pYName)->m_tXML_Data.GetValueNumericf() * fFactor;
			}

			pVec2[i] = TGfxVec2(x, y);
			pObject = pObject->FindNextSiblingElement("object");
		}
	}

	return pVec2;
}

int * ExtractInt(TXML_Element * pObjectGroup, const int iObjectsNumber, const char * pName, const bool bProperty)
{
	int * iNumbers = new int[iObjectsNumber];

	if (pObjectGroup != nullptr)
	{
		TXML_Element * pObject = pObjectGroup->FindFirstChildElement("object");

		for (int i = 0; i < iObjectsNumber; i++)
		{
			int n;

			if (bProperty)
			{
				n = pObject->FindTiledObjectProperty(pName)->FindAttribute("value")->m_tXML_Data.GetValueNumerici();
			}
			else
			{
				n = pObject->FindAttribute(pName)->m_tXML_Data.GetValueNumerici();
			}

			iNumbers[i] = n;
			pObject = pObject->FindNextSiblingElement("object");
		}
	}

	return iNumbers;
}

float * ExtractFloat(TXML_Element * pObjectGroup, const int iObjectsNumber, const char * pName, const float fFactor, const bool bProperty)
{
	float * fNumbers = new float[iObjectsNumber];

	if (pObjectGroup != nullptr)
	{
		TXML_Element * pObject = pObjectGroup->FindFirstChildElement("object");

		for (int i = 0; i < iObjectsNumber; i++)
		{
			float n;

			if (bProperty)
			{
				n = pObject->FindTiledObjectProperty(pName)->FindAttribute("value")->m_tXML_Data.GetValueNumericf();
			}
			else
			{
				n = pObject->FindAttribute(pName)->m_tXML_Data.GetValueNumericf();
			}

			fNumbers[i] = n * fFactor;
			pObject = pObject->FindNextSiblingElement("object");
		}
	}

	return fNumbers;
}

const char ** ExtractString(TXML_Element * pObjectGroup, const int iObjectsNumber, const char * pName, const bool bProperty)
{
	const char ** pStrings = new const char *[iObjectsNumber];

	if (pObjectGroup != nullptr)
	{
		TXML_Element * pObject = pObjectGroup->FindFirstChildElement("object");

		for (int i = 0; i < iObjectsNumber; i++)
		{
			const char * c;

			if (bProperty)
			{
				c = pObject->FindTiledObjectProperty(pName)->FindAttribute("value")->m_tXML_Data.GetValueString();
			}
			else
			{
				c = pObject->FindAttribute(pName)->m_tXML_Data.GetValueString();
			}

			pStrings[i] = c;
			pObject = pObject->FindNextSiblingElement("object");
		}
	}

	return pStrings;
}

void LoadTextures(TXML_Root * tXML_Root, TGfxTexture **& pTextures, int & iTextureNumber)
{
	TXML_Element * pTexturesElement = ExtractAndCreateObjects(tXML_Root, pTextures, iTextureNumber, "textures");
	const char ** pTexturesNames = ExtractString(pTexturesElement, iTextureNumber, "name", false);

	for (int i = 0; i < iTextureNumber; i++)
	{
		pTextures[i] = GfxTextureLoad(pTexturesNames[i]);
	}
	DeleteArrayOfPointer(pTexturesNames, iTextureNumber);
}

int * GetTextureIndex(TXML_Element * pObjectGroup, const int iObjectsNumber)
{
	int * pTextureIndex = ExtractInt(pObjectGroup, iObjectsNumber, "texture", true);
	return pTextureIndex;
}

void CorrectCoordinates(TGfxVec2 & tCoordinates, const TGfxVec2 & tRadius)
{
	tCoordinates = tCoordinates + tRadius;
	tCoordinates.y = -tCoordinates.y;
}


