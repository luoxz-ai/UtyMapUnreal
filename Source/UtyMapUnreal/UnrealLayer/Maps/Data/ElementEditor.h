#pragma once

#include "MapStorageType.h"
#include "Stylesheet.h"
#include "Range.h"
#include "Element.h"
#include "Range.h"
//#include "IPathResolver.h"


using namespace UtyDepend;

namespace UtyMap
{
	namespace Unreal
	{
		namespace Maps
		{
			namespace Data
			{
				// Specifies behavior of element editor.
				class IElementEditor
				{
					// Adds element.
				public:
					virtual void Add(EMapStorageType storageType, FElement* element, TLodRange<int>* levelOfDetails) = 0;

					// Edits element.
					virtual void Edit(EMapStorageType storageType, FElement* element, TLodRange<int>* levelOfDetails) = 0;

					// Marks element with given id.
					virtual void Delete(EMapStorageType storageType, long long elementId, TLodRange<int>* levelOfDetails) = 0;
				};

				// Default implementation of IElementEditor which
				// works with in-memory store.
				class FElementEditor : public IElementEditor
				{
				private:
					UStylesheet* const  _stylesheet;
					//IPathResolver* const  _resolver;

				public:
					FElementEditor(UStylesheet* stylesheet/*, IPathResolver* resolver*/);

					void Add(EMapStorageType storageType, FElement* element, TLodRange<int>* levelOfDetails);
					void Edit(EMapStorageType storageType, FElement* element, TLodRange<int>* levelOfDetails);
					void Delete(EMapStorageType storageType, long long elementId, TLodRange<int>* levelOfDetails);
				};
			}
		}
	}
}
